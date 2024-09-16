#ifndef GLOBAL_H
#define GLOBAL_H

#include "qapplication.h"
#include "qversionnumber.h"
#include "sfileinfo.h"
#include "smenu.h"
#define PROJECT_PATH QString("D:/Github/Sapphire-EnhancedDesktop")
#define EXE_PATH QCoreApplication::applicationDirPath()
#define CONTENT_PATH EXE_PATH + "/content.json"
#define STYLE_PATH EXE_PATH + "/style.ini"
#define DATA_PATH EXE_PATH + "/menu.json"


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
SAction *NAME = new SAction(TEXT);\
NAME->setName(TEXT);\
NAME->smenu = MENU;\
MENU->addAction(NAME);\
connect(NAME, &QAction::triggered, RECIEVER, [=]()FUCTION);

#define SET_ANCTION_MENU(NAME,TEXT,MENU,SUBMENU)\
SAction *NAME = new SAction(TEXT);\
SMenu* SUBMENU = new SMenu(MENU);\
MENU->addAction(NAME);\
NAME->setName(TEXT);\
NAME->smenu = MENU;\
SUBMENU->path = NAME->path();\
NAME->setMenu(SUBMENU);\



#define connectTo(NAME,TYPE,VALTYPE,FUNC)\
connect(psh->TYPE##Val(#NAME),&TYPE##Val::valueChanged,this,[=](VALTYPE value)FUNC);


#define MultiScreen bool(screenNum>1)

class MainWindow;
class SUnit;
class SFile;
class SMenu;
class StyleHelper;
class IconReader;
class SDir;

extern HWND shelldlldefview;
extern HWND progrmman;


//标识初始化
extern bool init;

extern QString UserDesktopPath;
extern QString PublicDesktopPath;
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
extern QMap<QString, SFile*> nowExitFiles;
extern QMap<QString, SFileInfo*> nowExitInfos;
extern QMap<QString, SDir*> nowExitDirs;
//标记排除文件
extern QList<QString> ExcludeFiles;
//多文件菜单
extern SMenu* unitsMenu;
//标记两种模式
extern bool editMode;
//全局自定义图标读取器
extern IconReader* pir;
//软件图标的QIcon类型
extern QIcon appIcon;
//托盘菜单
extern SMenu* trayMenu;
//用于标记所有含有容器的class
extern QList<QString> containerTypes;

//标记是否为管理员
extern bool isAdmin;
//标记是否为调试模式
extern bool isDebug;

//全局MenuHelper
extern MenuHelper* pmh;
//当前版本号
extern QVersionNumber version;
//标记是否为自启动
extern bool isAutoStart;
#endif // GLOBAL_H
