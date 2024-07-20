#ifndef REPAINTCOUNTERUNIT_H
#define REPAINTCOUNTERUNIT_H

#include "ed_unit.h"
#include <QWidget>

class RepaintCounterUnit : public ED_Unit
{
    Q_OBJECT
public:
    explicit RepaintCounterUnit(QWidget *parent = nullptr);
    int repainttime=0;

signals:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

Q_DECLARE_METATYPE(RepaintCounterUnit);

#endif // REPAINTCOUNTERUNIT_H
