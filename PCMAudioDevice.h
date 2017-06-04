#ifndef PCMAUDIODEVICE_H
#define PCMAUDIODEVICE_H

#include <QtWidgets>

typedef QList<QByteArray> PCMBuffer;

class PCMAudioDevice : public QIODevice
{
public:
    PCMAudioDevice(QObject *parent = nullptr);

    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 writeData(const char *data, qint64 len);
    virtual qint64 bytesAvailable() const;

    void setPCMBuffer(const PCMBuffer &buffer);
    
private:
    PCMBuffer pcmBuffer;
    PCMBuffer::iterator iter;
    size_t offInArray;
    qint64 countAllBytesInBuffer;
    qint64 countAllReadBytesInBuffer;
};

#endif // PCMAUDIODEVICE_H
