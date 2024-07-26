#include "slineranimation.h"
#include "qparallelanimationgroup.h"

SLinerAnimation::SLinerAnimation(QObject *parent)
    : QObject{parent}
{
    progressAnimation = new QPropertyAnimation(this);
    progressAnimation->setDuration(200);
    progressAnimation->setEasingCurve(QEasingCurve::InOutCubic);

    connect(progressAnimation,&QPropertyAnimation::finished,this,[this](){
        if(onEnd){
            emit finalFinished();
        }
    });

}

double SLinerAnimation::getProgress() const
{
    return progress;
}

void SLinerAnimation::setEasingCurve(QEasingCurve curve)
{
    progressAnimation->setEasingCurve(curve);
}

void SLinerAnimation::setTime(int time)
{
    progressAnimation->setDuration(time);
}

void SLinerAnimation::start()
{
    progressAnimation->stop();
    progressAnimation->setStartValue(startVal);
    progressAnimation->setEndValue(endVal);
    progressAnimation->start();

}

void SLinerAnimation::stop()
{
    progressAnimation->stop();
}

void SLinerAnimation::setStartValue(double val)
{
    startVal = val;
}

void SLinerAnimation::setEndValue(double val)
{
    endVal = val;
}
