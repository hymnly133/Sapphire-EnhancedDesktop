#include "roundshower.h"
#include "qpainter.h"
#include "qpainterpath.h"
#include "style.h"

roundShower::roundShower(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(parentWidget()->size());
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void roundShower::paintEvent(QPaintEvent *event)
{
    //创建一个图片，填充透明色
    QPixmap pix(parentWidget()->size());
    pix.fill(QColor(0,0,0,0));
    // 在改图片上填充一个圆角区域，需要设置抗锯齿
    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    //这里圆角区域需要根据dpi、size调整
    path.addRoundedRect(QRectF(parentWidget()->rect()).adjusted(0.5, 0.5, -0.5, -0.5), unit_radius, unit_radius);
    painter.fillPath(path, Qt::white);
    painter.end();
    // 在窗口上绘制该圆角图片
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // 该混合模式会根据source像素的透明度，调整目标的透明度
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.drawPixmap(0, 0, pix);
}
