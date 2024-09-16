#ifndef SYSFUNCTIONS_H
#define SYSFUNCTIONS_H
#include<windows.h>
#include <QIcon>
class MainWindow;
class SFile;

void test();

//处理多行显示
QString elidedLineText(QWidget *pWidget, int nLine, QString strText);

//
HWND findProperShell();

//
void initiateDesktop();

//接入图标层
bool inplace(QWidget* aim);

bool shellValid();

//向Wallpaper发送鼠标点击信号（待修复
void sentToWallpaper(QPoint winpos);

//获取主题颜色
QColor pixmapMainColor(QPixmap p, double bright  = 1);

//处理qDebug
void customMessageHandler(QtMsgType type,
                          const QMessageLogContext &context,
                          const QString &msg);

//写入Content
void writeContent();
//写入Menu
void writeMenu();

//写入Jsons
void writeJsons();

//读取Content
QMap<int, QJsonObject> readContent();

//读取menu
void readMenu();

//计算对角距离
double rectLen(int w, int h);

//系统路径转化
QString toWindowsPath(QString const& linuxPath);
QString toLinuxPath(QString const& windowsPath);

//调用WindowsAPI启动
QString shellrun(QString filename, QString para = "", bool admin = false);

//计算两个点之间的方形
QRect Point2Rect(QPoint point0, QPoint point1);

//souce*ratio+add*(1-ratio)
QColor mixColor(QColor source, QColor add, double ratio);
QColor winThemeColor();

//通过资源字符串获得资源
QString extractString(QString res);

//运行Shell风格的命令
void shellContextMenuRun(QString command, QString path);

//初始化一些容器类型
void initContainerTypes();

//初始化拖拽
void initDrop();

//判断是否为容器
bool isContainer(QWidget* aim);

//设置计划任务自动运行
void setTaskAutoRun(bool val);

//判断是否有计划任务自动运行
bool haveTaskAutoRun();
void ShowError(char *lpszText, DWORD dwErrCode);
#endif // SYSFUNCTIONS_H
