#include "filefunc.h"
#include "SysFunctions.h"
#include "mainwindow.h"
#include "qdir.h"
#include "qfileiconprovider.h"
#include "qregularexpression.h"
#include "qwinfunctions.h"
#include "windows.h"
#include "shellapi.h"
#include "commoncontrols.h"
#include "Commctrl.h"
#include <Shlobj.h>
#include"tchar.h"

MyFileInfo::MyFileInfo(QString path, int size)
{
    type = TYPE::SINGLE;
    name = path2Name(path);
    icons = path2Icon(path,size);
    if(icons.size()>1){
        type= TYPE::MULTI;
    }
    filePath = path;
}

MyFileInfo::MyFileInfo(QFileInfo qfi, int size):MyFileInfo(qfi.filePath(),size)
{
}

MyFileInfo path2MyFI(QString path,int size){
    return MyFileInfo(path,size);
}

QString path2Name(QString path){
    if(QFileInfo(path).isDir()) return QFileInfo(path).fileName();
    return QFileInfo(path).baseName();
}

QPixmap getWinIcon(QString path){

    QPixmap res ;
    LPCTSTR szFile = (LPCTSTR)path.utf16();
    SHFILEINFO sfi;
    if (!SHGetFileInfo(szFile, 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX)){
        qDebug()<<"failed1";
        return res;
    }

    // 获取大号图像列表
    IImageList *piml;
    int aim = (enable_highdef_icon)?4:2;
    if (FAILED(SHGetImageList(aim, IID_PPV_ARGS(&piml)))){

        qDebug()<<"failed2";
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

QPixmap resizeToRect(QPixmap source){
    qDebug()<<"Resizing from"<<source.size();
    QImage image = source.toImage();
    qDebug()<<"image"<<image.size();
    int width = source.width();
    int height = source.height();


    int start = 0;
    bool find = false;
    for(start = 0;start<=width-2&&!find;start++){
        //横向扫描
        for(int j=start;j<=width-1;j++){
            QColor pix = image.pixelColor(QPoint(j,start));
            if(pix.alpha()!=0){
                find = true;
                break;
            }
        }
        //竖向扫描
        for(int j=start;j<=height-1;j++){
            QColor pix = image.pixelColor(QPoint(start,j));
            if(pix.alpha()!=0){
                find = true;
                break;
            }
        }
    }

    int end=qMax(width,height)-1;
    find = false;
    for(;end>=start &&!find;end--){
        //横向扫描
        for(int j=0;j<=end;j++){
            QColor pix = image.pixelColor(QPoint(j,end));
            if(pix.alpha()!=0){
                find = true;
                break;
            }
        }
        //竖向扫描
        for(int j=0;j<=end;j++){
            QColor pix = image.pixelColor(QPoint(end,j));
            if(pix.alpha()!=0){
                find = true;
                break;
            }
        }
    }

    int f_start = qBound(0,start-1,qMin(width-1,height-1));
    int f_end = qBound(0,end,qMin(width-1,height-1));
    QPixmap res = source.copy(f_start,f_start,f_end+2-f_start,f_end+2-f_start);
    // qDebug()<<"Start"<<start<<"End"<<end<<"Result Size"<<res.size();
    return res;
}



QMap<int,QPixmap> path2Icon(QString path,int size){
    QMap<int,QPixmap> res;
    QFileInfo qfileinfo(path);

    if(isPic(path)&&use_pic_as_icon){
        QImage im;
        im.load(path);
        res[0] = QPixmap::fromImage(im);
    }

    else
    res[0] = resizeToRect(getWinIcon(toWindowsPath(path)));

    //针对steam游戏
    QSettings shortcut(qfileinfo.filePath(), QSettings::IniFormat);
    QString target = shortcut.value("InternetShortcut/URL").toString();
    QRegularExpression re("steam://rungameid/(\\d+)");
    QRegularExpressionMatch match = re.match(target);
    if (match.hasMatch())
    {

        // QString gameId = target.split("/")[-1];
        QString gameId = match.captured(1);
        qDebug()<<gameId;
        QString steamPath;
        QSettings reg("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);// 你的Steam安装路径
        steamPath = reg.value("SteamPath").toString()+"/appcache/librarycache";
        QDir directory(steamPath);
        QStringList steamfileList=directory.entryList(QDir::Files);



        steamfileList=directory.entryList(QDir::Files);


        foreach(const QString& steamfilename,steamfileList)
        {


            //小图标版本

            QString aim = gameId+"_icon";
            QString file = steamfilename.split('.')[0];

            if(file.mid(0,gameId.size())!=gameId) continue;


            //长竖图标版本
            // regex = QRegularExpression(gameId+"_library_600x900.jpg");
            aim = gameId+"_library_600x900";

            if(file.contains(aim))
            {
                res[1]=(QPixmap(directory.absoluteFilePath(steamfilename)));
                qDebug()<<"Find Verti";
            }

            //长横图标版本
            aim = gameId+"_header";
            // regex = QRegularExpression(gameId+"_header.jpg");
            if(file.contains(aim))
            {
                res[2]=(QPixmap(directory.absoluteFilePath(steamfilename)));
                qDebug()<<"Find Hori";
            }
        }

        // qDebug()<<QString::fromLocal8Bit(x.absoluteFilePath().toLocal8Bit())<<files.size();

    }
    qDebug()<<"get"<<res.size()<<"icons";
    return res;
}

LPITEMIDLIST GetItemIDListFromFilePath( QString strFilePath )
{
    if ( strFilePath.isNull() )
    {
        return NULL;
    }

    // 得到桌面的目录
    LPSHELLFOLDER pDesktopFolder = NULL;
    HRESULT hr = SHGetDesktopFolder( &pDesktopFolder );
    if ( FAILED(hr) )
    {
        return NULL;
    }

    // 将文件路径转换至OLECHAR格式
    OLECHAR strOleFilePath[MAX_PATH];
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, strFilePath.toStdString().c_str(), -1, strOleFilePath, MAX_PATH );

    // 得到文件路径对应的ItemIDList
    LPITEMIDLIST pItemIDList = NULL;
    hr = pDesktopFolder->ParseDisplayName( NULL, NULL, strOleFilePath, NULL, &pItemIDList, NULL );
    pDesktopFolder->Release();
    if ( FAILED(hr) )
    {
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

    if ( SUCCEEDED( ::SHGetDesktopFolder( &pDesktopFolder ) ) )
    {



        ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, path.toLocal8Bit(), -1,
                              szOleChar, MAX_PATH );


        // Convert the path to an ITEMIDLIST.
        hr = pDesktopFolder->ParseDisplayName( NULL,NULL,szOleChar,
                                              &chEaten,&pidl,&dwAttributes);

        pDesktopFolder->Release();
        if (SUCCEEDED( hr ))
            return pidl;
    }

    return NULL;
}

void OpenFileProperty(QString path)
{
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    qDebug()<<"on1"<<path;
    // Convert the path into a PIDL.
    path = toWindowsPath(path);
    qDebug()<<"on1"<<path;

    std::wstring wlpstrv = QString("properties").toStdWString();

    SHELLEXECUTEINFO sei;

    ZeroMemory(&sei,sizeof(sei));
    sei.cbSize = sizeof(sei);
    sei.lpFile = path.toStdWString().c_str(); //or path
    // sei.lpIDList = pidl;
    sei.lpVerb = wlpstrv.c_str();
    sei.fMask  = SEE_MASK_INVOKEIDLIST;
    ShellExecuteEx(&sei);
}

void creatAFile(QString name)
{
    QString filePath = *UserDesktopPath+"/"+name;

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        // 文件已成功打开

        file.close(); // 关闭文件
        qDebug() << "文件已创建：" << filePath;
    }
    else
    {
        qDebug() << "无法创建文件：" << filePath;
    }
}
