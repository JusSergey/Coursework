#ifndef DIRECTORVEW_H
#define DIRECTORVEW_H

#include <QtWidgets>

class DirectorView : public QWidget
{
    Q_OBJECT
public:
    explicit DirectorView(QWidget *parent = 0);

private:
    QSlider *sliderScaleWDT;
    QSlider *sliderScaleHGT;

    QSpinBox *valueSliderWDT;
    QDoubleSpinBox *valueSliderHGT;

public:
    inline float getScaleW() const { return valueSliderWDT->value() * 4; }
    inline float getScaleH() const { return valueSliderHGT->value(); }

private slots:
    void slotChangeSliderW(int value);
    void slotChangeSliderH(int value);

    void slotChangeSpinW(int value);
    void slotChangeSpinH(double value);

signals:
    void signalChangeScaleW(float scale);
    void signalChangeScaleH(float scale);
public slots:
};

#endif // DIRECTORVEW_H
