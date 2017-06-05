#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QtWidgets>
#include <QtMultimedia>
#include "AudioUtilityes.h"
#include "Effects.h"

class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWidget(QWidget *parent = 0);
    void setAudioBufferList(const AudioBuffersSimple &abuff);
    void resetBuffer();
    void refresh();

    void setScaleW(float scale) { scaleX = scale; refresh(); }
    void setScaleH(float scale) { scaleY = scale; refresh(); }

    const AudioBuffersSimple &getBufferConst() const { return audioBuffer; }
    AudioBuffersSimple &getBuffer() { return audioBuffer; }

    real getStartSelect() const;
    real getEndSelect() const;
    
protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    AudioBuffersSimple audioBuffer;
    QList<QLine> drawLines;

    float scaleX, scaleY;
    int offsetx;
    int startOffsetX;

    int selectIn, selectTo;

    QPoint startPress;
    Qt::MouseButton clickButton;

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

};

#endif // DRAWWIDGET_H
