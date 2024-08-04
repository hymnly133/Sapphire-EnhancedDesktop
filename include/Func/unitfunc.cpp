#include "unitfunc.h"
#include "global.h"
#include "qmimedata.h"
#include "scontainer.h"
#include"QDrag"
#include"sfile.h"
void dragOutG(SUnit *sender, QMouseEvent *event)
{
    // if(pCelectedUnits.size()>=4){
    //     foreach (auto k, pCelectedUnits) {
    //         k->setUpdatesEnabled(false);
    //     }
    // }
    if(event!=nullptr&&event->modifiers()==Qt::ControlModifier){
    //应用拖出事件
        QList<QUrl> urls;
        foreach (auto k, pCelectedUnits) {
            if(k->inherits("SFile")){
                urls<<QUrl("file:///"+((SFile*)k)->filePath);
            }
        }
        QMimeData *mimeData = new QMimeData;
        mimeData->setUrls(urls);
        QDrag* drag = new QDrag(activepmw);//拖拽控件
        drag->setMimeData(mimeData);//加载数据
        drag->exec(Qt::MoveAction | Qt::CopyAction | Qt::LinkAction,Qt::CopyAction);

        return;
    }

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

        //尝试切换屏幕
        if(screenNum>1){
            //多屏切换
            qDebug()<<"try to scan for switch pmw";
            foreach(auto pmw,pmws){
                if(pmw->geometry().contains(QCursor::pos()) && pmw!=activepmw){
                    qDebug()<<"ScreenChange! "<<pmw->objectName()<<"Should Be The Aim";
                    foreach (SUnit* tem, pCelectedUnits) {
                        tem->onSwitch(pmw);
                    }
                    //清楚原屏幕的长聚焦
                    foreach (SUnit* tem, *(activepmw->inside->contents)) {
                        tem->preSetLongFocus(false);
                    }
                    activepmw = pmw;
                    break;
                }
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
    })
}

void requireContexMenu(QContextMenuEvent *event, SUnit *sender)
{
    if(!pCelectedUnits.empty()){
        if(!sender->onCelect){
            cleanCelect();
            if(editMode)
            sender->editMenu->exec(event->globalPos());
            else
                sender->desktopMenu->exec(event->globalPos());

        }
        else{
            activepmw->multiMenu->exec(event->globalPos());
        }
    }
    else{
        if(editMode)
            sender->editMenu->exec(event->globalPos());
        else
            sender->desktopMenu->exec(event->globalPos());
    }
}

void updateCelect(SUnit *sender)
{
    if(activepmw->celectPointList.size()==2){
        QRect aimRect = Point2Rect(activepmw->celectPointList[0],activepmw->celectPointList[1]);
        foreach (SUnit* k , *(activepmw->inside->contents)) {
            if(!pCelectedUnits.contains(k)){
                if(aimRect.contains( k->geometry().center())){
                    qDebug()<<"CONTAIN";
                    pCelectedUnits.append(k);
                    k->setCelect(true);
                }
            }
            else{
                if(!aimRect.contains( k->geometry().center())){
                    qDebug()<<"RELEASE";
                    k->setCelect(false);

                }
            }
        }
    }
    // activepmw->pls->raise();
    activepmw->pls->update();
}
