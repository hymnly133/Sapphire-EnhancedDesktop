#ifndef SYSFUNCTIONS_H
#define SYSFUNCTIONS_H
#include<windows.h>
#include <QIcon>
#include"mainwindow.h"
#include "qfileinfo.h"
void Init(MainWindow* mainwindow);
void inplace();
void InitMouseHook();
QString Unicode2GBK(QString c);
QString Unicode2Utf(QString c);
QString GetCorrectUnicode(const QByteArray &ba);
QString elidedLineText(QWidget *pWidget, int nLine, QString strText);

struct FileInfo
{
    //定义返回的结构体
    enum TYPE{
        NORM = 0,
        HORI = 1,
        VERT = 2
    };
    bool multi =false;
    TYPE type;
    QString name;
    QString filePath;
    QIcon icon;
    bool operator<(FileInfo& another) const{
        return type<another.type;
    }
};
QList<FileInfo> scanalldesktopfiles();
QList<FileInfo>getFormFileInfo(QFileInfo x);
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
void readJson();
void setMyAppAutoRun(bool isStart);
ED_Unit* from_json(QJsonObject data);
QColor GetWindowsThemeColor();

#endif // SYSFUNCTIONS_H
