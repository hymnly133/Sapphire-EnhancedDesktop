#include <shlwapi.h>
#include<windows.h>
#include "global.h"
#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0A00
#endif

#include "glpicturebox.h"
#include"mainwindow.h"
#include "layershower.h"
#include "qpainter.h"
#include "qscreen.h"
#include "screenfunc.h"
#include"windows.h"
#include"winuser.h"
#include "shlobj.h"
#include"SysFunctions.h"
#include"QTextCodec"
#include <QJsonObject>
#include<QDir>
#include<QFileInfo>
#include<QStandardPaths>
#include<QFileIconProvider>
#include<QSettings>
#include<QIcon>
#include"QVector"
#include <QMetaType>
#include <QtCore>
#include<QMessageBox>
#include<windows.h>
#include<uxtheme.h>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <dwmapi.h>
#include <QStandardPaths>
#include"QWindowStateChangeEvent"
#include"QInputDialog"




static QMutex mutex;


QTextCodec* utf8 = QTextCodec::codecForName("utf-8");
QTextCodec* gbk = QTextCodec::codecForName("GBK");

HWND shelldlldefview = NULL;
HWND progrmman = NULL;

void customMessageHandler(QtMsgType type,
                          const QMessageLogContext &context,
                          const QString &msg)
{
    //Q_UNUSED(context)
    QDateTime _datetime = QDateTime::currentDateTime();
    QString szDate = _datetime.toString("yyyy-MM-dd hh:mm:ss.zzz");//"yyyy-MM-dd hh:mm:ss ddd"
    QString txt(szDate);
    switch (type) {
        case QtDebugMsg: { //调试信息提示
            txt += QString(" [Debug] ");
            break;
        }
        case QtInfoMsg: { //信息输出
            txt += QString(" [Info] ");
            break;
        }
        case QtWarningMsg: { //一般的warning提示
            txt += QString(" [Warning] ");
            break;
        }
        case QtCriticalMsg: { //严重错误提示
            txt += QString(" [Critical] ");
            break;
        }
        case QtFatalMsg: { //致命错误提示
            txt += QString(" [Fatal] ");
            //abort();
            break;
        }
        default: {
            txt += QString(" [Trace] ");
            break;
        }
    }

    txt.append( QString(" %1").arg(context.file) );
    txt.append( QString("<%1>: ").arg(context.line) );
    txt.append(msg);

    mutex.lock();
    QFile file(QApplication::applicationDirPath() + "/log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << txt << "\r\n";
    file.close();
    mutex.unlock();
}
QRect AbsoluteRect(QWidget* aim)
{
    auto tem = aim->geometry();
    auto pos = aim->parentWidget()->pos();
    return QRect(pos.x() + tem.x(), pos.y() + tem.y(), tem.width(), tem.height());
}

double rectLen(int w, int h)
{
    return sqrt(w*w + h*h);
}

QString toWindowsPath(QString const& linuxPath)
{
    QString windowsPath = linuxPath;
    windowsPath.replace("/", "\\");
    return windowsPath;
}

QString toLinuxPath(QString const& windowsPath)
{
    QString linuxPath = windowsPath;
    linuxPath.replace("\\", "/");
    return linuxPath;
}

void inplace(QWidget* aim)
{
    // 接入到图标层
    qDebug() << "try to inplace";
    if (shelldlldefview != NULL) {
        qDebug() << "Use Valid shell";
        SetParent((HWND)aim->winId(), shelldlldefview);
        SetWindowPos((HWND)aim->winId(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        SetWindowPos((HWND)aim->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        SetFocus((HWND)aim->winId());
    } else {
        qDebug() << "no Valid shell";
    }
}

void positionToScreen(QWidget* aim, int screenInd)
{
    if(aim->inherits("MainWindow")) {
        ((MainWindow*)aim)->updateSize();
    } else if(aim->inherits("LayerShower")) {
        ((LayerShower*)aim)->updateSize();
    }
    aim->move(pscs[screenInd]->geometry().topLeft() + Shift_Global);
    aim->move(2 * aim->pos() - aim->geometry().topLeft());

}


QString GetCorrectUnicode(const QByteArray &ba)
{

    QTextCodec::ConverterState state;
    QTextCodec *codec = utf8;
    QString text = codec->toUnicode(ba.constData(), ba.size(), &state);
    if (state.invalidChars > 0) {
        qDebug() << "GBK";
        text = gbk->toUnicode(ba);
    } else {
        qDebug() << "utf-8";
        text = ba;
    }
    return text;
}

QString elidedLineText(QWidget *pWidget, int nLine, QString strText)
{
    //调整输出
    if (nLine == 0) {
        return "";
    }

    QFontMetrics fontMetrics(pWidget->font());

    if (nLine == 1) {
        return fontMetrics.elidedText(strText, Qt::ElideRight, pWidget->width());
    }

    QStringList strListLine;

    for (int i = 0; i < strText.size(); i++) {
        if (fontMetrics.width(strText.left(i)) >= pWidget->width()) {
            strListLine.append(strText.left(i));
            if (strListLine.size() == nLine) {
                break;
            }
            strText = strText.right(strText.size() - i);
            i = 0;
        }
    }

    if (strListLine.size() < nLine) {
        if (!strText.isEmpty()) {
            strListLine.append(strText);
        }
    }

    bool bHasElided = true;
    if (fontMetrics.width(strText) < pWidget->width()) {
        bHasElided = false;
    }

    if (bHasElided && !strListLine.isEmpty()) {
        QString strLast = strListLine.last();
        QString strElided = "...";
        strLast.insert(strLast.length(), strElided);
        while (fontMetrics.width(strLast) >= pWidget->width()) {
            strLast = strLast.replace(0, 1, "");
        }

        strListLine.replace(strListLine.count() - 1, strLast);
    }
    QString strResult = strListLine.join("\n");

    return strResult;
}

QColor pixmapMainColor(QPixmap p, double bright) //p为目标图片 bright为亮度系数，为1则表示保持不变
{
    int step = 20; //步长：在图片中取点时两点之间的间隔，若为1,则取所有点，适当将此值调大有利于提高运行速度
    int t = 0; //点数：记录一共取了多少个点，用于做计算平均数的分母
    QImage image = p.toImage(); //将Pixmap类型转为QImage类型
    int r = 0, g = 0, b = 0; //三元色的值，分别用于记录各个点的rgb值的和
    for (int i = 0; i < p.width(); i += step) {
        for (int j = 0; j < p.height(); j += step) {
            if(image.valid(i, j)) { //判断该点是否有效
                t++; //点数加一
                QColor c = image.pixel(i, j); //获取该点的颜色
                r += c.red(); //将获取到的各个颜色值分别累加到rgb三个变量中
                b += c.blue();
                g += c.green();
            }
        }
    }
    return QColor(int(bright*r / t) > 255 ? 255 : int(bright*r / t),
                  int(bright*g / t) > 255 ? 255 : int(bright*g / t),
                  int(bright*b / t) > 255 ? 255 : int(bright*b / t)); //最后返回的值是亮度系数×平均数,若超出255则设置为255也就是最大值，防止乘与亮度系数后导致某些值大于255的情况。
}


QString  getDesktopPath()
{
    LPITEMIDLIST pidl;
    LPMALLOC pShellMalloc;
    char szDir[200];
    if (SUCCEEDED(SHGetMalloc(&pShellMalloc))) {
        if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl))) {
            // 如果成功返回true
            SHGetPathFromIDListA(pidl, szDir);
            pShellMalloc->Free(pidl);
        }
        pShellMalloc->Release();
    }

    return QString(szDir);
}

void sentToWallpaper(QPoint winpos)
{
    HWND wallpaper = NULL;
    HWND worker = NULL;

    qDebug() << "Entering Loop";
    // 循环查找WorkerW窗口
    do {
        worker = FindWindowExA(NULL, worker, "WorkerW", NULL);
        if (worker != NULL) {
            qDebug() << "Find WokerW";
            // 尝试找到SHELLDLL_DefView窗口
            HWND _wallpaper = FindWindowExA(worker, NULL, NULL, "WPELiveWallpaper");
            if (_wallpaper != NULL) {
                qDebug() << "Find WPELiveWallpaper";
                // 检查SHELLDLL_DefView的父窗口是否为当前的WorkerW窗口
                HWND parent = GetParent(_wallpaper);
                if (parent != NULL) {
                    qDebug() << "Find WPELiveWallpaper's Parent";
                    if (parent == worker) {
                        qDebug() << "Right!";

                        // 找到了正确的WorkerW窗口
                        wallpaper = _wallpaper;
                        break; // 结束循环
                    }
                }
            }
        }
    } while (worker != NULL);
    LPARAM lParam = MAKELPARAM(winpos.x(), winpos.y());
    if(wallpaper != NULL) {
        qDebug() << "sent" << (int*)wallpaper;
        PostMessage (wallpaper, WM_LBUTTONDOWN, VK_LBUTTON, lParam);
        // mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    }

    if(shelldlldefview != NULL) {
        qDebug() << "sent" << (int*)shelldlldefview;
        PostMessage (shelldlldefview, WM_LBUTTONDOWN, VK_LBUTTON, lParam);
        // mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    }
    return;
}


void writeContent()
{
    QJsonArray rootArrar;
    foreach (auto pmw, pmws) {
        qDebug() << "Writing" << pmw->objectName();
        QJsonObject rootObj =  pmw->to_json();
        rootArrar.append(rootObj);
    }
    for(int i = screenNum; i < jsonNum; i++) {
        qDebug() << "Writing Unused Json" << i;
        rootArrar.append(UnusedJsons[i]);
    }

    QJsonDocument document;
    document.setArray(rootArrar);

    QByteArray byte_array = document.toJson(QJsonDocument::Indented);
    QString json_str(byte_array);
    QFile file(CONTENT_PATH);


    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "file error!";
    }
    QTextStream in(&file);
    in.setCodec("UTF-8");
    in << json_str;

    file.close();   // 关闭file
}

QMap<int, QJsonObject> readContent()
{
    QMap<int, QJsonObject> res;

    QFile file(QApplication::applicationDirPath() + "/content.json");

    if(!file.exists()) {

        return res;
    }

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QString value = file.readAll();
    file.close();

    QJsonParseError parseJsonErr;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(), &parseJsonErr);
    if (! (parseJsonErr.error == QJsonParseError::NoError)) {
        QMessageBox::about(NULL, "提示", "配置文件错误！");
        return res;
    }


    if(document.isArray()) {

        QJsonArray arr = document.array();
        qDebug() << "Find array of" << arr.size();

        for(int i = 0; i < arr.size(); i++) {
            res.insert(i, arr[i].toObject());
        }
    } else {
        qDebug() << "Find object";
        res.insert(0, document.object());
    }
    return res;
}




QString shellrun(QString filename, QString para, bool admin)
{
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    HINSTANCE hNewExe = nullptr;
    SHELLEXECUTEINFO sei;
    std::wstring lpFile;
    std::wstring lpPara;


    para.replace('\'', '\"');
    para.replace('/', '\\');
    filename.replace('\'', '\"');
    filename.replace('/', '\\');
    qDebug() << QString("ShellRun: file:%1,para:%2").arg(filename).arg(para);



    ZeroMemory(&sei, sizeof(sei));
    sei.nShow = SW_SHOW; //or path

    if(isAdmin && !admin && para.isEmpty() && try_run_as_user) {
        //没有参数进行降权
        para = "\"" + filename + "\"";
        filename = "explorer.exe";
    }

    qDebug() << QString("After: file:%1,para:%2").arg(filename).arg(para);



    sei.cbSize = sizeof(sei);
    lpFile = filename.toStdWString();
    sei.lpFile = lpFile.c_str(); //or path


    WCHAR wszClassName[256];
    memset(wszClassName, 0, sizeof(wszClassName));


    if(admin) {
        const char* szVerb = "runas";
        MultiByteToWideChar(CP_ACP, 0, szVerb, strlen(szVerb) + 1, wszClassName, sizeof(wszClassName) / sizeof(wszClassName[0]));
        // sei.lpVerb = "runas";
    } else {
        const char* szVerb = "open";
        MultiByteToWideChar(CP_ACP, 0, szVerb, strlen(szVerb) + 1, wszClassName, sizeof(wszClassName) / sizeof(wszClassName[0]));
    }

    sei.lpVerb = wszClassName;
    lpPara = para.toStdWString();
    sei.lpParameters = lpPara.c_str();


    sei.fMask  = SEE_MASK_NOCLOSEPROCESS;

    sei.hInstApp  = hNewExe;
    if(ShellExecuteEx(&sei)) {
        return "Success!";
    } else {
        long long code = (long long)hNewExe;
        if(code > 32) {
            return "Success!";
        }
        QString sRet;
        switch(code) {
            case 0:
                sRet = QString("memory lack.");
                break;
            case 2:
                sRet = QString("filename is error.");
                break;
            case 3:
                sRet = QString("file path is error.");
                break;
            case 11:
                sRet = QString("exe is invaliable.");
                break;
            case 26:
                sRet = QString("shared error.");
                break;
            case 27:
                sRet = QString("file is error or be short.");
                break;
            case 28:
                sRet = QString("open time out.");
                break;
            case 29:
                sRet = QString("DDE task failed.");
                break;
            case 30:
                sRet = QString("undering other's DDE");
                break;
            case 31:
                sRet = QString("no linked process.");
                break;
            default:
                sRet = QString("unknow error.");
                break;
        }
        return QString("Shellrun ses:") + sRet;

    }


    // auto operate = (admin) ? "runas" : "open";
    // HINSTANCE hNewExe = ShellExecuteExW(NULL, operate, filename.toStdWString().c_str(), para.toStdWString().c_str(), NULL, SW_SHOW);

}


QRect Point2Rect(QPoint point0, QPoint point1)
{
    QPoint fixPoint0 = point0;
    QPoint fixPoint1 = point1;

    int x = (fixPoint0.x() < fixPoint1.x()) ? fixPoint0.x() : fixPoint1.x();
    int y = (fixPoint0.y() < fixPoint1.y()) ? fixPoint0.y() : fixPoint1.y();
    int w = qAbs(fixPoint0.x() - fixPoint1.x()) + 1;
    int h = qAbs(fixPoint0.y() - fixPoint1.y()) + 1;
    return QRect(x, y, w, h);  // 画长方形
}





void initiateDesktop()
{
    qDebug() << "Star Initiate";
    qDebug() << "Using Initiate Func1";
    HWND aim_shell = NULL;


    progrmman = FindWindowA("Progman", "Program Manager");




    qDebug() << "Entering Loop";

    aim_shell = findProperShell();
    if(aim_shell) {
        qDebug() << "find proper shell,it works fine";
        shelldlldefview = aim_shell;
    } else {
        //如果没有，尝试启动双层结构
        qDebug() << "Unable to find proper shell,try to invoke wokerW";
        if(progrmman != NULL) {
            qDebug() << "Find Program Manager";

            //向Progman发送0x052c以形成WokerW
            // Win7, 8, 8.1, Win 10 几乎全部版本，Win 11 21H2, 22H2, 23H2 全部已发布版本
            DWORD_PTR result;
            SendMessageTimeoutW(progrmman, 0x052C, 0, 0, SMTO_NORMAL, 0x03E8, &result);
            qDebug() << "Message Send";
            // 再次循环查找WorkerW窗口
            aim_shell = findProperShell();
            if(aim_shell) {
                shelldlldefview = aim_shell;
                qDebug() << "Try good!";
            } else {
                qDebug() << "Failed again";
            }
        }
    }
}

HWND findProperShell()
{
    if(shelldlldefview) {
        return shelldlldefview;
    }
    progrmman = FindWindowA("Progman", "Program Manager");

    // 循环查找WorkerW窗口
    HWND worker = NULL;
    HWND aim_shell = NULL;

    //1.双WorkerW结构
    qDebug() << "Try to find shell in double worker";
    do {
        worker = FindWindowExA(NULL, worker, "WorkerW", NULL);
        if (worker != NULL) {
            qDebug() << "Find WokerW";
            // 尝试找到SHELLDLL_DefView窗口
            HWND aim_shell = FindWindowExA(worker, NULL, "SHELLDLL_DefView", NULL);
            if (aim_shell != NULL) {
                qDebug() << "Find SHELLDLL_DefView";
                // 检查SHELLDLL_DefView的父窗口是否为当前的WorkerW窗口
                HWND parent = GetParent(aim_shell);
                if (parent != NULL) {
                    qDebug() << "Find SHELLDLL_DefView's Parent";
                    if (parent == worker) {
                        qDebug() << "Find!";

                        // 找到了正确的WorkerW窗口
                        shelldlldefview = aim_shell;
                        break; // 结束循环
                    }
                }
            }
        }
    } while (worker != NULL);

    if(shelldlldefview) {
        return shelldlldefview;
    }


    //2.programa/shell&workerw结构
    qDebug() << "Try to find shell with wokerW under progman";
    worker = FindWindowExA(progrmman, NULL, "WorkerW", NULL);
    if(worker) {
        qDebug() << "unable to find wokerW under progman";
        aim_shell = FindWindowExA(progrmman, NULL, "SHELLDLL_DefView", NULL);
        if(aim_shell) {
            qDebug() << "Find!";
            shelldlldefview = aim_shell;
        }

    }


    return shelldlldefview;
}

QColor mixColor(QColor source, QColor add, double ratio)
{
    return QColor(
               source.red() * ratio + add.red() * (1.0 - ratio),
               source.green() * ratio + add.green() * (1.0 - ratio),
               source.blue() * ratio + add.blue() * (1.0 - ratio)
           );
}

QColor winThemeColor()
{

    DWORD crColorization;
    BOOL fOpaqueBlend;
    QColor res;
    HRESULT result = DwmGetColorizationColor(&crColorization, &fOpaqueBlend);
    if (result == S_OK) {
        BYTE r, g, b;
        r = (crColorization >> 16) % 256;
        g = (crColorization >> 8) % 256;
        b = crColorization % 256;
        res = QColor(r, g, b);
    }
    return res;

}

QString extractString(QString res)
{
    QString code = QLocale::system().uiLanguages()[0];
    WCHAR wsBuffer[1024] = { 0 };
    memset(wsBuffer, 0, 1024 * sizeof(WCHAR));
    SHLoadIndirectString(res.toStdWString().c_str(), wsBuffer, 1024, NULL);
    return QString::fromStdWString(wsBuffer);
}

void shellContextMenuRun(QString command, QString path)
{
    command.replace("%V", path);
    command.replace("%v", path);
    command.replace("%1", path);
    if(command.contains(".exe")) {
        QString exe = command.mid(0, command.indexOf(".exe") + 4).simplified();
        if(exe[0] == "\"") {
            exe.remove("\"");
            exe.remove("\"");
        }


        QString para = command.mid(command.indexOf(".exe") + 5, command.length() - (command.indexOf(".exe") + 5) + 1).simplified();
        qDebug() << exe << para;
        qDebug() << shellrun(exe, para);
        return;
    }

    qDebug() << shellrun(command);
}

void test()
{
    GLPictureBox box;
    box.show();
}

bool isContainer(QWidget *aim)
{
    if(aim->inherits("SContainer")) {
        return true;
    }
    if(containerTypes.contains( aim->metaObject()->className())) {
        return true;
    }

    foreach (QString contanerType, containerTypes) {
        if(aim->inherits(contanerType.toStdString().c_str())) {
            return true;
        }
    }

    return false;

}

void initContainerTypes()
{
    containerTypes << "SContainer" << "SDir" << "MainWindow";
}

void initDrop()
{

    // HRESULT res = RevokeDragDrop(
}

void writeJsons()
{
    writeContent();
    writeMenu();
}

void writeMenu()
{
    pmh->write_json();
}

void readMenu()
{
    QJsonObject res;

    QFile file(DATA_PATH);

    if(!file.exists()) {
        writeMenu();
        return;
    }

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QString value = file.readAll();
    file.close();

    QJsonParseError parseJsonErr;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(), &parseJsonErr);
    if (! (parseJsonErr.error == QJsonParseError::NoError)) {
        QMessageBox::about(NULL, "提示", "菜单配置文件错误！");
        return;
    }

    pmh->read_json(document.object());
}
