#ifndef SYSFUNCTIONS_H
#define SYSFUNCTIONS_H
#include<windows.h>
#include <QIcon>
class MainWindow;
class SFile;



//处理多行显示
QString elidedLineText(QWidget *pWidget, int nLine, QString strText);

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

//计算对角距离
double rectLen(int w,int h);

//系统路径转化
QString toWindowsPath(QString const& linuxPath);
QString toLinuxPath(QString const& windowsPath);

//调用WindowsAPI启动
QString shellrun(QString path,QString para = "");

//计算两个点之间的方形
QRect Point2Rect(QPoint point0,QPoint point1);


#endif // SYSFUNCTIONS_H
