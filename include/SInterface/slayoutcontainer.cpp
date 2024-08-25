#include "slayoutcontainer.h"
#include "sdir.h"
#include "sfile.h"
#include "sunit.h"
#include "slayout.h"
#include"snotice.h"
#include "unitfunc.h"
SLayoutContainer::SLayoutContainer()
{
    qDebug() << "LayoutContainer Created";
};

void SLayoutContainer::clearPut(SUnit* aim, bool animated)
{
    inside->clearPut(aim, animated);
}

bool SLayoutContainer::OKForClearPut(SUnit* aim)
{
    return inside->OKForClearPut(aim);
}
void SLayoutContainer::afterResize(QResizeEvent *event)
{
    inside->afterResize();
    inside->UpdateContentPositon(false);
}

QJsonObject SLayoutContainer::to_json()
{
    return inside->to_json();;
}

void SLayoutContainer::load_json(QJsonObject rootObject)
{
    inside->load_json(rootObject);
}

void SLayoutContainer::setPMW(MainWindow *pmw)
{
    inside->pmw = pmw;
    foreach (auto content, inside->contents) {
        content->setPMW(pmw);
    }
}

void SLayoutContainer::updateColor()
{
    foreach (auto content, inside->contents) {
        content->updateColor();
    }
}

void SLayoutContainer::loadInsideAll()
{
    foreach (auto content, inside->contents) {
        content->startToLoad();
    }
}

void SLayoutContainer::updateAfterPut(SUnit *aim)
{
    // if(aim->inherits("SDir")) {
    //     ((SDir*)aim)->scanDir();
    // }
}

void SLayoutContainer::updateBeforePut(SUnit *aim)
{

}

void SLayoutContainer::updateAfterRemove(SUnit *aim)
{

}

void SLayoutContainer::updateBeforeRemove(SUnit *aim)
{

}

bool SLayoutContainer::initAUnit(SUnit *aim, bool notice, QPoint globalPos)
{
    if (!aim) {
        return false;
    }
    // qDebug() << "Mainwindow try to add a info" << info.filePath;
    if(!inside->OKForDefaultPut(aim, true)) {
        SNotice::notice("布局无法容纳目标，请调整布局", "布局错误");
        return false;
    };

    if(globalPos == QPoint(-1, -1)) {
        inside->defaultPut(aim, false);
    } else {
        aim->setParent(inside->pContainerW);
        aim->move(inside->pContainerW->mapFromGlobal(globalPos));
        inside->clearPut(aim, false);
    }
    aim->raise();
    if(notice && aim->inherits("SFile")) {
        SNotice::notice(QStringList() << ((SFile*)aim)->filePath, "增添文件", 3000);
    }
    return true;
}

bool SLayoutContainer::addAFile(QString path, bool notice, QPoint globalPos)
{
    return initAUnit(from_path(path, inside),  notice, globalPos);
}

void SLayoutContainer::setSimpleMode(bool val)
{
    for(SUnit * content : inside->contents) {
        content->setSimpleMode(val);
    }
}

void SLayoutContainer::setScale(double val)
{
    for(SUnit * content : inside->contents) {
        content->setScale(val);
    }
}

void SLayoutContainer::setOpacity(double val)
{
    // qDebug() << val;
    for(SUnit * content : inside->contents) {
        // qDebug() << "apply to " << content->objectName() << val;
        content->setOpacity(val);
    }
}

void SLayoutContainer::endUpdate()
{
    for(SUnit * content : inside->contents) {
        content->endUpdate();
    }
}

void SLayoutContainer::setSLayout(SLayout *aim)
{
    if(aim == nullptr) {
        return;
    }
    inside = aim;
    // qDebug() << "Setted inside sLayout";
}
