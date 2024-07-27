#include "SysFunctions.h"
#include "global.h"
#include "qtextcodec.h"
#include "sfile.h"
#include "sblockcontainer.h"
#include "sdock.h"
#include "seditbox.h"
#include "mainwindow.h"
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
#include "sshellfuncunit.h"
#include"QProcess"
#include "userfunc.h"

int main(int argc, char *argv[])
{
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(code);

    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // 除以96之后即可转换成dpi的数值,以2k屏幕为例,Windows默认dpi是125%,这里的值就是:1.25

    QApplication a(argc, argv);
    // eApp->init();




    static QSharedMemory *shareMem = new QSharedMemory("Sapphire"); //创建“SingleApp”的共享内存块
    if (!shareMem->create(1))//创建大小1b的内存
    {
        QMessageBox::about(NULL, "提示", "已经打开了另一个程序！");
        qApp->quit();
        return -1;
    }


    StyleHelper sh = StyleHelper();
    sh.readStyleIni();


    QFont font = qApp->font();
    font.setFamily(user_font);
    font.setPointSize(10);
    font.setHintingPreference(QFont::PreferNoHinting);
    qApp->setFont(font);


    qRegisterMetaType<SUnit>();
    qRegisterMetaType<SFile>();
    qRegisterMetaType<SContainer>();
    qRegisterMetaType<SBlockContainer>();
    qRegisterMetaType<SShellFuncUnit>();
    qRegisterMetaType<SDock>();
    qRegisterMetaType<SEditBox>();
    qRegisterMetaType<RepaintCounterUnit>();

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    #ifndef QT_DEBUG

    qInstallMessageHandler(customMessageHandler);

    #endif



    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Sapphire_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    SetUp();
    if(isQuit){
        qApp->quit();
        return -1;
    }



    scanForChange();
    int ret = a.exec();
    sh.writeStyleIni();

    if(ret==733){
        shareMem->detach();
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
        sh.writeStyleIni();

        return 0;


    }


    return 0;
}
