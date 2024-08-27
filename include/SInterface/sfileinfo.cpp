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

QString SFileInfo::filePath_red()
{
    auto info = QFileInfo(filePath);
    if(info.isSymLink()) {
        return info.symLinkTarget();
    }
    return filePath;
}

QString SFileInfo::dirPath()
{
    return QFileInfo(filePath).path();
}

QString SFileInfo::dirPath_red()
{
    auto info = QFileInfo(filePath);
    if(info.isSymLink()) {
        return QFileInfo(info.symLinkTarget()).path();
    }
    return QFileInfo(filePath).path();
}

bool SFileInfo::isSymLink()
{
    return QFileInfo(filePath).isSymLink();
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
    addInfo();
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
    QString oldpath = filePath;
    if(filePath == "") {
        qDebug() << "Try to move, But empty";
        return true;
    }
    QString newName =  dirPath + "/" + fullName();
    if(newName == filePath) {
        qDebug() << "The same file! no move";
        return true;
    }
    bool res = renameAbsolute(newName);
    if(!res) {
        SNotice::notice(QStringList() << filePath + ":" + "失败", "移动文件");
    };
    qDebug() << QString("move %1 to %2 ,").arg(oldpath).arg(newName) << res;
    return res;

}

bool SFileInfo::renameAbsolute(QString newpath)
{
    qDebug() << "Try to rename from" << filePath << "to" << newpath;
    bool renamed = QFile::rename(filePath, newpath);
    if(renamed) {
        removeInfo();
        filePath = newpath;
        addInfo();

        return true;
    } else {
        return false;
    }
}

bool SFileInfo::removeFile()
{

    qInfo() << QString("try to remove file : %1").arg(filePath);
    bool res = QFile::moveToTrash(filePath);
    if(!res) {
        qDebug() << "Unable to delete local file";
        SNotice::notice(QStringList() << filePath + ":" + ((res) ? "成功" : "失败"), "删除文件");
    } else {
        qDebug() << "Deleted";
        removeInfo();
    }
    return res;
}

void SFileInfo::openFile(bool Admin)
{

    shellrun(filePath, "", Admin);

}

void SFileInfo::removeInfo()
{
    qInfo() << QString("remove info : %1").arg(filePath);
    if(nowExitFiles.contains(filePath)) {
        nowExitFiles.remove(filePath);
    }

    if(isDir && nowExitDirs.contains(filePath_red())) {
        nowExitDirs.remove(filePath_red());
    }

    if(nowExitInfos.contains(filePath)) {
        nowExitInfos.remove(filePath);
    }
}

void SFileInfo::addInfo()
{
    qInfo() << QString("remove info : %1").arg(filePath);
    if(QFileInfo(filePath_red()).isDir()) {
        isDir = true;
        nowExitDirs[filePath_red()] = (SDir*)pUnit;
    }

    if(pUnit) {
        nowExitFiles[filePath] = pUnit;
    }

    if(!nowExitInfos.contains(filePath)) {
        nowExitInfos[filePath] = this;
    }
}
