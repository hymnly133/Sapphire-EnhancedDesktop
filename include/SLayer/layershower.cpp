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
#include "mainwindow.h"
#include "sinputdialog.h"
#include "stooltip.h"
#include "qapplication.h"
#include "qpainter.h"
#include "screenfunc.h"

LayerShower::LayerShower(MainWindow *parent,int screenId)
    : QWidget{nullptr}
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint );
    setAttribute(Qt::WA_TransparentForMouseEvents);

    pmw = parent;
    this->screenId = screenId;
    inplace(this);
    positionToScreen(this,screenId);

    qDebug()<<"Layer Shower Information:"<<rect()<<pos()<<geometry()<<mapToGlobal(QPoint(0,0)) ;
    show();
    setVisible(true);
}

void LayerShower::clearTooltip()
{
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
    // auto tem = QColor("green");
    // tem.setAlpha(100);
    // paintRect(this,tem);
    // qDebug()<<"ShowerRepaint";
    auto tem = winThemeColor();
    tem.setAlpha(100);


    switch (layer) {
    case Bottom:
        //绘制
        if(activepmw == pmw){
            if(!pCelectedUnits.empty()){
                qDebug()<<"celected";
                auto tem = winThemeColor();
                tem.setAlpha(255);
                qDebug()<<"paint";
                QRegion celectedRegion;
                foreach (SUnit* unit, pCelectedUnits) {
                    celectedRegion = celectedRegion.united(unit->geometry().adjusted(-10,-10,10,10));
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
        if(pmw!=nullptr)
            if(pmw->celectPointList.size()==2){
                // qDebug()<<"paintrect";
                QPoint point0 = pmw->celectPointList[0];
                QPoint point1 = pmw->celectPointList[1];
                QRect aimRect = Point2Rect(point0,point1);

                QPainter painter(this);
                painter.setClipRect(aimRect);
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

bool LayerShower::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if(eventType == QByteArray("windows_generic_MSG"))
    {
        // qDebug()<<"Called";
        MSG *pMsg = reinterpret_cast<MSG*>(message);
        if(pMsg->message == WM_DEVICECHANGE)
        {
            // qDebug()<<"Called111";
            updateScreen();
            return true;
        }
    }
    return false;
}

void LayerShower::focusInEvent(QFocusEvent *event)
{
    // pmws[0]->setFocus();
}



