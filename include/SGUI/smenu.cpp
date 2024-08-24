#include "smenu.h"
#include "SQSS.h"
#include "filefunc.h"
#include "qevent.h"
#include "qgraphicseffect.h"
#include "qpainter.h"
#include"global.h"
#include"mainwindow.h"
#include "saction.h"
#include "stylehelper.h"
#include"userfunc.h"
#define REG_Dir "HKEY_CLASSES_ROOT\\Directory"
#define REG_Folder "HKEY_CLASSES_ROOT\\Folder"
#define REG_Files "HKEY_CLASSES_ROOT"


QList<QPair<QString, QString>> SMenu::dirCommands;
QList<QPair<QString, QString>> SMenu::dirBGCommands;
QList<QPair<QString, QString>> SMenu::DesktopCommands;
QMap<QString, QList<QPair<QString, QString>>*> SMenu::filesCommands;
QMap<QString, QIcon> SMenu::icons;
QStringList SMenu::ignoreCommands;
SMenu::SMenu(QWidget *parent): QMenu(parent)
{
    rs = new roundShower(this);
    arect = new SAnimationRect(this);
    rs->distriRadius(&arect->nowRadius);
    rs->raise();
    arect->setTime(150);

    connect(arect, &SAnimationRect::finishedFinal, this, [ = ] {
        // setVisible(false);
        // arect->setFinal(false);
        // setFixedSize(aimSize);
    });
    connect(arect, &SAnimationRect::animationUpdating, this, [ = ]() {
        move(previousPos + arect->nowPos);
        setFixedSize(arect->nowSize);
        rs->raise();
        update();
    });
    connect(this, &QMenu::aboutToHide, this, [ = ]() {
        // arect->setEndValue(QPoint(0,0),
        //                    QSize(aimSize.width()*0.5,aimSize.height()),
        //                    0,0);
        // arect->setFinal();
        // arect->start();
    });
    init();
}

void SMenu::init()
{
    // setWindowFlags(Qt::Popup | Qt::FramelessWindowHint |Qt::NoDropShadowWindowHint);
    setContentsMargins(0, 0, 0, 0);
    setStyleSheet(QSS_SMenu());
    connect(psh, &StyleHelper::colorChanged, this, [ = ]() {
        setStyleSheet(QSS_SMenu());
    });
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags()  | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint );
}

void SMenu::raiseAction(QAction *action)
{
    QList<QAction*> actList = this->actions();
    actList.removeOne(action);
    foreach (QAction* act, actList) {
        removeAction(act);
    }
    foreach (QAction* act, actList) {
        addAction(act);
    }
}

void SMenu::exec(QPoint pos, bool multi)
{
    QMenu::exec(pos);
}

void SMenu::addShellCommand(QPair<QString, QString> command)
{
    if(command.first == "S") {
        sysComMenu->addSeparator();
        return;
    }
    if(exitsCommands.contains(command.first)) {
        return;
    }
    SET_ANCTION(thisact, command.first, sysComMenu, this, {
        shellContextMenuRun(command.second, path);
    });
    exitsCommands.append(command.first);
    thisact->setIcon(icons[command.first]);
}

void SMenu::addDirCommands()
{
    checkSysMenu();
    foreach (auto dircommand, dirCommands) {
        addShellCommand(dircommand);
    }
    checkSysMenu();
}

void SMenu::addDirBGCommands()
{
    checkSysMenu();
    foreach (auto dirBGcommand, dirBGCommands) {
        addShellCommand(dirBGcommand);
    }
    checkSysMenu();
}

void SMenu::addFileCommands(QString suffix)
{
    checkSysMenu();
    if(filesCommands.contains("*")) {
        foreach (auto val, (*(filesCommands.value("*")))) {
            addShellCommand(val);
        }
    }
    sysComMenu->addSeparator();
    if(filesCommands.contains(suffix)) {
        foreach (auto filecommand, *(filesCommands.value(suffix))) {
            addShellCommand(filecommand);
        }
    }
    checkSysMenu();
}

void SMenu::addTrayCommands()
{
    SET_ANCTION(openSettingWindow_act, tr("设置面板"), this, this, {
        openSettingWindow();
    })
    SET_ANCTION(reboot_act, tr("重启程序"), this, this, {
        SReboot();
    })
    SET_ANCTION(quit_act, tr("退出程序"), this, this, {
        SExit();
    })

}

void SMenu::checkSysMenu()
{
    if(!added) {
        syscomAct = new SAction(tr("更多"));
        addAction(syscomAct);
        sysComMenu = new SMenu;
        syscomAct->setMenu(sysComMenu);
        added = true;
    } else {

        //清楚空的菜单
        if(exitsCommands.empty()) {
            removeAction(syscomAct);
        }
    }

}

void SMenu::readDirReg(QString path)
{
    QSettings settingdir(path, QSettings::NativeFormat);
    qDebug() << "start to Read dirRegs" << path;
    if(settingdir.childGroups().contains("shell")) {
        readShellReg(path, dir);
    }
    dirCommands.append(QPair("S", ""));
    dirBGCommands.append(QPair("S", ""));
    if(settingdir.childGroups().contains("Background")) {
        readShellReg(path + "\\Background", dirBG);
    }
}

void SMenu::readFileReg(QString group)
{

    QSettings settingdir(QString(REG_Files) + "\\" + group, QSettings::NativeFormat);
    // qDebug()<<settingdir.childGroups();

    if(settingdir.childGroups().contains("shell")) {
        qDebug() << "FileReg" << group;
        readShellReg(QString(REG_Files) + "\\" + group, file);
    }
}

void SMenu::readFilesReg()
{
    QSettings settingdir(REG_Files, QSettings::NativeFormat);
    foreach(QString group, settingdir.childGroups()) {
        if(group == "*" || group[0] == ".") {
            readFileReg(group);
        }
    }
}

void SMenu::readShellReg(QString path, CommandType type)
{
    QSettings settings(path + "\\shell", QSettings::NativeFormat);
    QStringList groupsList = settings.childGroups();
    foreach(QString group, groupsList) {
        QString displayName = group;
        if(ignoreCommands.contains(group)) {
            continue;
        }
        // qDebug()<<group;
        readAReg(path + "\\shell\\" + group, type);
    }
}

void SMenu::readAReg(QString path, CommandType type)
{
    //path : XX/shell/xx
    QSettings settings(path, QSettings::NativeFormat);
    QString displayName = path.split('\\').last();
    qDebug() << settings.fileName();
    //如果默认值不为空
    if(settings.value(".").isValid()) {
        displayName = settings.value(".").toString();
    }

    if(settings.value("MUIVerb").isValid()) {
        displayName = settings.value("MUIVerb").toString();
    }

    //如果引用资源
    if(displayName[0] == '@') {
        displayName = extractString(displayName);
    }



    //如果带有快捷键标识
    if(displayName.contains('(')) {
        displayName.remove(displayName.indexOf('('), displayName.indexOf(')') + 1 - displayName.indexOf('('));
    }
    while(displayName.contains('&')) {
        displayName.remove('&');
    }
    qDebug() << displayName;

    //添加Icon
    if(settings.contains("Icon")) {
        QString iconPath = settings.value("Icon").toString();
        QIcon icon = QIcon(getWinIcon(iconPath, true));
        if(!icon.isNull()) {
            icons.insert(displayName, icon);
        } else if(iconPath.split(",").size() == 2) {
            icon = getShellIcon(iconPath.split(",")[0], iconPath.split(",")[1].toInt());
            if(!icon.isNull()) {
                icons.insert(displayName, icon);
            }
        }
    }

    foreach(QString group, settings.childGroups()) {
        //含有命令，进行处理
        if(group.toLower() == "command") {
            settings.beginGroup(group);
            QString com = settings.value(".").toString();
            switch (type) {
                case dir:
                    dirCommands.append(QPair(displayName, com));
                    break;

                case dirBG:
                    dirBGCommands.append(QPair(displayName, com));
                    break;

                case file:
                    auto list = path.split('\\');
                    QString suffix = list[1];
                    suffix.remove('.');
                    if(!filesCommands.contains(suffix)) {
                        filesCommands.insert(suffix, new QList<QPair<QString, QString>>);
                    }
                    filesCommands[suffix]->append(QPair(displayName, com));
                    break;

            }
        }
    }
}

void SMenu::initSysCommands()
{
    //此处是各种文件（夹）菜单
    ignoreCommands << "find" << "UpdateEncryptionSettings" << "explore" << "open" << "opennewprocess" << "opennewtab" << "opennewwindow" << "pintohome" << "pintohomefile";
    readDirReg(REG_Dir);
    dirCommands.append(QPair("S", ""));
    dirBGCommands.append(QPair("S", ""));
    readDirReg(REG_Folder);
    readFilesReg();

    //此处是托盘图标菜单
    trayMenu = new SMenu();
    trayMenu->addTrayCommands();

    // qDebug()<<dirCommands;
}

void SMenu::paintEvent(QPaintEvent *event)
{

    // qDebug()<<parentWidget()->metaObject()->className();
    QColor tem = themeColor();
    // qDebug()<<mapToGlobal(QPoint(0,0));
    QPainter painter(this);
    tem.setAlpha(arect->nowAlpha);
    painter.setBrush(tem);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    QMenu::paintEvent(event);

}

void SMenu::resizeEvent(QResizeEvent *event)
{

    rs->updateDisplay();
    QMenu::resizeEvent(event);
}

void SMenu::closeEvent(QCloseEvent *event)
{
    // emit aboutToHide();
    QMenu::closeEvent(event);
}

void SMenu::showEvent(QShowEvent *event)
{

    // qDebug() << "ShowSize" << size();
    // qDebug() << "ShowPos" << pos();
    emit aboutToShow();

    if(ismain) {
        activepmw->pls->clearTooltip();
        // if(title()!=""){

        // SToolTip* tip = SToolTip::tip(title(),pos(),false,true);
        //     connect(this,&QMenu::aboutToHide,tip,&SToolTip::end);
        // }
    }

    if(firstShow) {
        qDebug() << "firstShow" << size();
        aimSize = size();
        firstShow = false;
    }

    previousPos = pos();
    arect->setStartValue(QPoint(0, 0),
                         QSize(aimSize.width() * 0.5, 1),
                         0, 0);

    arect->setEndValue(QPoint(0, 0),
                       QSize(aimSize.width(), aimSize.height()),
                       240, 10);


    arect->start();
    setFixedSize(1, 1);
    setVisible(true);
    show();
    QMenu::showEvent(event);
}
