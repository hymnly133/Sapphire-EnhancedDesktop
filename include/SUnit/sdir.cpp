#include "sdir.h"
#include "mainwindow.h"
#include "qjsonarray.h"
#include "qtconcurrentrun.h"
#include "sflowlayout.h"
#include "stylehelper.h"
#include "unitfunc.h"
#include "userfunc.h"

SDir::SDir(SLayout *dis, int sizex, int sizey, QString filePath): SFile(dis, sizex, sizey, filePath)
{
    inside_f = new SFlowlayout(this, 5);
    inside = inside_f;

    ar_fold = new SAnimationRect(this);
    ar_fold->setTime(long_focus_animation_time);
    ar_fold->setStartValue(pos(), size(), 0, 0, 1);

    ar_expand = new SAnimationRect(this);
    ar_expand->setTime(long_focus_animation_time);
    ar_expand->setStartValue(pos(), size(), 0, 0, 0);

    setExpand(false);
    setFold(true);

    connectTo(long_focus_animation_time, int, int, {
        ar_fold->setTime(value);
        ar_expand->setTime(value);
    })

    connect(ar_fold, &SAnimationRect::animationUpdating, this, &SDir::whenFoldAnimationUpdate);
    connect(ar_expand, &SAnimationRect::animationUpdating, this, &SDir::whenExpandAnimationUpdate);
    connect(ar_expand, &SAnimationRect::finished, this, [ = ]() {
        // onAnimation = false;
        // usePreviousSize = false;
        if(!isExpand) {
            enterParent();
        }
    });
}

void SDir::setSimpleMode(bool val)
{
    if(isExpand) {
        SLayoutContainer::setSimpleMode(val);
    } else {
        SFile::setSimpleMode(val);
    }
}

void SDir::setScale(double val)
{
    SFile::setScale(val);
    SLayoutContainer::setScale(val);
}

void SDir::endUpdate()
{
    SFile::endUpdate();
    SLayoutContainer::endUpdate();
    updateExpandAnimation();
    whenFoldAnimationUpdate();
}


void SDir::afterResize(QResizeEvent *event)
{
    SLayoutContainer::afterResize(event);
    SFile::afterResize(event);
}

QJsonObject SDir::to_json()
{
    QJsonObject rootObject = SFile::to_json();
    QJsonObject contentObject = SLayoutContainer::to_json();
    rootObject.insert("content", contentObject);
    return rootObject;
}

void SDir::load_json(QJsonObject rootObject)
{
    SFile::load_json(rootObject);
    waitedToLoad = rootObject.value("content").toObject().value("contents").toArray();
}

void SDir::setPMW(MainWindow *pmw)
{
    SFile::setPMW(pmw);
    SLayoutContainer::setPMW(pmw);
}

void SDir::setFold(bool val)
{
    if(val == isFold) {
        return;
    }
    if(val && isExpand) {
        return ;
    }

    if(!val && !isEnabled()) {
        return;
    }



    isFold = val;
    showNameTip = val;


    updateFoldAnimation();
    // update();
}

void SDir::updateAfterPut(SUnit *aim)
{
    SLayoutContainer::updateAfterPut(aim);
    if(!isExpand) {
        aim->setEnabled(false);
    }
    aim->setOpacity(1 - ar_fold->nowRatio);


    if(!onLoading && moveFile) {
        if(aim->inherits("SFile")) {

            SFile* sfile = (SFile*)aim;
            //为了避免bug，不移动文件夹
            if(sfile->isDir) {
                return;
            }
            //排除已经在文件夹中的
            if(filePath_red() == sfile->dirPath()) {
                return ;
            }
            qDebug() << filePath_red() + "/" + sfile->fullName();
            sfile->moveToDir(filePath_red());
        }
    }
    // updateExpandAnimation();
}

void SDir::updateAfterRemove(SUnit *aim)
{
    // updateExpandAnimation();
}

void SDir::updateFoldAnimation()
{
    ar_fold->stop();

    if(isFold) {
        ar_fold->setEndValue(pos(), size(), 0, 0, 1);
    } else {
        ar_fold->setEndValue(pos(), size(), 0, 0, 0);
    }
    ar_fold->start();
}

void SDir::setExpand(bool val)
{
    if(isExpand == val) {
        return;
    }


    isExpand = val;
    movable = !val;
    thisResizable = !val;
    inside->setEnable(val);

    if(layout) {
        if(val) {
            leaveParent();
        } else {
            preEnterParent();
        }
    }

    if(val) {
        setFold(false);
        // startToLoad();
        loadInsideAll();
    } else {
        inside_f->scrollTo(0);
        updateFocusAnimation();
    }


    raise();
    updateExpandAnimation();
}

void SDir::updateExpandAnimation()
{
    // qDebug() << ar_expand->nowRatio;
    if(isExpand && ar_expand->nowRatio == 1) {
        return;
    }
    if(!isExpand && ar_expand->nowRatio == 0) {
        return;
    }
    ar_expand->stop();

    ar_expand->nowRatio =  (size().width() - SUnit::MySize().width()) * 1.0 / (aim_expandSize().width() - SUnit::MySize().width());
    qDebug() << ar_expand->nowRatio;
    if(isExpand) {
        // ar_expand->setStartValue(pos(), size(), 0, 0, 0);
        ar_expand->setEndValue(pos(), size(), 0, 0, 1);

    } else {
        // ar_expand->setStartValue(pos(), size(), 0, 0, 1);
        ar_expand->setEndValue(pos(), size(), 0, 0, 0);
    }
    ar_expand->start();
}



void SDir::preSetLongFocus(bool val)
{
    SFile::preSetLongFocus(val);
    setFold(!val);
}

void SDir::loadFromMyFI(MyFileInfo info, bool init)
{
    SFile::loadFromMyFI(info, init);
    scanDir();
}

void SDir::scanDir()
{
    scanForChangeInDir(filePath_red(), this, &newfiles);
    qDebug() << objectName() << "Scaned,newFiles:" << newfiles;
}

void SDir::startToLoad()
{
    qDebug() << objectName() << "StartToLoad";
    moveFile = false;
    auto jsonfiles_ = jsonFiles();
    foreach (QString newFile, newfiles) {
        if(jsonfiles_.contains(newFile)) {
            continue;
        }
        if(nowExitFiles.contains(newFile)) {
            continue;
        }
        qDebug() << objectName() << "load path:" << newFile;
        SFile* thisFile;
        if(QFileInfo(newFile).isDir()) {
            thisFile = new SDir(inside);
        } else {
            thisFile = new SFile(inside);
        }
        QtConcurrent::run(thisFile, &SFile::loadFromPath, newFile, true);
    }
    newfiles.clear();

    foreach (QJsonValue val, waitedToLoad) {
        if(val.toObject().contains("path")) {
            if(nowExitFiles.contains(val.toObject().value("path").toString())) {
                continue;
            }
        }

        SUnit* thisFile = from_class(val.toObject().value("Class").toString());
        initAUnit(thisFile, false);
        qDebug() << objectName() << "load Json:" <<  val.toObject().value("path").toString();

        QtConcurrent::run(thisFile, &SUnit::load_json, val.toObject());
    }
    waitedToLoad = QJsonArray();
    moveFile = true;
}

QStringList SDir::jsonFiles()
{
    QStringList res;
    foreach (QJsonValue val, waitedToLoad) {
        if(val.toObject().contains("path")) {
            res << val.toObject().value("path").toString();

        }
    }
    return res;
}

void SDir::single_click_action(QMouseEvent *event)
{
    if(!isExpand) {
        setExpand(true);
        event->accept();
    }
}

void SDir::updateColor()
{
    SFile::updateColor();
    SLayoutContainer::updateColor();
}

void SDir::whenFoldAnimationUpdate()
{
    gv->untransparentRatio = ar_fold->nowRatio;
    lb->setOpacity(ar_fold->nowRatio);
    SLayoutContainer::setOpacity(1 - ar_fold->nowRatio);
    // setOpacity(qBound(0.1, ar->nowRatio, 1.0));
    update();
}

void SDir::whenExpandAnimationUpdate()
{
    setFixedSize(MySize());
    if(!moving) {
        edmove(MyPos());
    }
}

void SDir::remove()
{
    QList<SUnit*> con;

    foreach (SUnit* unit, inside->contents) {
        if(unit->inherits("SFile")) {
            SFile* sfile = ((SFile*)unit);
            if(sfile->dirPath() == filePath_red()) {
                //只是映射出来的文件就删除映射/删除文件，不释放
                sfile->removeInfo();
                continue;
            }
        }

        unit->removeFromLayout();
        con.push_back(unit);
    }


    if(fileExist(filePath) && inDesktop(filePath)) {
        if(SFile::removeFile()) {
            SMultiFunc::remove();
        }
    } else {
        SMultiFunc::remove();
    }

    SFileInfo::removeInfo();

    foreach (auto content, con) {
        activepmw->inside->clearPut(content, true);
    }
}

void SDir::wheelEvent(QWheelEvent *event)
{
    SFile::wheelEvent(event);
    if(event->isAccepted()) {
        return;
    }
    int tem = event->angleDelta().y();
    if(isExpand) {
        ((SFlowlayout*)inside)->scroll(-tem);
        event->accept();
    } else {
        setExpand(true);
        event->accept();
    }

}

QSize SDir::aim_expandSize()
{
    return pmw->blockSize() * inside_f->row();
}

void SDir::updateBeforePut(SUnit *aim)
{

}


