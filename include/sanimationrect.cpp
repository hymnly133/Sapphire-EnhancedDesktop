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

    animations = new QParallelAnimationGroup(this);

    int animationTime =200;
    SET_ANIMATION(radiusAnimation,nowRadius,animationTime);
    SET_ANIMATION(alphaAnimation,nowAlpha,animationTime);
    SET_ANIMATION(sizeAnimation,nowSize,animationTime);
    SET_ANIMATION(posAnimation,nowPos,animationTime);


    connect(this,&SAnimationRect::nowAlpha_changed,this,[=](int val){
        emit animationUpdating(nowPos,nowSize,nowAlpha,nowRadius);
    });
    connect(this,&SAnimationRect::nowRadius_changed,this,[=](int val){
        emit animationUpdating(nowPos,nowSize,nowAlpha,nowRadius);
    });
    connect(this,&SAnimationRect::nowSize_changed,this,[=](QSize val){
        emit animationUpdating(nowPos,nowSize,nowAlpha,nowRadius);
    });
    connect(this,&SAnimationRect::nowPos_changed,this,[=](QPoint val){
        emit animationUpdating(nowPos,nowSize,nowAlpha,nowRadius);
    });

    connect(animations,&QParallelAnimationGroup::finished,this,[this](){
        if(onEnd){
            emit whenEndAnimationEnd();
        }
    });
}



void SAnimationRect::setStartValue(QPoint pos, QSize size, int alpha,int radius)
{
    nowPos = pos;
    nowSize = size;
    nowRadius = radius;
    nowAlpha = alpha;

}

void SAnimationRect::setTime(int time)
{
    alphaAnimation->setDuration(time);
    posAnimation->setDuration(time);
    radiusAnimation->setDuration(time);
    sizeAnimation->setDuration(time);
}

void SAnimationRect::setEndValue(QPoint pos, QSize size, int alpha, int radius)
{
    aimPos = pos;
    aimSize = size;
    aimRadius = radius;
    aimAlpha = alpha;

    posAnimation->setEndValue(pos);
    sizeAnimation->setEndValue(size);
    alphaAnimation->setEndValue(alpha);
    radiusAnimation->setEndValue(radius);

}

void SAnimationRect::setFinal(bool val)
{
    onEnd = val;
}

void SAnimationRect::start()
{
    posAnimation->setStartValue(nowPos);
    sizeAnimation->setStartValue(nowSize);
    alphaAnimation->setStartValue(nowAlpha);
    radiusAnimation->setStartValue(nowRadius);
    animations->start();
}

void SAnimationRect::stop()
{
    animations->stop();
}



