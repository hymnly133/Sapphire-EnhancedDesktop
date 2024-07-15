#include "ed_container.h"
#include "qdebug.h"
#include"QJsonObject"
#include "qjsonarray.h"

ED_Container::ED_Container(QWidget *parent,int sizeX,int sizeY,int row,int cal,int space,int spaceX,int spaceY )
    : ED_Unit(parent,sizeX,sizeY),row(row),col(cal),space(space),spaceX(spaceX),spaceY(spaceY)
{
    int temx,temy;
    type = Container;
    if(spaceX == 0)
         temx = space;
    else temx = spaceX;

    if(spaceY ==0)
        temy = space;
    else temy = spaceY;
    inside = new ED_Layout(this,row,cal,space,temx,temy);
}


void ED_Container::paintEvent(QPaintEvent *event){
    ED_Unit::paintEvent(event);
    Q_UNUSED(event);
}

QJsonObject ED_Container::to_json()
{
    QJsonObject rootObject = ED_Unit::to_json();
    QJsonObject contentObject = inside->to_json();
    rootObject.insert("content",contentObject);
    return rootObject;
}

void ED_Container::load_json(QJsonObject rootObject)
{
    ED_Unit::load_json(rootObject);
    inside->load_json(rootObject.value("content").toObject());

    row = inside->row;
    col = inside->col;
    space = inside->space;
    spaceX = inside->spaceX;
    spaceY = inside->spaceY;
}

void ED_Container::Say(){
    for(ED_Unit* content:*(inside->contents)){
        qDebug()<<content->pos()<<content->mapToGlobal(content->pos())<<content->size()<<"X,Y"<<content->indX<<content->indY;
    }
}

void ED_Container::InplaceAUnit(ED_Unit* aim){
    inside->InplaceAUnit(aim);
}
bool ED_Container::OKforput(ED_Unit* aim){
    return inside->OKforput(aim);
}
void ED_Container::setSimpleMode(bool val){
    ED_Unit::setSimpleMode(val);
    for(ED_Unit* content:*(inside->contents)){
        content->setSimpleMode(val);
    }
}

void ED_Container::setScale(double val){
    ED_Unit::setScale(val);
    for(ED_Unit* content:*(inside->contents)){
        content->setScale(val);
    }
}

void ED_Container::ed_update(){
    ED_Unit::ed_update();
    for(ED_Unit* content:*(inside->contents)){
        content->ed_update();
    }
}

void ED_Container::update_after_resize(){
    ED_Unit::update_after_resize();
    inside->Update_Positon();
}


