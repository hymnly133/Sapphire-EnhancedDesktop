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

//全局StyleHelper
extern StyleHelper* psh;
//移动中的Unit(待重构
extern bool moving_global;
extern SUnit* processor;
extern QList<SUnit*> pCelectedUnits;
//屏幕数量
extern int screenNum;
//未使用的Json数据
extern QMap<int,QJsonObject> UnusedJsons;
//目前存在的SFile
extern QMap<QString,SFile*> nowExits;

// static void HighDpiAdapt();

void SetUp();


//处理多行显示
QString elidedLineText(QWidget *pWidget, int nLine, QString strText);

//获取所有Unit
QList<SUnit*> units();

//扫描所有文件
QList<MyFileInfo> scanalldesktopfiles();
//从QFileInfo转化
QList<MyFileInfo>getFormFileInfo(QFileInfo x);

//绘制矩形/边框/光效

void paintRect(QWidget* aim,QColor color);
void paintSide(QWidget* aim,QColor color);
void paintLight(QWidget* aim,QColor color);
//在组件附近重绘
void repaintAround(QWidget* aim);
//接入图标层
void inplace(QWidget* aim);

void positionToScreen(QWidget* aim, int screenInd);
//向Wallpaper发送鼠标点击信号（待修复
void sentToWallpaper(QPoint winpos);
//获取主题颜色
QColor pixmapMainColor(QPixmap p, double bright  =1);

//处理qDebug
void customMessageHandler(QtMsgType type,
                          const QMessageLogContext &context,
                          const QString &msg);

//写入Json
void writeJson();
//读取Json
QMap<int,QJsonObject> readJson();
//设置自启
void setMyAppAutoRun(bool isStart);
//从Json数据生成并加载
SUnit* from_json(QJsonObject data, SLayout *parent);

//系统路径转化
QString toWindowsPath(QString const& linuxPath);
QString toLinuxPath(QString const& windowsPath);

//判断是否是图片
bool isPic(QString pah);

void resizeForActiveMW();

//调用WindowsAPI启动
QString shellrun(QString path,QString para = "");

//扫描文件改动
void scanForChange();

//拖出
void dragOut();
//移动选择的组件
void moveCelect();
//清楚选择的组件
void cleanCelect();
//释放选择的组件
void releaseCelect();

QRect Point2Rect(QPoint point0,QPoint point1);

QPair<SLayout *, QPoint> deepFind(SUnit* aim);
#endif // SYSFUNCTIONS_H
