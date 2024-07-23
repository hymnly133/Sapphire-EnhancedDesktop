#include "slinearlayout.h"
#include "sfile.h"

SLinearLayout::SLinearLayout(QWidget *parent)
    : SLayout(parent)
{
    for(int i=0;i<10;i++){
        blocks[i] = nullptr;
    }
}

void SLinearLayout::refresh()
{
    for(int i=0;i<10;i++){
        if(blocks[i]!=nullptr){
            blocks[i]->setInd(i);
        }
    }
}

QPoint SLinearLayout::pos2Ind(int posx, int posy)
{
    return QPoint(int(posx/fixedDis()),0);
}

QPoint SLinearLayout::ind2CenterPoint(int x, int y)
{
    return QPoint(fixedCX(x),  pContainer->height()/2);
}

QSize SLinearLayout::ind2Size(int xind, int yind)
{
    return QSize(insideHeight(),insideHeight());
}

QPoint SLinearLayout::ind2Pos(int xind, int yind)
{
    return QPoint(fixedDis()*xind,0);
}

bool SLinearLayout::Occupied(int x, int y)
{
    if(num()>9)
    return false;
}

SUnit *SLinearLayout::ind2Unit(int xind, int yind)
{
    return blocks[xind]->content;
}

QPoint SLinearLayout::defaultPutableInd(SUnit *aim)
{
    if(num()<=9)
    return QPoint(num(),0);
    else return QPoint(-1,-1);
}

QPoint SLinearLayout::clearPutableInd(SUnit *aim)
{
    if(num()>9)
       return QPoint(-1,-1);
    if (!num())return QPoint(0,0);

    int posx = pContainer->mapFromGlobal(pContainer->cursor().pos()).x();
    return QPoint((posx+fixedDis()/2)/fixedDis(),0);
}



void SLinearLayout::updateAfterRemove(SUnit *aim, int x, int y)
{

    for(int i =x;i<num();i++){

        blocks[i] = blocks[i+1];
    }
    blocks[num()] = nullptr;
    refresh();
    UpdateContentPositon();
}

void SLinearLayout::updateBeforePut(SUnit *aim, int x, int y )
{
    //将block队列位置调整对（ind仍为错）
    // qDebug()<<"Put"<<x;
    for(int i =num()-1;i>=x;i--){
        blocks[i+1] = blocks[i];
    }
    blocks[x] = new littleBlock(x,aim);
    refresh();
}

void SLinearLayout::load_json(QJsonObject rootObject)
{

    SLayout::load_json(rootObject);
    for(int i=0;i<num();i++){
        blocks[i] = new littleBlock(i,(*contents)[i]);
    }
}

void SLinearLayout::updateAfterPut(SUnit *aim, int x, int y)
{

}

void SLinearLayout::updateBeforePutAnimation(SUnit * aim, int x, int y)
{
    for(int i=0;i<num();i++){
        if(i!=x){
            blocks[i]->content->updateInLayout();
        }
    }
}
