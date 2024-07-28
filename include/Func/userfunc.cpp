#include "userfunc.h"
#include "SysFunctions.h"
#include "filefunc.h"
#include "global.h"
#include "qclipboard.h"
#include "qdir.h"
#include "qmimedata.h"
#include "qscreen.h"
#include "qstandardpaths.h"
#include "screenfunc.h"
#include"QInputDialog"
#include"QMessageBox"
#include "sfile.h"
#include"snotice.h"
#include "unitfunc.h"

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


void checkForKey(QKeyEvent *event)
{
    if( event ->matches( QKeySequence::Copy ) )
    {

        QList<QUrl> copyfiles;
        foreach (SUnit* unit, pCelectedUnits) {
            if(unit->inherits("SFile")){
                QUrl url=QUrl::fromLocalFile(((SFile*)unit)->filePath);    //待复制的文件
                if(url.isValid()){
                    copyfiles.push_back(url);
                }
            }
        }
        qDebug()<<"Copied"<<copyfiles;

        QMimeData *data=new QMimeData;
        data->setUrls(copyfiles);

        QClipboard *clip=QApplication::clipboard();
        clip->setMimeData(data);
        event->accept();
        return;
    }
    else if( event ->matches( QKeySequence::Paste ) ){

        QClipboard *clip=QApplication::clipboard();
        if(clip->mimeData()->hasUrls())//处理期望数据类型
        {
            QList<QUrl> list = clip->mimeData()->urls();//获取数据并保存到链表中
            for(int i = 0; i < list.count(); i++)
            {
                QString path = list[i].toLocalFile();
                QString newPath =  okPathAbsolute((*UserDesktopPath)+"/"+QFileInfo(path).fileName());
                bool Copied = QFile::copy(path,newPath);
                qDebug()<<"Try to copy to"<<newPath;
                if(Copied){
                    qDebug()<<"Copid";
                    activepmw->addAIcon(newPath,true,QCursor::pos());
                }
            }
        }

        event->accept();
        return;
    }
    else if(event->matches(QKeySequence::Cut)){
        QList<QUrl> copyfiles;
        foreach (SUnit* unit, pCelectedUnits) {
            if(unit->inherits("SFile")){
                QUrl url=QUrl::fromLocalFile(((SFile*)unit)->filePath);    //待复制的文件
                if(url.isValid()){
                    copyfiles.push_back(url);

                }
            }
        }
        qDebug()<<"Cut"<<copyfiles;

        QMimeData *data=new QMimeData;
        data->setUrls(copyfiles);

        QClipboard *clip=QApplication::clipboard();
        clip->setMimeData(data);

        foreach (SUnit* unit, pCelectedUnits) {
            if(unit->inherits("SFile")){
                QUrl url=QUrl::fromLocalFile(((SFile*)unit)->filePath);    //待复制的文件
                if(url.isValid()){
                    QFile::remove(((SFile*)unit)->filePath);
                }
            }
        }

        event->accept();
        return;
    }
    else if( event->key()==Qt::Key_F2){
        if(pFocusedUnit!=nullptr){
            if(pFocusedUnit->inherits("SFile")){
                ((SFile*)pFocusedUnit)->renameWithDialog();
                event->accept();
            }
        }
        else if(numCelected==1){
            if(pCelectedUnits[0]->inherits("SFile")){
                ((SFile*)pCelectedUnits[0])->renameWithDialog();
                event->accept();
            }
        }
    }
    else if( event->key()==Qt::Key_Delete){
        removeG();
    }
    event->ignore();
    return;
}

void resizeForActiveMW()
{
    int sizeX = QInputDialog::getInt(nullptr,"重布局","请输入布局列数(根据屏幕宽度)",activepmw->inside->row,1,1000,2);
    int sizeY = QInputDialog::getInt(nullptr,"重布局","请输入布局行数(根据屏幕高度)",activepmw->inside->col,1,1000,2);
    if(!sizeX) sizeX=10;
    if(!sizeY) sizeY=10;
    activepmw->inside->resize(sizeX,sizeY);
}

void setMyAppAutoRun(bool isStart)
{
    QString application_name = QApplication::applicationName();//获取应用名称


    //清理旧数据
    QSettings *settingsold = new QSettings(AUTO_RUN_KEY_OLD, QSettings::NativeFormat);
    if(settingsold->contains(application_name)){
        settingsold->remove(application_name);
    }



    QSettings *settings = new QSettings(AUTO_RUN_KEY, QSettings::NativeFormat);//创建QSetting, 需要添加QSetting头文件
    if(isStart)
    {
        QString application_path = QApplication::applicationFilePath();//找到应用的目录
        settings->setValue(application_name, "\""+application_path.replace("/", "\\")+"\"");//写入注册表
    }
    else
    {
        settings->remove(application_name);		//从注册表中删除
    }
}

void setSapphireRegDate(bool isSet)
{
    QString application_name = QApplication::applicationName();//获取应用名称

    QSettings *settings = new QSettings(SAPPHIRE_REG_PATH, QSettings::NativeFormat);//创建QSetting, 需要添加QSetting头文件
    if(isSet)
    {
        QString application_path = QApplication::applicationFilePath();//找到应用的目录
        settings->setValue(application_name+"Path", application_path.replace("/", "\\"));//写入注册表
    }
    else
    {
        settings->remove(application_name);		//从注册表中删除
    }
}
