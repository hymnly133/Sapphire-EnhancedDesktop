#include "slineranimation.h"
#include "qparallelanimationgroup.h"
#include "qpoint.h"
#include "qsize.h"

#include<experimental/any>
#define docheck(TYPE)\
if(froms[i].type() == typeid(TYPE)){\
        **any_cast<TYPE*>(dises[i]) = (any_cast<TYPE>(froms[i]))+((any_cast<TYPE>(tos[i]))-(any_cast<TYPE>(froms[i])))*pro;\
continue;}

using namespace  std::experimental;
SLinerAnimation::SLinerAnimation(QObject *parent)
    : QObject{parent}
{
    progressAnimation = new QPropertyAnimation(this,"progress");
    progressAnimation->setDuration(200);
    progressAnimation->setEasingCurve(QEasingCurve::InOutCubic);


    connect(progressAnimation,&QPropertyAnimation::finished,this,[this](){
        if(final){
            emit finalFinished();
        }
        emit finished();
    });
}

double SLinerAnimation::getProgress() const
{
    return progress;
}

void SLinerAnimation::setFinal(bool val)
{
    final = val;
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

