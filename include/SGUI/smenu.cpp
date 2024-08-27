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
#include"QJsonArray"
#include"QJsonDocument"
#define REG_Dir "HKEY_CLASSES_ROOT\\Directory"
#define REG_Folder "HKEY_CLASSES_ROOT\\Folder"
#define REG_Files "HKEY_CLASSES_ROOT"
#define ADD_SEPARATOR(LIST)\
if(!LIST.empty()&&LIST.last().name!="Separator")\
    LIST.append(SActionInfo::sparetor());\


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
    connect(arect, &SAnimationRect::animationUpdating, this, [ = ]()
    {
        move(previousPos + arect->nowPos);
        setFixedSize(arect->nowSize);
        rs->raise();
        update();
    });
    connect(this, &QMenu::aboutToHide, this, [ = ]()
    {

    });
    init();
    // adjustSize();
}

void SMenu::addAction(QAction *act)
{
    QMenu::addAction(act);
    adjustSize();
}

QMap<QString, SAction *> SMenu::sacts()
{
    QMap<QString, SAction *> res;
    actions();
    foreach (QAction* act, actions()) {
        SAction* sact = (dynamic_cast<SAction*>(act));
        // qDebug() << sact->name;
        res.insert(sact->name, sact);
    }
    return res;
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

void SMenu::raiseAction(SAction *action)
{
    QList<QAction*> actList = this->actions();
    actList.removeOne(action);
    foreach (QAction* act, actList) {
        QMenu::removeAction(act);
    }
    foreach (QAction* act, actList) {
        addAction(act);
    }
}

void SMenu::raiseAction(QString actname)
{
    QMap<QString, SAction*> actList = this->sacts();
    // foreach (auto act, actList) {

    // }
    actList.remove(actname);
    foreach (QAction* act, actList.values()) {
        QMenu::removeAction(act);
    }
    foreach (QAction* act, actList.values()) {
        addAction(act);
    }
}

SAction* SMenu::removeAction(QString actname)
{
    auto res =  action(actname);
    QMenu::removeAction(res);
    return res;
}

SAction *SMenu::action(QString actname)
{
    foreach (auto act, sacts()) {
        if(act->name == actname) {
            return act;
        }
    }
    return nullptr;
}

void SMenu::hideAction(QString actname)
{
    hideAction(action(actname));
}

void SMenu::hideAction(SAction *act)
{
    act->setVisible(false);
    adjustSize();
}

void SMenu::showAction(QString actname)
{
    showAction(action(actname));
}

void SMenu::showAction(SAction *act)
{
    act->setVisible(true);
    adjustSize();
}

void SMenu::adjustSize()
{
    qDebug() << "adjustsize";
    setMinimumSize(0, 0);
    setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    QMenu::adjustSize();
    aimSize = size();
    // setFixedSize(siz)
}

void SMenu::exec(QPoint pos)
{
    // foreach (auto act, sacts()) {
    //     qDebug() << act->path();
    // }
    qDebug() << sacts();
    QMenu::exec(pos);
}


void SMenu::addShellCommand(const SActionInfo& info)
{
    if(info.name == "Separator") {
        sysComMenu->addSeparator();
        return;
    }
    if(sacts().contains(info.name)) {
        return;
    }

    SAction* thisact = SAction::fromInfo(&info);
    sysComMenu->addAction(thisact);
    thisact->smenu = this;


    thisact->setIcon(pmh->icons[info.id]);
}

void SMenu::addDirCommands()
{
    checkSysMenu();
    foreach (auto dircommand, pmh->dirCommands) {
        addShellCommand(dircommand);
    }
    checkSysMenu();
}

void SMenu::addDirBGCommands()
{
    checkSysMenu();
    foreach (auto dirBGcommand, pmh->dirBGCommands) {
        addShellCommand(dirBGcommand);
    }
    checkSysMenu();
}

void SMenu::addDesktopCommands()
{
    checkSysMenu();
    foreach (auto desktopCommand, pmh->desktopCommands) {
        addShellCommand(desktopCommand);
    }
    checkSysMenu();
}

void SMenu::addFileCommands(QString suffix)
{
    checkSysMenu();
    if(pmh->filesCommands.contains("*")) {
        foreach (auto val, (*(pmh->filesCommands.value("*")))) {
            addShellCommand(val);
        }
    }
    sysComMenu->addSeparator();
    if(pmh->filesCommands.contains(suffix)) {
        foreach (auto filecommand, *(pmh->filesCommands.value(suffix))) {
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
        SET_ANCTION_MENU(syscomAct_, tr("更多"), this, sysComMenu_);

        sysComMenu = sysComMenu_;
        syscomAct = syscomAct_;
        added = true;
    } else {

        //清楚空的菜单
        if(sacts().empty()) {
            syscomAct->setVisible(false);
        }
    }

}

void MenuHelper::readDirReg(QString path)
{
    QSettings settingdir(path, QSettings::NativeFormat);
    qDebug() << "start to Read dirRegs" << path;

    if(settingdir.childGroups().contains("Background")) {
        readShellReg(path + "\\Background", dirBG);
    }



    ADD_SEPARATOR(dirCommands)
    ADD_SEPARATOR(dirBGCommands)
    if(settingdir.childGroups().contains("shell")) {
        readShellReg(path, dir);
    }
}

void MenuHelper::readFileReg(QString group)
{

    QSettings settingdir(QString(REG_Files) + "\\" + group, QSettings::NativeFormat);
    // qDebug()<<settingdir.childGroups();

    if(settingdir.childGroups().contains("shell")) {
        qDebug() << "FileReg" << group;
        readShellReg(QString(REG_Files) + "\\" + group, file);
    }
}

void MenuHelper::readFilesReg()
{
    QSettings settingdir(REG_Files, QSettings::NativeFormat);
    foreach(QString group, settingdir.childGroups()) {
        if(group == "*" || group[0] == ".") {
            readFileReg(group);
        }
    }
}

void MenuHelper::readShellReg(QString path, CommandType type)
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

void MenuHelper::readAReg(QString path, CommandType type)
{
    //path : XX/shell/xx
    QSettings settings(path, QSettings::NativeFormat);
    QString displayName = path.split('\\').last();
    QString id = displayName;
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
            icons.insert(id, icon);
        } else if(iconPath.split(",").size() == 2) {
            icon = getShellIcon(iconPath.split(",")[0], iconPath.split(",")[1].toInt());
            if(!icon.isNull()) {
                icons.insert(id, icon);
            }
        }
    }

    foreach(QString group, settings.childGroups()) {
        //含有命令，进行处理
        if(group.toLower() == "command") {
            settings.beginGroup(group);
            QString com = settings.value(".").toString();
            group2name.insert(group, displayName);
            SActionInfo thisinfo;
            thisinfo.name = displayName;
            thisinfo.id = id;
            thisinfo.command = com;

            switch (type) {
                case dir:

                    addToListFromInfo(dirCommands, thisinfo);
                    // dirCommands.append(QPair(displayName, thisinfo));
                    break;

                case dirBG:
                    addToListFromInfo(dirBGCommands, thisinfo);
                    break;

                case file:
                    auto list = path.split('\\');
                    QString suffix = list[1];
                    suffix.remove('.');
                    if(!filesCommands.contains(suffix)) {
                        filesCommands.insert(suffix, new QList<SActionInfo>);
                    }
                    addToListFromInfo(*filesCommands[suffix], thisinfo);
                    break;

            }
        }
    }
}

void MenuHelper::scanSysCommands()
{
    //此处是各种文件（夹）菜单
    ignoreCommands << "find" << "UpdateEncryptionSettings" << "explore" << "open" << "opennewprocess" << "opennewtab" << "opennewwindow" << "pintohome" << "pintohomefile";
    readDirReg(REG_Dir);
    ADD_SEPARATOR(dirCommands)
    ADD_SEPARATOR(dirBGCommands)
    readDirReg(REG_Folder);
    readFilesReg();

    //此处是托盘图标菜单
    trayMenu = new SMenu();
    trayMenu->addTrayCommands();

    // qDebug()<<dirCommands;
}

QJsonArray MenuHelper::list2json(const QList<SActionInfo >& list)
{
    QJsonArray arr;
    foreach (auto data, list) {
        arr.append(data.to_json());
    }
    return arr;
}

void MenuHelper::json2list(QList<SActionInfo> &list, QJsonArray arr)
{
    foreach (auto data, arr) {
        SActionInfo info;
        info.load_json(data.toObject());
        addToListFromInfo(list, info);
    }
}


void MenuHelper::write_json()
{
    QJsonObject rootObject;
    rootObject.insert("dir", list2json(dirCommands));
    rootObject.insert("dirBG", list2json(dirBGCommands));
    rootObject.insert("desktop", list2json(desktopCommands));

    QJsonArray filesArr;
    foreach (QString suffix, filesCommands.keys()) {
        QJsonObject thisSufObj;
        thisSufObj.insert("suffix", suffix);
        thisSufObj.insert("commands", list2json(*filesCommands[suffix]));
        filesArr.append(thisSufObj);
    }

    rootObject.insert("files", filesArr);

    QJsonDocument document;
    document.setObject(rootObject);

    QByteArray byte_array = document.toJson(QJsonDocument::Indented);
    QString json_str(byte_array);
    QFile file(DATA_PATH);


    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "file error!";
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    in << json_str;

    file.close();   // 关闭file
}

void MenuHelper::read_json(QJsonObject data)
{
    QJsonObject rootObject;
    if(data.contains("dir")) {
        json2list(dirCommands, data.value("dir").toArray());
    }
    if(data.contains("dirBG")) {
        json2list(dirBGCommands, data.value("dirBG").toArray());
    }
    if(data.contains("desktop")) {
        json2list(desktopCommands, data.value("desktop").toArray());
    }

    if(data.contains("files")) {
        foreach (auto val, data.value("files").toArray()) {
            QJsonObject thisSufObj = val.toObject();
            QString suffix = thisSufObj.value("suffix").toString();
            if(!filesCommands.contains(suffix)) {
                {
                    filesCommands.insert(suffix, new QList<SActionInfo>);
                }
                json2list(*filesCommands[suffix], thisSufObj.value("commands").toArray());
            }
        }
    }
}

void MenuHelper::addToListFromInfo(QList<SActionInfo>  &list, SActionInfo &info)
{
    foreach (auto thisinfo, list) {
        if(thisinfo.id == info.id) {
            return;
        }
    }

    if((&list == &dirCommands) || (&list == &desktopCommands)) {
        foreach (auto thisinfo, dirBGCommands) {
            if(thisinfo.id == info.id) {
                return;
            }
        }
    }


    if(!info.id.isEmpty()) {
        list.append( info);
    } else {
        list.append( info);
    }

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

    emit aboutToShow();

    if(ismain) {
        activepmw->pls->clearTooltip();
        // if(title()!=""){

        // SToolTip* tip = SToolTip::tip(title(),pos(),false,true);
        //     connect(this,&QMenu::aboutToHide,tip,&SToolTip::end);
        // }
    }

    if(firstShow || alwaysRequireRefresh) {
        adjustSize();
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

QJsonObject SActionInfo::to_json()
{
    QJsonObject root;
    root.insert("name", name);
    if(name != "Separator") {
        root.insert("id", id);
        if(!command.isEmpty()) {
            root.insert("command", command);
        }
        root.insert("visible", visible);
    }
    return root;
}

void SActionInfo::load_json(QJsonObject root)
{
    name = root.value("name").toString();
    if(root.contains("command")) {
        command = root.value("command").toString();
    }
    if(root.contains("id")) {
        id = root.value("id").toString();
    }
    visible = root.value("visible").toBool();
}

SActionInfo SActionInfo::sparetor()
{
    SActionInfo info;
    info.name = "Separator";
    return info;
}

MenuHelper::MenuHelper(QObject *parent): QObject(parent)
{
    pmh = this;
    tray =  new SActionField(tr("托盘"), "tray");
    setting =  new SActionField(tr("系统设置"), "setting");
    create = new SActionField(tr("新建"), "create");
    more = new SActionField(tr("更多"), "more");
}
