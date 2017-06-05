#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QtWidgets>
#include <QtMultimedia>
#include "PCMAudioDevice.h"
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
    void playing();

    void setScaleW(float scale) { scaleX = scale; refresh(); }
    void setScaleH(float scale) { scaleY = scale; refresh(); }

    const AudioBuffersSimple &getBufferConst() const { return audioBuffer; }
    AudioBuffersSimple &getBuffer() { return audioBuffer; }

    real getStartSelect() const;
    real getEndSelect() const;
    
protected:
    virtual void paintEvent(QPaintEvent *event);
//    virtual void keyPressEvent(QKeyEvent *event);
//    void keyPressEvent(QKeyEvent *event);

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
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

};

#endif // DRAWWIDGET_H
