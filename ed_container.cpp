#include "ed_container.h"

ED_Container::ED_Container(QWidget *parent, int sizeX, int sizeY):ED_Unit(parent,sizeX,sizeY)
{
        type = Container;
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


void ED_Container::afterResize(QResizeEvent *event){
    inside->UpdateContentPositon();
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
}
void ED_Container::clearPut(ED_Unit* aim, bool animated){
    inside->clearPut(aim,animated);
}

bool ED_Container::OKForClearPut(ED_Unit* aim){
    return inside->OKForClearPut(aim);
}
