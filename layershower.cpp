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
#include "screenfunc.h"

LayerShower::LayerShower(QWidget *parent)
    : QWidget{parent}
{
    // setWindowState(Qt::WindowFullScreen);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setWindowFlags(Qt::FramelessWindowHint);
    inplace((QWidget* )this);
    qDebug()<<"DesktopSize"<<pdt->size();
    // qDebug()<<1;
    setFixedSize(pdt->size()*2);
    // qDebug()<<2;
    // qDebug()<<3;
    show();
        // qDebug()<<4;
    setVisible(true);
        // qDebug()<<5;
    move(0,0);
    // qDebug()<<6;
        // setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    // setGeometry(pdt->geometry())
    qDebug()<<"Layer Shower Information:"<<rect()<<pos()<<geometry()<<mapToGlobal(QPoint(0,0)) ;
    move(2*pos()-geometry().topLeft());
    qDebug()<<"Layer Shower Information Fixed:"<<rect()<<pos()<<geometry()<<mapToGlobal(QPoint(0,0)) ;

}

void LayerShower::paintEvent(QPaintEvent *event)
{
    foreach(MainWindow* pmw,pmws){
        QList<QPoint>& drawParamList = pmw->drawParamList;

        if (drawParamList.count() >= 2) {
            QPainter painter(this);
            QColor tem = GetWindowsThemeColor();
            QPoint shift =mapFromGlobal( pmw->mapToGlobal(QPoint(0,0)));
            qDebug()<<"shift"<<shift;
            QPoint fixPoint0 = drawParamList[0]+shift;
            QPoint fixPoint1 = drawParamList[1]+shift;
            qDebug()<<"Point0"<<fixPoint0;
            qDebug()<<"Point1"<<fixPoint1;
            tem.setAlpha(200);
            painter.setBrush(tem);
            int x = (fixPoint0.x() < fixPoint1.x()) ? fixPoint0.x() : fixPoint1.x();
            int y = (fixPoint0.y() < fixPoint1.y()) ? fixPoint0.y() : fixPoint1.y();
            int w = qAbs(fixPoint0.x() - fixPoint1.x()) + 1;
            int h = qAbs(fixPoint0.y() - fixPoint1.y()) + 1;
            painter.drawRect(x, y, w, h);  // 画长方形
        }
    }


#ifdef QT_DEBUG

    if(pMovingUnit!=nullptr){
        // qDebug()<<"Repaint!";
        QPainter painter(this);
        auto tem  =mapFromGlobal( pMovingUnit->mapToGlobal(QPoint(0,0)));
        painter.drawRect(tem.x(),tem.y(),pMovingUnit->width(),pMovingUnit->height());
    }

#endif

}



