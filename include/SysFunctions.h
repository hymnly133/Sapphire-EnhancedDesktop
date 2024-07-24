#ifndef SYSFUNCTIONS_H
#define SYSFUNCTIONS_H
#include<windows.h>
#include <QIcon>
#include "layershower.h"
#include "qdesktopwidget.h"
#include "qfileinfo.h"
#include"filefunc.h"
#include "sunit.h"
class MainWindow;
class SFile;

extern bool init;
extern bool firstNotice;
extern bool isQuit;
extern QMap<int,MainWindow*> pmws;
extern QMap<int,QScreen*> pscs;
extern QDesktopWidget* pdt;
extern LayerShower* pls;
extern StyleHelper* psh;
extern SUnit* pMovingUnit;
extern int screenNum;
extern QMap<int,QJsonObject> UnusedJsons;
extern QMap<QString,SFile*> nowExits;

// static void HighDpiAdapt();

void SetUp();


QString Unicode2GBK(QString c);
QString Unicode2Utf(QString c);
QString GetCorrectUnicode(const QByteArray &ba);
QString elidedLineText(QWidget *pWidget, int nLine, QString strText);
extern QString* UserDesktopPath;
extern QString* PublicDesktopPath;
extern bool onLoading;

QList<SUnit*> units();

QList<MyFileInfo> scanalldesktopfiles();
QList<MyFileInfo>getFormFileInfo(QFileInfo x);

void paintRect(QWidget* aim,QColor color);
void repaintAround(QWidget* aim);

void inplace(QWidget* aim);
void inplace2(QWidget* aim);
QString  getDesktopPath();
void sentToWallpaper(QPoint winpos);
QColor pixmapMainColor(QPixmap p, double bright  =1);
void paintSide(QWidget* aim,QColor color);
void paintLight(QWidget* aim,QColor color);
void customMessageHandler(QtMsgType type,
                          const QMessageLogContext &context,
                          const QString &msg);

void writeJson();
QMap<int,QJsonObject> readJson();
void setMyAppAutoRun(bool isStart);
SUnit* from_json(QJsonObject data, SLayout *parent);
QString toWindowsPath(QString const& linuxPath);
QString toLinuxPath(QString const& windowsPath);

bool isPic(QString pah);

QString shellrun(QString path,QString para = "");

void scanForChange();
#endif // SYSFUNCTIONS_H
