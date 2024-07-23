#include "sshellfuncunit.h"
#include "qevent.h"
#include "qfileiconprovider.h"
#include"stooltip.h"
#include"QProcess"
QString shell2name(int shellid){
    /*#define CSIDL_DESKTOP 0x0000
#define CSIDL_INTERNET 0x0001
#define CSIDL_PROGRAMS 0x0002
#define CSIDL_CONTROLS 0x0003
#define CSIDL_PRINTERS 0x0004
#define CSIDL_FAVORITES 0x0006
#define CSIDL_STARTUP 0x0007
#define CSIDL_RECENT 0x0008
#define CSIDL_SENDTO 0x0009
#define CSIDL_BITBUCKET 0x000a
#define CSIDL_STARTMENU 0x000b
#define CSIDL_MYDOCUMENTS CSIDL_PERSONAL
#define CSIDL_DESKTOPDIRECTORY 0x0010
#define CSIDL_DRIVES 0x0011
#define CSIDL_NETWORK 0x0012
#define CSIDL_NETHOOD 0x0013
#define CSIDL_FONTS 0x0014
#define CSIDL_TEMPLATES 0x0015
#define CSIDL_COMMON_STARTMENU 0x0016
#define CSIDL_COMMON_PROGRAMS 0x0017
#define CSIDL_COMMON_STARTUP 0x0018
#define CSIDL_COMMON_DESKTOPDIRECTORY 0x0019
#define CSIDL_PRINTHOOD 0x001b
*/
    switch(shellid){
    case 0x000a:
        return "回收站";
    default:
        return "暂不支持";
    }
}

SShellFuncUnit::SShellFuncUnit(SLayout *dis):SMultiFunc(dis)
{
    QImage im;
    im.load(":/icon/sysicon/Computer");
    setPix((resizeToRect(QPixmap::fromImage(im))));
    setname("我的电脑");
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
        setname("回收站");
    }
    else{
        QImage im;
        im.load(":/icon/sysicon/Computer");
        setPix((resizeToRect(QPixmap::fromImage(im))));
        setname("我的电脑");
    }
    update();

}

bool SShellFuncUnit::ProcessPath(QString path)
{
    return QFile::moveToTrash(path);
}





