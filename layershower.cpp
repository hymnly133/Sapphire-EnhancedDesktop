#include "layershower.h"
#include "SysFunctions.h"
#include <QMouseEvent>
#include <QDebug>
#include <QWidget>
#include <QFileDialog>
#include <qtimer.h>
#include "QScreen"
#include "QThread"
#include"QStyle"
#include "qapplication.h"
#include "qpainter.h"

LayerShower::LayerShower(QWidget *parent)
    : QWidget{parent}
{
    setWindowState(Qt::WindowFullScreen);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setWindowFlags(Qt::FramelessWindowHint);
    inplace((QWidget* )this);
    setFixedSize(QGuiApplication::primaryScreen()->availableSize());
    move(0,0);
    qDebug()<<rect()<<pos()<<geometry()<<mapToGlobal(QPoint(0,0)) ;

    move(-geometry().x(),-geometry().y());

    pls = this;
}

void LayerShower::paintEvent(QPaintEvent *event)
{
    qDebug()<<"111`1";
    QList<QPoint>& drawParamList = pmw->drawParamList;

    if (drawParamList.count() >= 2) {
        QPainter painter(this);
        QColor tem = GetWindowsThemeColor();
        tem.setAlpha(200);
        painter.setBrush(tem);
        int x = (drawParamList[0].x() < drawParamList[1].x()) ? drawParamList[0].x() : drawParamList[1].x();
        int y = (drawParamList[0].y() < drawParamList[1].y()) ? drawParamList[0].y() : drawParamList[1].y();
        int w = qAbs(drawParamList[0].x() - drawParamList[1].x()) + 1;
        int h = qAbs(drawParamList[0].y() - drawParamList[1].y()) + 1;
        painter.drawRect(x, y, w, h);  // 画长方形
    }
}
