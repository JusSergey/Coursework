#include "SettingEffect.h"

SettingEffect::SettingEffect(const std::initializer_list<QString> &_lst, QWidget *parent)
    : QWidget(parent)
{

    QVBoxLayout *vbl = new QVBoxLayout(this);

    for (const QString &str : _lst) {
        Container tmp;
        tmp.label  = new QLabel(str, this);
        tmp.slider = new MSlider(Qt::Horizontal, this);
        tmp.spinBox= new MDoubleSpinBox(this);

        tmp.swapChanders();

        tmp.slider->setRange(1, 100);
        tmp.spinBox->setRange(0.01, 1.0);

        tmp.slider->setValue(tmp.slider->maximum());
        tmp.spinBox->setValue(tmp.spinBox->maximum());

        tmp.slider->setSingleStep(1);
        tmp.spinBox->setSingleStep(0.1);

        _pairs[str] = tmp;

        connectContainer(tmp);

        QHBoxLayout *hbl = new QHBoxLayout(this);
        hbl->addWidget(tmp.label);
        hbl->addWidget(tmp.slider);
        hbl->addWidget(tmp.spinBox);

        vbl->addLayout(hbl);
    }

    // init buttons
    Ok = new QPushButton("&Ok", this);
    Cancel = new QPushButton("&Cancel", this);

    QHBoxLayout *lButtons = new QHBoxLayout(this);
    lButtons->addWidget(Ok);
    lButtons->addWidget(Cancel);

    initClickedButtons();

    ckeckAllFile = new QCheckBox("Весь файл", this);

    vbl->addWidget(ckeckAllFile);
    vbl->addLayout(lButtons);

    setLayout(vbl);

    setWindowTitle("Налаштування");

    setFixedWidth(250);
}

SettingEffect::~SettingEffect()
{
    qDebug() << "SettinEffect destructor";
}

void SettingEffect::connectContainer(SettingEffect::Container &c)
{
    connect(c.slider,  SIGNAL(valueChanged(int)),    this, SLOT(slotChangeSlider(int)));
    connect(c.spinBox, SIGNAL(valueChanged(double)), this, SLOT(slotChangeSpinBox(double)));
}

void SettingEffect::initClickedButtons()
{
    connect(Cancel, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(Ok, SIGNAL(clicked(bool)), this, SLOT(slotClickedButtonOk()));
}

void SettingEffect::slotChangeSpinBox(double value)
{
    ((MDoubleSpinBox*)sender())->changer->setValue(value * 100);
}

void SettingEffect::slotChangeSlider(int value)
{
    ((MSlider*)sender())->changer->setValue((double)value / 100);
}

void SettingEffect::slotClickedButtonOk()
{
    std::map<QString, float> result;

    for (const auto &value : _pairs)
        result[value.first] = value.second.spinBox->value();

    emit signalClickedButtonOk(ckeckAllFile->isChecked(), result);

    close();
}
