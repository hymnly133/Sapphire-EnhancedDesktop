#include "ed_blockcontainer.h"
#include "ed_blocklayout.h"
#include "qdebug.h"
#include"QJsonObject"
#include "qjsonarray.h"
#include"mainwindow.h"
ED_BlockContainer::ED_BlockContainer(QWidget *parent,int sizeX,int sizeY,int row,int cal,int space,int spaceX,int spaceY )
    : ED_Container(parent,sizeX,sizeY)
{
    int temx,temy;
    if(spaceX == 0)
         temx = space;
    else temx = spaceX;

    if(spaceY ==0)
        temy = space;
    else temy = spaceY;

    inside = new ED_BlockLayout(this,row,cal,space,temx,temy);
    outSizeAnimation = new QPropertyAnimation(this,"outSizeFix");
    outSizeAnimation->setDuration(focus_animation_time);
    outSizeAnimation->setEasingCurve(QEasingCurve::InSine);

    connect(this,&::ED_BlockContainer::outSizeFix_changed,this,[=](double val){
        whenLongFocusAnimationChange();
    });


    longFocusAnimations->addAnimation(outSizeAnimation);
    QAction *act1 = new QAction("切换长聚焦效果(测试版)");
        myMenu->addAction(act1);
            connect(act1, &QAction::triggered, this, [=](){
            if(onLongFocus) setPreLongFocus(false);
            enableLongFocusEffect = !enableLongFocusEffect;
        });
}


void ED_BlockContainer::paintEvent(QPaintEvent *event){
    ED_Unit::paintEvent(event);
    Q_UNUSED(event);
}



QJsonObject ED_BlockContainer::to_json()
{
    QJsonObject rootObjct = ED_Container::to_json();
    rootObjct.insert("enableLongFocusEffect",enableLongFocusEffect);
    return rootObjct;
}

void ED_BlockContainer::load_json(QJsonObject rootObject)
{
    ED_Container::load_json(rootObject);
    if(rootObject.contains("enableLongFocusEffect"))
        enableLongFocusEffect = rootObject.value("enableLongFocusEffect").toBool();
}

void ED_BlockContainer::updateLongFocusAnimation()
{
    longFocusAnimations->stop();
    outSizeAnimation->setStartValue(outSizeFix);
    outSizeAnimation->setEndValue(aim_outSizeFix());
    longFocusAnimations->start();
}




void ED_BlockContainer::whenLongFocusAnimationChange()
{
    setFixedSize(MySize());
    if(!moving)
    move(MyPos());
}

void ED_BlockContainer::setPreLongFocus(bool val)
{
    if(!enableLongFocusEffect) return;
    else ED_Unit::setPreLongFocus(val);
    return;
}


void ED_BlockContainer::Say(){
    for(ED_Unit* content:*(inside->contents)){
        qDebug()<<content->pos()<<content->mapToGlobal(content->pos())<<content->size()<<"X,Y"<<content->indX<<content->indY;
    }
}



