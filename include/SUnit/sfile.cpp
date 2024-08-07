#include "global.h"
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
    open();
}

void SFile::open(bool Admin)
{
    if(Admin){
        shellrun(filePath,"",true);
    }
    else{
        QString cmd= QString("file:///")+filePath;
        qDebug("cmd = %s",qPrintable(cmd));
        QDesktopServices::openUrl(QUrl(cmd));
    }
}


QJsonObject SFile::to_json()
{
    QJsonObject rootObject = SMultiFunc::to_json();
    rootObject.insert("path",filePath);
    return rootObject;
}

void SFile::load_json(QJsonObject rootObject)
{
    SUnit::load_json(rootObject);
    if(rootObject.contains("name")) setName(rootObject.value("name").toString());
    setFullShow(rootObject.value("fullShow").toBool());
    filePath =(rootObject.value("path").toString());


    pixPath = rootObject.value("pixPath").toString();
    if(pixPath!=""){
        if(QFile::exists(pixPath))
            SMultiFunc::setPix(path2Icon(pixPath)[0]);
        else{
            SNotice::notice(pixPath,tr("预加载图片丢失！"));
            pixPath = "";
            //图片丢失处理
        }
    }
    loadFromPath(filePath,false);
}


void SFile::processorTip()
{
    if(isDir)
        SToolTip::tip("移动至“"+name+"”");
    else
        SToolTip::tip("用“"+name+"”打开");;
}

void SFile::onShiftContextMenu(QContextMenuEvent *event)
{
    qDebug()<<filePath;
    ContextMenu::show(QStringList() << filePath, (void *)pmw->winId(), event->globalPos());
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
}

void SFile::renameFile(QString newNameWithSuffix)
{
    QString folder = QFileInfo(filePath).path();
    QString newName =  folder+"/"+newNameWithSuffix;
    if(newName==filePath) return;
    qDebug()<<"From"<<filePath<<"to"<<newName;
    // qDebug()<<QFileInfo(filePath).isReadable();
    bool renamed = QFile::rename(filePath,newName);
    if(renamed){
        QString oldSuffix = suffix();

        nowExits.remove(filePath);
        nowExits[newName] = this;
        filePath = newName;
        setName(path2Name(newName));
        QString newSuffix =suffix();
        if(oldSuffix!=newSuffix){
            //重新加载图标
            setPix(filePath,false);
        }
    }
    SNotice::notice(QStringList()<<newName+":"+((renamed)?"成功":"失败"),"重命名文件");
}

void SFile::renameWithDialog()
{
    SInputDialog* sip = SInputDialog::showInput("请输入新文件名",fullName(),mapToGlobal(QPoint(width(),0))+QPoint(10,0));
    connect(sip,&SInputDialog::finalText,this,&SFile::renameFile);
}

void SFile::setName(QString name)
{
    SMultiFunc::setName(name);
    setObjectName(this->name);
}

void SFile::recoverForDefault()
{
    pixPath = "";
    setPix(filePath,false);
}



void SFile::remove()
{
    if(nowExits.contains(filePath)){
        nowExits.remove(filePath);
    }

    bool res = QFile::moveToTrash(filePath);
    if(!res){
    qDebug()<<"Unable to delete local file";
    // QString cmd = "del \""+filePath+"\"";
    // system(cmd.toStdString().c_str());
    }
    else{
        qDebug()<<"Deleted";
    }

    SUnit::remove();
}

void SFile::setupDesktopMenu()
{
    SUnit::setupDesktopMenu();
    desktopMenu->path = filePath;

    SET_ANCTION(actOpen,tr("打开"),desktopMenu,this,{
        open();
    });

    if(!isDir){
        SET_ANCTION(actOpenAdmin,tr("以管理员身份打开"),desktopMenu,this,{
            open(true);
        });
    }


    SET_ANCTION(actRename,tr("重命名"),desktopMenu,this,{
        renameWithDialog();
    });

#ifdef QT_DEBUG

    SET_ANCTION(actSoftDelet,tr("软删除"),desktopMenu,this,{
        SMultiFunc::remove();
    });
#endif

    SET_ANCTION(actDelete,tr("删除"),desktopMenu,this,{
        remove();
    });

    SET_ANCTION(actOpenFileProperty,tr("属性"),desktopMenu,this,{
        OpenFileProperty(this->filePath);
    })
    if(isDir){
        desktopMenu->addDirCommands();
    }
    else{
        desktopMenu->addFileCommands(targetSuffix());
    }
}


void SFile::loadFromMyFI(MyFileInfo info,bool init){

    filePath = info.filePath;
    if(QFileInfo(filePath).isDir()){
        isDir = true;
        requireMulti = true;
    }

    nowExits[filePath] = this;


    if(init)
    setName(info.name);



    if(pixPath=="")
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
        pixPath = "";
    }

}

void SFile::loadFromPath(QString filepath,bool init)
{
    qDebug()<<"Loading form path:"<<filepath;

    MyFileInfo info = path2MyFI(filepath);
    loadFromMyFI(info,init);
}

