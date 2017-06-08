#include "DirectorView.h"

constexpr int MIN_SLIDER_H = 1;
constexpr int MAX_SLIDER_H = 100;
constexpr int MIN_SLIDER_W = 1;
constexpr int MAX_SLIDER_W = 1024;

constexpr double MIN_SPIN_H = 0.1;
constexpr double MAX_SPIN_H = 100;
constexpr double MIN_SPIN_W = 1;
constexpr double MAX_SPIN_W = 1024;

constexpr auto TITLE_SCALE_W = "Масштаб X";
constexpr auto TITLE_SCALE_H = "Масштаб Y";

DirectorView::DirectorView(QWidget *parent) : QWidget(parent)
{
    QLabel *labelScaleW = new QLabel(TITLE_SCALE_W, this);
    QLabel *labelScaleH = new QLabel(TITLE_SCALE_H, this);

    QFont font;
    font.setFamily("monospace");

    labelScaleH->setFont(font);
    labelScaleW->setFont(font);

    sliderScaleHGT = new QSlider(Qt::Orientation::Horizontal, this);
    sliderScaleWDT = new QSlider(Qt::Orientation::Horizontal, this);

    valueSliderHGT = new QDoubleSpinBox(this);
    valueSliderWDT = new QSpinBox(this);

    sliderScaleHGT->setRange(MIN_SLIDER_H, MAX_SLIDER_H);
    sliderScaleWDT->setRange(MIN_SLIDER_W, MAX_SLIDER_W);

    valueSliderHGT->setRange(MIN_SPIN_H, MAX_SPIN_H);
    valueSliderWDT->setRange(MIN_SPIN_W, MAX_SPIN_W);

    valueSliderHGT->setSingleStep(0.1);
    valueSliderWDT->setSingleStep(2);

    valueSliderWDT->setFixedWidth(valueSliderWDT->width());
    valueSliderHGT->setFixedWidth(valueSliderWDT->width());

    sliderScaleHGT->setSingleStep(10);
    sliderScaleWDT->setSingleStep(40);

    QHBoxLayout *l1 = new QHBoxLayout;
    QHBoxLayout *l2 = new QHBoxLayout;

    l1->addWidget(labelScaleW);
    l1->addWidget(sliderScaleWDT);
    l1->addWidget(valueSliderWDT);

    l2->addWidget(labelScaleH);
    l2->addWidget(sliderScaleHGT);
    l2->addWidget(valueSliderHGT);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addLayout(l1);
    mainLayout->addLayout(l2);

    setMaximumHeight(valueSliderWDT->size().height()*2);

    setLayout(mainLayout);

    connect(sliderScaleWDT, SIGNAL(valueChanged(int)), this, SLOT(slotChangeSliderW(int)));
    connect(sliderScaleHGT, SIGNAL(valueChanged(int)), this, SLOT(slotChangeSliderH(int)));

    connect(valueSliderWDT, SIGNAL(valueChanged(int)), this, SLOT(slotChangeSpinW(int)));
    connect(valueSliderHGT, SIGNAL(valueChanged(double)), this, SLOT(slotChangeSpinH(double)));

    valueSliderHGT->setValue(1);
    valueSliderWDT->setValue(1);

}

void DirectorView::slotChangeSliderW(int value)
{
    valueSliderWDT->setValue((double)value / 10);
    emit signalChangeScaleW(valueSliderWDT->value() * 4);
}

void DirectorView::slotChangeSliderH(int value)
{
    valueSliderHGT->setValue((double)value / 10);
    emit signalChangeScaleH(valueSliderHGT->value());
}

void DirectorView::slotChangeSpinW(int value)
{
    sliderScaleWDT->setValue(value * 10);
    emit signalChangeScaleW(value * 4);
}

void DirectorView::slotChangeSpinH(double value)
{
    sliderScaleHGT->setValue(value * 10);
    emit signalChangeScaleH(value);
}

