#include "smenu.h"
#include "SQSS.h"
#include "qevent.h"
#include "qgraphicseffect.h"
#include "qmath.h"
#include "qpainter.h"
#include "qpainterpath.h"
namespace
{
const int SHADOW_WIDTH = 5;
const QString QSS = "QMenu{background-color:transparent;margin:7px;border-radius:20px;}";
}

SMenu::SMenu(QWidget *parent):QMenu(parent)
{
    rs = new roundShower(this);
    arect = new SAnimationRect(this);
    rs->distriRadius(&arect->nowRadius);
    rs->raise();
    arect->setTime(150);

    connect(arect,&SAnimationRect::whenEndAnimationEnd,this,[=]{
        // setVisible(false);
        // arect->setFinal(false);
        // setFixedSize(aimSize);
    });
    connect(arect,&SAnimationRect::animationUpdating,this,[=](QPoint pos,QSize size,int,int){
        move(previousPos+pos);
        setFixedSize(size);
        rs->raise();
        update();
    });
    connect(this,&QMenu::aboutToHide,this,[=](){
        // arect->setEndValue(QPoint(0,0),
        //                    QSize(aimSize.width()*0.5,aimSize.height()),
        //                    0,0);
        // arect->setFinal();
        // arect->start();
    });
    init();
}

void SMenu::init()
{
    // setWindowFlags(Qt::Popup | Qt::FramelessWindowHint |Qt::NoDropShadowWindowHint);
    setContentsMargins(0,0,0,0);
    setStyleSheet(QSS_SMenu());
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags()  | Qt::FramelessWindowHint| Qt::NoDropShadowWindowHint );
}

void SMenu::raiseAction(QAction *action)
{
    QList<QAction*> actList= this->actions();
    actList.removeOne(action);
    foreach (QAction* act, actList) {
        removeAction(act);
    }
    foreach (QAction* act, actList) {
        addAction(act);
    }
}

void SMenu::paintEvent(QPaintEvent *event)
{

    // qDebug()<<parentWidget()->metaObject()->className();
    QColor tem = winThemeColor();
    // qDebug()<<mapToGlobal(QPoint(0,0));
    QPainter painter(this);
    tem.setAlpha(arect->nowAlpha);
    painter.setBrush(tem);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    QMenu::paintEvent(event);

}

void SMenu::resizeEvent(QResizeEvent *event)
{

    rs->updateDisplay();
    QMenu::resizeEvent(event);
}

void SMenu::closeEvent(QCloseEvent *event)
{

    QMenu::closeEvent(event);
}

void SMenu::showEvent(QShowEvent *event)
{

    qDebug()<<"ShowSize"<<size();
    qDebug()<<"ShowPos"<<pos();
    if(firstShow){
        aimSize = size();
        firstShow = false;
    }
    previousPos = pos();
    arect->setStartValue(QPoint(0,0),
                         QSize(aimSize.width()*0.5,1),
                         0,0);

    arect->setEndValue(QPoint(0,0),
                       QSize(aimSize.width(),aimSize.height()),
                       240,10);


    arect->start();
    setFixedSize(1,1);
    setVisible(true);
    show();
    QMenu::showEvent(event);
}
