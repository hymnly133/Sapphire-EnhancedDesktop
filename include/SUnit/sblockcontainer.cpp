#include "sblockcontainer.h"
#include "sblocklayout.h"
#include "qdebug.h"
#include"QJsonObject"
#include "qjsonarray.h"
#include"mainwindow.h"
#include "userfunc.h"

SBlockContainer::SBlockContainer(SLayout *dis, int outsizex, int outsizey, int row, int col, double boradXR, double boradYR, double spaceXR, double spaceYR)
    :SContainer(dis,outsizex,outsizey)
{
    if(pmw==nullptr)
        qDebug()<<"null";

    requireLongFocusOutDeltaTime = true;

    row = qMax(row,2);
    col = qMax(col,2);


    inside = new SBlockLayout(this,row,col,boradXR,boradYR,spaceXR,spaceYR);

    outSizeAnimation = new QPropertyAnimation(this,"outSizeFix");
    outSizeAnimation->setDuration(long_focus_animation_time);
    outSizeAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    connect(this,&::SBlockContainer::outSizeFix_changed,this,[=](double val){
        whenLongFocusAnimationChange();
    });


    longFocusAnimations->addAnimation(outSizeAnimation);

}

SBlockContainer::SBlockContainer(const SBlockContainer &other)
    :SBlockContainer(other.layout,other.sizeX,other.sizeY
                      ,((SBlockLayout*)other.inside)->row
                      ,((SBlockLayout*)other.inside)->col
                      ,((SBlockLayout*)other.inside)->boradXR
                      ,((SBlockLayout*)other.inside)->boradYR
                      ,((SBlockLayout*)other.inside)->spaceXR
                      ,((SBlockLayout*)other.inside)->spaceYR
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
    outSizeAnimation->setDuration(long_focus_animation_time);
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

void SBlockContainer::setupEditMenu()
{
    SUnit::setupEditMenu();

    QAction *act1 = new QAction(tr("切换长聚焦效果(测试版)"));
    editMenu->addAction(act1);
    connect(act1, &QAction::triggered, this, [=](){
        if(onLongFocus) preSetLongFocus(false);
        enableLongFocusEffect = !enableLongFocusEffect;
    });

    SET_ANCTION(act2,tr("调整布局"),editMenu,this,{
        resizeForWithDialog((SBlockLayout*)inside);
    });
}




void SBlockContainer::Say(){
    for(SUnit* content:*(inside->contents)){
        qDebug()<<content->pos()<<content->mapToGlobal(content->pos())<<content->size()<<"X,Y"<<content->indX<<content->indY;
    }
}



