#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QtWidgets>
#include <QMediaPlayer>

class PlayerWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerWindow(QWidget *parent = 0);
   ~PlayerWindow();

private:
    QSlider *pSlider;
    QPushButton *pPause;
    QPushButton *pPlay;
    QPushButton *pStop;
    QMediaPlayer *player;
    QTimer       *timer;


public slots:
    void slotPlay();
    void slotPause();
    void slotStop();

private slots:
    void slotTimeout();
    void slotSliderMove();

public:
    void setSource(QString path);
};

#endif // PLAYERWINDOW_H
