#include "AudioUtilityes.h"

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

