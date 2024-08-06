#include "scolorview.h"
#include "qdebug.h"
#include "qevent.h"
#include "qpainter.h"
#include"QColorDialog"

SColorView::SColorView(QWidget *parent, QColor aim)
    : QWidget{parent}
{
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(60,60);
    now = aim;
}

void SColorView::mousePressEvent(QMouseEvent *event)
{

    QColor color = QColorDialog::getColor(now, this);
    qDebug() << "颜色选择" << color << color.name();
    if(!color.isValid()){
        return;
        //点击 关闭 或 cancel 颜色无效
    } else {
        now = color;
        update();
        event->accept();
        emit colorChanged(now);
    }

}

void SColorView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(now);
    painter.drawRect(rect());
}
