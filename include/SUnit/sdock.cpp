#include "sdock.h"
#include "global.h"
#include "slinearlayout.h"
#include "stooltip.h"
#include"sunit.h"
#include"QPainterPath"
#include"QPainter"
#include"QJsonObject"


SDock::SDock(SLayout *dis, int outSizeX, int outSizeY): SContainer(dis,outSizeX,outSizeY)
{
    setMainColor(winThemeColor());
    inside = new SLinearLayout(this);
}

void SDock::paintEvent(QPaintEvent *event){
    // paintRect(this,QColor(0,0,155,aim_Alpha));

    if(onSwitch||!useContentColor) return SUnit::paintEvent(event);
    QPainter painter(this);

    SLinearLayout* myInside = (SLinearLayout*)inside;

    QLinearGradient linearGradient;
    linearGradient.setStart(QPoint(0,0));
    auto tem = displayColor_Alphaed();
    int count=0;


    if(myInside->direction == SLinearLayout::Horr){
        linearGradient.setFinalStop(QPoint(width(),0));
        for(int i=0;i<myInside->list.size();i++){
            count++;
            auto temm = inside->ind2Unit(i,0)->mainColor;
            temm = mixColor(temm,mainColor,nowMainColorRatio);
            float ratio = 1.0*(inside->ind2CenterPoint(i,0).x())/width();
            temm.setAlpha(colorAlpha*0.8);
            linearGradient.setColorAt(ratio, temm);
        }
    }
    else{
        linearGradient.setFinalStop(QPoint(0,height()));
        for(int i=0;i<myInside->list.size();i++){
            count++;
            auto temm = inside->ind2Unit(i,0)->mainColor;
            temm = mixColor(temm,mainColor,nowMainColorRatio);
            float ratio = 1.0*(inside->ind2CenterPoint(i,0).y())/height();
            temm.setAlpha(colorAlpha*0.8);
            linearGradient.setColorAt(ratio, temm);
        }
    }


    if(count){
        linearGradient.setColorAt(0, QColor(255,255,255,0));
        linearGradient.setColorAt(1, QColor(255,255,255,0));
    }
    else{
        linearGradient.setColorAt(0, tem);
        linearGradient.setColorAt(1, tem);
    }


    // 使用线性渐变创建一个画刷，用来填充

    QBrush brush(linearGradient);
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    // x, y, w, h
    painter.drawRect(rect());


}

void SDock::mouse_enter_action()
{
    inside->say();
}

bool SDock::onBigger()
{
    SLinearLayout* myInside = (SLinearLayout*)inside;

    if(myInside->direction == SLinearLayout::Horr){
        return setBlockSize(sizeX+1,sizeY);
    }
    else{
        return setBlockSize(sizeX,sizeY+1);
    }

}

bool SDock::onSmaller()
{
    SLinearLayout* myInside = (SLinearLayout*)inside;

    if(myInside->direction == SLinearLayout::Horr){
        return setBlockSize(sizeX-1,sizeY);
    }
    else{
        return setBlockSize(sizeX,sizeY-1);
    }
}

bool SDock::switchDirection()
{
    SLinearLayout* myInside = (SLinearLayout*)inside;

    if(setBlockSize(sizeY,sizeX)){
        foreach (auto unit, myInside->list) {
            map[unit] = unit->indX;
            unit->setVisible(false);
        }
        connect(positionAnimations,&QParallelAnimationGroup::finished,this,&SDock::switchFinishedSlot);

        //成功设置则转化方向
        if(myInside->direction == SLinearLayout::Horr){
            myInside->setDirection(SLinearLayout::Vert);
        }
        else{
            myInside->setDirection(SLinearLayout::Horr);
        }
        onSwitch = true;
        return true;
    }
    else{
        SToolTip::tip("当前布局无法容纳改变后Dock，请调整布局或者减小大小");
        return false;
    }
}

void SDock::endUpdate()
{
    SContainer::endUpdate();
    ((SLinearLayout*)inside)->refresh();
    inside->UpdateContentPositon(false);
}

void SDock::switchFinishedSlot()
{
    foreach (SUnit* unit, map.keys()) {
        unit->setVisible(true);
    }
    qDebug()<<"result";
    qDebug()<<disconnect(positionAnimations,&QParallelAnimationGroup::finished,this,&SDock::switchFinishedSlot);
    map.clear();
    onSwitch = false;
}

void SDock::setupEditMenu()
{
    SUnit::setupEditMenu();
    SET_ANCTION(actSwitchDirection,改变方向,editMenu,this,{
        switchDirection();
    });
    SET_ANCTION(actSwichUseContentColor,切换适应颜色,editMenu,this,{
        useContentColor = !useContentColor;
        update();
    });
}

QJsonObject SDock::to_json()
{
    QJsonObject root = SContainer::to_json();
    root.insert("useContentColor",useContentColor);
    return root;
}

void SDock::load_json(QJsonObject rootObject)
{
    if(rootObject.contains("useContentColor")) useContentColor = rootObject.value("useContentColor").toBool();
    SContainer::load_json(rootObject);
}
