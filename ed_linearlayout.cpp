#include "ed_linearlayout.h"
#include "ed_block.h"

ED_LinearLayout::ED_LinearLayout(QWidget *parent)
    : ED_Layout(parent)
{
    for(int i=0;i<10;i++){
        blocks[i] = nullptr;
    }
}

void ED_LinearLayout::refresh()
{
    for(int i=0;i<10;i++){
        if(blocks[i]!=nullptr){
            blocks[i]->setInd(i);
        }
    }


}

QPoint ED_LinearLayout::pos2Ind(int posx, int posy)
{
    return QPoint(int(posx/fixedDis()),0);
}

QPoint ED_LinearLayout::ind2CenterPoint(int x, int y)
{
    return QPoint(fixedCX(x),pContainer->height()/2);
}

QSize ED_LinearLayout::ind2Size(int xind, int yind)
{
    return QSize(pContainer->height(),pContainer->height());
}

QPoint ED_LinearLayout::ind2Pos(int xind, int yind)
{
    return QPoint(fixedDis()*xind,0);
}

bool ED_LinearLayout::Occupied(int x, int y)
{
    if(num()>9)
    return false;
}

ED_Unit *ED_LinearLayout::ind2Unit(int xind, int yind)
{
    return blocks[xind]->content;
}

QPoint ED_LinearLayout::defaultPutableInd(ED_Unit *aim)
{
    if(num()<=9)
    return QPoint(num(),0);
    else return QPoint(-1,-1);
}

QPoint ED_LinearLayout::clearPutableInd(ED_Unit *aim)
{
    if(num()>9)
       return QPoint(-1,-1);
    if (!num())return QPoint(0,0);

    int posx = pContainer->mapFromGlobal(pContainer->cursor().pos()).x();
    return QPoint((posx+fixedDis()/2)/fixedDis(),0);
}



void ED_LinearLayout::updateAfterRemove(ED_Unit *aim, int x, int y)
{

    for(int i =x;i<num();i++){

        blocks[i] = blocks[i+1];
    }
    blocks[num()] = nullptr;
    refresh();
    UpdateContentPositon();
}

void ED_LinearLayout::updateBeforePut(ED_Unit *aim, int x, int y )
{
    //将block队列位置调整对（ind仍为错）
    qDebug()<<"Put"<<x;
    for(int i =num()-1;i>=x;i--){
        blocks[i+1] = blocks[i];
    }
    blocks[x] = new littleBlock(x,aim);
    refresh();
}

void ED_LinearLayout::load_json(QJsonObject rootObject)
{

    ED_Layout::load_json(rootObject);
    for(int i=0;i<num();i++){
        blocks[i] = new littleBlock(i,(*contents)[i]);
    }
}

void ED_LinearLayout::updateAfterPut(ED_Unit *aim, int x, int y)
{

}

void ED_LinearLayout::updateBeforePutAnimation(ED_Unit * aim, int x, int y)
{
    for(int i=0;i<num();i++){
        if(i!=x){
            blocks[i]->content->updateInLayout();
        }
    }
}
