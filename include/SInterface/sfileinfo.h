#ifndef SFILEINFO_H
#define SFILEINFO_H
#include"QString"
#include "filefunc.h"
#include "qfileinfo.h"

class SFile;
class MyFileInfo;
class SFileInfo
{
public:
    SFileInfo(SFile *unit = nullptr);
    SFileInfo(SFile* unit, QString path);
    //宿主unit
    SFile* pUnit  =  nullptr;

    //绝对路径
    QString filePath;

    QString filePath_red();

    //aaa.bb
    QString fullName();

    //后缀
    QString suffix();

    //解析快捷方式后的后缀
    QString suffix_red();

    //aaa.bb 中的aaa
    QString baseName();

    QString dirPath();

    QString dirPath_red();
    bool isDir = false;
    bool isSymLink();

    //以下仅针对文件以及全局变量进行操作

    //文件重命名(系统性)，只输入文件名，重命名后仍在原文件夹
    virtual bool renameFile(QString newNameWithSuffix);
    //移动到目标文件夹
    virtual bool moveToDir(QString dirPath);

    //重命名（传入新的路径）
    virtual bool renameAbsolute(QString newpath);
    //文件删除(系统性)
    virtual bool removeFile();
    //文件打开(系统性)
    void openFile(bool Admin = false);
    //文件打开(系统性)
    void openTargetDirAndCelect();


    //Info系统删除
    void removeInfo();
    //Info系统添加
    void addInfo();

    QJsonObject to_json();
    void load_json(QJsonObject rootObject);

    virtual void loadFromPath(QString filepath);
    virtual void loadFromMyFI(MyFileInfo info);
};

inline QString SFileInfo::fullName()
{
    return QFileInfo(filePath).fileName();
}

inline QString SFileInfo::suffix()
{
    return QFileInfo(filePath).suffix();
}

inline QString SFileInfo::suffix_red()
{
    if(QFileInfo(filePath).isSymLink()) {
        return QFileInfo(QFileInfo(filePath).symLinkTarget()).suffix();
    } else {
        return suffix();
    }
}

inline QString SFileInfo::baseName()
{
    return path2Name(filePath);
}

#endif // SFILEINFO_H
