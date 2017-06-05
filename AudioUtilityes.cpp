#include "AudioUtilityes.h"
#include <iostream>
using std::cout;

AudioBuffers::AudioBuffers(const AudioBuffersSimple &lst) : AudioBuffersSimple(lst), audioSize(0)
{
    updateAudioSize();
}

void AudioBuffers::setAudioBuffers(const AudioBuffers &abuff)
{
    *(static_cast<AudioBuffersSimple*>(this)) = abuff;
    audioSize = abuff.audioSize;
}

void AudioBuffers::setAudioBuffers(const AudioBuffersSimple &abuff)
{
    *(static_cast<AudioBuffersSimple*>(this)) = abuff;
    updateAudioSize();
}

void AudioBuffers::updateAudioSize()
{
    audioSize = 0;

    for (const auto &obj : *this)
        audioSize += obj.byteCount() / sizeof(value_t);
}


//////////////////////////////////


AudioBuffersRAM::AudioBuffersRAM(const AudioBuffers &buffers) : AudioBuffers(buffers)
{
}

value_t &AudioBuffersRAM::operator [](size_t pos)
{
    size_t idx = 0;

    for (auto &obj : *this) {

        idx += obj.byteCount() / sizeof(value_t);

        if (idx >= pos){
            return ((value_t*)obj.data())[idx - pos - 1];
        }
    }

    throw std::out_of_range("AudioBuffersRAM::operator [](size_t pos) : out_of_range");

}
