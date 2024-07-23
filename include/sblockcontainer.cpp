#include "sblockcontainer.h"
#include "sblocklayout.h"
#include "qdebug.h"
#include"QJsonObject"
#include "qjsonarray.h"
#include"mainwindow.h"

SBlockContainer::SBlockContainer(SLayout *dis, int outsizex, int outsizey, int row, int cal, int space, int spaceX, int spaceY)
    :SContainer(dis,outsizex,outsizey)
{
    if(pmw==nullptr)
        qDebug()<<"null";

    requireLongFocusOutDeltaTime = true;

    int temx,temy;
    if(spaceX == 0)
        temx = space;
    else temx = spaceX;

    if(spaceY ==0)
        temy = space;
    else temy = spaceY;

    inside = new SBlockLayout(this,row,cal,space,temx,temy);

    outSizeAnimation = new QPropertyAnimation(this,"outSizeFix");
    outSizeAnimation->setDuration(focus_animation_time);
    outSizeAnimation->setEasingCurve(QEasingCurve::InSine);

    connect(this,&::SBlockContainer::outSizeFix_changed,this,[=](double val){
        whenLongFocusAnimationChange();
    });


    longFocusAnimations->addAnimation(outSizeAnimation);
    QAction *act1 = new QAction("切换长聚焦效果(测试版)");
    myMenu->addAction(act1);
    connect(act1, &QAction::triggered, this, [=](){
        if(onLongFocus) preSetLongFocus(false);
        enableLongFocusEffect = !enableLongFocusEffect;
    });
}

SBlockContainer::SBlockContainer(const SBlockContainer &other)
    :SBlockContainer(other.layout,other.sizeX,other.sizeY
                      ,((SBlockLayout*)other.inside)->row
                      ,((SBlockLayout*)other.inside)->col
                      ,((SBlockLayout*)other.inside)->space
                      ,((SBlockLayout*)other.inside)->spaceX
                      ,((SBlockLayout*)other.inside)->spaceY
                      )
{}



void SBlockContainer::paintEvent(QPaintEvent *event){
    SUnit::paintEvent(event);
    Q_UNUSED(event);
}



QJsonObject SBlockContainer::to_json()
{
    QJsonObject rootObjct = SContainer::to_json();
    rootObjct.insert("enableLongFocusEffect",enableLongFocusEffect);
    return rootObjct;
}

void SBlockContainer::load_json(QJsonObject rootObject)
{
    SContainer::load_json(rootObject);
    if(rootObject.contains("enableLongFocusEffect"))
        enableLongFocusEffect = rootObject.value("enableLongFocusEffect").toBool();
}

void SBlockContainer::updateLongFocusAnimation()
{
    longFocusAnimations->stop();
    outSizeAnimation->setStartValue(outSizeFix);
    outSizeAnimation->setEndValue(aim_outSizeFix());
    longFocusAnimations->start();
}




void SBlockContainer::whenLongFocusAnimationChange()
{
    SUnit::whenFocusAnimationChange();
    setFixedSize(MySize());
    if(!moving)
    move(MyPos());
}

void SBlockContainer::preSetLongFocus(bool val)
{
    if(!enableLongFocusEffect) return;
    else SUnit::preSetLongFocus(val);
    return;
}

void SBlockContainer::setLongFocus(bool val)
{
    SUnit::setLongFocus(val);
    raise();
}




void SBlockContainer::Say(){
    for(SUnit* content:*(inside->contents)){
        qDebug()<<content->pos()<<content->mapToGlobal(content->pos())<<content->size()<<"X,Y"<<content->indX<<content->indY;
    }
}



