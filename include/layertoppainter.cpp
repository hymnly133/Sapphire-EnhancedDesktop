#include "layertoppainter.h"
#include "qpainter.h"
#include "qscreen.h"
#include "screenfunc.h"



LayerTopPainter::LayerTopPainter(MainWindow *parent, int screenInd):LayerShower(parent,screenInd)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    // this->setWindowFlags(Qt::WindowStaysOnTopHint);
}

void LayerTopPainter::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    paint.setPen(Qt::transparent);
    QColor tem("red");
    tem.setAlpha(10);
    paint.setBrush(tem);
    paint.drawRect(rect());
}
