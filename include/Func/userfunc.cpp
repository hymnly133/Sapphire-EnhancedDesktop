#include "userfunc.h"
#include "SysFunctions.h"
#include "filefunc.h"
#include "global.h"
#include "qclipboard.h"
#include "qdir.h"
#include "qfuture.h"
#include "qmimedata.h"
#include "qscreen.h"
#include "qstandardpaths.h"
#include "qtconcurrentrun.h"
#include "qthreadpool.h"
#include "sblocklayoutsettingwindow.h"
#include "screenfunc.h"
#include"QInputDialog"
#include"QMessageBox"
#include "sfile.h"
#include"snotice.h"
#include "unitfunc.h"


void SetUp()
{
    QString application_name = QApplication::applicationName();//获取应用名称
    QSettings *settings = new QSettings(AUTO_RUN_KEY, QSettings::NativeFormat);//创建QSetting, 需要添加QSetting头文件
    enable_auto_run = !settings->value(application_name).isNull();
    initiateDesktop();
    ExcludeFiles.append("desktop.ini");
    qDebug() << "Setting Up...";
    UserDesktopPath = new  QString(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation)[0]);
    qDebug() << "User Desktop:" << *UserDesktopPath;
    PublicDesktopPath = new QString("C:/Users/Public/Desktop");
    qDebug() << "Public Desktop:" << *PublicDesktopPath;
    pdt = QApplication::desktop();
    pscs[0] = QGuiApplication::primaryScreen();
    Shift_Global = -pscs[0]->virtualGeometry().topLeft();
    // 通过循环可以遍历每个显示器
    QList<QScreen *> list_screen = QGuiApplication::screens();
    screenNum = list_screen.size();
    for (int i = 0; i < screenNum; i++) {
        QScreen * qs = list_screen.at(i);
        QRect rect = qs->geometry();
        qDebug() << "Setting Screen" << i << rect << (pscs[0] == qs);
        pscs[i] = list_screen[i];
    }
    qDebug() << "Setting MainWinodws";
    setupMainWindows();
    loadMainWindows();
    qDebug() << "Final:";
    for(int i = 0; i < screenNum; i++) {
        qDebug() << "Mainwindow" << i << pmws[i]->mapToGlobal(QPoint(0, 0));
    }
    activepmw = pmws[0];
    if(init) {
        SNotice::notice(QStringList() << "为了您拥有更好的体验，Sapphire编写了使用手册" << "您可以在软件目录内找到", "欢迎使用Sapphire！", 15000);
    }
    onLoading  = false;
}


void checkForKey(QKeyEvent *event)
{
    //为了数据安全，在未找到完美的剪切方案前剪切由复制取代
    if( event ->matches( QKeySequence::Copy) || event->matches(QKeySequence::Cut) ) {
        QList<QUrl> copyfiles;
        foreach (SUnit* unit, pCelectedUnits) {
            if(unit->inherits("SFile")) {
                QUrl url = QUrl::fromLocalFile(((SFile*)unit)->filePath);  //待复制的文件
                if(url.isValid()) {
                    copyfiles.push_back(url);
                }
            }
        }
        qDebug() << "Copied" << copyfiles;
        QMimeData *data = new QMimeData;
        data->setUrls(copyfiles);
        QClipboard *clip = QApplication::clipboard();
        clip->setMimeData(data);
        event->accept();
        return;
    } else if( event ->matches( QKeySequence::Paste ) ) {
        QClipboard *clip = QApplication::clipboard();
        if(clip->mimeData()->hasUrls()) { //处理期望数据类型
            QList<QUrl> list = clip->mimeData()->urls();//获取数据并保存到链表中
            for(int i = 0; i < list.count(); i++) {
                QString path = list[i].toLocalFile();
                QString newPath =  okPath((*UserDesktopPath) + "/" + QFileInfo(path).fileName());
                bool Copied = false;
                if(QFileInfo(path).isFile()) {
                    Copied = QFile::copy(path, newPath);
                } else {
                    Copied = copyDir(path, newPath, true);
                }
                qDebug() << "Try to copy to" << newPath;
                if(Copied) {
                    qDebug() << "Copid";
                    activepmw->addAIcon(newPath, true, QCursor::pos());
                }
            }
        }
        event->accept();
        return;
    } else if( event->key() == Qt::Key_F2) {
        if(pFocusedUnit != nullptr) {
            if(pFocusedUnit->inherits("SFile")) {
                ((SFile*)pFocusedUnit)->renameWithDialog();
                event->accept();
            }
        } else if(numCelected == 1) {
            if(pCelectedUnits[0]->inherits("SFile")) {
                ((SFile*)pCelectedUnits[0])->renameWithDialog();
                event->accept();
            }
        }
    } else if( event->key() == Qt::Key_Delete) {
        removeG();
    } else if( event->matches(QKeySequence::Refresh) || event->key() == Qt::Key_F5) {
        foreach (auto pmw, pmws) {
            pmw->refresh();
        }
        event->accept();
    } else if( event->matches(QKeySequence::New)) {
        activepmw->creatNewFileMenu->exec(QCursor::pos());
        event->accept();
        return;
    }
    event->ignore();
    return;
}


void setMyAppAutoRun(bool isStart)
{
    QString application_name = QApplication::applicationName();//获取应用名称
    //清理旧数据
    QSettings *settingsold = new QSettings(AUTO_RUN_KEY_OLD, QSettings::NativeFormat);
    if(settingsold->contains(application_name)) {
        settingsold->remove(application_name);
    }
    QSettings *settings = new QSettings(AUTO_RUN_KEY, QSettings::NativeFormat);//创建QSetting, 需要添加QSetting头文件
    if(isStart) {
        QString application_path = QApplication::applicationFilePath();//找到应用的目录
        settings->setValue(application_name, "\"" + application_path.replace("/", "\\") + "\""); //写入注册表
    } else {
        settings->remove(application_name);		//从注册表中删除
    }
}

void setSapphireRegDate(bool isSet)
{
    QString application_name = QApplication::applicationName();//获取应用名称
    QSettings *settings = new QSettings(SAPPHIRE_REG_PATH, QSettings::NativeFormat);//创建QSetting, 需要添加QSetting头文件
    if(isSet) {
        QString application_path = QApplication::applicationFilePath();//找到应用的目录
        settings->setValue(application_name + "Path", application_path.replace("/", "\\")); //写入注册表
    } else {
        settings->remove(application_name);		//从注册表中删除
    }
}


void toDesktopMode()
{
    editMode = false;
    SNotice::notice("切换为桌面模式", "模式切换", 1000);
}

void toEditMode()
{
    editMode = true;
    SNotice::notice("切换为编辑模式", "模式切换", 1000);
}

void SExit()
{
    foreach(auto pmw, pmws) {
        pmw->close();
        if(pmw->plsBG != nullptr) {
            pmw->plsBG->close();
        }
        pmw->pls->close();
        // pmw->plsB->close();
    }
    qApp->exit(0);
}

void switchMode()
{
    if(editMode) {
        toDesktopMode();
    } else {
        toEditMode();
    }
}


void resizeForWithDialog(SBlockLayout *aimlayout)
{
    SBlockLayoutSettingWindow* dialog = new SBlockLayoutSettingWindow(aimlayout);
    dialog->show();
}

void updateFont()
{
    QFont font = qApp->font();
    font.setFamily(user_font);
    font.setPointSize(font_size);
    font.setHintingPreference(QFont::PreferNoHinting);
    qApp->setFont(font);
}

void scanForChange()
{
    if(onLoading) {
        return;
    }
    //Scan For Loss
    QList<QString> keyList = nowExits.keys();
    QMap<QString, SFile*> loss;
    QVector<QString> newFiles;
    //使用QFileInfo判断
    // foreach (QString path, keyList) {
    //     if(!QFileInfo::exists(path)){
    //         if(QFileInfo(path).isFile())continue;
    //         // if(QFileInfo(path).isShortcut())continue;
    //         if(QFileInfo(path).isSymLink())continue;
    //         loss[path] = nowExits[path];
    //     }
    //     else if(ExcludeFiles.contains( QFileInfo(path).fileName())){
    //         loss[path] = nowExits[path];
    //     }
    // }
    //使用自编写判断
    foreach (QString path, keyList) {
        if(!fileExist(path)) {
            loss[path] = nowExits[path];
        } else if(ExcludeFiles.contains( QFileInfo(path).fileName())) {
            loss[path] = nowExits[path];
        }
    }
    //Scan For New
    QDir uPathDir(*UserDesktopPath);
    qDebug() << "Scanningfor" << *UserDesktopPath;
    uPathDir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::System);
    QStringList files = uPathDir.entryList();
    foreach (QString name, files) {
        QString absolute = uPathDir.absoluteFilePath(name);
        if(!nowExits.contains(absolute)) {
            if(ExcludeFiles.contains(QFileInfo(absolute).fileName())) {
                continue;
            }
            newFiles.append(absolute);
        }
    }
    QDir pPathDir(*PublicDesktopPath);
    qDebug() << "Scanningfor" << *PublicDesktopPath;
    pPathDir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::System);
    foreach (QString name, pPathDir.entryList()) {
        QString absolute = pPathDir.absoluteFilePath(name);
        if(!nowExits.contains(absolute)) {
            if(ExcludeFiles.contains(QFileInfo(absolute).fileName())) {
                continue;
            }
            newFiles.append(absolute);
        }
    }
    QStringList removefiles;
    QList<QString> removed = loss.keys();
    foreach (QString path, removed) {
        if(nowExits.contains(path)) {
            nowExits[path]->remove();
        }
        removefiles << path;
    }
    //提示
    if(!removefiles.empty()) {
        SNotice::notice(removefiles, "移除文件", 5000);
    }
    QStringList newfiles;
    foreach (QString newfile, newFiles) {
        pmws[0]->addAIcon(newfile, true);
        pmws[0]->endUpdate();
    }
    //提示
    if(!newfiles.empty()) {
        SNotice::notice(newfiles, "新增文件", 5000);
    }
    writeJson();
}

bool loadMainWindows()
{
    QMap<int, QJsonObject> jsons = readJson();
    jsonNum = jsons.size();
    if(jsonNum == 0) {
        //初始化
        qDebug() << "No json,start initing";
        init = true;
        if(screenNum != 1) {
            QMessageBox::about(NULL, "提示", QString( "没有布局文件，即将初始化~\n你有%1个屏幕，将分别初始化\n请注意弹出窗口！").arg(screenNum));
        } else {
            QMessageBox::about(NULL, "提示", "没有布局文件，即将初始化~\n请注意弹出窗口！");
        }
        QList<MyFileInfo> iconns = scanalldesktopfiles();
        for(int i = 0; i < screenNum; i++) {
            //对于第一次初始化，由于是顺序加载，所以直接单线程加载
            iconns = pmws[i]->Init(iconns);
        }
        if(!iconns.empty()) {
            QMessageBox::about(nullptr, "(⊙o⊙)！！", QString("你选择的布局不足以容纳所有图标，请重启软件并重新选择布局\n（剩余：%1个）").arg(iconns.size()));
            isQuit = true;
            return false;
        }
    } else {
        if(jsonNum < screenNum) {
            QMessageBox::about(nullptr, "注意！", QString("存在%1个布局数据，检测到%2个屏幕，将开始初始化\n请注意弹出窗口！").arg(jsonNum).arg(screenNum));
        }
        if(jsonNum > screenNum) {
            QMessageBox::about(nullptr, "注意！", QString("存在%1个桌面数据，检测到%2个屏幕，将按顺序加载！多余的桌面数据不会被清理").arg(jsonNum).arg(screenNum));
            for(int i = screenNum; i < jsonNum; i++) {
                UnusedJsons[i] = jsons[i];
            }
        }
        //加载
        for(int i = 0; i < screenNum; i++) {
            //对于加载数据，由于数据确定，使用多线程加载
            if(jsons.contains(i)) {
                pmws[i]->load_json(jsons[i]);
            } else {
                pmws[i]->MainWindow::Init(true);
            }
        }
    }
    return true;
}

void setupMainWindows()
{
    for(int i = 0; i < screenNum; i++) {
        qDebug() << "Creating Mainwindow" << i;
        pmws[i] = new MainWindow(nullptr, i);
    }
}

