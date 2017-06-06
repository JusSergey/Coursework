#ifndef SETTINGEFFECT_H
#define SETTINGEFFECT_H

#include <QtWidgets>
#include <map>

class SettingEffect : public QWidget
{
    Q_OBJECT
public:
    explicit SettingEffect(const std::initializer_list<QString> &_lst, QWidget *parent = 0);
   ~SettingEffect();

private:
    struct MSlider : public QSlider {
        MSlider(Qt::Orientation o, QWidget *parent) :
            QSlider(o, parent),
            changer(nullptr)
        {}
            QDoubleSpinBox *changer;
    };

    struct MDoubleSpinBox : public QDoubleSpinBox {
        MDoubleSpinBox(QWidget *parent) :
            QDoubleSpinBox(parent),
            changer(nullptr)
        {}
        QSlider *changer;
    };

    struct Container
    {
        MSlider* slider = nullptr;
        MDoubleSpinBox* spinBox = nullptr;
        QLabel* label = nullptr;

        Container &operator = (const Container &) = default;

        void swapChanders() {
            if (!slider || !spinBox) {
                qDebug() << "!slider || !spinBox : TRUE";
                QApplication::exit(-1);
            }
            slider->changer = spinBox;
            spinBox->changer = slider;
        }
    };

private:
    std::map<QString, Container> _pairs;
    QPushButton *Ok, *Cancel;

    QCheckBox *ckeckAllFile;

private:
    void connectContainer(Container &c);
    void initClickedButtons();

private slots:
    void slotChangeSpinBox(double value);
    void slotChangeSlider(int value);
    void slotClickedButtonOk();

signals:
    void signalClickedButtonOk(bool isAllFile, const std::map<QString, float> &c);

public slots:
};

#endif // SETTINGEFFECT_H
