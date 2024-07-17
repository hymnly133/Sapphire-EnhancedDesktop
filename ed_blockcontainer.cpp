#include "ed_blockcontainer.h"
#include "ed_blocklayout.h"
#include "qdebug.h"
#include"QJsonObject"
#include "qjsonarray.h"

ED_BlockContainer::ED_BlockContainer(QWidget *parent,int sizeX,int sizeY,int row,int cal,int space,int spaceX,int spaceY )
    : ED_Container(parent,sizeX,sizeY),row(row),col(cal),space(space),spaceX(spaceX),spaceY(spaceY)
{
    int temx,temy;
    if(spaceX == 0)
         temx = space;
    else temx = spaceX;

    if(spaceY ==0)
        temy = space;
    else temy = spaceY;
    inside = new ED_BlockLayout(this,row,cal,space,temx,temy);
}


void ED_BlockContainer::paintEvent(QPaintEvent *event){
    ED_Unit::paintEvent(event);
    Q_UNUSED(event);
}

QJsonObject ED_BlockContainer::to_json()
{

    return ED_Container::to_json();
}

void ED_BlockContainer::load_json(QJsonObject rootObject)
{
    ED_Container::load_json(rootObject);
    ED_BlockLayout* inside_ = (ED_BlockLayout*)inside;
    row = inside_->row;
    col = inside_->col;
    space = inside_->space;
    spaceX = inside_->spaceX;
    spaceY = inside_->spaceY;
}

void ED_BlockContainer::Say(){
    for(ED_Unit* content:*(inside->contents)){
        qDebug()<<content->pos()<<content->mapToGlobal(content->pos())<<content->size()<<"X,Y"<<content->indX<<content->indY;
    }
}

