#include<windows.h>
#include "filefunc.h"
#include"mainwindow.h"
#include "layershower.h"
#include "qapplication.h"
#include "qbitmap.h"
#include "qpainter.h"
#include "qpainterpath.h"
#include "qscreen.h"
#include "scontainer.h"
#include "screenfunc.h"
#include"windows.h"
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
#include"sfile.h"
#include"QWindowStateChangeEvent"
#include"snotice.h"
#include"QInputDialog"




static QMutex mutex;
bool firstNotice = true;
bool init = false;
bool isQuit = false;
QMap<int,MainWindow*> pmws;
MainWindow* activepmw = nullptr;
SUnit* processor;
QMap<int,QScreen*> pscs;
int screenNum;
int jsonNum;
StyleHelper* psh;
QDesktopWidget* pdt;
QString* UserDesktopPath;
QString* PublicDesktopPath;
bool moving_global;
QList<SUnit*> pCelectedUnits;
SUnit* pFocusedUnit = nullptr;
bool onLoading = true;
QMap<QString,SFile*> nowExits;
QMap<int,QJsonObject> UnusedJsons;
QList<QString> ExcludeFiles;

QTextCodec* utf8 = QTextCodec::codecForName("utf-8");
QTextCodec* gbk = QTextCodec::codecForName("GBK");

HWND shelldlldefview = NULL;


#define AUTO_RUN_KEY	"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
//设置/取消自启动
//isStart: true(开机启动)    false(开机不启动)

void SetUp()
{
    QString application_name = QApplication::applicationName();//获取应用名称
    QSettings *settings = new QSettings(AUTO_RUN_KEY, QSettings::NativeFormat);//创建QSetting, 需要添加QSetting头文件
    enable_auto_run = !settings->value(application_name).isNull();

    ExcludeFiles.append("desktop.ini");

    qDebug()<<"Setting Up...";
    UserDesktopPath = new  QString(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation)[0]);
    qDebug()<<"User Desktop:"<<*UserDesktopPath;
    PublicDesktopPath=new QString("C:/Users/Public/Desktop");
    qDebug()<<"Public Desktop:"<<*PublicDesktopPath;
    pdt = QApplication::desktop();
    pscs[0]=QGuiApplication::primaryScreen();

    Shift_Global = -pscs[0]->virtualGeometry().topLeft();



    // 通过循环可以遍历每个显示器
    QList<QScreen *> list_screen = QGuiApplication::screens();
    screenNum = list_screen.size();

    for (int i = 0; i < screenNum; i++)
    {
        QScreen * qs =list_screen.at(i);
        QRect rect = qs->geometry();
        qDebug()<<"Setting Screen"<<i<< rect<< (pscs[0] == qs);
        pscs[i] = list_screen[i];
    }
    qDebug()<<"Setting layerShower";

    QMap<int,QJsonObject> jsons = readJson();
    jsonNum = jsons.size();

    qDebug()<<"Find"<<jsonNum<<"Jsons";
    qDebug()<<"Find"<<screenNum<<"Screens";


    if(jsonNum == 0){
        //初始化
        init = true;
        if(screenNum!=1)
            QMessageBox::about(NULL, "提示",QString( "没有布局文件，即将初始化~\n你有%1个屏幕，将分别初始化\n请注意弹出窗口！").arg(screenNum));
        else{
            QMessageBox::about(NULL, "提示", "没有布局文件，即将初始化~\n请注意弹出窗口！");
        }

        qDebug()<<"Scaning";
        QList<MyFileInfo> iconns = scanalldesktopfiles();
        for(int i=0;i<screenNum;i++){
            qDebug()<<"Processing Mainwindow"<<i;
            pmws[i] = new MainWindow(nullptr,i);
            iconns = pmws[i]->Init(iconns);
        }
        if(!iconns.empty()){
            QMessageBox::about(nullptr,"(⊙o⊙)！！",QString("你选择的布局不足以容纳所有图标，请重启软件并重新选择布局\n（剩余：%1个）").arg(iconns.size()));
            isQuit = true;
            return;
        }
    }
    else{
        if(jsonNum<screenNum)
            QMessageBox::about(nullptr,"注意！",QString("存在%1个布局数据，检测到%2个屏幕，将开始初始化\n请注意弹出窗口！").arg(jsonNum).arg(screenNum));
        if(jsonNum>screenNum){
            QMessageBox::about(nullptr,"注意！",QString("存在%1个桌面数据，检测到%2个屏幕，将按顺序加载！多余的桌面数据不会被清理").arg(jsonNum).arg(screenNum));
            for(int i=screenNum;i<jsonNum;i++){
                UnusedJsons[i] = jsons[i];
            }
        }
        //加载
        for(int i=0;i<screenNum;i++){
            qDebug()<<"Processing Mainwindow"<<i;
            pmws[i] = new MainWindow(nullptr,i);
            if(jsons.contains(i)){
                pmws[i]->load_json(jsons[i]);
            }
            else{
                pmws[i]->Init();
            }
        }

    }
    for(int i=0;i<screenNum;i++){
        pmws[i]->endUpdate();
        pmws[i]->show();
    }

    qDebug()<<"Final:";
    for(int i=0;i<screenNum;i++){
        qDebug()<<"Mainwindow"<<i<<pmws[i]->mapToGlobal(QPoint(0,0));
    }
    activepmw = pmws[0];
    if(init)
    {
        SNotice::notice(QStringList()<<"现在Sapphire将会实时更新桌面文件！"<<"你在Sapphire中对图标的操作均会对应到系统文件中！","重要通知!",15000);
        SNotice::notice(QStringList()<<"为了使用部分桌面功能"<<"Sapphire会开始使用管理员权限","有关管理员权限",5000);
    }

    onLoading  =false;
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



void paintRect(QWidget* aim,QColor color){
    bool another = true;

    if(aim->inherits("SUnit")) {
        color.setAlpha(color.alpha()*((SUnit*)aim)->nowPadRatio);
        another = ((SUnit*)aim)->showRect;
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
    // qDebug()<<"Light color"<<color;
    bool another = true;
    bool choosen = false;
    int aim_alpha_start = light_alpha_start;
    int aim_alpha_end= light_alpha_end;
    if(aim->inherits("SUnit")) {
        aim_alpha_end*=((SUnit*)aim)->nowPadRatio;
        aim_alpha_start+=((SUnit*)aim)->nowPadRatio;
        // qDebug()<<color.alpha();
        another = ((SUnit*)aim)->showLight;
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
        color.setAlpha(aim_alpha_start);
        if(aim->inherits("SUnit")) {
            color.setAlpha(color.alpha()*((SUnit*)aim)->nowPadRatio);
        }
        radialGradient->setColorAt(0,color);
        color.setAlpha(aim_alpha_end);
        if(aim->inherits("SUnit")) {
            color.setAlpha(color.alpha()*((SUnit*)aim)->nowPadRatio);
            // qDebug()<<color.alpha();
        }
        radialGradient->setColorAt(1.0,color);
        painter.setBrush(QBrush(*radialGradient));
        painter.drawRect(aim->rect());//在相应的坐标画出来
    }
}

void paintSide(QWidget* aim,QColor color){
    bool another = true;
    if(aim->inherits("SUnit")){

        another = ((SUnit*)aim)->showSide;
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

void inplace(QWidget* aim) {
    // 接入到图标层
    qDebug()<<"Star Inplacing";
    qDebug()<<"Using Inplacing Func1";
    HWND background = NULL;
    HWND worker = NULL;

    qDebug()<<"Entering Loop";
    // 循环查找WorkerW窗口
    do {
        worker = FindWindowExA(NULL, worker, "WorkerW", NULL);
        if (worker != NULL) {
            qDebug()<<"Find WokerW";
            // 尝试找到SHELLDLL_DefView窗口
            HWND shelldlldefview = FindWindowExA(worker, NULL, "SHELLDLL_DefView", NULL);
            if (shelldlldefview != NULL) {
                qDebug()<<"Find SHELLDLL_DefView";
                // 检查SHELLDLL_DefView的父窗口是否为当前的WorkerW窗口
                HWND parent = GetParent(shelldlldefview);
                if (parent != NULL) {
                    qDebug()<<"Find SHELLDLL_DefView's Parent";
                    if (parent == worker) {
                        qDebug()<<"Right!";

                        // 找到了正确的WorkerW窗口
                        background = shelldlldefview;
                        break; // 结束循环
                    }
                }
            }
        }
    } while (worker != NULL);

    // 如果找到了正确的WorkerW窗口，设置父窗口
    if (background == NULL)
    {
        qDebug() << "Unable to find proper WorkerW,trying to use Fuc 2";

        HWND pPM = FindWindowA("Progman", "Program Manager");
        if(pPM!=NULL){
            qDebug()<<"Find Program Manager";
            // 尝试找到SHELLDLL_DefView窗口
            HWND shelldlldefview = FindWindowExA(pPM, NULL, "SHELLDLL_DefView", NULL);
            if(shelldlldefview!=NULL){
                qDebug()<<"Find SHELLDLL_DefView";
                // 检查SHELLDLL_DefView的父窗口是否为当前的WorkerW窗口
                HWND parent = GetParent(shelldlldefview);
                if(parent!=NULL){
                    qDebug()<<"Find SHELLDLL_DefView's Parent";
                    if (parent == pPM) {
                        qDebug()<<"Right!";
                        // 找到了正确的WorkerW窗口
                        background = shelldlldefview;// 结束循环
                    }
                }
            }
        }
    }


    if (background != NULL) {
        SetParent((HWND)aim->winId(), background);
        SetWindowPos((HWND)aim->winId(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        SetWindowPos((HWND)aim->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        SetFocus((HWND)aim->winId());
        shelldlldefview = background;
    }
    else{
        qDebug() << "Unable to find proper Program manager,Inplacing failed";
    }
}
void positionToScreen(QWidget* aim,int screenInd){
    aim->setFixedSize(pscs[screenInd]->availableSize());
    aim->move(pscs[screenInd]->geometry().topLeft()+Shift_Global);
    aim->move(2*aim->pos()-aim->geometry().topLeft());
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

QList<MyFileInfo> scandesktopfiles(const QString &desktopPath)
{
    //对于指定桌面路径，返还桌面路径中的文件信息的列表
    QList<MyFileInfo> files;
    QDir desktopDir(desktopPath);
    desktopDir.setFilter(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot|QDir::System);
    QFileInfoList fileInfoList=desktopDir.entryInfoList();
    foreach(const QFileInfo &x,fileInfoList)
    {
        files.append(MyFileInfo(x));
    }
    std::sort(files.begin(), files.end());
    return files;
}

QList<MyFileInfo> scanalldesktopfiles()
{
    //寻找桌面路径，并返回两个桌面中所有文件信息的列表
    QList<MyFileInfo> files;

    files.append(scandesktopfiles(*PublicDesktopPath));
    files.append(scandesktopfiles(*UserDesktopPath));

    std::sort(files.begin(),files.end());
    qDebug()<<*PublicDesktopPath<<*UserDesktopPath;
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
    pmws[screenInd(aim)]->repaint(rrect);

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

void sentToWallpaper(QPoint winpos){
    HWND wallpaper = NULL;
    HWND worker = NULL;

    qDebug()<<"Entering Loop";
    // 循环查找WorkerW窗口
    do {
        worker = FindWindowExA(NULL, worker, "WorkerW", NULL);
        if (worker != NULL) {
            qDebug()<<"Find WokerW";
            // 尝试找到SHELLDLL_DefView窗口
            HWND _wallpaper = FindWindowExA(worker, NULL, NULL, "WPELiveWallpaper");
            if (_wallpaper != NULL) {
                qDebug()<<"Find WPELiveWallpaper";
                // 检查SHELLDLL_DefView的父窗口是否为当前的WorkerW窗口
                HWND parent = GetParent(_wallpaper);
                if (parent != NULL) {
                    qDebug()<<"Find WPELiveWallpaper's Parent";
                    if (parent == worker) {
                        qDebug()<<"Right!";

                        // 找到了正确的WorkerW窗口
                        wallpaper = _wallpaper;
                        break; // 结束循环
                    }
                }
            }
        }
    } while (worker != NULL);
    LPARAM lParam=MAKELPARAM(winpos.x(),winpos.y());
    if(wallpaper!=NULL){
        qDebug()<<"sent"<<(int*)wallpaper;
        PostMessage (wallpaper, WM_LBUTTONDOWN, VK_LBUTTON, lParam);
        // mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    }

    if(shelldlldefview!=NULL){
        qDebug()<<"sent"<<(int*)shelldlldefview;
            PostMessage (shelldlldefview, WM_LBUTTONDOWN, VK_LBUTTON, lParam);
        // mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    }
    return;
}


void writeJson(){
    QJsonArray rootArrar;
    foreach (auto pmw, pmws) {
        qDebug()<<"Writing"<<pmw->objectName();
        QJsonObject rootObj =  pmw->to_json();
        rootArrar.append(rootObj);
    }
    for(int i=screenNum;i<jsonNum;i++){
        qDebug()<<"Writing Unused Json"<<i;
        rootArrar.append(UnusedJsons[i]);
    }

    QJsonDocument document;
    document.setArray(rootArrar);

    QByteArray byte_array = document.toJson(QJsonDocument::Indented);
    QString json_str(byte_array);
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

QMap<int,QJsonObject> readJson(){
    QMap<int,QJsonObject> res;

    QFile file("content.json");

    if(!file.exists()){

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


    if(document.isArray()){

        QJsonArray arr = document.array();
        qDebug()<<"Find array of"<<arr.size();

        for(int i=0;i<arr.size();i++){
            res.insert(i,arr[i].toObject());
        }
    }
    else{
        qDebug()<<"Find object";
        res.insert(0,document.object());
    }
    return res;
}




bool isPic(QString pah)
{
    bool bRet = false;
    QFile fi(pah);
    if (fi.open(QIODevice::ReadOnly)) {
        QPixmap pix;
        pix.loadFromData(fi.readAll());
        bRet = !pix.isNull();
        fi.close();
    }
    return bRet;
}

SUnit *from_json(QJsonObject data, SLayout *layout)
{
    QString name = data.value("Class").toString();
    QString newname = name.replace("ED_","S");
    int id = QMetaType::type(name.toStdString().c_str());
    if (id == QMetaType::UnknownType){
        qDebug()<<"error0";
        id = QMetaType::type(newname.toStdString().c_str());
        if(id == QMetaType::UnknownType){
            qDebug()<<"error1";
            return nullptr;
        }
    }
    qDebug()<<name;
    // auto k = QMetaType::create(id);
    SUnit *unit = static_cast<SUnit*>(QMetaType::create(id));
    unit->setPMW(layout->pmw);
    unit->setParent(layout->pContainer);
    unit->load_json(data);
    return unit;
}

QList<SUnit *> units(){
    QList<SUnit*> res;
    foreach(auto pmw,pmws){
        res.append(pmw->findChildren<SUnit*>());
    }
    return res;
}

QString shellrun(QString filename, QString para)
{
    HINSTANCE hNewExe = ShellExecuteA(NULL, "open", filename.toLocal8Bit(), para.toLocal8Bit(), NULL, SW_SHOW);
    long long code = (long long)hNewExe;
    if(code>32) return "Success!";
    QString sRet;
    switch(code)
    {
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
    return sRet;
}

void scanForChange()
{
    if(onLoading) return;
    //Scan For Loss
    QList<QString> keyList = nowExits.keys();
    QMap<QString,SFile*> loss;
    QVector<QString> newFiles;
    foreach (QString path, keyList) {
        if(!QFileInfo::exists(path)){
            if(QFileInfo(path).isFile())continue;
            // if(QFileInfo(path).isShortcut())continue;
            if(QFileInfo(path).isSymLink())continue;
            loss[path] = nowExits[path];
        }
        else if(ExcludeFiles.contains( QFileInfo(path).fileName())){
            loss[path] = nowExits[path];
        }
    }




    //Scan For New
    QDir uPathDir(*UserDesktopPath);
    qDebug()<<"Scanningfor"<<*UserDesktopPath;
    uPathDir.setFilter(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot|QDir::System);
    QStringList files = uPathDir.entryList();
    foreach (QString name, files) {
        QString absolute = uPathDir.absoluteFilePath(name);
        if(!nowExits.contains(absolute)){
            if(ExcludeFiles.contains(QFileInfo(absolute).fileName())) continue;
            newFiles.append(absolute);
        }
    }



    QDir pPathDir(*PublicDesktopPath);
    qDebug()<<"Scanningfor"<<*PublicDesktopPath;
    pPathDir.setFilter(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot|QDir::System);

    foreach (QString name, pPathDir.entryList()) {
        QString absolute = pPathDir.absoluteFilePath(name);
        if(!nowExits.contains(absolute)){
            newFiles.append(absolute);
        }
    }

    QStringList removefiles;
    QList<QString> removed = loss.keys();
    foreach (QString path, removed) {
        if(nowExits.contains(path))
            nowExits[path]->Remove();
        removefiles<<path;
        //提示
    }
    if(!removefiles.empty())
    SNotice::notice(removefiles,"移除文件",5000);

    QStringList newfiles;
    foreach (QString newfile, newFiles) {
        pmws[0]->addAIcon(newfile);
        pmws[0]->endUpdate();
        newfiles<<newfile;
        //提示
    }
    if(!newfiles.empty())
    SNotice::notice(newfiles,"新增文件",5000);

    writeJson();
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
void dragOut()
{
    // if(pCelectedUnits.size()>=4){
    //     foreach (auto k, pCelectedUnits) {
    //         k->setUpdatesEnabled(false);
    //     }
    // }
    foreach (auto k, pCelectedUnits) {
        k->onDragedOut();
    }
    moving_global = true;
}



void cleanCelect()
{
    bool animation = numCelected>4;
    foreach (auto k, pCelectedUnits) {
        k->setCelect(false,animation);
    }
    pCelectedUnits.clear();
    moving_global = false;
}

void releaseCelect()
{
    foreach (auto k, pCelectedUnits) {
        // k->setUpdatesEnabled(true);
        QPair<SLayout*,QPoint >res = deepFind(k);
        res.first->putUnit(k,res.second,true);
        k->moving = false;
        k->premove = false;
        k->updateFocusAnimation();
        if(processor!=nullptr){
            processor->onProcessAnother(k);
        }
    }
    if(numCelected<=1)cleanCelect();
    moving_global = false;
}

void moveCelect()
{
    foreach (auto k, pCelectedUnits) {
        k->move(activepmw->mapFromGlobal(QCursor::pos())-k->relativeP);
        // k->update();
    }

    //processor
    if(pCelectedUnits.size()==1){
        QPoint ind = activepmw->inside->SLayout::pos2Ind(activepmw->mapFromGlobal(QCursor::pos()));
        SUnit* aim = activepmw->inside->SLayout::ind2Unit(ind);
        if(aim!=nullptr){
            aim->preSetLongFocus(true);
        }
        foreach (SUnit* tem, *(activepmw->inside->contents)) {
            if(tem!=aim &&tem->preLongFocus){
                tem->preSetLongFocus(false);
            }
        }
    }
}

QPair<SLayout*,QPoint > deepFind(SUnit *aim)
{
    foreach(SContainer* container,*(activepmw->inside->insideContainers)){
        if(container->geometry().contains(aim->geometry().center())){
            QPoint ind = container->inside->clearPutableInd(aim);
            if(ind!=QPoint(-1,-1)){
                return QPair<SLayout*,QPoint>(container->inside,ind);
            }
        }
    }

    return QPair<SLayout*,QPoint>(activepmw->inside,activepmw->inside->clearPutableInd(aim));
}

void resizeForActiveMW()
{
    int sizeX = QInputDialog::getInt(nullptr,"重布局","请输入布局列数(根据屏幕宽度)",activepmw->inside->row,1,1000,2);
    int sizeY = QInputDialog::getInt(nullptr,"重布局","请输入布局行数(根据屏幕高度)",activepmw->inside->col,1,1000,2);
    if(!sizeX) sizeX=10;
    if(!sizeY) sizeY=10;
    activepmw->inside->resize(sizeX,sizeY);
}
