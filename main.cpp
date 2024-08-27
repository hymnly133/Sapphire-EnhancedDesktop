#include "SysFunctions.h"
#include "global.h"
#include "qtextcodec.h"
#include "sdir.h"
#include "sfile.h"
#include "sblockcontainer.h"
#include "sdock.h"
#include "seditbox.h"
#include "mainwindow.h"
#include <shlobj.h>
#include<windows.h>
#include <QJsonObject>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include"QMessageBox"
#include"QDesktopWidget"
#include "qscreen.h"
#include "qsharedmemory.h"
#include "repaintcounterunit.h"
#include "sglshower.h"
#include "sshellfuncunit.h"
#include"QProcess"
#include "userfunc.h"
#include "QBreakpadHandler.h"
int main(int argc, char *argv[])
{
#ifndef QT_DEBUG
    //安装log重定向
    qInstallMessageHandler(customMessageHandler);
    isDebug = false;
#endif
    //环境与系统性设置
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(code);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
    QBreakpadInstance.setDumpPath("crashes");
    qDebug() << "Sapphire Startup";


    static QSharedMemory *shareMem = new QSharedMemory("Sapphire"); //创建“SingleApp”的共享内存块
    if (!shareMem->create(1)) { //创建大小1b的内存
        QMessageBox::about(NULL, "提示", "已经打开了另一个程序！");
        qApp->quit();
        return -1;
    }

    //元对象注册
    qRegisterMetaType<SUnit>();
    qRegisterMetaType<SFile>();
    qRegisterMetaType<SContainer>();
    qRegisterMetaType<SBlockContainer>();
    qRegisterMetaType<SShellFuncUnit>();
    qRegisterMetaType<SDock>();
    qRegisterMetaType<SEditBox>();
    qRegisterMetaType<RepaintCounterUnit>();
    qRegisterMetaType<SGLShower>();
    qRegisterMetaType<SDir>();


    //注册容器对象
    initContainerTypes();

    //翻译
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Sapphire_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }



    preSetupG();

    // if(isQuit) {
    //     qApp->quit();
    //     return -1;
    // }
    int ret = a.exec();

    if(ret == 733) {
        shareMem->detach();
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
        return 0;
    }
    return 0;
}
