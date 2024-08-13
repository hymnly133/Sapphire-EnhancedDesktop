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
#include "global.h"
#include "mainwindow.h"
#include "sinputdialog.h"
#include "stooltip.h"
#include "qapplication.h"
#include "qpainter.h"
#include "screenfunc.h"

LayerShower::LayerShower(MainWindow *parent, int screenId)
    : QWidget{nullptr}
{
    qDebug() << "Layer Thread" << QThread::currentThread();
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    pmw = parent;
    this->screenId = screenId;
    inplace(this);
    positionToScreen(this, screenId);
    setFixedSize(size() * 2);
    qDebug() << "Layer Shower Information:" << rect() << pos() << geometry() << mapToGlobal(QPoint(0, 0));
    finalPos = pos();
    finalSize = size();
    show();
    setVisible(true);
}

void LayerShower::clearTooltip()
{
    qDebug() << "Cleared";
    QList<SToolTip*> list = this->findChildren<SToolTip*>();
    foreach (auto tem, list) {
        tem->end();
    }
}

void LayerShower::clearInputDialog()
{
    QList<SInputDialog*> listInput = this->findChildren<SInputDialog*>();
    foreach (auto tem, listInput) {
        tem->end();
    }
}

void LayerShower::paintEvent(QPaintEvent *event)
{
    if(onBootAnimation) {
        //启动动画
        QPainter painter(this);
        painter.setBrush(themeColor());
        painter.setPen(Qt::NoPen);
        painter.drawRect(rect());
        return;
    }
    auto tem = themeColor();
    tem.setAlpha(100);
    switch (layer) {
        case Bottom:
            //绘制
            if(activepmw == pmw) {
                if(!pCelectedUnits.empty()) {
                    qDebug() << "celected";
                    auto tem = themeColor();
                    tem.setAlpha(255);
                    qDebug() << "paint";
                    QRegion celectedRegion;
                    foreach (SUnit* unit, pCelectedUnits) {
                        celectedRegion = celectedRegion.united(unit->geometry().adjusted(-10, -10, 10, 10));
                    }
                    QPainter painter(this);
                    painter.setClipRegion(celectedRegion);
                    painter.setBrush(tem);
                    painter.drawRect(rect());
                }
            }
            break;
        //绘制边框
        case Upper:
            //绘制框选
            if(pmw != nullptr)
                if(pmw->celectPointList.size() == 2) {
                    // qDebug()<<"paintrect";
                    QPoint shift = mapFromGlobal(pmw->mapToGlobal(QPoint(0, 0)));
                    QPoint point0 = pmw->celectPointList[0] + shift;
                    QPoint point1 = pmw->celectPointList[1] + shift;
                    QRect aimRect = Point2Rect(point0, point1);
                    //
                    QPainter painter(this);
                    // painter.setClipRect(aimRect);
                    painter.setPen(QColor("green"));
                    painter.setBrush(tem);
                    painter.drawRect(aimRect);
                }
            break;
    }
    //绘制框
#ifdef QT_DEBUG
    // if(pMovingUnit!=nullptr){
    //     // qDebug()<<"Repaint!";
    //     QPainter painter(this);
    //     auto tem  =mapFromGlobal( pMovingUnit->mapToGlobal(QPoint(0,0)));
    //     painter.drawRect(tem.x(),tem.y(),pMovingUnit->width(),pMovingUnit->height());
    // }
#endif
}

void LayerShower::startBootAnimation()
{
    qDebug() << "LayerShower Showing Boot Animation";
    ar = new SAnimationRect(this);
    ar->setTime(1000);
    ar->setStartValue(QPoint(width() / 2, height() / 2), size() / 2, 0, 100);
    ar->setEndValue(QPoint(0, 0), size(), 255, 0);
    connect(ar, &SAnimationRect::animationUpdating, this, &LayerShower::whenBootAnimationUpdation);
    ar->start();
}



bool LayerShower::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if(eventType == QByteArray("windows_generic_MSG")) {
        // qDebug()<<"Called";
        MSG *pMsg = reinterpret_cast<MSG*>(message);
        if(pMsg->message == WM_DEVICECHANGE) {
            // qDebug()<<"Called111";
            updateScreen();
            return true;
        }
    }
    return QWidget::nativeEvent(eventType, message, result);
}

void LayerShower::focusInEvent(QFocusEvent *event)
{
    // pmws[0]->setFocus();
}

void LayerShower::whenBootAnimationUpdation()
{
    qDebug() << "LayerShower Updating Boot Animation";
    move(finalPos + ar->nowPos);
    setFixedSize(ar->nowSize);
    update();
}



