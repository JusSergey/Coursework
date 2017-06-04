#include "PCMAudioDevice.h"

PCMAudioDevice::PCMAudioDevice(QObject *parent) : QIODevice(parent)
{
    open(QIODevice::ReadOnly);
    iter = pcmBuffer.end();
    offInArray = 0;
}

qint64 PCMAudioDevice::readData(char *data, qint64 maxlen)
{
    size_t copied = 0;
    size_t left_bytes = maxlen;

    for(; (iter != pcmBuffer.end()) && (copied < maxlen); ++iter, offInArray = 0) {

        PCMBuffer::value_type &obj = *iter;

        const size_t readsz = (left_bytes + offInArray > obj.size() ? obj.size() : left_bytes) - offInArray;

        memcpy(data + copied, (void*) obj.data() + offInArray, readsz);

        left_bytes -= readsz;
        copied += readsz;
        offInArray += readsz;

        if (copied >= maxlen)
            break;
    }

    countAllReadBytesInBuffer += copied;
    return copied;
}

qint64 PCMAudioDevice::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data)
    Q_UNUSED(len)
    return 0;
}

qint64 PCMAudioDevice::bytesAvailable() const
{
    return countAllBytesInBuffer - countAllReadBytesInBuffer;
}

void PCMAudioDevice::setPCMBuffer(const PCMBuffer &buffer)
{
    pcmBuffer = buffer;
    iter = pcmBuffer.begin();

    countAllBytesInBuffer = 0;
    countAllReadBytesInBuffer = 0;

    for (const PCMBuffer::value_type &value : pcmBuffer)
        countAllBytesInBuffer += value.size();
}
