#ifndef SLINERANIMATION_H
#define SLINERANIMATION_H

#include "qpropertyanimation.h"
#include <QObject>
//单向的线性动画
class SLinerAnimation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double nowProgress MEMBER nowProgress NOTIFY nowProgress_Changed)
public:
    explicit SLinerAnimation(QObject *parent = nullptr);
    double nowProgress = 0.0;
    double startVal = 0;
    double endVal = 1;
    bool final;

    void addVal(QString name, int from, int *dis, int to);
    void addVal(QString name, double from, double *dis, double to);
    void addVal(QString name, QSize from, QSize *dis, QSize to);
    void addVal(QString name, QPoint from, QPoint *dis, QPoint to);



    QMap<QString, QVariant> starts;
    QMap<QString, QVariant> aims;
    QMap<QString, QVariant> ends;

    QPropertyAnimation* progressAnimation;
    void setTime(int time);
    void start();
    void stop();
    void setEasingCurve(QEasingCurve curve);
    void setStartValue(double val);
    void setEndValue(double val);
    void clear();

signals:
    void nowProgress_Changed(double);
    void finished();
    void animationUpdating();

public slots:
    void updateVals(double);

};

#endif // SLINERANIMATION_H
