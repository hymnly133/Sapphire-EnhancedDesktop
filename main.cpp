#include "SysFunctions.h"
#include "sfile.h"
#include "sblockcontainer.h"
#include "sdock.h"
#include "seditbox.h"
#include "layershower.h"
#include "mainwindow.h"
#include<windows.h>
#include <QJsonObject>
#include"shidetextblock.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include"QMessageBox"
#include"QDesktopWidget"
#include "qscreen.h"
#include "qsharedmemory.h"
#include "repaintcounterunit.h"
#include "sshellfuncunit.h"


int main(int argc, char *argv[])
{


    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // 除以96之后即可转换成dpi的数值,以2k屏幕为例,Windows默认dpi是125%,这里的值就是:1.25


    QApplication a(argc, argv);
    double dpi  = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 96;
    qDebug()<<dpi;
    // eApp->init();




    static QSharedMemory *shareMem = new QSharedMemory("Sapphire"); //创建“SingleApp”的共享内存块
    if (!shareMem->create(1))//创建大小1b的内存
    {
        QMessageBox::about(NULL, "提示", "已经打开了另一个程序！");
        qApp->quit();
        return -1;
    }



    QFont font = qApp->font();
    font.setPointSize(10);
    // font.setFamily("MiSans");
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

    StyleHelper sh = StyleHelper();
    sh.readStyleIni();

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
    a.exec();
    sh.writeStyleIni();


    return 0;
}
