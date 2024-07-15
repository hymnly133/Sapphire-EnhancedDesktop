#include "SysFunctions.h"
#include "ed_block.h"
#include "ed_container.h"
#include "ed_dock.h"
#include "ed_editbox.h"
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
    qRegisterMetaType<ED_Container>();
    qRegisterMetaType<ED_HideTextBlock>();
    qRegisterMetaType<ED_Dock>();
    qRegisterMetaType<ED_EditBox>();
    qRegisterMetaType<Hitokoto>();
    qRegisterMetaType<Weather>();



    #ifndef QT_DEBUG
    qInstallMessageHandler(customMessageHandler);
    // setMyAppAutoRun(true);
    #endif

    readStyleIni();

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Enhanced-desktop_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow mw ;
    //鼠标钩子
    // InitMouseHook();

    mw.show();

    a.exec();

    writeStyleIni();

    writeJson();


    return 0;
}
