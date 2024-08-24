#include "sshellfuncunit.h"
#include "guifunc.h"
#include "qevent.h"
#include "sfile.h"
#include "smenu.h"
#include"stooltip.h"
#include"QProcess"




SShellFuncUnit::SShellFuncUnit(SLayout *dis, Type type): SMultiFunc(dis)
{
    requireMulti = true;
    this->type = type;
    updateDisplay();
}

QJsonObject SShellFuncUnit::to_json()
{
    QJsonObject root =  SMultiFunc::to_json();
    root.insert("type", (int)type);
    return root;
}

void SShellFuncUnit::load_json(QJsonObject rootObject)
{
    SMultiFunc::load_json(rootObject);
    if(rootObject.contains("type")) {
        type = (Type)rootObject.value("type").toInt();
        updateDisplay();
    }
}

void SShellFuncUnit::processorTip()
{
    if(type == Multi || type == Trash) {
        SToolTip::tip(tr("移动到回收站"));
    }
}

void SShellFuncUnit::double_click_action(QMouseEvent *event)
{
    if(type == Multi) {
        if(event->modifiers() == Qt::ShiftModifier) {
            // QProcess process;
            // process.setProgram("cmd.exe");
            // process.setArguments({"/c", "whoami", "/user"});
            // process.start();
            // process.waitForFinished();
            // QByteArray output = process.readAllStandardOutput();
            // QString str(output);
            // int start = str.lastIndexOf(" ");
            // int begin = str.lastIndexOf("\r\n");
            // QString id = str.mid(start + 1, (begin - start - 1));
            // QString dis = "C:\\$Recycle.Bin\\" + id;
            // qDebug() << dis;
            // QProcess::startDetached("explorer", QStringList() << dis);
            shellrun("explorer", "::{645FF040-5081-101B-9F08-00AA002F954E}");
        } else {
            QProcess::startDetached("explorer");
        }
    } else if(type == Trash) {
        shellrun("explorer", "::{645FF040-5081-101B-9F08-00AA002F954E}");
    } else {
        QProcess::startDetached("explorer");
    }
}
void SShellFuncUnit::setProcessor(bool val)
{
    SMultiFunc::setProcessor(val);
    updateDisplay();
}

void SShellFuncUnit::processFile(SFileInfo* sfileInfo)
{
    if(containTrash()) {
        if(sfileInfo->pUnit) {
            ((SFile*)sfileInfo)->remove();
        } else {
            sfileInfo->removeFile();
        }
    }
}



// bool SShellFuncUnit::ProcessPath(QString path)
// {
//     return QFile::moveToTrash(path);
// }

void SShellFuncUnit::onShiftContextMenu(QContextMenuEvent *event)
{
    // windowsMenu->exec(event->globalPos());
}

void SShellFuncUnit::setupDesktopMenu()
{
    SMultiFunc::setupDesktopMenu();

    if(containComputer()) {
        SET_ANCTION(actOpenSetting, tr("Win设置"), desktopMenu, this, {
            SToolTip::tip(shellrun("ms-settings:wheel"));
        });

        SET_ANCTION(actOpenControl, tr("控制面板"), desktopMenu, this, {
            QProcess::startDetached("control");
        });

        SET_ANCTION(actOpenComMana, tr("计算机管理"), desktopMenu, this, {
            SToolTip::tip(shellrun("compmgmt.msc"));
            // QProcess process;
            // process.setProgram("cmd.exe");
            // process.setArguments({"compmgmt.msc"});
            // process.start();
        });

        SET_ANCTION(actOpenSysAdvance, tr("高级系统设置"), desktopMenu, this, {
            SToolTip::tip(shellrun("SystemPropertiesAdvanced"));
        });
        SET_ANCTION(actOpenTaskmgr, tr("任务管理器"), desktopMenu, this, {
            SToolTip::tip(shellrun("taskmgr"));
        });
    }

    if(containTrash()) {
        SET_ANCTION(actOpenDustBin, tr("打开回收站"), desktopMenu, this, {
            SToolTip::tip(shellrun("explorer", "::{645FF040-5081-101B-9F08-00AA002F954E}"));
        });
        SET_ANCTION(actClean, tr("清空回收站"), desktopMenu, this, {
            SHEmptyRecycleBin(NULL, NULL, NULL);
        });
    }
}

void SShellFuncUnit::setupEditMenu()
{
    SMultiFunc::setupEditMenu();
    if(type == Multi) {
        SET_ANCTION(actClean, tr("分离"), editMenu, this, {
            split();
        });
    }
}

void SShellFuncUnit::updateDisplay()
{
    QImage im;
    QString name;
    if(type == Multi) {
        if(isProcessor) {
            im.load(":/icon/sysicon/Dustbin");
            name = tr("回收站");

        } else {
            im.load(":/icon/sysicon/Computer");
            name = tr("我的电脑");
        }
    } else if(type == Trash) {
        im.load(":/icon/sysicon/Dustbin");
        name = tr("回收站");
    } else {
        im.load(":/icon/sysicon/Computer");
        name = tr("我的电脑");
    }

    setPix(resizeToRect(QPixmap::fromImage(im)));
    setName(name);
}

void SShellFuncUnit::split()
{
    auto k1 = new SShellFuncUnit(layout, Computer);
    auto k2 = new SShellFuncUnit(layout, Trash);
    remove();
}





