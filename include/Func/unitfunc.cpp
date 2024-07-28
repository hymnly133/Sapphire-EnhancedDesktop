#include "unitfunc.h"
#include "global.h"
#include "scontainer.h"

void dragOutG(SUnit *sender)
{
    // if(pCelectedUnits.size()>=4){
    //     foreach (auto k, pCelectedUnits) {
    //         k->setUpdatesEnabled(false);
    //     }
    // }
    foreach (auto k, pCelectedUnits) {
        k->onDragedOut();
    }
    moving_global = true;
}

void cleanCelect(SUnit *sender)
{
    bool animation = numCelected>4;
    foreach (auto k, pCelectedUnits) {
        k->setCelect(false,animation);
    }
    pCelectedUnits.clear();
    moving_global = false;
}

void releaseCelect(SUnit *sender)
{
    foreach (auto k, pCelectedUnits) {
        // k->setUpdatesEnabled(true);
        QPair<SLayout*,QPoint >res = deepFind(k);
        res.first->putUnit(k,res.second,true);
        k->moving = false;
        k->premove = false;
        k->updateFocusAnimation();
        if(processor!=nullptr){
            processor->onProcessAnother(k);
        }
    }
    if(numCelected<=1)cleanCelect(sender);
    moving_global = false;
}

void moveCelect(SUnit *sender)
{
    foreach (auto k, pCelectedUnits) {
        k->move(activepmw->mapFromGlobal(QCursor::pos())-k->relativeP);
        // k->update();
    }

    //processor
    if(!pCelectedUnits.empty()){
        QPoint ind = activepmw->inside->SLayout::pos2Ind(activepmw->mapFromGlobal(QCursor::pos()));
        SUnit* aim = activepmw->inside->SLayout::ind2Unit(ind);
        if(aim!=nullptr){
            aim->preSetLongFocus(true);
        }
        foreach (SUnit* tem, *(activepmw->inside->contents)) {
            if(tem!=aim &&tem->preLongFocus){
                tem->preSetLongFocus(false);
            }
        }
    }
}



QPair<SLayout*,QPoint > deepFind(SUnit *aim)
{
    foreach(SContainer* container,*(activepmw->inside->insideContainers)){
        if(container->geometry().contains(aim->geometry().center())){
            QPoint ind = container->inside->clearPutableInd(aim);
            if(ind!=QPoint(-1,-1)){
                return QPair<SLayout*,QPoint>(container->inside,ind);
            }
        }
    }

    return QPair<SLayout*,QPoint>(activepmw->inside,activepmw->inside->clearPutableInd(aim));
}


SUnit *from_json(QJsonObject data, SLayout *layout)
{
    QString name = data.value("Class").toString();
    QString newname = name.replace("ED_","S");
    int id = QMetaType::type(name.toStdString().c_str());
    if (id == QMetaType::UnknownType){
        qDebug()<<"error0";
        id = QMetaType::type(newname.toStdString().c_str());
        if(id == QMetaType::UnknownType){
            qDebug()<<"error1";
            return nullptr;
        }
    }
    qDebug()<<name;
    // auto k = QMetaType::create(id);
    SUnit *unit = static_cast<SUnit*>(QMetaType::create(id));
    unit->setPMW(layout->pmw);
    unit->setParent(layout->pContainer);
    unit->load_json(data);
    return unit;
}


QList<SUnit *> units(){
    QList<SUnit*> res;
    foreach(auto pmw,pmws){
        res.append(pmw->findChildren<SUnit*>());
    }
    return res;
}

void switchSimpleModeG(SUnit *sender)
{
    eachDoAsUnit({
        unit->setSimpleMode(!unit->simpleMode);
    });
}

void swtichAlwayShowG(SUnit *sender)
{
    eachDoAsUnit({
        unit->setAlwaysShow(!unit->alwaysShow);
    });
}


void switchFullShowG(SUnit *sender)
{
    eachDoAsUnit({
        if(unit->inherits("SMultiFunc"))
        ((SMultiFunc*)unit)->setFullShow(!((SMultiFunc*)unit)->fullShow);
    });
}

void removeG(SUnit *sender)
{
    eachDoAsUnit({
        unit->remove();
    });
}
