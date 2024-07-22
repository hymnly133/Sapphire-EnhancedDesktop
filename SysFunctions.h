#ifndef SYSFUNCTIONS_H
#define SYSFUNCTIONS_H
#include<windows.h>
#include <QIcon>
#include "layershower.h"
#include "mainwindow.h"
#include "qdesktopwidget.h"
#include "qfileinfo.h"


extern QMap<int,MainWindow*> pmws;
extern QMap<int,QScreen*> pscs;
extern QDesktopWidget* pdt;
extern LayerShower* pls;
extern StyleHelper* psh;
extern ED_Unit* pMovingUnit;
extern int screenNum;
extern QMap<int,QJsonObject> UnusedJsons;

// static void HighDpiAdapt();

void SetUp();


QString Unicode2GBK(QString c);
QString Unicode2Utf(QString c);
QString GetCorrectUnicode(const QByteArray &ba);
QString elidedLineText(QWidget *pWidget, int nLine, QString strText);
extern QString* UserDesktopPath;
extern QString* PublicDesktopPath;
extern bool onLoading;


QVector<MyFileInfo> scanalldesktopfiles();
QVector<MyFileInfo>getFormFileInfo(QFileInfo x);

void paintRect(QWidget* aim,QColor color);
void repaintAround(QWidget* aim);

void inplace(QWidget* aim);
void inplace2(QWidget* aim);
QString  getDesktopPath();
void sentToWallpaper(QPoint winpos);
QColor pixmapMainColor(QPixmap p, double bright);
void paintSide(QWidget* aim,QColor color);
void paintLight(QWidget* aim,QColor color);
void customMessageHandler(QtMsgType type,
                          const QMessageLogContext &context,
                          const QString &msg);

void writeJson();
QMap<int,QJsonObject> readJson();
void setMyAppAutoRun(bool isStart);
ED_Unit* from_json(QJsonObject data, MainWindow *parent);
QString toWindowsPath(QString const& linuxPath);
QString toLinuxPath(QString const& windowsPath);


#endif // SYSFUNCTIONS_H
