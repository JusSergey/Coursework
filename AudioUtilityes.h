#ifndef AUDIOUTILITYES_H
#define AUDIOUTILITYES_H
#include <QAudioBuffer>
#include <QList>

typedef signed short value_t;
typedef QList<QAudioBuffer> AudioBuffersSimple;


/********************************
 *
 *
 *
 *
 *
 *
 ********************************/

class AudioBuffers : public AudioBuffersSimple
{
public:
    AudioBuffers(const AudioBuffersSimple &lst);
    AudioBuffers() = default;

    inline size_t getAudioSize() const { return audioSize; }

    void setAudioBuffers(const AudioBuffers &abuff);

private:
    size_t audioSize = 0;
};



class AudioBuffersRAM : public AudioBuffers
{
public:
    AudioBuffersRAM(const AudioBuffers &buffers);
    void setBuffer(const AudioBuffers &buffers);
    value_t &operator [](size_t pos);
};

#endif // AUDIOUTILITYES_H
