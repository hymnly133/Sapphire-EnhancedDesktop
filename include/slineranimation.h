#ifndef SLINERANIMATION_H
#define SLINERANIMATION_H

#include "qpropertyanimation.h"
#include <QObject>

//单向的线性动画
class SLinerAnimation : public QObject
{
    Q_OBJECT
public:
    explicit SLinerAnimation(QObject *parent = nullptr);
    double progress =0;
    double getProgress() const;
    double startVal = 0;
    double endVal = 1;

    QPropertyAnimation* progressAnimation;
    void setEasingCurve(QEasingCurve curve);
    bool onEnd = false;
    void setFinal(bool val = true);
    void setTime(int time);
    void start();
    void stop();
    void setStartValue(double val);
    void setEndValue(double val);
signals:
    void progressChanged();
    void finalFinished();
private:
    Q_PROPERTY(double progress READ getProgress NOTIFY progressChanged)
};

#endif // SLINERANIMATION_H
