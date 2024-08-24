#include "SAnimationRect.h"
#include"qparallelanimationgroup.h"
#include"qpropertyanimation.h"

#define SET_ANIMATION(ANIMATION,NAME,TIME)\
ANIMATION = new QPropertyAnimation(this,#NAME);\
    ANIMATION->setDuration(TIME);\
    ANIMATION->setEasingCurve(QEasingCurve::InOutQuad);\
    animations->addAnimation(ANIMATION);

SAnimationRect::SAnimationRect(QWidget *parent)
    : QObject{parent}
{
    // animations = new QParallelAnimationGroup(this);
    // int animationTime = 200;
    // SET_ANIMATION(radiusAnimation, nowRadius, animationTime);
    // SET_ANIMATION(alphaAnimation, nowAlpha, animationTime);
    // SET_ANIMATION(sizeAnimation, nowSize, animationTime);
    // SET_ANIMATION(posAnimation, nowPos, animationTime);
    // connect(this, &SAnimationRect::nowAlpha_changed, this, [ = ](int val) {
    //     emit animationUpdating(nowPos, nowSize, nowAlpha, nowRadius);
    // });
    // connect(this, &SAnimationRect::nowRadius_changed, this, [ = ](int val) {
    //     emit animationUpdating(nowPos, nowSize, nowAlpha, nowRadius);
    // });
    // connect(this, &SAnimationRect::nowSize_changed, this, [ = ](QSize val) {
    //     emit animationUpdating(nowPos, nowSize, nowAlpha, nowRadius);
    // });
    // connect(this, &SAnimationRect::nowPos_changed, this, [ = ](QPoint val) {
    //     emit animationUpdating(nowPos, nowSize, nowAlpha, nowRadius);
    // });
    ani = new SLinerAnimation(this);
    connect(ani, &SLinerAnimation::finished, this, [this]() {
        if(onEnd) {
            emit finishedFinal();
        } else {
            emit finished();
        }
        emit finishedAll();
    });
    connect(ani, &SLinerAnimation::animationUpdating, this, &SAnimationRect::animationUpdating);
}



void SAnimationRect::setStartValue(QPoint pos, QSize size, int alpha, int radius, double ratio)
{
    nowPos = pos;
    nowSize = size;
    nowRadius = radius;
    nowAlpha = alpha;
    nowRatio = ratio;
}

void SAnimationRect::setTime(int time)
{
    // alphaAnimation->setDuration(time);
    // posAnimation->setDuration(time);
    // radiusAnimation->setDuration(time);
    // sizeAnimation->setDuration(time);
    ani->setTime(time);
}

void SAnimationRect::setEndValue(QPoint pos, QSize size, int alpha, int radius, double ratio)
{
    aimPos = pos;
    aimSize = size;
    aimRadius = radius;
    aimAlpha = alpha;
    aimRatio = ratio;
    // posAnimation->setEndValue(pos);
    // sizeAnimation->setEndValue(size);
    // alphaAnimation->setEndValue(alpha);
    // radiusAnimation->setEndValue(radius);
}

void SAnimationRect::setFinal(bool val)
{
    onEnd = val;
}

void SAnimationRect::start()
{
    // posAnimation->setStartValue(nowPos);
    // sizeAnimation->setStartValue(nowSize);
    // alphaAnimation->setStartValue(nowAlpha);
    // radiusAnimation->setStartValue(nowRadius);
    // animations->start();
    ani->stop();
    ani->clear();
    ani->addVal("pos", nowPos, &nowPos, aimPos);
    ani->addVal("size", nowSize, &nowSize, aimSize);
    ani->addVal("radius", nowRadius, &nowRadius, aimRadius);
    ani->addVal("alpha", nowAlpha, &nowAlpha, aimAlpha);
    ani->addVal("ratio", nowRatio, &nowRatio, aimRatio);
    ani->start();
}

void SAnimationRect::stop()
{
    // animations->stop();
}



