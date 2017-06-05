#ifndef AUDIOUTILITYES_H
#define AUDIOUTILITYES_H
#include <QAudioBuffer>
#include <QList>

typedef signed short value_t;
typedef QList<QAudioBuffer> AudioBuffersSimple;

class AudioBuffers : public AudioBuffersSimple
{
public:
    AudioBuffers(const AudioBuffersSimple &lst);
    AudioBuffers() = default;

    inline size_t getAudioSize() const { return audioSize; }

    void setAudioBuffers(const AudioBuffers &abuff);
    void setAudioBuffers(const AudioBuffersSimple &abuff);
    void updateAudioSize();

private:
    size_t audioSize = 0;
};

struct WaveHead {
    static const constexpr size_t lenstr = 4;

    char chunkId [lenstr];

    __uint32_t chunkSize;

    char format[lenstr];
    char subchunk1Id[lenstr];

    __uint32_t subchunk1Size;
    __uint16_t audioFormat;
    __uint16_t numChannels;
    __uint32_t sampleRate;
    __uint32_t byteRate;
    __uint16_t blockAlign;
    __uint16_t bitsPerSample;

    char subchunk2Id[lenstr];

    __uint32_t subchunk2Size;

};

#endif // AUDIOUTILITYES_H
