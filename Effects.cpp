#include "Effects.h"
#include <functional>

void loop(const std::function<void(value_t &obj)> &callback, AudioBuffersSimple &buffer, real in, real to) {

    int g = 0;

    // прохід всього файла
    if (in == -1 && to == -1) {

        for (QAudioBuffer &tbuff : buffer) {

            auto max = tbuff.byteCount() / sizeof(value_t);
            short *sbuff = (short *)tbuff.constData();

            for (int i = 0; i < max; ++i, ++g)
                callback( sbuff[i] );
        }
    }


    // прохід по діапазону
    else {

        in *= buffer.front().format().channelCount();
        to *= buffer.front().format().channelCount();
        for (QAudioBuffer &tbuff : buffer) {

            auto max = tbuff.byteCount() / sizeof(value_t);
            short *sbuff = (short *)tbuff.constData();

            for (int i = 0; i < max; ++i, ++g) {

                if (g >= in) {
                    if (g <= to) {
                        callback( sbuff[i] );
                    }
                    else return;
                }

            }
        }
    }
}

void Effects::Normalize(AudioBuffersSimple &buffer, float ampl, real in, real to)
{
    int maxAmpl = 0;

    loop( [&] (value_t &value) {
        const int absolute = std::abs(value);

        if (absolute > maxAmpl)
            maxAmpl = absolute;

    }, buffer, in, to);

    const float scale = ((float)((1 << (sizeof(value_t)*8 - 1)) - 1)) / maxAmpl * ampl;

    loop( [&] (value_t &value) {
        value *= scale;
    }, buffer, in, to);

}

void Effects::ReverseX(AudioBuffersSimple &buffer, real in, real to)
{
    AudioBuffersSimple sim;

    auto callBackupRLChannels = [] (AudioBuffersSimple &buffer){

        for (auto &b : buffer) {

            value_t *data = (value_t *)b.data();
            const size_t size = b.byteCount() / sizeof(value_t);

            for (int i = 0; i < size-1; i += 2) {
                std::swap(data[i], data[i+1]);
            }
        }

    };

    if (in == -1 && to == -1) {

        for (auto b = buffer.rbegin(); b != buffer.rend(); ++b) {

            std::vector<value_t> result;

            result.reserve(b->byteCount() / sizeof(value_t) + 1);

            const value_t *array = (const value_t*)b->data();

            for (int i = b->byteCount() / sizeof(value_t) - 1; i >= 0; i--) {
                result.push_back(array[i]);
            }

            auto rb = QByteArray((const char *)result.data(), result.size()*sizeof(value_t));
            auto ab = QAudioBuffer(rb, buffer.front().format());

            sim.push_back(std::move(ab));
        }
        callBackupRLChannels(sim);

    }

    else {

        const size_t sz = buffer.front().byteCount() / sizeof(value_t);
        value_t result[sz];
        int i = 0;

        enum InsertPos {Front, Back};

        AudioBuffersSimple prev, last;

        auto callpush = [&](value_t *arr, int realsz, InsertPos ins, AudioBuffersSimple &simple) -> void {

            if (realsz) {
                switch (ins) {
                case Front: simple.push_front(QAudioBuffer(QByteArray((const char *)arr + (sz-realsz)*sizeof(value_t), realsz * sizeof(value_t)), buffer.front().format())); break;
                case Back: simple.push_back(QAudioBuffer(QByteArray((const char *)arr, realsz * sizeof(value_t)), buffer.front().format())); break;
                }
            }
        };


        loop([&](value_t &value){
            if (i >= sz) {
                callpush(result, i, Back, prev);
                i = 0;
            }
            result[i] = value;
            ++i;
        }, buffer, 0, in-1);

        callpush(result, i, Back, prev);
        i = 0;

        loop([&](value_t &value){

            if (i >= sz) {
                callpush(result, i, Front, sim);
                i = 0;
            }
            result[sz - i - 1] = value;
            ++i;
        }, buffer, in, to);

        callpush(result, i, Front, sim);
        callBackupRLChannels(sim);
        i = 0;

        loop([&](value_t &value){
            if (i >= sz) {
                callpush(result, i, Back, last);
                i = 0;
            }
            result[i] = value;
            ++i;
        }, buffer, to+1, AudioBuffers(buffer).getAudioSize());

        callpush(result, i, Back, last);
        i = 0;

        for (auto i : sim)
            prev.push_back(i);

        for (auto i : last)
            prev.push_back(i);

        sim = std::move(prev);

    }

    buffer = std::move(sim);
}

void Effects::ReverseY(AudioBuffersSimple &buffer, real in, real to)
{
    loop([](value_t &value){
        value = -value;
    }, buffer, in, to);
}

void Effects::FadeIn(AudioBuffersSimple &buffer, float force, float ampl, real in, real to, real sz)
{
    const int diapason = (in < 0 || to < 0) ? (sz + 1) : (to - in + 1);
    const real step = (0.5l / diapason) * force;

    real scaller = 1.0 - force;

    loop([&](value_t &value){
        value *= scaller * ampl;
        scaller += step;
    }, buffer, in, to);
}

void Effects::FadeOut(AudioBuffersSimple &buffer, float force, float ampl, real in, real to, real sz)
{
    const int diapason = (in < 0 || to < 0) ? (sz + 1) : (to - in + 1);
    const real step = (0.5l / diapason) * force;

    real scaller = 1.0;

    loop([&](value_t &value){
        value *= scaller * ampl;
        scaller -= step;
    }, buffer, in, to);
}

void Effects::NoiseLogarithmic(AudioBuffersSimple &buffer, float force, real in, real to)
{
    loop([&](value_t &value){
        float mul = log(0.1 + 8.0 / value) * 0.3 * (value > 1 ? 1 : -1);
        mul = (mul >= 1 ? 0.9 : (mul <= -1 ? -0.9 : mul));
        value = (value * (1.0 - force)) + value * (mul * (force));
    }, buffer, in, to);
}

void Effects::Sinusoidal(AudioBuffersSimple &buffer, float force, float ampl, float hz, real in, real to)
{
    double i = 0;

    const QAudioFormat &format = buffer.front().format();

    const float step = 3.1415*2 / format.sampleRate() / hz / format.channelCount();

    force /= format.channelCount();

    loop([&](value_t &value){
        auto scale = sin(3.1415 / value + ( i+= step)) * force;
        value = (value * scale + value * (1.0 - force)) * ampl;
    }, buffer, in, to);
}

void resizing(AudioBuffersSimple &buffer, std::function<bool (size_t a, size_t b, size_t c, size_t d)> cmp, real in, real to) {
    in = in < 0 ? 0 : in;


    size_t g = 0;

    AudioBuffersSimple sim;

    in *= buffer.front().format().channelCount();
    to *= buffer.front().format().channelCount();

    for (auto &ab : buffer) {

        value_t *arr = (value_t *)ab.data();
        size_t size = ab.byteCount() / sizeof(value_t);

        if (cmp(g+size, in, g, to))
            sim.push_back(ab);

        else {
            std::vector<value_t> rs;
            rs.reserve(size);

            for (size_t i = 0; i < size; ++i)
                if (cmp(g+i, in, g+i, to))
                    rs.push_back(arr[i]);

            if (!rs.empty())
                sim.push_back(QAudioBuffer(QByteArray((const char *)rs.data(), rs.size()*sizeof(value_t)), ab.format()));

        }
        g += size;
    }

    buffer = std::move(sim);
}

void Effects::Cut(AudioBuffersSimple &buffer, real in, real to)
{
    resizing(buffer, [](size_t a, size_t b, size_t c, size_t d){ return (a < b || c > d); }, in, to);
}

void Effects::Crop(AudioBuffersSimple &buffer, real in, real to)
{
    resizing(buffer, [](size_t a, size_t b, size_t c, size_t d){ return a >= b && c <= d; }, in, to);
}
