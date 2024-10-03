#include "unitfunc.h"
#include "global.h"
#include "qmimedata.h"
#include "qtconcurrentrun.h"
#include "scontainer.h"
#include"mainwindow.h"
#include"qdrag.h"
#include "screenfunc.h"
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
        //系统拖出事件
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
        k->setSelect(false, animation);
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

    bool changed = false;

    //尝试切换屏幕
    if(screenNum > 1) {
        //多屏切换
        // qDebug() << "try to scan for switch pmw";
        foreach(auto pmw, pmws) {
            qDebug() << pmw->screenInd << pmw->geometry() << QCursor::pos();
            if(pmw->geometry().contains(QCursor::pos() + Shift_Global) && pmw != activepmw) {
                qDebug() << "ScreenChange! " << pmw->objectName() << "Should Be The Aim";
                foreach (SUnit* tem, pCelectedUnits) {
                    tem->onSwitch(pmw);
                }
                //清除原屏幕的长聚焦
                foreach (SUnit* tem, activepmw->inside->contents) {
                    tem->preSetLongFocus(false);
                }

                if(processor) {
                    processor->preSetLongFocus(false);
                }
                activepmw = pmw;
                changed = true;
                break;
            }
        }
    }

    if(!changed) {
        QtConcurrent::run(findProcessor);
    }


}











QPair<SLayout*, QPoint > deepFind(SUnit *aim)
{
    auto k =  activepmw->inside->insideLayouts;
    foreach(SLayout* layout, k) {
        // qDebug() << (layout == nullptr);
        // auto p = layout;

        qDebug() << layout->pContainerS->objectName();
        if(layout->pContainerS->geometry().contains(aim->geometry().center())) {
            if(!layout->OKForClearPut(aim)) {
                continue;
            }
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
    qDebug() << "from class:" << Class;
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

    if(id == QMetaType::type("SUnit")) {
        qWarning() << "Ignore generating SUnit  from Class!";
        return nullptr;
    }
    // auto k = QMetaType::create(id);
    SUnit *unit = static_cast<SUnit*>(QMetaType::create(id));
    return unit;
}
SUnit *from_json(QJsonObject data, SLayout *layout)
{

    if(data.contains("path")) {
        QString path = data.value("path").toString();
        if(!fileExist(path )) {
            qDebug() << path << "path in json not found!,stop Loading";
            return nullptr;
        }
    }

    QString name = data.value("Class").toString();
    if(name == "SFile" && QFileInfo(data.value("path").toString()).isDir()) {
        name = "SDir";
    }

    QString newname = name.replace("ED_", "S");
    int id = QMetaType::type(name.toStdString().c_str());
    if (id == QMetaType::UnknownType) {
        qDebug() << "error0:UnknownType";
        id = QMetaType::type(newname.toStdString().c_str());
        if(id == QMetaType::UnknownType) {
            qDebug() << "error1:UnknownType";
            return nullptr;
        }
    }
    qDebug() << name;

    // auto k = QMetaType::create(id);
    SUnit *unit = static_cast<SUnit*>(QMetaType::create(id));
    // if(unit->inherits("SFile")) {
    //     SFile* sfile = (SFile*)unit;

    // }

    data.value("Class").toString();
    unit->setPMW(layout->pmw);
    unit->setParent(layout->pContainerW);
    unit->load_json(data);
    return unit;
}
SFile *from_path(QString path, SLayout *layout)
{

    if(!fileExist(path )) {
        qDebug() << path << "path not found!,stop Loading";
        return nullptr;
    }

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
SFile *from_info(MyFileInfo& info, SLayout *layout)
{
    if(!fileExist(info.filePath )) {
        qDebug() << info.filePath << "path in info not found!,stop Loading";
        return nullptr;
    }
    SFile *unit;
    int id;
    if(QFileInfo(info.filePath).isDir()) {
        id = QMetaType::type("SDir");
    } else {
        id = QMetaType::type("SFile");
    }


    if (id == QMetaType::UnknownType) {
        return nullptr;
    }

    unit = static_cast<SFile*>(QMetaType::create(id));
    unit->loadFromMyFI(info, true);
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
    if(!sender->isSelect) {
        cleanCelect();
        sender->setSelect(true, true);
    }
    if(sender->isSelect && pCelectedUnits.size() > 1) {
        activepmw->multiMenu->exec(event->globalPos());
        // if() {

        // } else {
        //     if(editMode) {
        //         sender->editMenu->exec(event->globalPos());
        //     } else {
        //         sender->desktopMenu->exec(event->globalPos());
        //     }
        // }
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
                    k->setSelect(true);
                }
            } else {
                if(!aimRect.contains( k->geometry().center())) {
                    qDebug() << "RELEASE";
                    k->setSelect(false);

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





void findProcessor()
{
    //processor
    if(!pCelectedUnits.empty() && moving_global) {
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

    }
}

void raiseUnderMoving(SUnit* sender)
{
    // if(!moving_global) {
    //     return;
    // }
    if(sender) {
        sender->raise();
    }
    eachDoAsUnit({
        unit->raise();
    });
}
