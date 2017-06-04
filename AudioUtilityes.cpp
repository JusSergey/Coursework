#include "AudioUtilityes.h"
#include <iostream>
using std::cout;

AudioBuffers::AudioBuffers(const AudioBuffersSimple &lst) : AudioBuffersSimple(lst), audioSize(0)
{
    std::printf("AUDIOBUFFERSIMPLE");
    for (const auto &obj : lst)
        audioSize += obj.byteCount() / sizeof(value_t);

}

void AudioBuffers::setAudioBuffers(const AudioBuffers &abuff)
{
    (cout << "INIT\n").flush();
    *(static_cast<AudioBuffersSimple*>(this)) = abuff;
    (cout << "FINISH\n").flush();
    audioSize = abuff.audioSize;
}


//////////////////////////////////


AudioBuffersRAM::AudioBuffersRAM(const AudioBuffers &buffers) : AudioBuffers(buffers)
{

}

value_t &AudioBuffersRAM::operator [](size_t pos)
{
    size_t idx = 0;

//    std::printf("START");

    for (auto &obj : *this) {

        idx += obj.byteCount() / sizeof(value_t);

        if (idx >= pos){
//            std::printf("[=%d=][%d, %d]\n", idx-pos-1, idx, pos);
            return ((value_t*)obj.data())[idx - pos - 1];
        }
//        idx++;
    }
//    std::printf("FINISH");

    throw std::out_of_range("AudioBuffersRAM::operator [](size_t pos) : out_of_range");

}
