#include "repaintcounterunit.h"
#include"sunit.h"
#include "qpainter.h"

RepaintCounterUnit::RepaintCounterUnit(SLayout *layout, int sizex, int sizey)
    : SUnit{layout,sizex,sizey}
{

}

void RepaintCounterUnit::paintEvent(QPaintEvent *event)
{
    SUnit::paintEvent(event);
    QPainter painter(this);
    painter.drawText(rect(), Qt::AlignHCenter, QString::number(repainttime));
    repainttime+=1;
}
