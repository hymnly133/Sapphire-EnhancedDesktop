#include "repaintcounterunit.h"
#include"ed_unit.h"
#include "qpainter.h"

RepaintCounterUnit::RepaintCounterUnit(QWidget *parent)
    : ED_Unit{parent,1,1}
{

}

void RepaintCounterUnit::paintEvent(QPaintEvent *event)
{
    ED_Unit::paintEvent(event);
    QPainter painter(this);
    painter.drawText(rect(), Qt::AlignHCenter, QString::number(repainttime));
    repainttime+=1;
}
