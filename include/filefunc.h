#ifndef FILEFUNC_H
#define FILEFUNC_H

#include "qfileinfo.h"
#include "qpixmap.h"
#include "style.h"
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

QPixmap getWinIcon(int winshelobj);
MyFileInfo path2MyFI(QString path,int size=512);
QMap<int,QPixmap> path2Icon(QString path,int size=512);
QString path2Name(QString path);
QPixmap resizeToRect(QPixmap source);
#endif // FILEFUNC_H
