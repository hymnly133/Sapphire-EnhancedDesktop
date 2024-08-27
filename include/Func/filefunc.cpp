#include "filefunc.h"
#include "SysFunctions.h"
#include "global.h"
#include "guifunc.h"
#include "mainwindow.h"
#include "qdir.h"
#include "qjsonarray.h"
#include "qmimedatabase.h"
#include "qregularexpression.h"
#include "qwinfunctions.h"
#include "sfile.h"
#include "sinputdialog.h"
#include "shellapi.h"
#include "commoncontrols.h"
#include "Commctrl.h"
#include <Shlobj.h>
#include"snotice.h"
#include <QFile>
#include"shlwapi.h"
#define SHELLICONPATH "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Icons"
MyFileInfo::MyFileInfo(QString path, int size)
{
    type = TYPE::Default ;
    name = path2Name(path);
    filePath = path;
    bool res = false;
    aimIcon = path2Icon(filePath, 512, &res);
    if(res) {
        type = Aim;
    }

}

MyFileInfo::MyFileInfo(QFileInfo qfi, int size): MyFileInfo(qfi.filePath(), size)
{
}


MyFileInfo path2MyFI(QString path, int size)
{
    return MyFileInfo(path, size);
}

QString path2Name(QString path)
{
    QFileInfo info(path);
    if(info.isDir()) {
        if(info.isSymLink()) {
            return QFileInfo(info.symLinkTarget()).fileName();
        } else {
            return info.fileName();
        }
    } else {
        return QFileInfo(path).baseName();
    }
}

QPixmap getWinIcon(QString path, bool isSmall)
{
    if(path[0] == "\"") {
        path.replace("\"", "");
    } else if(path[0] == "\'") {
        path.replace("\'", "");
    }
    QPixmap res ;
    LPCTSTR szFile = (LPCTSTR)path.utf16();
    SHFILEINFO sfi;
    if (!SHGetFileInfo(szFile, 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX)) {
        qDebug() << "failed1";
        return res;
    }
    // 获取大号图像列表
    IImageList *piml;
    int aim = (enable_highdef_icon) ? 4 : 2;
    if(isSmall) {
        aim = 2;
    }
    if (FAILED(SHGetImageList(aim, IID_PPV_ARGS(&piml)))) {
        qDebug() << "failed2";
        return res;
    };
    // 提取图标
    HICON hico;
    piml->GetIcon(sfi.iIcon, ILD_TRANSPARENT, &hico);
    // 清理资源
    piml->Release();
    // 返回图标
    res = QtWin::fromHICON(hico);
    return res;
}


QList<MyFileInfo> scanFilesInDir(const QString &dirPath)
{
    //对于指定桌面路径，返还桌面路径中的文件信息的列表
    QList<MyFileInfo> files;
    QDir dir(dirPath);
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::System);
    QStringList fileList = dir.entryList();
    foreach(const QString &x, fileList) {
        files.append(dir.absoluteFilePath(x));
    }
    std::sort(files.begin(), files.end());
    return files;
}

QList<MyFileInfo> scanalldesktopfiles()
{
    //寻找桌面路径，并返回两个桌面中所有文件信息的列表
    QList<MyFileInfo> files;
    files.append(scanFilesInDir(PublicDesktopPath));
    files.append(scanFilesInDir(UserDesktopPath));
    std::sort(files.begin(), files.end());
    qDebug() << PublicDesktopPath << UserDesktopPath;
    return files;
}

QPixmap path2Icon(QString path, int size, bool* pres)
{
    QPixmap res;
    QFileInfo qfileinfo(path);
    if(isPic(path) && use_pic_as_icon) {
        QImage im;
        im.load(path);
        res = QPixmap::fromImage(im);
        return res;
    } else {
        res = resizeToRect(getWinIcon(toWindowsPath(path)));
    }
    //针对steam游戏
    if(qfileinfo.suffix() == "url") {
        QSettings shortcut(qfileinfo.filePath(), QSettings::IniFormat);
        QString target = shortcut.value("InternetShortcut/URL").toString();
        QRegularExpression re("steam://rungameid/(\\d+)");
        QRegularExpressionMatch match = re.match(target);
        if (match.hasMatch()) {
            // QString gameId = target.split("/")[-1];
            QString gameId = match.captured(1);
            qDebug() << gameId;
            QString steamPath;
            QSettings reg("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);// 你的Steam安装路径
            steamPath = reg.value("SteamPath").toString() + "/appcache/librarycache";
            QDir directory(steamPath);
            QStringList steamfileList = directory.entryList(QDir::Files);
            steamfileList = directory.entryList(QDir::Files);
            foreach(const QString& steamfilename, steamfileList) {
                QString file = steamfilename.split('.')[0];
                QString aim;
                if(file.mid(0, gameId.size()) != gameId) {
                    //排除错误匹配
                    continue;
                }

                if(default_steam_icon_type == 0) {
                    if(pres) {
                        *pres = true;
                    }
                    break;

                } else if(default_steam_icon_type == 1) {
                    //长竖图标版本
                    // regex = QRegularExpression(gameId+"_library_600x900.jpg");
                    aim = gameId + "_library_600x900";
                    if(file.contains(aim)) {
                        res = (QPixmap(directory.absoluteFilePath(steamfilename)));
                        if(pres) {
                            *pres = true;
                        }
                        break;
                        // qDebug() << "Find Verti";
                    }

                } else {
                    //长横图标版本
                    aim = gameId + "_header";
                    // regex = QRegularExpression(gameId+"_header.jpg");
                    if(file.contains(aim)) {
                        res = (QPixmap(directory.absoluteFilePath(steamfilename)));
                        if(pres) {
                            *pres = true;
                        }
                        break;
                        // qDebug() << "Find Hori";
                    }
                }

            }
            // qDebug()<<QString::fromLocal8Bit(x.absoluteFilePath().toLocal8Bit())<<files.size();
        }
    }

    qDebug() << "get" << res.size() << "icons";
    return res;
}

LPITEMIDLIST GetItemIDListFromFilePath( QString strFilePath )
{
    if ( strFilePath.isNull() ) {
        return NULL;
    }
    // 得到桌面的目录
    LPSHELLFOLDER pDesktopFolder = NULL;
    HRESULT hr = SHGetDesktopFolder( &pDesktopFolder );
    if ( FAILED(hr) ) {
        return NULL;
    }
    // 将文件路径转换至OLECHAR格式
    OLECHAR strOleFilePath[MAX_PATH];
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, strFilePath.toStdString().c_str(), -1, strOleFilePath, MAX_PATH );
    // 得到文件路径对应的ItemIDList
    LPITEMIDLIST pItemIDList = NULL;
    hr = pDesktopFolder->ParseDisplayName( NULL, NULL, strOleFilePath, NULL, &pItemIDList, NULL );
    pDesktopFolder->Release();
    if ( FAILED(hr) ) {
        return NULL;
    }
    return pItemIDList;
}

LPITEMIDLIST GetIDListFromPath(QString path)
{
    LPITEMIDLIST pidl = NULL;
    LPSHELLFOLDER pDesktopFolder;
    OLECHAR szOleChar[MAX_PATH];
    ULONG chEaten;
    ULONG dwAttributes;
    HRESULT hr;
    if ( SUCCEEDED( ::SHGetDesktopFolder( &pDesktopFolder ) ) ) {
        ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, path.toLocal8Bit(), -1,
                               szOleChar, MAX_PATH );
        // Convert the path to an ITEMIDLIST.
        hr = pDesktopFolder->ParseDisplayName( NULL, NULL, szOleChar,
                                               &chEaten, &pidl, &dwAttributes);
        pDesktopFolder->Release();
        if (SUCCEEDED( hr )) {
            return pidl;
        }
    }
    return NULL;
}

void OpenFileProperty(QString path)
{
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    path = toWindowsPath(path);
    std::wstring wlpstrv = QString("properties").toStdWString();
    SHELLEXECUTEINFO sei;
    ZeroMemory(&sei, sizeof(sei));
    sei.cbSize = sizeof(sei);
    sei.lpFile = path.toStdWString().c_str(); //or path
    // sei.lpIDList = pidl;
    sei.lpVerb = wlpstrv.c_str();
    sei.fMask  = SEE_MASK_INVOKEIDLIST;
    ShellExecuteEx(&sei);
}


bool isPic(QString pah)
{
    bool bRet = false;
    QStringList type;
    type << "jpg" << "jpeg" << "png" << "bmp" << "tif" << "webp" << "raw";
    if(type.contains(QFileInfo(pah).suffix())) {
        bRet = true;
    }

    // QMimeDatabase db;
    // QMimeType mime = db.mimeTypeForFile(pah);
    // if (mime.name().startsWith("image/")) {
    //     bRet = true;
    // }
    return bRet;
}





void FileHelper::creatNewFile(FileType type)
{
    QString defaultname = "";
    switch (type) {
        case txt:
            defaultname = "新建 文本文档.txt";
            break;
        case docx:
            defaultname = "新建 Word文档.docx";
            break;
        case pptx:
            defaultname = "新建 Powerpoint演示文稿.pptx";
            break;
        case empty:
            break;
    }
    SInputDialog* sid = SInputDialog::showInput("请输入文件名", defaultname);
    QPoint nowPos = QCursor::pos();
    connect(sid, &SInputDialog::finalText, activepmw, [ = ](QString name) {
        creatAFileInDesktop(name, true, nowPos);
    });
}

void FileHelper::creatNewDir()
{
    QString defaultname = "新文件夹";
    SInputDialog* sid = SInputDialog::showInput("请输入文件夹名", defaultname);
    QPoint nowPos = QCursor::pos();
    connect(sid, &SInputDialog::finalText, activepmw, [ = ](QString name) {
        creatADirInDesktop(name, true, nowPos);
    });
}


bool creatAFileInDesktop(QString name, bool notice, QPoint globalPos)
{
    if(name == "") {
        name = "空文件";
    }
    QString path = okPath(UserDesktopPath + "/" + name);
    bool ret = creatAFile(path);
    if(ret) {
        return activepmw->addAFile(path, notice, globalPos);
    } else {
        return false;
    }
}

bool fileExist(const QString &path)
{
    if(QFileInfo((path)).isDir() && QFileInfo::exists(path)) {
        return true;
    }
    if(QFileInfo::exists(path)) {
        return true;
    }
    if(QFileInfo(path).exists()) {
        return true;
    }
    if (FILE *file = fopen(toWindowsPath(path).toStdString().c_str(), "r")) {
        fclose(file);
        return true;
    }
    if (PathFileExists(path.toStdWString().c_str())) {
        qDebug() << "winExits";
        return true;
    }
    // if (access(toWindowsPath(path).toStdString().c_str(), F_OK) == 0)
    // {
    //     printf("1.txt exists.\n");
    //     return true;
    // }
    return false;
}

bool creatAFile(const QString &absolutePath)
{
    qDebug() << "SystemCreate" << absolutePath;
    QFile file(absolutePath);
    if (file.open(QIODevice::WriteOnly)) {
        // 文件已成功打开
        file.close(); // 关闭文件
        qDebug() << "文件已创建：" << absolutePath;
        return true;
    } else {
        qDebug() << "无法创建文件：" << absolutePath;
        return false;
    }
}


QString okName(QString fileName)
{
    QString resOkPath = okPath(UserDesktopPath + '/' + fileName);
    return resOkPath.mid(resOkPath.lastIndexOf("/") + 1, resOkPath.length());
}

QString okPath(QString absolutePath)
{
    if(QFileInfo::exists(absolutePath)) {
        QFileInfo finfo(absolutePath);
        int existNums = 0;
        QString nametemplate;
        if(finfo.suffix() == "") {
            nametemplate = "(%1)";
        } else {
            nametemplate = "(%1).";
        }
        QString newName = finfo.baseName() + nametemplate.arg(existNums) + finfo.suffix();
        while(QFileInfo::exists(finfo.path() + "/" + newName)) {
            existNums++;
            newName = finfo.baseName() + nametemplate.arg(existNums) + finfo.suffix();
        }
        SNotice::notice(QStringList() << "已存在，重定向为" << newName, "重定向");
        return finfo.path() + "/" + newName;
    }
    return absolutePath;
}

bool creatADir(const QString &dirPath)
{
    //创建文件夹
    QDir dir(dirPath);
    if (!dir.exists()) {
        bool res =  dir.mkdir(dirPath);
        if(res) {
            qDebug() << "创建成功";
        } else {
            qDebug() << "创建失败";
        }
        return res;
    }
    qDebug() << "已存在";
    return true;
}

bool creatADirInDesktop(QString name, bool notice, QPoint globalPos)
{
    if(name == "") {
        name = "新文件夹";
    }
    QString path = okPath(UserDesktopPath + "/" + name);
    bool ret = creatADir(path);
    if(ret) {
        return activepmw->addAFile(path, notice, globalPos);
    } else {
        return false;
    }
}

bool copyDir(const QString& fromDir, const QString& toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if (!targetDir.exists()) {
        //如果目标目录不存在，则进行创建
        if (!targetDir.mkdir(targetDir.absolutePath())) {
            return false;
        }
    }
    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList) {
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..") {
            continue;
        }
        if (fileInfo.isDir()) {
            // 当为目录时，递归的进行copy
            if (!copyDir(fileInfo.filePath(),
                         targetDir.filePath(fileInfo.fileName()),
                         coverFileIfExist)) {
                return false;
            }
        } else {
            //当允许覆盖操作时，将旧文件进行删除操作
            if (coverFileIfExist && targetDir.exists(fileInfo.fileName())) {
                targetDir.remove(fileInfo.fileName());
            }
            // 进行文件拷贝
            if (!QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()))) {
                return false;
            }
        }
    }
    return true;
}

QIcon getShellIcon(QString path, int ind)
{
    path = path.simplified();
    if(path[0] == "\"") {
        path.replace("\"", "");
    } else if(path[0] == "\'") {
        path.replace("\'", "");
    }
    QIcon icon;
    if(path.isNull()) {
        return icon;
    }
    if(path.toLower() == "shell32.dll") {
        path = "shell32.dll";//系统强制文件重定向
        QSettings set(SHELLICONPATH, QSettings::NativeFormat);
        if(set.value(QString::number(ind)).isValid()) {
            QStringList rePath = set.value(QString::number(ind)).toString().split(',');
            icon =  getShellIcon(rePath[0], rePath[1].toInt());
        }
        if(!icon.isNull()) {
            return icon;
        }
    }
    HICON* hIconL[10];
    HICON* hIconS[10];
    //iconIndex为负数就是指定资源标识符, 为正数就是该图标在资源文件中的顺序序号, 为-1时不能使用ExtractIconEx提取图标
    QScopedArrayPointer<HICON> icons(new HICON[10]);
    int res = ExtractIconEx((wchar_t *)path.utf16(), ind, icons.data(), NULL, 1);
    if(res) {
        icon = QtWin::fromHICON(icons[0]);
        DestroyIcon(icons[0]);
    }
    // else if(hIconS){
    // icon = QtWin::fromHICON(*hIconS);
    // DestroyIcon(*hIconS);
    // }
    return icon;
}

QJsonObject mergeJsonObject(QJsonObject src, QJsonObject other)
{
    for(auto it = other.constBegin(); it != other.constEnd(); ++it) {
        if(src.contains(it.key())) {
            if(src.value(it.key()).isObject() && other.value(it.key()).isObject()) {
                QJsonObject one(src.value(it.key()).toObject());
                QJsonObject two(other.value(it.key()).toObject());

                mergeJsonObject(one, two);
                src[it.key()] = one;
            } else if(src.value(it.key()).isArray() && other.value(it.key()).isArray()) {
                QJsonArray arr = other.value(it.key()).toArray();
                QJsonArray srcArr = src.value(it.key()).toArray();
                for(int i = 0; i < arr.size(); i++) {
                    srcArr.append(arr[i]);
                }
                src[it.key()] = srcArr;
            }
        } else {
            src[it.key()] = it.value();
        }
    }
    return src;
}

// void addFileInfo(SFileInfo *info)
// {

// }

// void removeFileInfo(SFileInfo *info)
// {

// }

bool inDesktop(QString path)
{
    return
        QFileInfo(path).path() == UserDesktopPath || QFileInfo(path).path() == PublicDesktopPath;
}
