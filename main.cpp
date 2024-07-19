#include "SysFunctions.h"
#include "ed_block.h"
#include "ed_blockcontainer.h"
#include "ed_dock.h"
#include "ed_editbox.h"
#include "layershower.h"
#include "mainwindow.h"
#include<windows.h>
#include <QJsonObject>
#include"ed_hidetextblock.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include"QMessageBox"
#include"QDesktopWidget"
#include "qscreen.h"
#include "qsharedmemory.h"


int main(int argc, char *argv[])
{
    // QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    QApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication a(argc, argv);

    // 获取多显示器,通过list存储当前主机所有显示器


    static QSharedMemory *shareMem = new QSharedMemory("Sapphire"); //创建“SingleApp”的共享内存块
    if (!shareMem->create(1))//创建大小1b的内存
    {
        QMessageBox::about(NULL, "提示", "已经打开了另一个程序！");
        qApp->quit();
        return -1;
    }

    qRegisterMetaType<ED_Unit>();
    qRegisterMetaType<ED_Block>();
    qRegisterMetaType<ED_BlockContainer>();
    qRegisterMetaType<ED_HideTextBlock>();
    qRegisterMetaType<ED_Dock>();
    qRegisterMetaType<ED_EditBox>();

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

    a.exec();
    sh.writeStyleIni();


    return 0;
}
