#include "scontainer.h"
#include "global.h"
#include"mainwindow.h"
SContainer::SContainer(SLayout *dis,int sizex,int sizey):SUnit(dis,sizex,sizey)
{
    type = Container;
    setMainColor(background_color);
}
void SContainer::setSimpleMode(bool val){
    SUnit::setSimpleMode(val);
    for(SUnit* content:*(inside->contents)){
        content->setSimpleMode(val);
    }
}

void SContainer::setScale(double val){
    SUnit::setScale(val);
    for(SUnit* content:*(inside->contents)){
        content->setScale(val);
    }
}

void SContainer::endUpdate(){
    SUnit::endUpdate();
    for(SUnit* content:*(inside->contents)){
        content->endUpdate();
    }
}

void SContainer::Say()
{

}


void SContainer::afterResize(QResizeEvent *event){
    inside->UpdateContentPositon(false);
}

QJsonObject SContainer::to_json()
{
    QJsonObject rootObject = SUnit::to_json();
    QJsonObject contentObject = inside->to_json();
    rootObject.insert("content",contentObject);
    return rootObject;
}

void SContainer::load_json(QJsonObject rootObject)
{
    SUnit::load_json(rootObject);
    inside->load_json(rootObject.value("content").toObject());
}

void SContainer::setPMW(MainWindow *pmw)
{
    SUnit::setPMW(pmw);
    inside->pmw = pmw;
    foreach (auto content, *(inside->contents)) {
        content->setPMW(pmw);
    }
}

void SContainer::updateColor()
{
    setMainColor(background_color);
}

void SContainer::remove()
{
    QList<SUnit*> con;
    foreach (auto content, *(inside->contents)) {
        content->removeFromLayout();
        con.push_back(content);
    }
    SUnit::remove();
    foreach (auto content, con) {
        activepmw->inside->clearPut(content,true);
    }
}


void SContainer::clearPut(SUnit* aim, bool animated){
    inside->clearPut(aim,animated);
}

bool SContainer::OKForClearPut(SUnit* aim){
    return inside->OKForClearPut(aim);
}
