#include "mainwindow.h"
#include "sbgshower.h"
#include "snotice.h"
#include "sshellfuncunit.h"
#include "sblockcontainer.h"
#include "sfile.h"
#include "sdock.h"
#include"QInputDialog"
#include "seditbox.h"
#include "shidetextblock.h"
#include "filefunc.h"
#include "qgraphicseffect.h"
#include "qmessagebox.h"
#include "qmimedata.h"
#include "qpainter.h"
#include "repaintcounterunit.h"
#include "roundshower.h"
#include "ui_mainwindow.h"
#include "SysFunctions.h"
#include <QMouseEvent>
#include <QDebug>
#include <QWidget>
#include <QFileDialog>
#include <qtimer.h>
#include "QScreen"
#include"ContextMenu/contextmenu.h"
#include "QThread"
#include"style.h"
#include"qmenu.h"
#include"QClipboard"
#include"QMimeData"


#define SET_ANCTION(NAME,TEXT,MENU,FUCTION)\
QAction *NAME = new QAction(#TEXT);\
MENU->addAction(NAME);\
connect(NAME, &QAction::triggered, this, [=]()FUCTION);

void MainWindow::setupActions()
{
    // 只要将某个QAction添加给对应的窗口, 这个action就是这个窗口右键菜单中的一个菜单项了
    // 在窗口中点击鼠标右键, 就可以显示这个菜单
    myMenu = new QMenu(this);
    // 给当前窗口添加QAction对象

    SET_ANCTION(act1,改变可见,myMenu,
    { setShoweredVisibal(!showeredVisibal); })

    SET_ANCTION(act2,切换精简,myMenu,
    {
    for(SUnit* content:*(inside->contents)){
        content->changeSimpleMode();
    }})


    SET_ANCTION(act3,新增文件,myMenu,
    {
        QFileDialog* fd = new QFileDialog();
        QStringList filePaths =QFileDialog::getOpenFileNames(this, QStringLiteral("选择文件"),"D:/",nullptr,nullptr,QFileDialog::Options(QFileDialog::DontResolveSymlinks));;
        foreach (const QString& filePath, filePaths) {
            addAIcon(filePath);
        }
    })

    SET_ANCTION(act4,退出程序,myMenu,
    {

    foreach(auto pmw,pmws){
            pmw->close();
            if(plsBG!=nullptr)
            plsBG->close();
            pls->close();
            plsB->close();
        }
    })

    QAction *creatNewAction = new QAction(myMenu);
    creatNewAction->setText("新建");
    myMenu->addAction(creatNewAction);

    QMenu *creatNewMenu = new QMenu();
    creatNewAction->setMenu(creatNewMenu);


    #ifdef QT_DEBUG

    SET_ANCTION(act5,获取背景,myMenu,{
        capture();
    })

    SET_ANCTION(actupdate,EndUpdate,myMenu,{
        endUpdate();
    })

    #endif


    SET_ANCTION(act6,新建小型格子,creatNewMenu,{
        auto bc = new SBlockContainer(inside,2,2,2,2,5,10,10);
    })



    SET_ANCTION(act7,新建中型格子,creatNewMenu,{
        auto bc = new SBlockContainer(inside,3,3,3,3,5,15,15);
        // InitAUnit(bc);
    })


    SET_ANCTION(act8,新建大型格子,creatNewMenu,{
        auto bc = new SBlockContainer(inside,4,4,4,4,5,20,20);
        // InitAUnit(bc);
    })


    SET_ANCTION(act9,新建Dock栏,creatNewMenu,{
        auto dock = new SDock(inside);
        // InitAUnit(dock);
    })


    SET_ANCTION(act10,新建设置箱,creatNewMenu,{
        auto dock = new SEditBox(inside);
        // InitAUnit(dock);
    })


#ifdef QT_DEBUG

    SET_ANCTION(act11,新建重绘盒,creatNewMenu,{
        auto dock = new RepaintCounterUnit(inside);
        // InitAUnit(dock);
    })
#endif

    SET_ANCTION(act12,新建系统盒子,creatNewMenu,{
        auto dock = new SShellFuncUnit(inside);
        // InitAUnit(dock);
    })
}
void MainWindow::setupUnits()
{
    // setMouseTracking(true);
    // 设置背景
    bgshower = new SBGShower(this);
    bgshower->setFixedSize(size());
    bgshower->setVisible(enable_background_blur);
    bgshower->move(0, 0);
    bgshower->setVisible(true);
    bgshower->lower();


    setBlur(enable_background_blur);


    // 初始化一些
    bg = QPixmap(":/images/background");

    setVisible(true);
    // update();
    // bgshower->update();
}

void MainWindow::setupLayout(int x, int y)
{
    inside = new SBlockLayout(this, x, y, 5, 10, 10);
    inside->isMain = true;
    QRect tem = QRect(0,0,pscs[screenInd]->availableSize().width(),pscs[screenInd]->availableSize().height());
    inside->setStandalongRect(tem);
}

MainWindow::MainWindow(MainWindow *parent, int screenInd)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    setObjectName("MainWindow"+QString::number(screenInd));
    ui->setupUi(this);

    this->screenInd = screenInd;
    setFocusPolicy( Qt::StrongFocus );

    setWindowState(Qt::WindowFullScreen);
    setAttribute(Qt::WA_TranslucentBackground);
    // setWindowFlags(Qt::FramelessWindowHint);
    setAcceptDrops(true);
    inplace(this);
    positionToScreen(this,screenInd);
    //shift:Windows主屏幕位于全局的位置;



    // plsBG = new LayerBackground(this,screenInd);

    plsB = new LayerShower(this,screenInd);
    plsB->layer = LayerShower::Bottom;


    pls = new LayerShower(this,screenInd);
    pls->layer =LayerShower::Upper;


    qDebug()<<"MainWindow"<<screenInd<<"Information Fixed:"<<rect()<<pos()<<geometry()<<mapToGlobal(QPoint(0,0))<<mapFromGlobal(QPoint(0,0)) ;

    setupUnits();
    setupLayout(10,10);
    setupActions();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePer01second())); // slotCountMessage是我们需要执行的响应函数
    timer->start(50);                                                   // 每隔0.1s


    connect(this,&MainWindow::showerSize_changed,this,[=](QSize val){
        changeShower->repaint();
    });

    connect(this,&MainWindow::showerRadius_changed,this,[=](int val){
    });

    changeShower = new roundShower(this);
    changeShower->distri(&showerSize,&showerRadius);
    changeShower->setVisible(true);
    changeShower->move(0,0);

    showerSizeAnimation = new QPropertyAnimation(this,"showerSize");
    showerRadiusAnimation = new QPropertyAnimation(this,"showerRadius");
    showerSizeAnimation->setDuration(500);
    showerSizeAnimation->setEasingCurve(QEasingCurve::InSine);
    showerRadiusAnimation->setDuration(500);
    showerRadiusAnimation->setEasingCurve(QEasingCurve::InSine);

    showerAnimations = new QParallelAnimationGroup(this);
    showerAnimations->addAnimation(showerRadiusAnimation);
    showerAnimations->addAnimation(showerSizeAnimation);


    connect(showerAnimations,&QParallelAnimationGroup::finished,this,[=](){
        if (showeredVisibal){
            inside->setVisible(true);
            changeShower->lower();
        }
    });

    setTransparent(true);
    setShoweredVisibal(true);
    updateBG();


}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::raiseLayers()
{
    plsB->raise();
    raise();
    pls->raise();
}

void MainWindow::setScale(double scale)
{
    scale = qBound(0.001,scale,1.0);
    foreach(SUnit *content , *(inside->contents))
    {
        content->setScale(scale);
    }
    globalScale = scale;
    SEditBox *ed = findChild<SEditBox *>();
    if(ed!=nullptr){
        if(ed->scale_Slider!=nullptr){
            ed->scale_Slider->blockSignals(true);
            ed->scale_Slider->setValue(scale*100);
            ed->scale_Slider->blockSignals(false);
        }
    }
}

QJsonObject MainWindow::to_json(){
    QJsonObject rootObject;
    rootObject.insert("ind",screenInd);
    rootObject.insert("content",inside->to_json());
    return rootObject;
}

void MainWindow::load_json(QJsonObject rootObject)
{
    qDebug()<<objectName()<<"Loading json";
    screenInd = rootObject.value("ind").toInt();
    setupLayout(10,10);
    inside->load_json(rootObject.value("content").toObject());
    endUpdate();
}

QList<MyFileInfo> MainWindow::Init(QList<MyFileInfo> data)
{
    Init();
    while(!data.empty())
    {
        qDebug()<<"remmains"<<data.size();
        if(!inside->OKForDefaultPut(new SUnit())){
            break;
        }
        else{
            qDebug()<<"ok";
            addAIcon(data[0]);
            data.removeAt(0);
        }
        // inside->printOccupied();
    }
    qDebug()<<"Init"<<objectName() <<"Done,Remainin"<<data.size()<<"file data";
    return data;
}

void MainWindow::endUpdate()
{
    if(inside!=nullptr)
    foreach(SUnit *content , *(inside->contents))
    {
        content->endUpdate();
    }
}



void MainWindow::Init()
{
    qDebug()<<"Init Mainwindow"<<screenInd;
    QString name;
    if(screenInd==0) name+="主屏幕";
    else name+= "副屏幕";

    name+="初始化";
    int sizeX = QInputDialog::getInt(nullptr,name,"请输入布局列数(根据屏幕宽度)",20,1,1000,2);
    int sizeY = QInputDialog::getInt(nullptr,name,"请输入布局行数(根据屏幕高度)",12,1,1000,2);
    if(!sizeX) sizeX=10;
    if(!sizeY) sizeY=10;

    setupLayout(sizeX,sizeY);

    if(screenInd==0){
        auto su = new SShellFuncUnit(inside);
        auto eb = new SEditBox(inside);
    }
}

void MainWindow::capture()
{
    update();
    buffer = QPixmap();
    buffer = grab(rect());
}

void MainWindow::updateBG()
{
    if(transparent){
        if(!bgshower->cap)
        capture();
    }
    else{
        bgshower->captrued = bg;
    }
}

void MainWindow::setShoweredVisibal(bool val){
    // changeShower->raise();

    if (!val){
        foreach (SUnit* content,*(inside->contents)) {
            if(content->alwaysShow)
            content->setVisible(false);
        }
        if(!(showerAnimations->state()==QParallelAnimationGroup::Running))
        capture();

        foreach (SUnit* content,*(inside->contents)) {
            content->setVisible(true);
        }

        inside->setVisible(false);
    }


    showeredVisibal = val;


    updata_animation();
}

void MainWindow::updata_animation()
{

    showerAnimations->stop();

    showerSizeAnimation->setStartValue(showerSize);
    showerSizeAnimation->setEndValue(aim_showerSize());
    showerRadiusAnimation->setStartValue(showerRadius);
    showerRadiusAnimation->setEndValue(aim_showerRadius());

    showerAnimations->start();

}

void MainWindow::addAIcon(QString path)
{
    if(nowExits.contains(path)){
        SNotice::notice(QStringList()<<"文件已存在");
    }
    else{
        addAIcon(path2MyFI(path));
    }

}

void MainWindow::addAIcon(QFileInfo qinfo)
{
     addAIcon(MyFileInfo(qinfo));
}

void MainWindow::addAIcon(MyFileInfo info)
{

    if(!inside->OKForDefaultPut(new SFile())) return ;

    qDebug() <<"Adding to Desktop"<< info.name << info.type;

    SFile *tem = nullptr;
    tem = new SFile(inside);
    tem->loadFromMyFI(info,true);
    // qDebug()<<tem->colorAlpha;

    if (tem)
    {
        tem->raise();
    }

}

void MainWindow::appendPoints(QPoint p)
{
    if ( celectPointList.count() == 2 ) {
        celectPointList.removeLast(); // 移除最后一个点
    }
    celectPointList.append(p);
}

void MainWindow::updatePer01second()
{
    if (enable_intime_repaint){
        repaint();
    }

    // qDebug()<<rect()<<pos()<<geometry()<<mapToGlobal(QPoint(0,0));
}


void MainWindow::dropEvent(QDropEvent *e)
{
    if(e->mimeData()->hasUrls())//处理期望数据类型
    {
        bool move =false;
        QMessageBox::StandardButton box;
        box = QMessageBox::question(this, "你拖入了文件", "创建映射（是）/ 移动文件（否）?", QMessageBox::Yes|QMessageBox::No);
        if(box==QMessageBox::No){
            move  =true;
        }

        QList<QUrl> list = e->mimeData()->urls();//获取数据并保存到链表中
        for(int i = 0; i < list.count(); i++)
        {
            QString path = list[i].toLocalFile();

            if(move){
                QString newName =  (*UserDesktopPath)+"/"+QFileInfo(path).fileName();
                bool removed = QFile::rename(path,newName);
                    qDebug()<<"move"<<newName;
                if(removed){
                    qDebug()<<"moved";
                }
                addAIcon(newName);
            }
            else{
                addAIcon(path);
            }

        }
    }
    else
    {
        e->ignore();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData()->hasUrls())//判断数据类型
    {
        e->acceptProposedAction();//接收该数据类型拖拽事件
    }
    else
    {
        e->ignore();//忽略
    }
}



void MainWindow::closeEvent(QCloseEvent *event)//关闭窗口会先处理该事件函数
{
    pls->close();
    event->accept();
    psh->writeStyleIni();
    writeJson();
}



void MainWindow::keyPressEvent(QKeyEvent *event)
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
                QString newName =  (*UserDesktopPath)+"/"+QFileInfo(path).fileName();
                bool removed = QFile::rename(path,newName);
                qDebug()<<"move"<<newName;
                if(removed){
                    qDebug()<<"moved";
                }
                addAIcon(newName);
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

    event->ignore();
    return;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    // if(event->key()==Qt::Key_Shift){
    //     onShift = false;
    // }
    event->accept();
}



void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if(event->modifiers() == Qt::ShiftModifier){
        // SettingWindow* k = new SettingWindow();
        StyleSettingWindow* k = new StyleSettingWindow;
        k->show();
    }
    else
        myMenu->exec(event->globalPos());

}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() ==Qt::ShiftModifier){
        qDebug()<<event->angleDelta();
        setScale((globalScale+(event->angleDelta().y()/1000.0)));
    }
}



void MainWindow::focusInEvent(QFocusEvent *event)
{
    pls->raise();
    // lower();
    qDebug()<<objectName()<<"FoucusIn";
    scanForChange();
}

void MainWindow::focusOutEvent(QFocusEvent *event)
{
    qDebug()<<objectName()<<"FoucusOut";
}



void MainWindow::enterEvent(QEvent *event)
{
    activepmw = this;
    qDebug()<<objectName()<<"Enter";
    // if(!onLoading)l
    // // scanForChange();
    // raise();
}

void MainWindow::leaveEvent(QEvent *event)
{
    qDebug()<<objectName()<<"Leave"<<cursor().pos()<<rect()<<geometry();
}

void MainWindow::showEvent(QShowEvent *event)
{
        raiseLayers();
    QMainWindow::showEvent(event);
    raiseLayers();
    // lower();
    endUpdate();
}

void MainWindow::paintEvent(QPaintEvent *ev)
{
    QPainter paint(this);
    paint.setPen(Qt::transparent);
    paint.setBrush(QColor(0, 0, 0, 1));
    paint.drawRect(rect());

    if (!transparent)
    {
        QPainter painter(this);
        painter.drawPixmap(rect(), bg);
    }
    // qDebug()<<showeredVisibal;
    // qDebug()<<(showerAnimations->state()==QAnimationGroup::Running);
    if((!showeredVisibal)||(showeredVisibal&&showerAnimations->state()==QAnimationGroup::Running)){
        QPainter painter(this);
        painter.drawPixmap(rect(), buffer);
    }
    Q_UNUSED(ev);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *ev)
{
    setShoweredVisibal(!showeredVisibal);
    Q_UNUSED(ev);
}

void MainWindow::mousePressEvent(QMouseEvent* event){
    cleanCelect();

    appendPoints(event->pos());
    inside->printOccupied();
    pls->Clear();

    if(firstNotice&&init){
        firstNotice = false;
    }

#ifdef QT_DEBUG
    // SToolTip::Tip("样例文本");
    // // SNotice::notice(QStringList()<<"现在Sapphire将会实时更新桌面文件！"<<"你在Sapphire中对图标的操作均会对应到系统文件中！","重要通知!",15000);
    // SNotice::notice(QStringList()<<"infoTestinfoTestinfoTestinfoTestinfoTest","TitleTest");
#endif
    // SNotice::notice(QStringList()<<"infoTestfdfdfghgfdfghjhgfdhgffghfhdgfggfhdfghdfghdffdfdfdsdsdssds","TitleTest");
    // raiseLayers();
    pls->raise();

}



void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    appendPoints(event->pos());
    pls->update();

    celectPointList.clear();
    if(celectPointList.size()==2){
        QRect aimRect = Point2Rect(celectPointList[0],celectPointList[1]);
        foreach (SUnit* k , *(inside->contents)) {
            if(aimRect.contains( k->rect().center())){
                pCelectedUnits.append(k);
                k->setCelect(true);
            }
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

    appendPoints(event->pos());

    if(celectPointList.size()==2){
        QRect aimRect = Point2Rect(celectPointList[0],celectPointList[1]);
        foreach (SUnit* k , *(inside->contents)) {
            if(!pCelectedUnits.contains(k)){
                if(aimRect.contains( k->geometry().center())){
                    qDebug()<<"CONTAIN";
                    pCelectedUnits.append(k);
                    k->setCelect(true);
                }
            }
            else{
                if(!aimRect.contains( k->geometry().center())){
                    qDebug()<<"RELEASE";
                    k->setCelect(false);

                }
            }
        }
    }
    pls->raise();
    pls->update();
}



void MainWindow::onSelectBackground()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择背景文件", "", "Images (*.png *.jpg *.bmp);;Videos (*.mp4 *.avi *.mkv)");
    qDebug() << "Selected file:" << fileName; // 调试输出文件路径
    if (!fileName.isEmpty())
    {
        if (fileName.endsWith(".png") || fileName.endsWith(".jpg") || fileName.endsWith(".bmp"))
        {
            QPalette palette;
            QPixmap pixmap(fileName);
            if (pixmap.isNull())
            {
                qDebug() << "Failed to load image";
            }
            else
            {
                bg = pixmap;
                update();
                qDebug() << "Image set as background";
            }
        }
    }
}

void MainWindow::setTransparent(bool val)
{
    transparent = val;
    if (enable_background_blur)
    {
        bgshower->setVisible(!val);
        bgshower->captrued = bg;
    }

    if(val){
        setFixedSize(pscs[screenInd]->availableSize());
    }
    else{
            // setFixedSize(pscs[screenInd]->size()-QSize(10,10));
            setFixedSize(pscs[screenInd]->availableSize());
    }
    update();
    updateBG();
}

void MainWindow::setBlur(bool val)
{
    enable_background_blur = val;
    bgshower->setEnabled(val);
    bgshower->setVisible(val);
    bgshower->lower();
    updateBG();
    // qDebug()<<transparent<<val;
}
