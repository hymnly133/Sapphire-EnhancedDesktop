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



//转化为MyFileInfo
MyFileInfo path2MyFI(QString path,int size=512);

//获取带有编号信息的Icon图片
QMap<int,QPixmap> path2Icon(QString path,int size=512);

//basename（file）/ fullname（dir） 以防止以xx.yy形式命名的文件夹误判
QString path2Name(QString path);


//打开属性窗口
void OpenFileProperty(QString path);

//判断是否是图片
bool isPic(QString pah);

//扫描文件改动
void scanForChange();

//扫描所有文件
QList<MyFileInfo> scanalldesktopfiles();

//从QFileInfo转化
QList<MyFileInfo>getFormFileInfo(QFileInfo x);

#endif // FILEFUNC_H
