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
    QImage im;
    im.load(":/icon/sysicon/Computer");
    setPix((resizeToRect(QPixmap::fromImage(im))));
    setName("我的电脑");
    windowsMenu = new SMenu();

    QAction* windowsMenuAction  =new QAction(this);
    windowsMenuAction->setText("Windows功能");
    windowsMenuAction->setMenu(windowsMenu);

    myMenu->addAction(windowsMenuAction);
    // myMenu->setDefaultAction(windowsMenuAction);
    myMenu->raiseAction(windowsMenuAction);

    SET_ANCTION(actOpenSetting,Win11设置,windowsMenu,{
        SToolTip::Tip(shellrun("ms-settings:wheel"));
        // QProcess process;
        // process.setProgram("cmd.exe");
        // process.setArguments({"ms-settings:wheel"});
        // process.start();
                                         });

    SET_ANCTION(actOpenControl,控制面板,windowsMenu,{

        QProcess::startDetached("control");
    });

    SET_ANCTION(actOpenComMana,计算机管理,windowsMenu,{
        SToolTip::Tip(shellrun("compmgmt.msc"));
        // QProcess process;
        // process.setProgram("cmd.exe");
        // process.setArguments({"compmgmt.msc"});
        // process.start();
    });

    SET_ANCTION(actOpenSysAdvance,高级系统设置,windowsMenu,{
        SToolTip::Tip(shellrun("SystemPropertiesAdvanced"));
    });

    SET_ANCTION(actOpenDustBin,打开回收站,windowsMenu,{
        SToolTip::Tip(shellrun("explorer","::{645FF040-5081-101B-9F08-00AA002F954E}"));
    });
    SET_ANCTION(actClean,清空回收站,windowsMenu,{
        SHEmptyRecycleBin(NULL,NULL,NULL);
    });
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
    SToolTip::Tip("移动到回收站");
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





