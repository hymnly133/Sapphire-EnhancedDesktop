#include "slineranimation.h"
#include "qdebug.h"
#include "qparallelanimationgroup.h"
#include "qpoint.h"
#include "qsize.h"


#define DoAs(TYPE)\
*((TYPE*)aims[name].value<void *>()) = starts[name].value<TYPE>()+nowProgress*(ends[name].value<TYPE>()-starts[name].value<TYPE>());



SLinerAnimation::SLinerAnimation(QObject *parent)
    : QObject{parent}
{
    progressAnimation = new QPropertyAnimation(this, "nowProgress");
    progressAnimation->setDuration(200);
    progressAnimation->setEasingCurve(QEasingCurve::InOutCubic);


    connect(progressAnimation, &QPropertyAnimation::finished, this, [this]() {
        emit finished();
    });

    connect(this, &SLinerAnimation::nowProgress_Changed, this, &SLinerAnimation::updateVals);
}

// double SLinerAnimation::getProgress()
// {
//     return progress;
// }


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
    progressAnimation->setStartValue(0);
    progressAnimation->setEndValue(1);
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

void SLinerAnimation::clear()
{
    starts.clear();
    aims.clear();
    ends.clear();
}


void SLinerAnimation::updateVals(double)
{
    // qDebug() << nowProgress;
    foreach (QString name, starts.keys()) {
        if(starts[name].isValid()) {
            switch (starts[name].type()) {
                case QVariant::Int:
                    // *aims[name].value<int*>() = starts[name].value<int>() + progress*(ends[name].value<int>() - starts[name].value<int>());
                    DoAs(int);
                    break;
                case QVariant::Double:
                    DoAs(double);
                    break;
                case QVariant::Point:
                    DoAs(QPoint);
                    break;
                case QVariant::Size:
                    DoAs(QSize);
                    break;
                default:
                    break;
            }
        }
    }
    emit animationUpdating();
}


void SLinerAnimation::addVal(QString name, int from, int *dis, int to)
{
    if(from == to) {
        return;
    }
    starts.insert(name, from);
    aims.insert(name, QVariant::fromValue((void *) dis));
    ends.insert(name, to);
}
void SLinerAnimation::addVal(QString name, QPoint from, QPoint *dis, QPoint to)
{
    if(from == to) {
        return;
    }
    starts.insert(name, from);
    aims.insert(name, QVariant::fromValue((void *) dis));
    ends.insert(name, to);
}
void SLinerAnimation::addVal(QString name, double from, double *dis, double to)
{
    if(from == to) {
        return;
    }
    starts.insert(name, from);
    aims.insert(name, QVariant::fromValue((void *) dis));
    ends.insert(name, to);
}
void SLinerAnimation::addVal(QString name, QSize from, QSize *dis, QSize to)
{
    if(from == to) {
        return;
    }
    starts.insert(name, from);
    aims.insert(name, QVariant::fromValue((void *) dis));
    ends.insert(name, to);
}
