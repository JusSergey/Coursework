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


signals:

private slots:
    void slotPlay();
    void slotPause();
    void slotStop();
    void slotTimeout();
    void slotSliderMove();

public:
    void setSource(QString path);
};

#endif // PLAYERWINDOW_H
