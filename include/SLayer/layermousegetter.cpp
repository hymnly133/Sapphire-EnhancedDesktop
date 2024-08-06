#include "layermousegetter.h"
#include "SysFunctions.h"
#include"mainwindow.h"
#include "qevent.h"
#include "qpainter.h"
#include"stooltip.h"
#include "stylehelper.h"
#define SET_ANCTION(NAME,TEXT,MENU,FUCTION)\
QAction *NAME = new QAction(#TEXT);\
    MENU->addAction(NAME);\
    connect(NAME, &QAction::triggered, this, [=]()FUCTION);
LayerMouseGetter::LayerMouseGetter(MainWindow *parent, int screenId):QWidget(nullptr)
{
    pmw = parent;
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint );
    inplace(this);
    positionToScreen(this,screenId);
    myMenu = new QMenu(this);
    SET_ANCTION(act1,pls,myMenu,
    {
                    pmw->pls->raise();
        pmw->pls->update();
    })
    SET_ANCTION(act2,plsM,myMenu,
                {

                })
    SET_ANCTION(act3,plsB,myMenu,
                {
                    pmw->plsB->raise();
        pmw->plsB->update();
                })


}

void LayerMouseGetter::mousePressEvent(QMouseEvent *event)
{

}

void LayerMouseGetter::mouseDoubleClickEvent(QMouseEvent *event)
{
    // qDebug()<<"mouseGetter"<<!activepmw->isVisible();
    //     activepmw->setVisible(!activepmw->isVisible());
    SToolTip::tip("MouseGetter");
    qDebug()<<pmw->pls->geometry()<<pmw->pls->visibleRegion();
}

void LayerMouseGetter::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    paint.setPen(Qt::transparent);
    paint.setBrush(QColor(0, 0, 0, 1));
    paint.drawRect(rect());
}

void LayerMouseGetter::contextMenuEvent(QContextMenuEvent *event)
{
    if(event->modifiers() == Qt::ShiftModifier){
        // SettingWindow* k = new SettingWindow();
        StyleSettingWindow* k = new StyleSettingWindow;
        k->show();
    }
    else
        myMenu->exec(event->globalPos());
}


