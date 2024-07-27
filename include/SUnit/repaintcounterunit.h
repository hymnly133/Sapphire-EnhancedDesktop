#ifndef REPAINTCOUNTERUNIT_H
#define REPAINTCOUNTERUNIT_H

#include "sunit.h"
#include <QWidget>

class RepaintCounterUnit : public SUnit
{
    Q_OBJECT
public:
    explicit RepaintCounterUnit(SLayout* layout = nullptr,int sizex = 1,int sizey = 1);
    int repainttime=0;

signals:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

Q_DECLARE_METATYPE(RepaintCounterUnit);

#endif // REPAINTCOUNTERUNIT_H
