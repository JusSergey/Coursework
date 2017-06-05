#include "PlayerWindow.h"

PlayerWindow::PlayerWindow(QWidget *parent) : QWidget(parent)
{
    pPause = new QPushButton(QIcon(":/icons/pause.png"), "", this);
    pPlay  = new QPushButton(QIcon(":/icons/play.png" ), "", this);
    pStop  = new QPushButton(QIcon(":/icons/stop.png" ), "", this);

    auto *lButtons = new QHBoxLayout;
    lButtons->addWidget(pStop);
    lButtons->addWidget(pPlay);
    lButtons->addWidget(pPause);

    pSlider = new QSlider(Qt::Orientation::Horizontal, this);
//    pSlider->setRange(0, 100);
    auto lAll = new QVBoxLayout;
    lAll->addWidget(pSlider);
    lAll->addLayout(lButtons);

    setLayout(lAll);

    player = new QMediaPlayer(this);

    connect(pPlay, SIGNAL(clicked(bool)), this, SLOT(slotPlay()));
    connect(pPause, SIGNAL(clicked(bool)), this, SLOT(slotPause()));
    connect(pStop, SIGNAL(clicked(bool)), this, SLOT(slotStop()));
    connect(pSlider, SIGNAL(sliderMoved(int)), this, SLOT(slotSliderMove()));

    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));

    setMaximumHeight(64);
    setMaximumWidth(300);
    setMinimumWidth(200);

    pPlay->setFocus();

    setWindowTitle("Плеєр");
}

PlayerWindow::~PlayerWindow()
{
    player->stop();
}

void PlayerWindow::slotPlay()
{
    player->play();
    timer->start(100);
}

void PlayerWindow::slotPause()
{
    player->pause();
    timer->stop();
}

void PlayerWindow::slotStop()
{
    pSlider->setValue(0);
    player->stop();
    timer->stop();
}

void PlayerWindow::slotTimeout()
{
    pSlider->setValue(player->position() * 100 / player->duration());
}

void PlayerWindow::slotSliderMove()
{
    player->setPosition(player->duration() / 100 * pSlider->value());
}

void PlayerWindow::setSource(QString path)
{
    player->stop();
    player->setMedia(QMediaContent(QUrl::fromLocalFile(path)));
}
