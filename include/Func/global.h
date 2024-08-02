#ifndef GLOBAL_H
#define GLOBAL_H

#include "mainwindow.h"

#define numCelected pCelectedUnits.size()
#define AUTO_RUN_KEY	"HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run"
#define AUTO_RUN_KEY_OLD	"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define NO_POS QPoint(-1,-1)
#define NO_IND QPoint(-1,-1)
#define SAPPHIRE_REG_PATH "HKEY_CURRENT_USER\\Software\\Hymnly\\Sapphire"
#define SET_ANCTION(NAME,TEXT,MENU,RECIEVER,FUCTION)\
QAction *NAME = new QAction(#TEXT);\
    MENU->addAction(NAME);\
    connect(NAME, &QAction::triggered, RECIEVER, [=]()FUCTION);

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
extern QMap<int,MainWindow*> pmws;
extern MainWindow* activepmw;
//Screens
extern QMap<int,QScreen*> pscs;
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
extern QMap<int,QJsonObject> UnusedJsons;
//目前存在的SFile
extern QMap<QString,SFile*> nowExits;

extern QList<QString> ExcludeFiles;
extern SMenu* unitsMenu;
extern bool editMode;
#endif // GLOBAL_H
