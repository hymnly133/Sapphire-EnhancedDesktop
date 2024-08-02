#include "sshellfuncunit.h"
#include "guifunc.h"
#include "qevent.h"
#include "smenu.h"
#include"stooltip.h"
#include"QProcess"

#define SET_ANCTION(NAME,TEXT,MENU,FUCTION)\
QAction *NAME = new QAction(#TEXT);\
    MENU->addAction(NAME);\
    connect(NAME, &QAction::triggered, this, [=]()FUCTION);

SShellFuncUnit::SShellFuncUnit(SLayout *dis):SMultiFunc(dis)
{
    requireMulti = true;
    QImage im;
    im.load(":/icon/sysicon/Computer");
    setPix((resizeToRect(QPixmap::fromImage(im))));
    setName("我的电脑");


}


QJsonObject SShellFuncUnit::to_json()
{
    QJsonObject root =  SMultiFunc::to_json();
    return root;
}

void SShellFuncUnit::load_json(QJsonObject rootObject)
{
    SMultiFunc::load_json(rootObject);
}

void SShellFuncUnit::processorTip()
{
    SToolTip::tip("移动到回收站");
}

void SShellFuncUnit::double_click_action(QMouseEvent *event)
{
    if(event->modifiers() == Qt::ShiftModifier)
    {
        QProcess process;
        process.setProgram("cmd.exe");
        process.setArguments({"/c","whoami","/user"});
        process.start();
        process.waitForFinished();
        QByteArray output= process.readAllStandardOutput();
        QString str(output);
        int start = str.lastIndexOf(" ");
        int begin = str.lastIndexOf("\r\n");
        QString id = str.mid(start+1,(begin-start-1));
        QString dis = "C:\\$Recycle.Bin\\"+id;
        qDebug()<<dis;
        QProcess::startDetached("explorer",QStringList()<<dis);
    }
    else
        QProcess::startDetached("explorer");
}



void SShellFuncUnit::onCelectedProcessor(bool val)
{
    if(val){
        QImage im;
        im.load(":/icon/sysicon/Dustbin");
        setPix((resizeToRect(QPixmap::fromImage(im))));
        setName("回收站");
    }
    else{
        QImage im;
        im.load(":/icon/sysicon/Computer");
        setPix((resizeToRect(QPixmap::fromImage(im))));
        setName("我的电脑");
    }
    update();

}

bool SShellFuncUnit::ProcessPath(QString path)
{
    return QFile::moveToTrash(path);
}

void SShellFuncUnit::onShiftContextMenu(QContextMenuEvent *event)
{
    // windowsMenu->exec(event->globalPos());
}

void SShellFuncUnit::setupDesktopMenu()
{
    SMultiFunc::setupDesktopMenu();

    SET_ANCTION(actOpenSetting,Win设置,desktopMenu,{
        SToolTip::tip(shellrun("ms-settings:wheel"));
    });

    SET_ANCTION(actOpenControl,控制面板,desktopMenu,{

        QProcess::startDetached("control");
    });

    SET_ANCTION(actOpenComMana,计算机管理,desktopMenu,{
        SToolTip::tip(shellrun("compmgmt.msc"));
        // QProcess process;
        // process.setProgram("cmd.exe");
        // process.setArguments({"compmgmt.msc"});
        // process.start();
    });

    SET_ANCTION(actOpenSysAdvance,高级系统设置,desktopMenu,{
        SToolTip::tip(shellrun("SystemPropertiesAdvanced"));
    });
    SET_ANCTION(actOpenTaskmgr,任务管理器,desktopMenu,{
        SToolTip::tip(shellrun("taskmgr"));
    });

    SET_ANCTION(actOpenDustBin,打开回收站,desktopMenu,{
        SToolTip::tip(shellrun("explorer","::{645FF040-5081-101B-9F08-00AA002F954E}"));
    });
    SET_ANCTION(actClean,清空回收站,desktopMenu,{
        SHEmptyRecycleBin(NULL,NULL,NULL);
    });
}





