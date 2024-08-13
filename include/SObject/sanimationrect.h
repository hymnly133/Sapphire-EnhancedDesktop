#ifndef SANIMATIONRECT_H
#define SANIMATIONRECT_H

#include "qparallelanimationgroup.h"
#include "qpropertyanimation.h"
#include "roundshower.h"
#include <QWidget>

class SAnimationRect : public QObject
{
    Q_OBJECT
public:
    explicit SAnimationRect(QWidget *parent = nullptr);
    Q_PROPERTY(int nowRadius MEMBER nowRadius NOTIFY nowRadius_changed)
    Q_PROPERTY(QSize nowSize MEMBER nowSize NOTIFY nowSize_changed)
    Q_PROPERTY(int nowAlpha MEMBER nowAlpha NOTIFY nowAlpha_changed)
    Q_PROPERTY(QPoint nowPos MEMBER nowPos NOTIFY nowPos_changed)

public:
    int nowRadius = 0;
    int aimRadius = 0;

    int nowAlpha = 255;
    int aimAlpha;

    QSize nowSize = QSize(0, 0);
    QSize aimSize;

    QPoint nowPos = QPoint(0, 0);
    QPoint aimPos = QPoint(0, 0);

    QPropertyAnimation* radiusAnimation;
    QPropertyAnimation* sizeAnimation;
    QPropertyAnimation* alphaAnimation;
    QPropertyAnimation* posAnimation;

    QParallelAnimationGroup* animations;
    bool onEnd = false;
    void end();


    void setStartValue(QPoint pos, QSize size, int alpha, int radius);

    void setEndValue(QPoint pos, QSize size, int alpha, int radius);
    void setTime(int time);
    void setFinal(bool val = true);
    void start();
    void stop();

signals:
    void nowRadius_changed(int val);
    void nowAlpha_changed(int val);
    void nowSize_changed(QSize val);
    void nowPos_changed(QPoint val);
    void animationUpdating(QPoint, QSize, int, int);
    void finishedFinal();
    void finished();
    void finishedAll();

};

#endif // SANIMATIONRECT_H
