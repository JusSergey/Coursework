#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets>
#include <QtMultimedia>
#include "DrawWidget.h"
#include "DirectorView.h"
#include "AudioUtilityes.h"

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

class MainWidget : public QMainWindow
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    QAudioDecoder *decoder;
    DrawWidget *drawView;
    DirectorView *directorView;
    QMenuBar *menu;

    AudioBuffersSimple readfile;
    size_t readfilesize;
    QString fpath;

private slots:
    // File Action Slots
    void slotButtonSelectFile();
    void slotButtonOpenDialogDelectFile();
    void slotDecoderBufferReady();
    void slotAudioDecodeFinish();

    // Menu Slots
    void slotOpen();
    void slotSave();
    void slotClose();
    void slotExit();

    // Listener Slots Navigator
    void slotChangeScaleW(float scale);
    void slotChangeScaleH(float scale);

    // Single Slots
    void slotNormalize();
    void slotFadeIn();
    void slotFadeOut();
    void slotReversY();
    void slotReversX();
    void slotNoiseLog();
    void slotSinosyde();

    // Ext Slots
    void slotAcceptNormalize(bool isAllFile, std::map<QString, float> values);
    void slotAcceptFadeIn(bool isAllFile, std::map<QString, float> values);
    void slotAcceptFadeOut(bool isAllFile, std::map<QString, float> values);
    void slotAcceptReversY(bool isAllFile, std::map<QString, float> values);
    void slotAcceptReversX(bool isAllFile, std::map<QString, float> values);
    void slotAcceptNoiseLog(bool isAllFile, std::map<QString, float> values);
    void slotAcceptSinosyde(bool isAllFile, std::map<QString, float> values);

private:
    void initMenuBar();
    void setEnableAction(QString action, bool enable);
    WaveHead getHeadForFile(const AudioBuffersSimple &buffer);
};

#endif // MAINWIDGET_H
