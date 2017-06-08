#include "DrawWidget.h"

DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent),
    offsetx(0),
    scaleX(1),
    scaleY(1),
    selectIn(0),
    selectTo(0)
{
    installEventFilter(this);
}

void DrawWidget::setAudioBufferList(const AudioBuffersSimple &abuff)
{
    audioBuffer = abuff;
    refresh();
}

void DrawWidget::resetBuffer()
{
    audioBuffer.clear();
}


void DrawWidget::refresh()
{
    repaint();
}


real DrawWidget::getStartSelect() const
{
    const QAudioFormat &f1= audioBuffer.front().format();
    const real step1 = (f1.bytesPerFrame() / f1.channelCount() + scaleX) * f1.channelCount();

    real a = real(selectIn - offsetx) * step1 * (scaleX / 4);

    const QAudioFormat &f2 = audioBuffer.front().format();
    const real step2 = (f2.bytesPerFrame() / f2.channelCount() + scaleX) * f2.channelCount();

    real b = real(selectTo - offsetx) * step2 * (scaleX / 4);

    return std::min(a, b);
}

real DrawWidget::getEndSelect() const
{

    const QAudioFormat &f1= audioBuffer.front().format();
    const real step1 = (f1.bytesPerFrame() / f1.channelCount() + scaleX) * f1.channelCount();

    real a = real(selectIn - offsetx) * step1 * (scaleX / 4);

    const QAudioFormat &f2 = audioBuffer.front().format();
    const real step2 = (f2.bytesPerFrame() / f2.channelCount() + scaleX) * f2.channelCount();

    real b = real(selectTo - offsetx) * step2 * (scaleX / 4);

    return std::max(a, b);
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    int positionX = 0;

    if (audioBuffer.isEmpty())
        return;

    const QAudioBuffer &f = audioBuffer.front();
    const size_t step = f.format().bytesPerFrame() / f.format().channelCount();

    QPainter painter(this);

    if (selectIn != selectTo) {
        painter.setPen(0xAAAAFF);
        for (int i = std::min(selectIn, selectTo); i <= std::max(selectIn, selectTo); ++i)
            painter.drawLine(QPoint(i, 0), QPoint(i, height()));
    }

    const int offsety[] = {height() * 0.25, height() * 0.75};

    painter.setPen(Qt::red);

    for (int i = 0; i < 2; i++)
        painter.drawLine(QPoint(0, offsety[i]), QPoint(width(), offsety[i]));

    painter.setPen(Qt::gray);

    painter.drawLine(QPoint(0, height() / 2), QPoint(width(), height() / 2));

    painter.setPen(Qt::black);

    const int ScaleWidgetY = (1 << 16) / height() * 2;

    const int StartPosDraw = -scaleX*offsetx;

    const int EndPosDraw = width()*scaleX - offsetx*scaleX;

    QPoint previous[2], current[2];

    int counter = 0;

    for (QAudioBuffer &buffer : audioBuffer) {

        for (int i = 0; i < buffer.byteCount(); i += step + scaleX) {

            const int CurrentPosX = (positionX >> 1);

            if (CurrentPosX > EndPosDraw)
                return;

            if (CurrentPosX > StartPosDraw) {

                const char id = (i/step)&1;
                current[id].setX(CurrentPosX/scaleX + offsetx);
                current[id].setY(-*((const short int*)(buffer.constData() + i)) / (ScaleWidgetY / scaleY) + offsety[id]);

                if (!previous[id].isNull()) {
                    painter.drawLine(previous[id], current[id]);
                    counter++;
                }
                previous[id] = current[id];
            }
            ++positionX;
        }

    }
}

void DrawWidget::mousePressEvent(QMouseEvent *event)
{
    setFocus();
    clickButton = event->button();

    if (event->button() == Qt::LeftButton) {
        startOffsetX = event->pos().x();
    }

    else {
        selectTo = selectIn = event->pos().x();
    }

    refresh();
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (clickButton == Qt::RightButton) {
        selectTo = event->pos().x();
    }

    if (clickButton == Qt::LeftButton) {
        offsetx += event->pos().x() - startOffsetX;
        startOffsetX = event->pos().x();
    }

    refresh();
}

void DrawWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        selectTo = event->pos().x();
}
