#include<windows.h>
#include "layershower.h"
#include"mainwindow.h"
#include "qapplication.h"
#include "qpainter.h"
#include "qpainterpath.h"
#include"windows.h"
#include "shlobj.h"
#include"mousehook.h"
#include"SysFunctions.h"
#include"QTextCodec"
#include <QJsonObject>
#include<QDir>
#include<QFileInfo>
#include<QStandardPaths>
#include<QFileIconProvider>
#include<QSettings>
#include<QIcon>
#include <QMetaType>
#include <QtCore>
#include<QMessageBox>
#include<windows.h>
#include<uxtheme.h>

MainWindow* pmw;;
MouseHook* pmh;
LayerShower* pls;

QTextCodec* utf8 = QTextCodec::codecForName("utf-8");
QTextCodec* gbk = QTextCodec::codecForName("GBK");

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <dwmapi.h>
static QMutex mutex;

#define AUTO_RUN_KEY	"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
//设置/取消自启动
//isStart: true(开机启动)    false(开机不启动)

QColor GetWindowsThemeColor()
{
    DWORD crColorization;
    BOOL fOpaqueBlend;
    QColor res;
    HRESULT result = DwmGetColorizationColor(&crColorization, &fOpaqueBlend);
    if (result == S_OK)
    {
        BYTE r, g, b;
        r = (crColorization >> 16) % 256;
        g = (crColorization >> 8) % 256;
        b = crColorization % 256;
        res = QColor(r, g, b);
    }
    return res;
}


void setMyAppAutoRun(bool isStart)
{
    QString application_name = QApplication::applicationName();//获取应用名称
    QSettings *settings = new QSettings(AUTO_RUN_KEY, QSettings::NativeFormat);//创建QSetting, 需要添加QSetting头文件
    if(isStart)
    {
        QString application_path = QApplication::applicationFilePath();//找到应用的目录
        settings->setValue(application_name, application_path.replace("/", "\\"));//写入注册表
    }
    else
    {
        settings->remove(application_name);		//从注册表中删除
    }
}

void customMessageHandler(QtMsgType type,
                          const QMessageLogContext &context,
                          const QString &msg)
{
    //Q_UNUSED(context)
    QDateTime _datetime = QDateTime::currentDateTime();
    QString szDate = _datetime.toString("yyyy-MM-dd hh:mm:ss.zzz");//"yyyy-MM-dd hh:mm:ss ddd"
    QString txt(szDate);
switch (type)
    {
    case QtDebugMsg://调试信息提示
    {
        txt += QString(" [Debug] ");
        break;
    }
    case QtInfoMsg://信息输出
    {
        txt += QString(" [Info] ");
        break;
    }
    case QtWarningMsg://一般的warning提示
    {
        txt += QString(" [Warning] ");
        break;
    }
    case QtCriticalMsg://严重错误提示
    {
        txt += QString(" [Critical] ");
        break;
    }
    case QtFatalMsg://致命错误提示
    {
        txt += QString(" [Fatal] ");
        //abort();
        break;
    }
    default:
    {
        txt += QString(" [Trace] ");
        break;
    }
    }

    txt.append( QString(" %1").arg(context.file) );
    txt.append( QString("<%1>: ").arg(context.line) );
    txt.append(msg);

    mutex.lock();
    QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << txt << "\r\n";
    file.close();
    mutex.unlock();
}
QRect AbsoluteRect(QWidget* aim){
    auto tem = aim->geometry();
    auto pos = aim->parentWidget()->pos();
    return QRect(pos.x()+tem.x(),pos.y()+tem.y(),tem.width(),tem.height());
}

ED_Unit* from_json(QJsonObject data){
    QString name = data.value("Class").toString();
    int id = QMetaType::type(name.toStdString().c_str());
    if (id == QMetaType::UnknownType){
        qDebug()<<"error";
        return nullptr;
        ; // ERROR HERE

    }
    auto k = QMetaType::create(id);
    ED_Unit *unit = static_cast<ED_Unit*>(QMetaType::create(id));
    unit->load_json(data);
    return unit;

}

void paintRect(QWidget* aim,QColor color){
    bool another = true;
    bool choosen = false;
    if(aim->inherits("ED_Unit")) {
        another = ((ED_Unit*)aim)->showRect;
        choosen = ((ED_Unit*)aim)->onmouse;
    }
    if(ShowRect&(another)){
        QPainter painter(aim);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(color);
        painter.drawRect(aim->rect());;
    }
}

double rectLen(int w,int h){
    return sqrt(w*w+h*h);
}

void paintLight(QWidget* aim,QColor color){
    bool another = true;
    bool choosen = false;
    if(aim->inherits("ED_Unit")) {
        another = ((ED_Unit*)aim)->showLight;
        choosen = ((ED_Unit*)aim)->onmouse;
    }


    if(ShowLight&(another)){
        auto pos =aim->mapFromGlobal(aim->cursor().pos());
        QRadialGradient* radialGradient;
        if(enable_light_track&&choosen){
            radialGradient = new QRadialGradient(aim->width()/2 , aim->height()/2, rectLen(aim->width(),aim->height())/2,pos.x() ,pos.y());
        }
        else{
            radialGradient = new QRadialGradient(aim->width()/2 , aim->height()/2, rectLen(aim->width(),aim->height())/2,0, aim->height());
        }
        //创建了一个QRadialGradient对象实例，参数分别为中心坐标，半径长度和焦点坐标,如果需要对称那么中心坐标和焦点坐标要一致
        QPainter painter(aim);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        color.setAlpha(light_alpha_start);
        radialGradient->setColorAt(0,color);
        color.setAlpha(light_alpha_end);
        radialGradient->setColorAt(1.0,color);
        painter.setBrush(QBrush(*radialGradient));
        painter.drawRect(aim->rect());//在相应的坐标画出来
    }
}

void paintSide(QWidget* aim,QColor color){
    bool another = true;
    if(aim->inherits("ED_Unit")){

        another = ((ED_Unit*)aim)->showSide;
        // 恢复默认混合模式，绘制边框，如果没有则不用
        if(ShowSide&&another){
            QPainter painter(aim);
            QPainterPath path;
            //这里圆角区域需要根据dpi、size调整
            path.addRoundedRect(QRectF(aim->rect()).adjusted(1.5, 1.5, -1.5, -1.5), unit_radius, unit_radius);
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            painter.setPen(QPen(QColor(0xCA64EA), 1.0));
            painter.drawPath(path);
        }
    }
    else if(ShowSide){
        QPainter p(aim);
        p.setPen(color); //设置画笔记颜色
        p.drawRect(0, 0, aim->width() -1, aim->height() -1); //绘制边框
    }
}

void mouse_move(int x,int y){

    qDebug()<<"move"<<x<<y;
    // pmw->move(x,y);
}
void mouse_on(int x,int y){

    qDebug()<<"on"<<x<<y;
}
void mouse_off(int x,int y){

    qDebug()<<"off"<<x<<y;
}

void InitMouseHook(){
    pmh = new MouseHook();
    pmh->SetMouseMoveCallBack(mouse_move);
    pmh->SetMouseOffCallBack(mouse_off);
    pmh->SetMouseOnCallBack(mouse_on);
}


void Init(MainWindow* mainwindow){
    //初始化
    qDebug()<<"Initing";
    //无边框全屏

    //注入壁纸
    inplace((QWidget* )mainwindow);

    qDebug()<<"Initing done";
}



void inplace(QWidget* aim) {
    // 接入到图标层
    HWND background = NULL;
    HWND hwnd = FindWindowA("Progman", "Program Manager");
    HWND worker = NULL;

    // 循环查找WorkerW窗口
    do {
        worker = FindWindowExA(NULL, worker, "WorkerW", NULL);
        if (worker != NULL) {
            // 尝试找到SHELLDLL_DefView窗口
            HWND shelldlldefview = FindWindowExA(worker, NULL, "SHELLDLL_DefView", NULL);
            if (shelldlldefview != NULL) {
                // 检查SHELLDLL_DefView的父窗口是否为当前的WorkerW窗口
                HWND parent = GetParent(shelldlldefview);
                if (parent == worker) {
                    // 找到了正确的WorkerW窗口
                    background = worker;
                    break; // 结束循环
                }
            }
        }
    } while (worker != NULL);

    // 如果找到了正确的WorkerW窗口，设置父窗口
    if (background != NULL) {
        SetParent((HWND)aim->winId(), background);
        SetWindowPos((HWND)aim->winId(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        SetWindowPos((HWND)aim->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        SetFocus((HWND)aim->winId());
    } else {
        // 如果没有找到合适的WorkerW窗口，可以在这里处理错误
        qDebug() << "未能找到合适的WorkerW窗口";
    }
}

void inplace2(QWidget* pmw2) {
    // 接入到壁纸层
    HWND background = NULL;
    HWND hwnd = FindWindowA("Progman", "Program Manager");
    HWND worker = NULL;

    // 循环查找WorkerW窗口
    do
    {
        worker = FindWindowExA(NULL,worker,"WorkerW",NULL);
        if(worker!=NULL){
            char buff[200] = {0};

            int ret = GetClassName(worker,(WCHAR*)buff,sizeof(buff)*2);
            if(ret == 0){
                int err = GetLastError();
                qDebug()<<"err:"<<err;
            }
            //QString className = QString::fromUtf16((char16_t*)buff);
        }
        if(GetParent(worker) == hwnd){
            background = worker;
        }
    }while(worker !=NULL);
    SetParent((HWND)pmw2->winId(),background);

    // 如果找到了符合条件的 WorkerW 窗口，设置 Qt 窗口的父窗口
}

QList<FileInfo>getFormFileInfo(QFileInfo x){
    QList<FileInfo > files;
    FileInfo file;
    file.type = FileInfo::NORM;
    file.filePath=x.absoluteFilePath();
    QString fileName = x.fileName();
    int lastDotIndex = fileName.lastIndexOf('.');
    if (lastDotIndex != -1)
    {
        fileName.remove(lastDotIndex, fileName.length() - lastDotIndex);
    }
    file.name=fileName;
    qDebug()<<x.suffix().toLower()<<x.symLinkTarget();
    if (x.suffix().toLower() == "lnk" && enable_lnk_redirect)
    {
        // 处理快捷方式（.lnk 文件）
        QString target = x.symLinkTarget();
        if (!target.isEmpty())
        {
            qDebug()<<"redirect"<<target<<"success";

            QDir targetDir(QFileInfo(target).absolutePath());
            // QStringList iconFiles = targetDir.entryList(QStringList() << "*.ico", QDir::Files);
            // if (!iconFiles.isEmpty())
            // {
            //     file.icon = QIcon(targetDir.absoluteFilePath(iconFiles.first())); // 或者根据需要设置其他类型
            // }
            // if(file.icon.isNull())
            // {
            QFileIconProvider iconProvider;
            file.icon =iconProvider.icon(QFileInfo(target));
            // }
        }
    }
    else
    {
        QFileIconProvider a;

        a.setOptions(QFileIconProvider::Option::DontUseCustomDirectoryIcons);
        file.icon = a.icon(x);
    }
    //针对steam游戏
    QSettings shortcut(x.filePath(), QSettings::IniFormat);
    QString target = shortcut.value("InternetShortcut/URL").toString();
    QRegularExpression re("steam://rungameid/(\\d+)");
    QRegularExpressionMatch match = re.match(target);
    if (match.hasMatch())
    {
        file.multi =true;
        QString gameId = match.captured(1);
        //QString steamPath = "C:/Program Files (x86)/Steam/appcache/librarycache"; // 你的Steam安装路径
        QString steamPath;
        QSettings reg("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);
        steamPath = reg.value("SteamPath").toString()+"/appcache/librarycache";
        QStringList result;
        QDir directory(steamPath);
        QStringList steamfileList=directory.entryList(QDir::Files);

        QRegularExpression regex(gameId+"_library_600x900");
        //长竖图标版本
        steamfileList=directory.entryList(QDir::Files);


        foreach(const QString& steamfilename,steamfileList)
        {
            if(regex.match(steamfilename).hasMatch())
            {
                file.icon=QIcon(directory.absoluteFilePath(steamfilename));
                file.type = FileInfo::HORI;
                files.append(file);
            }
        }
        regex = QRegularExpression(gameId+"_header");
        //长横图标版本
        foreach(const QString& steamfilename,steamfileList)
        {
            if(regex.match(steamfilename).hasMatch())
            {
                file.icon=QIcon(directory.absoluteFilePath(steamfilename));
                file.type = FileInfo::VERT;
                files.append(file);
            }
        }


        regex = QRegularExpression(gameId+"_icon");
        //小图标版本
        foreach(const QString& steamfilename,steamfileList)
        {
            if(regex.match(steamfilename).hasMatch())
            {
                result.append(directory.absoluteFilePath(steamfilename));
                file.icon=QIcon(directory.absoluteFilePath(steamfilename));
                file.type = FileInfo::NORM;
            }
        }
    }
    files.append(file);
    qDebug()<<QString::fromLocal8Bit(x.absoluteFilePath().toLocal8Bit())<<files.size();
    return files;
    //以上为针对steam游戏
}

QList<FileInfo> scandesktopfiles(const QString &desktopPath)
{
    //对于指定桌面路径，返还桌面路径中的文件信息的列表
    QList<FileInfo> files;
    QDir desktopDir(desktopPath);
    desktopDir.setFilter(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);
    QFileInfoList fileInfoList=desktopDir.entryInfoList();
    foreach(const QFileInfo &x,fileInfoList)
    {
        files.append(getFormFileInfo(x));
    }
    std::sort(files.begin(), files.end());
    return files;
}


QList<FileInfo> scanalldesktopfiles()
{
    //寻找桌面路径，并返回两个桌面中所有文件信息的列表
    QList<FileInfo> files;
    // QString userdesktoppath=QDir::homePath()+"/Desktop";
    // files.append(scandesktopfiles(userdesktoppath));
    QString publicdesktoppath=QDir::toNativeSeparators("C:/Users/Public/Desktop");
    files.append(scandesktopfiles(publicdesktoppath));
    QString another = getDesktopPath();
    files.append(scandesktopfiles(another));
    std::sort(files.begin(),files.end());
    qDebug()<<publicdesktoppath<<another;
    return files;
}

QString GetCorrectUnicode(const QByteArray &ba)
{

    QTextCodec::ConverterState state;
    QTextCodec *codec = utf8;
    QString text = codec->toUnicode(ba.constData(), ba.size(), &state);
    if (state.invalidChars > 0)
    {
        qDebug()<<"GBK";
        text = gbk->toUnicode(ba);
    }
    else
    {
        qDebug()<<"utf-8";
        text = ba;
    }
    return text;
}

QString elidedLineText(QWidget *pWidget, int nLine, QString strText)
{
    //调整输出
    if (nLine == 0)
        return "";

    QFontMetrics fontMetrics(pWidget->font());

    if (nLine == 1) {
        return fontMetrics.elidedText(strText, Qt::ElideRight, pWidget->width());
    }

    QStringList strListLine;

    for (int i = 0; i < strText.size(); i++)
    {
        if (fontMetrics.width(strText.left(i)) >= pWidget->width())
        {
            strListLine.append(strText.left(i));
            if (strListLine.size() == nLine)
            {
                break;
            }
            strText = strText.right(strText.size() - i);
            i = 0;
        }
    }

    if (strListLine.size() < nLine)
    {
        if (!strText.isEmpty()) {
            strListLine.append(strText);
        }
    }

    bool bHasElided = true;
    if (fontMetrics.width(strText) < pWidget->width())
    {
        bHasElided = false;
    }

    if (bHasElided && !strListLine.isEmpty())
    {
        QString strLast = strListLine.last();
        QString strElided = "...";
        strLast.insert(strLast.length(), strElided);
        while (fontMetrics.width(strLast) >= pWidget->width())
        {
            strLast = strLast.replace(0, 1, "");
        }

        strListLine.replace(strListLine.count() - 1, strLast);
    }
    QString strResult = strListLine.join("\n");

    return strResult;
}


QColor pixmapMainColor(QPixmap p, double bright) //p为目标图片 bright为亮度系数，为1则表示保持不变
{
    int step=20; //步长：在图片中取点时两点之间的间隔，若为1,则取所有点，适当将此值调大有利于提高运行速度
    int t=0; //点数：记录一共取了多少个点，用于做计算平均数的分母
    QImage image=p.toImage(); //将Pixmap类型转为QImage类型
    int r=0,g=0,b=0; //三元色的值，分别用于记录各个点的rgb值的和
    for (int i=0;i<p.width();i+=step) {
        for (int j=0;j<p.height();j+=step) {
            if(image.valid(i,j)){ //判断该点是否有效
                t++; //点数加一
                QColor c=image.pixel(i,j); //获取该点的颜色
                r+=c.red(); //将获取到的各个颜色值分别累加到rgb三个变量中
                b+=c.blue();
                g+=c.green();
            }
        }
    }
    return QColor(int(bright*r/t)>255?255:int(bright*r/t),
                  int(bright*g/t)>255?255:int(bright*g/t),
                  int(bright*b/t)>255?255:int(bright*b/t)); //最后返回的值是亮度系数×平均数,若超出255则设置为255也就是最大值，防止乘与亮度系数后导致某些值大于255的情况。
}

void repaintAround(QWidget* aim){
    auto tem = aim->geometry();
    auto rrect = QRect(tem.x()-100,tem.y()-100,tem.width()+200,tem.height()+200);
    pmw->repaint(rrect);

}

QString  getDesktopPath()
{
    LPITEMIDLIST pidl;
    LPMALLOC pShellMalloc;
    char szDir[200];
    if (SUCCEEDED(SHGetMalloc(&pShellMalloc)))
    {
        if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl))) {
            // 如果成功返回true
            SHGetPathFromIDListA(pidl, szDir);
            pShellMalloc->Free(pidl);
        }
        pShellMalloc->Release();
    }

    return QString(szDir);
}

#define Write(TYPE,DIS,NAME)                            \
styleIni->setValue(#DIS"/"#NAME, QString::number(NAME));\

void writeStyleIni(){
    qDebug()<<"Write";
    //Qt中使用QSettings类读写ini文件
    //QSettings构造函数的第一个参数是ini文件的路径,第二个参数表示针对ini文件,第三个参数可以缺省
    QSettings *styleIni = new QSettings("style.ini", QSettings::IniFormat);
    //向ini文件中写入内容,setValue函数的两个参数是键值对

    Write(Int,color,sleep_alpha);
    Write(Int,color,active_alpha);

    Write(Int,color,sleep_alpha_deep);
    Write(Int,color,active_alpha_deep);

    Write(Float,color,sleep_color_ratio);
    Write(Float,color,active_color_ratio);

    Write(Int,effect,light_alpha_start);
    Write(Int,effect,light_alpha_end);

    Write(Int,effect,icon_shadow_alpha);
    Write(Int,effect,icon_shadow_blur_radius);

    Write(Int,effect,unit_shadow_alpha);
    Write(Int,effect,unit_shadow_blur_radius);

    Write(Int,render,unit_radius);

    Write(Bool,render,ShowRect);
    Write(Bool,render,ShowSide);
    Write(Bool,render,ShowLight);

    Write(Bool,render,enable_background_transparent);
    Write(Bool,render,enable_background_blur);
    Write(Bool,render,enable_light_track);

    Write(Bool,render,enable_intime_repaint);

    Write(Bool,preference,enable_image_fill);
    Write(Bool,preference,muilt_icon_default_type);

    Write(Double,preference,scale_fix_ratio);

    Write(Bool,preference,enable_lnk_redirect);

    //写入完成后删除指针
    delete styleIni;
}


#define Read(TYPE,DIS,NAME)                                   \
NAME = styleIni->value( #DIS"/"#NAME ).to##TYPE();  \

void readStyleIni(){

    QFileInfo fi("style.ini");
    if(fi.exists()){
        QSettings *styleIni = new QSettings("style.ini", QSettings::IniFormat);

        Read(Int,color,sleep_alpha);
        Read(Int,color,active_alpha);

        Read(Int,color,sleep_alpha_deep);
        Read(Int,color,active_alpha_deep);

        Read(Float,color,sleep_color_ratio);
        Read(Float,color,active_color_ratio);

        Read(Int,effect,light_alpha_start);
        Read(Int,effect,light_alpha_end);

        Read(Int,effect,icon_shadow_alpha);
        Read(Int,effect,icon_shadow_blur_radius);

        Read(Int,effect,unit_shadow_alpha);
        Read(Int,effect,unit_shadow_blur_radius);

        Read(Int,render,unit_radius);

        Read(Bool,render,ShowRect);
        Read(Bool,render,ShowSide);
        Read(Bool,render,ShowLight);

        Read(Bool,render,enable_background_transparent);
        Read(Bool,render,enable_background_blur);
        Read(Bool,render,enable_light_track);

        Read(Bool,render,enable_intime_repaint);

        Read(Bool,preference,enable_image_fill);
        Read(Bool,preference,muilt_icon_default_type);

        Read(Double,preference,scale_fix_ratio);

        Read(Bool,preference,enable_lnk_redirect);

        delete styleIni;
    }
    else{
        writeStyleIni();
    }
}

void writeJson(){
    QJsonObject rootObj =  pmw->inside->to_json();
    QJsonDocument document;
    document.setObject(rootObj);

    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    //根据实际填写路径
    QFile file("content.json");


    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
    {
        qDebug() << "file error!";
    }
    QTextStream in(&file);
    in.setCodec("UTF-8");
    in << json_str;

    file.close();   // 关闭file
}

void readJson(){
    QFile file("content.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString value = file.readAll();
    file.close();

    QJsonParseError parseJsonErr;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(), &parseJsonErr);
    if (! (parseJsonErr.error == QJsonParseError::NoError)) {
        QMessageBox::about(NULL, "提示", "配置文件错误！");
        return;
    }

    QJsonObject jsonObject = document.object();
    pmw->inside->load_json(jsonObject);

}
