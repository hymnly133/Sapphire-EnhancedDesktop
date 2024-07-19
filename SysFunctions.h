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

// static void HighDpiAdapt();

void SetUp();


QString Unicode2GBK(QString c);
QString Unicode2Utf(QString c);
QString GetCorrectUnicode(const QByteArray &ba);
QString elidedLineText(QWidget *pWidget, int nLine, QString strText);
extern QString* UserDesktopPath;
extern QString* PublicDesktopPath;



struct MyFileInfo
{
    //定义返回的结构体
    enum TYPE{
        SINGLE = 0,
        MULTI =1
    };
    TYPE type;
    QString name;
    QString filePath;
    QMap<int,QPixmap> icons;
    bool operator<(MyFileInfo& another) const{
        return type<another.type;
    }
    MyFileInfo(QString path,int size=512);
    MyFileInfo(QFileInfo qfi,int size = 512);
    QPixmap aimIcon(){
        if(okForAim())
            return icons[default_steam_icon_type];
        else{
            qDebug()<<"No aim icon!,use default";
            return icons[0];
        }
    }
    bool okForAim(){
        return icons.contains(default_steam_icon_type);
    }
};

QVector<MyFileInfo> scanalldesktopfiles();
QVector<MyFileInfo>getFormFileInfo(QFileInfo x);
void paintRect(QWidget* aim,QColor color);
void repaintAround(QWidget* aim);
void inplace(QWidget* aim);
void inplace2(QWidget* aim);
QString  getDesktopPath();
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
QColor GetWindowsThemeColor();
QString toWindowsPath(QString const& linuxPath);
QString toLinuxPath(QString const& windowsPath);

MyFileInfo path2MyFI(QString path,int size=512);
QMap<int,QPixmap> path2Icon(QString path,int size=512);
QString path2Name(QString path);
#endif // SYSFUNCTIONS_H
