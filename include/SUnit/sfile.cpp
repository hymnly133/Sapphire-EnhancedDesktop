#include "global.h"
#include "qthread.h"
#include "sinputdialog.h"
#include"sunit.h"
#include "sfile.h"
#include "SysFunctions.h"
#include "QWidget"
#include"QProcess"
#include "filefunc.h"
#include "qboxlayout.h"
#include"SysFunctions.h"
#include "qevent.h"
#include "qfileinfo.h"
#include "qlabel.h"
#include"QDebug"
#include"QUrl"
#include"QDesktopServices"
#include"QTextCodec"
#include "qmimedata.h"
#include"mainwindow.h"
#include "qpainter.h"
#include"QGraphicsDropShadowEffect"
#include"ContextMenu/contextmenu.h"
#include"QFileDialog"
#include"stooltip.h"
#include"snotice.h"

SFile::SFile(SLayout *dis, int sizex, int sizey, QString filePath): SMultiFunc(dis, sizex, sizey), SFileInfo(this)
{
    if(filePath != "") {
        loadFromPath(filePath, true);
    }
}


void SFile::mouse_enter_action()
{
    SMultiFunc::mouse_enter_action();

}


void SFile::double_click_action(QMouseEvent *event)
{
    //最终双击执行
    openFile();
}


QJsonObject SFile::to_json()
{
    QJsonObject rootObject = SMultiFunc::to_json();
    rootObject.insert("path", filePath);
    return rootObject;
}

void SFile::load_json(QJsonObject rootObject)
{
    SUnit::load_json(rootObject);
    if(rootObject.contains("name")) {
        setName(rootObject.value("name").toString());
    }
    setFullShow(rootObject.value("fullShow").toBool());
    filePath = (rootObject.value("path").toString());


    pixPath = rootObject.value("pixPath").toString();
    if(pixPath != "") {
        if(QFile::exists(pixPath)) {
            SMultiFunc::setIconFromPath(pixPath, false);
        } else {
            SNotice::notice(pixPath, tr("预加载图片丢失！"));
            pixPath = "";
            //图片丢失处理
        }
    }
    loadFromPath(filePath, false);
}


void SFile::processorTip()
{

    SToolTip::tip("用“" + name + "”打开");

}

void SFile::onShiftContextMenu(QContextMenuEvent *event)
{
    qDebug() << filePath;
    ContextMenu::show(QStringList() << filePath, (void *)pmw->winId(), event->globalPos());
}


void SFile::processFile(SFileInfo* sfileInfo)
{
    // QString path = sfileInfo->filePath;
    // // qDebug() << path << filePath;

    // path = "\"" + path + "\"";
    // qDebug() << shellrun(filePath, path);

}

void SFile::loadAimIcon(const MyFileInfo& info)
{
    SMultiFunc::setPix(info.aimIcon);
}




void SFile::renameWithDialog()
{
    SInputDialog* sip = SInputDialog::showInput("请输入新文件名", fullName(), mapToGlobal(QPoint(width(), 0)) + QPoint(10, 0));
    connect(sip, &SInputDialog::finalText, this, &SFile::rename);
}

void SFile::setName(QString name)
{
    SMultiFunc::setName(name);
    setObjectName(this->name);
}

bool SFile::rename(QString newNameWithSuffix)
{
    QString oldSuffix = suffix();
    bool res = SFileInfo::renameFile(newNameWithSuffix);

    if(res) {
        setName(path2Name(filePath));
        QString newSuffix = suffix();
        if(oldSuffix != newSuffix) {
            //重新加载图标
            setIconFromPath(filePath, false);
        }
    }

    return res;
}

void SFile::recoverForDefault()
{
    pixPath = "";
    setIconFromPath(filePath, false);
}




void SFile::remove()
{
    if(fileExist(filePath) && inDesktop(filePath)) {
        if(SFileInfo::removeFile()) {
            SMultiFunc::remove();
        }
    } else {
        SMultiFunc::remove();
    }
    SFileInfo::removeInfo();
}

void SFile::setupDesktopMenu()
{
    SUnit::setupDesktopMenu();
    desktopMenu->path = filePath;

    SET_ANCTION(actOpen, tr("打开"), desktopMenu, this, {
        openFile();
    });

    if(!isDir) {
        SET_ANCTION(actOpenAdmin, tr("以管理员身份打开"), desktopMenu, this, {
            openFile(true);
        });
    }


    SET_ANCTION(actRename, tr("重命名"), desktopMenu, this, {
        renameWithDialog();
    });

#ifdef QT_DEBUG

    SET_ANCTION(actSoftDelet, tr("软删除"), desktopMenu, this, {
        SMultiFunc::remove();
    });
#endif

    SET_ANCTION(actDelete, tr("删除"), desktopMenu, this, {
        remove();
    });

    SET_ANCTION(actOpenFileProperty, tr("属性"), desktopMenu, this, {
        OpenFileProperty(this->filePath);
    })
    if(isDir) {
        desktopMenu->addDirBGCommands();
        desktopMenu->addDirCommands();
    } else {
        desktopMenu->addFileCommands(suffix_red());
    }
}

void SFile::single_click_action(QMouseEvent *event)
{
    qDebug() << "File info: " << filePath; // 调试信息
}


void SFile::loadFromMyFI(const MyFileInfo& info, bool init)
{
    qDebug() << QString("Loading Form MyFI:%1,at thread:").arg(info.filePath) << (QThread::currentThread());
    SFileInfo::loadFromMyFI(info);


    if(init) {
        setName(info.name);
    }



    if(pixPath == "") {
        loadAimIcon(info);
    }

    if(init && info.type == MyFileInfo::Aim)
        switch (default_steam_icon_type) {
            case 0:
                break;
            case 1:
                setBlockSize(1, 2);
                setFullShow(true);
                break;
            case 2:
                setBlockSize(2, 1);
                setFullShow(true);
                break;
            default:
                break;
        }


    gv->limitInisde = true;
    qDebug() << &info;
    qDebug() << lb;
    qDebug() << name;
    if(init) {
        pixPath = "";
    }

}

void SFile::loadFromPath(QString filepath, bool init)
{
    // qDebug() << "Loading form path:" << filepath;

    MyFileInfo info = path2MyFI(filepath);
    loadFromMyFI(info, init);
}

