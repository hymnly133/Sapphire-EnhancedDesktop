#include "unitfunc.h"
#include "global.h"
#include "qmimedata.h"
#include "scontainer.h"
#include"mainwindow.h"
#include"qdrag.h"
#include"sfile.h"
#include"QWinMime"
#include"sdir.h"
void dragOutG(SUnit *sender, QMouseEvent *event)
{
    // if(pCelectedUnits.size()>=4){
    //     foreach (auto k, pCelectedUnits) {
    //         k->setUpdatesEnabled(false);
    //     }
    // }
    if(event != nullptr && event->modifiers() == Qt::ControlModifier) {
        //应用拖出事件
        QList<QUrl> urls;
        foreach (auto k, pCelectedUnits) {
            if(k->inherits("SFile")) {
                urls << QUrl("file:///" + (dynamic_cast<SFile*>(k))->filePath);
            }
        }
        QMimeData *mimeData = new QMimeData;
        mimeData->setUrls(urls);
        QDrag* drag = new QDrag(activepmw);//拖拽控件
        drag->setMimeData(mimeData);//加载数据
        drag->exec(Qt::MoveAction | Qt::CopyAction | Qt::LinkAction, Qt::CopyAction);

        return;
    }

    foreach (auto k, pCelectedUnits) {
        k->onDragedOut();
    }
    moving_global = true;
}

void cleanCelect(SUnit *sender)
{
    bool animation = numCelected > 4;
    foreach (auto k, pCelectedUnits) {
        k->setCelect(false, animation);
    }
    pCelectedUnits.clear();
    moving_global = false;
}

void releaseCelect(SUnit *sender)
{
    foreach (auto k, pCelectedUnits) {
        // k->setUpdatesEnabled(true);
        QPair<SLayout*, QPoint >res = deepFind(k);
        res.first->putUnit(k, res.second, true);
        k->moving = false;
        k->premove = false;
        if(processor != nullptr) {
            processor->processAnother(k);
        }
    }
    if(numCelected <= 1) {
        cleanCelect(sender);
    }
    moving_global = false;
}

void moveCelect(SUnit *sender)
{
    foreach (auto k, pCelectedUnits) {
        k->move(activepmw->mapFromGlobal(QCursor::pos()) - k->relativeP);
        // k->update();
    }

    //processor
    if(!pCelectedUnits.empty()) {
        SUnit* aim = activepmw->inside->SLayout::pos2Unit(activepmw->mapFromGlobal(QCursor::pos()));
        // SUnit* aim = activepmw->inside->SLayout::ind2Unit(ind);
        if(aim != nullptr) {
            aim->preSetLongFocus(true);
        }
        foreach (SUnit* tem, activepmw->inside->contents) {
            if(tem != aim && tem->preLongFocus) {
                tem->preSetLongFocus(false);
            }
        }

        //尝试切换屏幕
        if(screenNum > 1) {
            //多屏切换
            qDebug() << "try to scan for switch pmw";
            foreach(auto pmw, pmws) {
                if(pmw->geometry().contains(QCursor::pos()) && pmw != activepmw) {
                    qDebug() << "ScreenChange! " << pmw->objectName() << "Should Be The Aim";
                    foreach (SUnit* tem, pCelectedUnits) {
                        tem->onSwitch(pmw);
                    }
                    //清楚原屏幕的长聚焦
                    foreach (SUnit* tem, activepmw->inside->contents) {
                        tem->preSetLongFocus(false);
                    }
                    activepmw = pmw;
                    break;
                }
            }
        }
    }
}











QPair<SLayout*, QPoint > deepFind(SUnit *aim)
{
    auto k =  activepmw->inside->insideLayouts;
    foreach(SLayout* layout, k) {
        qDebug() << (layout == nullptr);
        auto p = layout;
        qDebug() << layout->pContainerS->objectName();
        if(layout->pContainerS->geometry().contains(aim->geometry().center())) {
            QPoint ind = layout->clearPutableInd(aim);
            if(ind != QPoint(-1, -1)) {
                return QPair<SLayout*, QPoint>(layout, ind);
            }
        }
    }

    return QPair<SLayout*, QPoint>(activepmw->inside, activepmw->inside->clearPutableInd(aim));
}

SUnit *from_class(QString Class)
{
    QString newname = Class.replace("ED_", "S");
    int id = QMetaType::type(Class.toStdString().c_str());
    if (id == QMetaType::UnknownType) {
        qDebug() << "error0";
        id = QMetaType::type(newname.toStdString().c_str());
        if(id == QMetaType::UnknownType) {
            qDebug() << "error1";
            return nullptr;
        }
    }
    qDebug() << Class;
    // auto k = QMetaType::create(id);
    SUnit *unit = static_cast<SUnit*>(QMetaType::create(id));
    return unit;
}
SUnit *from_json(QJsonObject data, SLayout *layout)
{
    QString name = data.value("Class").toString();
    if(name == "SFile" && QFileInfo(data.value("path").toString()).isDir()) {
        name = "SDir";
    }
    QString newname = name.replace("ED_", "S");
    int id = QMetaType::type(name.toStdString().c_str());
    if (id == QMetaType::UnknownType) {
        qDebug() << "error0";
        id = QMetaType::type(newname.toStdString().c_str());
        if(id == QMetaType::UnknownType) {
            qDebug() << "error1";
            return nullptr;
        }
    }
    qDebug() << name;
    // auto k = QMetaType::create(id);
    SUnit *unit = static_cast<SUnit*>(QMetaType::create(id));


    data.value("Class").toString();
    unit->setPMW(layout->pmw);
    unit->setParent(layout->pContainerW);
    unit->load_json(data);
    return unit;
}
SFile *from_path(QString path, SLayout *layout)
{
    SFile *unit;
    int id;
    if(QFileInfo(path).isDir()) {
        id = QMetaType::type("SDir");
    } else {
        id = QMetaType::type("SFile");
    }


    if (id == QMetaType::UnknownType) {
        return nullptr;
    }

    unit = static_cast<SFile*>(QMetaType::create(id));
    unit->loadFromPath(path, true);
    if(layout) {
        unit->setPMW(layout->pmw);
        unit->setParent(layout->pContainerW);
    }

    return unit;
}

QList<SUnit *> units()
{
    QList<SUnit*> res;
    foreach(auto pmw, pmws) {
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
        (dynamic_cast<SMultiFunc * >(unit))->setFullShow(!(dynamic_cast<SMultiFunc * >(unit))->fullShow);
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
    sender->tryToSetupMenu();
    if(!pCelectedUnits.empty()) {
        if(!sender->onCelect) {
            cleanCelect();
            if(editMode) {
                sender->editMenu->exec(event->globalPos());
            } else {
                sender->desktopMenu->exec(event->globalPos());
            }

        } else {
            activepmw->multiMenu->exec(event->globalPos());
        }
    } else {
        if(editMode) {
            sender->editMenu->exec(event->globalPos());
        } else {
            sender->desktopMenu->exec(event->globalPos());
        }
    }
}

void updateCelect(SUnit *sender)
{
    if(activepmw->celectPointList.size() == 2) {
        QRect aimRect = Point2Rect(activepmw->celectPointList[0], activepmw->celectPointList[1]);
        foreach (SUnit* k, activepmw->inside->contents) {
            if(!pCelectedUnits.contains(k)) {
                if(aimRect.contains( k->geometry().center())) {
                    qDebug() << "CONTAIN";
                    pCelectedUnits.append(k);
                    k->setCelect(true);
                }
            } else {
                if(!aimRect.contains( k->geometry().center())) {
                    qDebug() << "RELEASE";
                    k->setCelect(false);

                }
            }
        }
    }
    // activepmw->pls->raise();
    activepmw->pls->update();
}

void foldG()
{
    foreach (SDir* dir, nowExitDirs) {
        if(dir->inherits("SDir")) {
            dir->setExpand(false);
            dir->setFold(true);
        }
    }
}




