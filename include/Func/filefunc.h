#ifndef FILEFUNC_H
#define FILEFUNC_H
#include "qfileinfo.h"
#include "qpixmap.h"
#include "style.h"
#include"QFileInfo"

// void ScreatNewFile(FileType type){}
enum FileType {
    txt = 0,
    docx = 1,
    pptx = 2,
    empty = 3
};

//用于提供用户调用的含GUI的方法
class fileCreator: public QObject
{
    Q_OBJECT
public:

    // 从已有预设的类型新建文件,用于菜单调用
    static void creatNewFile(FileType type);
    static void creatNewDir();

};

// 在桌面路径创建name文件，并添加到软件
bool creatAFileInDesktop(QString name, bool notice = false, QPoint globalPos = QPoint(-1, -1));

// 获取输入的文件路径，并创建文件的主程序，作为系统方法，不与桌面相关
bool creatAFile(const QString &filePath);

// 在桌面路径创建name文件夹，并添加到软件
bool creatADirInDesktop(QString name, bool notice = false, QPoint globalPos = QPoint(-1, -1));

//获取输入的文件夹路径，并创建文件夹的主程序去，作为系统方法，不与桌面相关
bool creatADir(const QString & dirPath);

//复制文件夹的系统方法
bool copyDir(const QString& fromDir, const QString& toDir, bool coverFileIfExist);

//若重复文件，进行重命名(传入绝对路径）
QString okPath(QString absolutePath);

//若重复文件，进行重命名(传入filename,只针对UserDesktopPath）
QString okName(QString fileName);

struct MyFileInfo {
    //定义返回的结构体
    enum TYPE {
        SINGLE = 0,
        MULTI = 1
    };
    TYPE type;
    QString name;
    QString filePath;
    QMap<int, QPixmap> icons;
    bool operator<(MyFileInfo& another) const
    {
        return type < another.type;
    }
    MyFileInfo(QString path, int size = 512);
    MyFileInfo(QFileInfo qfi, int size = 512);
    QPixmap aimIcon()
    {
        if(okForAim()) {
            return icons[default_steam_icon_type];
        } else {
            qDebug() << "No aim icon!,use default";
            return icons[0];
        }
    }
    bool okForAim()
    {
        return icons.contains(default_steam_icon_type);
    }
};


//从文件获得Icon
QPixmap getWinIcon(QString path, bool small = false);

//从Shell索引获得Icon
QIcon getShellIcon(QString path, int ind);

//转化为MyFileInfo
MyFileInfo path2MyFI(QString path, int size = 512);

//获取带有编号信息的Icon图片
QMap<int, QPixmap> path2Icon(QString path, int size = 512);

//basename（file）/ fullname（dir） 以防止以xx.yy形式命名的文件夹误判
QString path2Name(QString path);


//打开属性窗口
void OpenFileProperty(QString path);

//判断是否是图片
bool isPic(QString pah);



//扫描所有文件
QList<MyFileInfo> scanalldesktopfiles();

//从QFileInfo转化
QList<MyFileInfo>getFormFileInfo(QFileInfo x);

bool fileExist(const QString& path);


#endif // FILEFUNC_H
