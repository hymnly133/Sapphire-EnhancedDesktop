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
#include"QDesktopWidget"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<ED_Unit>();
    qRegisterMetaType<ED_Block>();
    qRegisterMetaType<ED_BlockContainer>();
    qRegisterMetaType<ED_HideTextBlock>();
    qRegisterMetaType<ED_Dock>();
    qRegisterMetaType<ED_EditBox>();
    qRegisterMetaType<Hitokoto>();
    qRegisterMetaType<Weather>();

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    #ifndef QT_DEBUG

    debug = false;
    qInstallMessageHandler(customMessageHandler);

    #endif

    readStyleIni();

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Sapphire_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow mw ;
    LayerShower ls;
    //鼠标钩子
    // InitMouseHook();

    mw.show();
    ls.show();

    a.exec();



    return 0;
}
