#include "unitfunc.h"
#include "global.h"
#include "scontainer.h"

void dragOut()
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

void cleanCelect()
{
    bool animation = numCelected>4;
    foreach (auto k, pCelectedUnits) {
        k->setCelect(false,animation);
    }
    pCelectedUnits.clear();
    moving_global = false;
}

void releaseCelect()
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
    if(numCelected<=1)cleanCelect();
    moving_global = false;
}

void moveCelect()
{
    foreach (auto k, pCelectedUnits) {
        k->move(activepmw->mapFromGlobal(QCursor::pos())-k->relativeP);
        // k->update();
    }

    //processor
    if(pCelectedUnits.size()==1){
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
