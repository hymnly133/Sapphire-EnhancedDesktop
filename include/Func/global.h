#ifndef GLOBAL_H
#define GLOBAL_H

#include "qapplication.h"
#define PROJECT_PATH QString("D:/Github/Sapphire-EnhancedDesktop")
#define EXE_PATH QCoreApplication::applicationDirPath()

#ifdef QT_DEBUG
    #define xmlDirPath PROJECT_PATH+"/xml"
    #define xlsxDirPath  PROJECT_PATH+"/xlsx"
    #define iconDirPath  PROJECT_PATH+"/icon"
#else
    #define xmlDirPath EXE_PATH+"/xml"
    #define xlsxDirPath EXE_PATH+"/xlsx"
    #define iconDirPath EXE_PATH+"/icon"
#endif

#define numCelected pCelectedUnits.size()
#define AUTO_RUN_KEY	"HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run"
#define AUTO_RUN_KEY_OLD	"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define NO_POS QPoint(-1,-1)
#define NO_IND QPoint(-1,-1)
#define SAPPHIRE_REG_PATH "HKEY_CURRENT_USER\\Software\\Hymnly\\Sapphire"
#define SET_ANCTION(NAME,TEXT,MENU,RECIEVER,FUCTION)\
QAction *NAME = new QAction(TEXT);\
    MENU->addAction(NAME);\
    connect(NAME, &QAction::triggered, RECIEVER, [=]()FUCTION);

#define connectTo(NAME,TYPE,VALTYPE,FUNC)\
connect(psh->TYPE##Val(#NAME),&TYPE##Val::valueChanged,this,[=](VALTYPE value)FUNC);

class MainWindow;
class SUnit;
class SFile;
class SMenu;
class StyleHelper;
class IconReader;
//标识初始化
extern bool init;

extern QString* UserDesktopPath;
extern QString* PublicDesktopPath;
//标识加载中
extern bool onLoading;
//标识第一次通知
extern bool firstNotice;
//标识加载失败即将退出
extern bool isQuit;
//MainWindows
extern QMap<int, MainWindow*> pmws;
extern MainWindow* activepmw;
//Screens
extern QMap<int, QScreen*> pscs;
//desktop
extern QDesktopWidget* pdt;
extern SUnit* pFocusedUnit;
//全局StyleHelper
extern StyleHelper* psh;
//移动中的Unit(待重构
extern bool moving_global;
extern SUnit* processor;
extern QList<SUnit*> pCelectedUnits;
//屏幕数量
extern int screenNum;
//布局文件数量
extern int jsonNum;
//未使用的Json数据
extern QMap<int, QJsonObject> UnusedJsons;
//目前存在的SFile
extern QMap<QString, SFile*> nowExits;

extern QList<QString> ExcludeFiles;
extern SMenu* unitsMenu;
extern bool editMode;
extern IconReader* pir;
extern QIcon appIcon;

#endif // GLOBAL_H
