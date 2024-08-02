#include "slinearlayout.h"
#include "SysFunctions.h"
#include "global.h"

SLinearLayout::SLinearLayout(QWidget *parent)
    : SLayout(parent)
{

}

void SLinearLayout::refresh()
{
    for(int i=0;i<list.size();i++){
        list[i]->indX = i;
    }
}

QPoint SLinearLayout::pos2Ind(int posx, int posy)
{
    int posaim;
    if(direction ==Horr){
        posaim = posx;
    }
    else{
        posaim = posy;
    }
    qDebug()<<"required"<<int(posaim/fixedDis());
    return QPoint(int(posaim/fixedDis()),0);
}


QSize SLinearLayout::ind2Size(int xind, int yind)
{
    return(QSize(insideSize(),insideSize()));
}

QPoint SLinearLayout::ind2Pos(int xind, int yind)
{
    if(direction == Horr){
        return QPoint(fixedDis()*(xind+0.5)-ind2Size(xind,yind).width()/2,(shortSide()-insideSize())/2);
    }
    else{
        return QPoint((shortSide()-insideSize())/2,fixedDis()*(xind+0.5)-ind2Size(xind,yind).height()/2);
    }
}

bool SLinearLayout::Occupied(int x, int y)
{
    return nowNum>10;
}

SUnit *SLinearLayout::ind2Unit(int xind, int yind)
{
    return list[xind];
}

QPoint SLinearLayout::defaultPutableInd(SUnit *aim)
{
    if(nowNum<=9)
    return QPoint(nowNum,0);
    else return QPoint(-1,-1);
}

QPoint SLinearLayout::clearPutableInd(SUnit *aim)
{
    if(list.size()>9)
       return QPoint(-1,-1);

    QPoint posaim;

    posaim = pContainer->mapFromGlobal(aim->mapToGlobal(QPoint(0,0)+aim->rect().center()));

    if(direction ==Horr)
    return QPoint((posaim.x()+fixedDis()/2)/fixedDis(),0);
    else
    return QPoint((posaim.y()+fixedDis()/2)/fixedDis(),0);

}



void SLinearLayout::updateAfterRemove(SUnit *aim, int x, int y)
{
    list.removeAt(x);
    refresh();
    aim->setBlockSize(1,1);
    UpdateContentPositon();
}

void SLinearLayout::updateBeforePut(SUnit *aim, int x, int y )
{
    //将block队列位置调整对（ind仍为错）
    // qDebug()<<"Put"<<x;

    refresh();
}

void SLinearLayout::updateAfterPut(SUnit *aim, int x, int y)
{
    refresh();
    UpdateContentPositon(false);
}

void SLinearLayout::updateBeforePutAnimation(SUnit * aim, int x, int y)
{
    list.insert(x,aim);
    refresh();
    UpdateContentPositon(!onLoading);
}

void SLinearLayout::setDirection(Dire aimDirection)
{
    if(aimDirection==direction) return;
    direction = aimDirection;
}

void SLinearLayout::say()
{
    for(int i =0;i<list.size();i++)
        qDebug()<<i<<list[i]->objectName();
}
void SLinearLayout::load_json(QJsonObject rootObject)
{
    SLayout::load_json(rootObject);
    if(rootObject.contains("nowNum")) nowNum = rootObject.value("nowNum").toInt();
    else nowNum = contents->size();
    if(rootObject.contains("direction")) direction = (Dire)rootObject.value("direction").toInt();
    refresh();
}

QJsonObject SLinearLayout::to_json()
{
    QJsonObject root = SLayout::to_json();
    root.insert("nowNum",nowNum);
    root.insert("direction",(int)direction);
    return root;
}


