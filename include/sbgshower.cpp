#include "sbgshower.h"
#include "mainwindow.h"
#include "qgraphicseffect.h"
#include "qpainter.h"
#include "QDebug"

SBGShower::SBGShower(QWidget *parent)
    : QWidget{parent}
{
    pmw = (MainWindow*)parent;
    QGraphicsBlurEffect* ef = new QGraphicsBlurEffect(this);
    ef->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
    ef->setEnabled(true);
    ef->setBlurRadius(30);
    setGraphicsEffect(ef);
    setWindowTitle("BG_Shower");
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setAttribute(Qt::WA_TranslucentBackground);
}

void SBGShower::paintEvent(QPaintEvent * ev){
    // qDebug()<<"bg_pre_painted"<<pmw->transparent;
    auto tem = updateMask();
    QPainter painter(this);
    painter.setClipRegion(tem);
    if(((MainWindow*)parentWidget())->transparent &&cap){
        painter.drawPixmap(rect(),captrued);
    }
    else{
        painter.drawPixmap(rect(),pmw->bg);
    }
}

QRegion SBGShower::updateMask(){
    QRegion tem = pmw->inside->region;
    // if(pMovingUnit){
    //     auto pos = pMovingUnit->mapToGlobal(QPoint(0,0));
    //     auto geo = pMovingUnit->geometry();
    //      tem = tem.united(QRect(pos.x(),pos.y(),geo.width(),geo.height()));
    // }
    return tem;
}
