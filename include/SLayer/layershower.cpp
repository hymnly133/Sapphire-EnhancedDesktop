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
#include "qbitmap.h"
#include "sinputdialog.h"
#include "stooltip.h"
#include "qapplication.h"
#include "qpainter.h"
#include "screenfunc.h"
#include"guifunc.h"
#include"stylehelper.h"
LayerShower::LayerShower(MainWindow *parent, int screenId)
    : QWidget{nullptr}
{
    qDebug() << "Layer Thread" << QThread::currentThread();
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    pmw = parent;
    this->screenInd = screenId;
    inplace(this);
    positionToScreen(this, screenId);

    qDebug() << "Layer Shower Information:" << rect() << pos() << geometry() << mapToGlobal(QPoint(0, 0));
    finalPos = pos();
    finalSize = size();
    QImage im(":/appIcon/Sapphire.png");
    QPixmap iconMapp = QPixmap::fromImage(im);
    iconMapp = iconMapp.scaled(pmw->size() / 4, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    iconMap = new QPixmap(iconMapp);
    show();
    setVisible(true);


    connectTo(always_fill_screen, int, int, {
        updateSize();
    });
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

    if(state != normal) {
        //启动动画
        QPainter painter(this);
        painter.setBrush(applyAlpha(themeColor(), ar->nowAlpha));
        painter.setPen(Qt::NoPen);
        painter.drawRect(QRect(ar->nowPos, ar->nowSize));


        //图片路径可以通过右击工程的图片获取

        // QPixmap alphaChannel = pixmap.createMaskFromColor(QColor(ar->nowAlpha, ar->nowAlpha, ar->nowAlpha), Qt::MaskInColor);
        // pixmap.setMask(alphaChannel);



        painter.drawPixmap(mapFromGlobal(pmw->mapToGlobal(pmw->rect().center())) - QPoint(iconMap->width() / 2, iconMap->height() / 2), applyUntransparentRatio(*iconMap, ar->nowAlpha * 1.0 / 255));

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

void LayerShower::startBootAnimationIn()
{
    qDebug() << "LayerShower star tBoot Animation In";

    ar = new SAnimationRect(this);
    connect(ar, &SAnimationRect::animationUpdating, this, &LayerShower::whenBootAnimationUpdation);

    ar->setTime(500);
    ar->setStartValue(QPoint(0, 0), size(), 0, 0);
    ar->setEndValue(QPoint(0, 0), size(), 255, 0);

    //绑定In事件
    connect(ar, &SAnimationRect::finished, this, [ = ]() {
        emit bootAnimationInEnd();
        state = bootOn;
        //解绑In事件
        disconnect(ar, &SAnimationRect::finished, this, nullptr);
    });
    ar->start();
    emit bootAnimationInStart();
}

void LayerShower::startBootAnimationOut()
{
    state = bootOut;

    //绑定Out事件
    connect(ar, &SAnimationRect::finishedFinal, this,  [ = ]() {
        emit bootAnimationOutEnd();
        state = normal;
        //解绑Out事件
        disconnect(ar, &SAnimationRect::finished, this, nullptr);
    });
    ar->setFinal();
    ar->setEndValue(QPoint(0, 0), size(), 0, 0);
    ar->start();
    emit bootAnimationOutStart();
}

void LayerShower::whenBootAnimationUpdation()
{
    // qDebug() << "LayerShower Updating Boot Animation";
    update();
}

void LayerShower::updateSize()
{
    QSize aim = pscs[screenInd]->availableSize();
    if(!MultiScreen) {
        aim = pscs[screenInd]->size();
    }

    if(MultiScreen) {
        setFixedSize(aim * 2.5);
    } else {
        setFixedSize(aim);
    }
    update();

}



