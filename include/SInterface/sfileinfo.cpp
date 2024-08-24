#include "sfileinfo.h"
#include "filefunc.h"
#include "global.h"
#include"SysFunctions.h"
#include "qdir.h"
#include "qurl.h"
#include"snotice.h"
#include"QDesktopServices"
SFileInfo::SFileInfo(SFile *unit)
{
    pUnit = unit;
}

SFileInfo::SFileInfo(SFile *unit, QString path): SFileInfo(unit)
{
    loadFromPath(path);
}

QString SFileInfo::dirPath()
{
    return QFileInfo(filePath).path();
}
void SFileInfo::loadFromPath(QString filepath)
{
    qDebug() << "Loading form path:" << filepath;

    MyFileInfo info = path2MyFI(filepath);
    loadFromMyFI(info);
}

void SFileInfo::loadFromMyFI(MyFileInfo info)
{
    filePath = info.filePath;
    if(QFileInfo(filePath).isDir()) {
        isDir = true;
        nowExitDirs[filePath] = (SDir*)pUnit;
    }

    if(pUnit) {
        nowExitFiles[filePath] = pUnit;
    }

    nowExitInfos[filePath] = this;
}

bool SFileInfo::renameFile(QString newNameWithSuffix)
{
    QString folder = QFileInfo(filePath).path();
    QString newName =  folder + "/" + newNameWithSuffix;
    if(newName == filePath) {
        return true;
    }
    bool res = renameAbsolute(newName);
    if(!res) {
        SNotice::notice(QStringList() << filePath + ":" + "失败", "重命名文件");
    };
    return res;
}

bool SFileInfo::moveToDir(QString dirPath)
{
    QString newName =  dirPath + "/" + fullName();
    if(newName == filePath) {
        return true;
    }
    bool res = renameAbsolute(newName);
    if(!res) {
        SNotice::notice(QStringList() << filePath + ":" + "失败", "移动文件");
    };
    // qDebug()<<QFileInfo(filePath).isReadable();
    return res;

}

bool SFileInfo::renameAbsolute(QString newpath)
{
    qDebug() << "From" << filePath << "to" << newpath;
    bool renamed = QFile::rename(filePath, newpath);
    if(renamed) {
        nowExitFiles.remove(filePath);
        if(pUnit) {
            nowExitFiles[newpath] = pUnit;
        }

        if(isDir) {
            nowExitDirs.remove(filePath);
            if(pUnit) {
                nowExitDirs[newpath] = (SDir*)pUnit;
            }
        }

        nowExitInfos.remove(filePath);
        nowExitInfos[newpath] = this;

        filePath = newpath;
        return true;
    } else {
        return false;
    }
}

bool SFileInfo::removeFile()
{


    bool res = QFile::moveToTrash(filePath);
    if(!res) {
        qDebug() << "Unable to delete local file";
        SNotice::notice(QStringList() << filePath + ":" + ((res) ? "成功" : "失败"), "删除文件");
    } else {
        qDebug() << "Deleted";
    }
    removeInfo();
    return res;
}

void SFileInfo::openFile(bool Admin)
{

    shellrun(filePath, "", Admin);

}

void SFileInfo::removeInfo()
{
    if(nowExitFiles.contains(filePath)) {
        nowExitFiles.remove(filePath);
    }

    if(isDir && nowExitDirs.contains(filePath)) {
        nowExitDirs.remove(filePath);
    }

    nowExitInfos.remove(filePath);
}
