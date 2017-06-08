#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets>
#include <QtMultimedia>
#include "DrawWidget.h"
#include "DirectorView.h"
#include "AudioUtilityes.h"
#include "PlayerWindow.h"

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
    PlayerWindow *player;
    QMenuBar *menu;

    AudioBuffers readfile;
    size_t readfilesize;
    QString fpath;
    QList<QAction *> mActions;
    bool isOpenFile;

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);

private slots:
    // File Action Slots
    void slotDecoderBufferReady();
    void slotAudioDecodeFinish();

    // Menu Slots
    void slotOpen();
    void slotSave();
    void slotSaveAs();
    void slotClose();
    void slotPlayer();

    // Listener Slots Navigator
    void slotChangeScaleW(float scale);
    void slotChangeScaleH(float scale);

    // Single Slots
    void slotNormalize();
    void slotFadeIn();
    void slotFadeOut();
    void slotReverseY();
    void slotReverseX();
    void slotNoiseLog();
    void slotPulse();
    void slotCut();
    void slotCrop();

    // Ext Slots
    void slotAcceptNormalize(bool isAllFile, std::map<QString, float> values);
    void slotAcceptFadeIn(bool isAllFile, std::map<QString, float> values);
    void slotAcceptFadeOut(bool isAllFile, std::map<QString, float> values);
    void slotAcceptReverseY(bool isAllFile, std::map<QString, float> values);
    void slotAcceptReverseX(bool isAllFile, std::map<QString, float> values);
    void slotAcceptNoiseLog(bool isAllFile, std::map<QString, float> values);
    void slotAcceptPulse(bool isAllFile, std::map<QString, float> values);

private:
    void saveForPlayer();
    void openFile(QString path);
    void saveFile(QString path);
    void initMenuBar();
    void setEnableAction(QString action, bool enable);
};

#endif // MAINWIDGET_H
