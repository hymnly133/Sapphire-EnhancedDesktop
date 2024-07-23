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
#include "qpainter.h"
#include"QGraphicsDropShadowEffect"
#include"ContextMenu/contextmenu.h"
#include"QFileDialog"
#include"stooltip.h"
#define SET_ANCTION(NAME,TEXT,FUCTION)\
QAction *NAME = new QAction(#TEXT);\
    myMenu->addAction(NAME);\
    connect(NAME, &QAction::triggered, this, [=]()FUCTION);

SFile::SFile(SLayout *dis, int sizex, int sizey,QString filePath):SMultiFunc(dis,sizex,sizey)
{
    if(filePath!="")
    loadFromPath(filePath,true);
}


void SFile::mouse_enter_action(){
    SMultiFunc::mouse_enter_action();
    QFileInfo fileInfo(filePath);
    qDebug() << "File info: " << fileInfo.filePath()<<scale<<gv->m_scale<<gv->displaySize<<gv->actualSize; // 调试信息
}


void SFile::double_click_action(QMouseEvent *event){
    //最终双击执行
    QString cmd= QString("file:///")+filePath;
    qDebug("cmd = %s",qPrintable(cmd));
    QDesktopServices::openUrl(QUrl(cmd));
}


QJsonObject SFile::to_json()
{
    QJsonObject rootObject = SMultiFunc::to_json();
    rootObject.insert("path",filePath);
    rootObject.insert("useFileIcon",useFileIcon);
    return rootObject;
}

void SFile::load_json(QJsonObject rootObject)
{
    SMultiFunc::load_json(rootObject);
    filePath =(rootObject.value("path").toString());
    useFileIcon =rootObject.value("useFileIcon").toBool();
    loadFromPath(filePath,false);
}


void SFile::processorTip()
{
    if(isDir)
        SToolTip::Tip("移动至“"+name+"”");
    else
        SToolTip::Tip("用“"+name+"”打开");;
}

void SFile::onShiftContextMenu(QContextMenuEvent *event)
{
    qDebug()<<filePath;
    ContextMenu::show(QStringList() << filePath, (void *)pls->winId(), event->globalPos());
}


bool SFile::ProcessPath(QString path)
{
    qDebug()<<path<<filePath;
    bool removed = false;
    if(isDir){
        QString newName =  filePath+"/"+QFileInfo(path).fileName();
        removed = QFile::rename(path,newName);
        qDebug()<<"new name"<<newName;
    }
    else{
        QProcess* CalcPro = new QProcess(this);;
        CalcPro->start("cmd",QStringList()<<"/c"<<filePath<<path);
    }
    return removed;
}

void SFile::loadAimIcon(MyFileInfo info)
{
    SMultiFunc::setPix(info.aimIcon());
}

void SFile::setPix(QString pixPath, bool save)
{
    SMultiFunc::setPix(pixPath,save);

    if(save)
        useFileIcon = false;
}


void SFile::loadFromMyFI(MyFileInfo info,bool init){

    filePath = info.filePath;
    if(QFileInfo(filePath).isDir()) isDir = true;

    if(init)
    setname(info.name);



    if(useFileIcon)
    loadAimIcon(info);

    if(info.okForAim()&&init)
    switch (default_steam_icon_type) {
    case 0:
        break;
    case 1:
        setBlockSize(1,2);
        setFullShow(true);
        break;
    case 2:
        setBlockSize(2,1);
        setFullShow(true);
        break;
    default:
        break;
    }


    gv->limitInisde = true;
    lb->setText(elidedLineText(lb, 3, name));
    if(init){
        useFileIcon = true;
        pixPath = "";
    }

    endUpdate();
}

void SFile::loadFromPath(QString filepath,bool init)
{
    qDebug()<<"Loading form path:"<<filepath;
    MyFileInfo info = path2MyFI(filepath);
    loadFromMyFI(info,init);
}

