#include "global.h"
#include"QMap"
#include "qicon.h"
#include "sdir.h"
#include<QJsonObject>
bool firstNotice = true;
bool init = false;
bool isQuit = false;
QMap<int, MainWindow*> pmws;
MainWindow* activepmw = nullptr;
SUnit* processor;
QMap<int, QScreen*> pscs;
int screenNum;
int jsonNum;
StyleHelper* psh;
QDesktopWidget* pdt;
QString UserDesktopPath;
QString PublicDesktopPath;
bool moving_global;
QList<SUnit*> pCelectedUnits;
SUnit* pFocusedUnit = nullptr;
bool onLoading = true;
QMap<QString, SFile*> nowExitFiles;
QMap<QString, SFileInfo*> nowExitInfos;
QMap<QString, SDir*> nowExitDirs;
QMap<int, QJsonObject> UnusedJsons;
QList<QString> ExcludeFiles;
IconReader* pir;
bool editMode = false;
QIcon appIcon;
SMenu* trayMenu;
QList<QString> containerTypes;
bool isAdmin;
bool isDebug = true;
