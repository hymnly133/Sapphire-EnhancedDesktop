#include "filefunc.h"
#include "SysFunctions.h"
#include "qdir.h"
#include "qfileiconprovider.h"
#include "qregularexpression.h"
#include "qwinfunctions.h"
#include "windows.h"
#include "shellapi.h"
#include "commoncontrols.h"
#include "Commctrl.h"
MyFileInfo path2MyFI(QString path,int size){
    return MyFileInfo(path,size);
}

QString path2Name(QString path){
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

    int end=255;
    find = false;
    for(end=255;end>=47 &&!find;end--){
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
    qDebug()<<"Start"<<start<<"End"<<end<<"Result Size"<<res.size();
    return res;
}

QMap<int,QPixmap> path2Icon(QString path,int size){
    QMap<int,QPixmap> res;
    QFileInfo qfileinfo(path);
    QFileIconProvider iconProvider;
    // qDebug()<<x.suffix().toLower()<<x.symLinkTarget();

        // res[0]=(iconProvider.icon(path).pixmap(size));
    res[0] = resizeToRect(getWinIcon(toWindowsPath(path)));

    //针对steam游戏
    QSettings shortcut(qfileinfo.filePath(), QSettings::IniFormat);
    QString target = shortcut.value("InternetShortcut/URL").toString();
    QRegularExpression re("steam://rungameid/(\\d+)");
    QRegularExpressionMatch match = re.match(target);
    if (match.hasMatch())
    {
        QString gameId = match.captured(1);
        QString steamPath;
        QSettings reg("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);// 你的Steam安装路径
        steamPath = reg.value("SteamPath").toString()+"/appcache/librarycache";
        QDir directory(steamPath);
        QStringList steamfileList=directory.entryList(QDir::Files);



        steamfileList=directory.entryList(QDir::Files);


        foreach(const QString& steamfilename,steamfileList)
        {
            QRegularExpression regex;

            //小图标版本

            regex = QRegularExpression(gameId+"_icon");
            if(regex.match(steamfilename).hasMatch())
            {
                res[0]=(QPixmap(directory.absoluteFilePath(steamfilename)));
                qDebug()<<"Find Little";
            }


            //长竖图标版本
            regex = QRegularExpression(gameId+"_library_600x900");
            if(regex.match(steamfilename).hasMatch())
            {
                res[1]=(QPixmap(directory.absoluteFilePath(steamfilename)));
                qDebug()<<"Find Verti";
            }

            //长横图标版本

            regex = QRegularExpression(gameId+"_header");
            if(regex.match(steamfilename).hasMatch())
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

