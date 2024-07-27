#include "roundshower.h"
#include "qpainter.h"
#include "qpainterpath.h"

roundShower::roundShower(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(parent->size());
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setMinimumSize(0,0);

}

void roundShower::distri(QSize* sizedis,int* radiusdis)
{
    follow = false;
    pSize = sizedis;
    pRadius = radiusdis;
}

void roundShower::distriRadius(int *radiusdis)
{
    pRadius = radiusdis;
}



void roundShower::updateDisplay()
{
    if(follow){
        setFixedSize(parentWidget()->size());
    }

}


void roundShower::paintEvent(QPaintEvent *event)
{
    // 在改图片上填充一个圆角区域，需要设置抗锯齿
    QPixmap tem(size());
    tem.fill(QColor(0,0,0,0));
    QPainter painter(&tem);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;

    //这里圆角区域需要根据dpi、size调整
    QRectF aimrect;
    if(follow)
        aimrect = QRectF(0,0,aim_size().width(),aim_size().height()), aim_radius(), aim_radius();
    else{
        switch(aliment){

        case Center:
            aimrect=QRectF((parentWidget()->width()-aim_size().width())/2,(parentWidget()->height()-aim_size().height())/2,aim_size().width(),aim_size().height()), aim_radius(), aim_radius();
            break;
        case Default:
            aimrect=QRectF(0,0,aim_size().width(),aim_size().height()), aim_radius(), aim_radius();
            break;
        }
    }
    path.addRoundedRect(aimrect,aim_radius(),aim_radius());
    painter.fillPath(path, Qt::white);
    painter.end();
    // 在窗口上绘制该圆角图片
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // 该混合模式会根据source像素的透明度，调整目标的透明度
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.drawPixmap(0, 0, tem);

}
