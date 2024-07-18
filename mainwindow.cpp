#include "mainwindow.h"
#include "ed_bgshower.h"
#include "ed_blockcontainer.h"
#include "ed_block.h"
#include "ed_dock.h"
#include "ed_editbox.h"
#include "ed_hidetextblock.h"
#include "qgraphicseffect.h"
#include "qmimedata.h"
#include "qpainter.h"
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
ED_Unit *pMovingUnit = nullptr;
QDesktopWidget* pdt;
bool debug = true;
StyleHelper* psh;


#define SET_ANCTION(NAME,TEXT,FUCTION)\
QAction *NAME = new QAction(#TEXT);\
myMenu->addAction(NAME);\
connect(NAME, &QAction::triggered, this, [=]()FUCTION);

void MainWindow::setupActions()
{
    // 只要将某个QAction添加给对应的窗口, 这个action就是这个窗口右键菜单中的一个菜单项了
    // 在窗口中点击鼠标右键, 就可以显示这个菜单
    myMenu = new QMenu(this);
    // 给当前窗口添加QAction对象

    SET_ANCTION(act1,改变可见,
    { setShoweredVisibal(!showeredVisibal); })

    SET_ANCTION(act2,切换精简,
    {
    for(ED_Unit* content:*(inside->contents)){
        content->changeSimpleMode();
    }})


    SET_ANCTION(act3,新增文件,
    {
        QFileDialog* fd = new QFileDialog();
        QStringList filePaths =QFileDialog::getOpenFileNames(this, QStringLiteral("选择文件"),"D:/",nullptr,nullptr,QFileDialog::Options(QFileDialog::DontResolveSymlinks));;
        foreach (const QString& filePath, filePaths) {
            addAIcon(filePath);
        }
    })

    SET_ANCTION(act4,退出程序,
    {

        close();
        pls->close();

    })


    #ifdef QT_DEBUG

    SET_ANCTION(act5,获取背景,{
        capture();
    })

    #endif


    SET_ANCTION(act6,新建小型格子,{
        auto bc = new ED_BlockContainer(this,2,2,2,2,5,10,10);
        InitAUnit(bc);
    })



    SET_ANCTION(act7,新建中型格子,{
        auto bc = new ED_BlockContainer(this,3,3,3,3,5,15,15);
        InitAUnit(bc);
    })


    SET_ANCTION(act8,新建大型格子,{
        auto bc = new ED_BlockContainer(this,4,4,4,4,5,20,20);
        InitAUnit(bc);
    })


    SET_ANCTION(act9,新建Dock栏,{
        auto dock = new ED_Dock(this);
        InitAUnit(dock);
    })


    SET_ANCTION(act10,新建设置箱,{
        auto dock = new ED_EditBox(this);
        InitAUnit(dock);
    })

}
void MainWindow::setupUnits()
{
    // setMouseTracking(true);
    // 设置背景
    bgshower = new ED_BGShower(this);
    bgshower->setFixedSize(size());
    bgshower->setVisible(enable_background_blur);
    bgshower->move(0, 0);
    bgshower->setVisible(true);
    bgshower->lower();
    inside = new ED_BlockLayout(this, 20, 12, 5, 10, 10);
    inside->isMain = true;
    setBlur(enable_background_blur);


    // qDebug()<<edlayout->W_Container()<<edlayout->H_Container();

    QFileInfo fi("content.json");
    if(fi.exists()){
        readJson();
    }
    else{
        InitDesktop();
    }


    // 初始化一些
    bg = QPixmap(":/images/background");

    setVisible(true);
    inside->UpdateRegion();
    update();
    // bgshower->update();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    pmw = this;
    pdt = QApplication::desktop();
    ui->setupUi(this);
    setWindowState(Qt::WindowFullScreen);
    setAttribute(Qt::WA_TranslucentBackground);
    setAcceptDrops(true);
    Init(this);
    setFixedSize(QGuiApplication::primaryScreen()->availableSize());
    move(0,0);
    qDebug()<<rect()<<pos()<<geometry()<<mapToGlobal(QPoint(0,0)) ;

    move(-geometry().x(),-geometry().y());
    setupUnits();

    setupActions();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePer01second())); // slotCountMessage是我们需要执行的响应函数
    timer->start(50);                                                   // 每隔0.1s


    connect(this,&MainWindow::showerSize_changed,this,[=](QSize val){
        changeShower->repaint();
    });

    connect(this,&MainWindow::showerRadius_changed,this,[=](int val){
        qDebug()<<val;
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

    setShoweredVisibal(true);
    ed_update();
    updateBG();
}

void MainWindow::InitAUnit(ED_Unit *aim,bool animated)
{
    // connect(aim, &ED_Unit::sendSelf, this, &MainWindow::getObject);
    inside->defaultPut(aim,animated);
    // aim->ed_update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setScale(double scale)
{
    scale = qBound(0.001,scale,1.0);
    foreach(ED_Unit *content , *(inside->contents))
    {
        content->setScale(scale);
    }
    globalScale = scale;
    ED_EditBox *ed = findChild<ED_EditBox *>();
    if(ed!=nullptr){
        if(ed->scale_Slider!=nullptr){
            ed->scale_Slider->blockSignals(true);
            ed->scale_Slider->setValue(scale*100);
            ed->scale_Slider->blockSignals(false);
        }
    }
}

void MainWindow::ed_update()
{
    foreach(ED_Unit *content , *(inside->contents))
    {
        content->ed_update();
    }
}

void MainWindow::InitDesktop()
{
    // 获取图标
    QList<FileInfo> iconns = scanalldesktopfiles();
    QList<QString> nametem;
    addAIcon(iconns);

    auto eb = new ED_EditBox(this);
    InitAUnit(eb);

}

void MainWindow::capture()
{
    inside->setVisible(false,true);
    repaint();
    QThread::msleep(100);
    QScreen *screen = QGuiApplication::primaryScreen();
    bgshower->captrued = screen->grabWindow(0).copy(rect());
    bgshower->cap = true;
    inside->setVisible(true,true);
}

void MainWindow::updateBG()
{
    if(enable_background_transparent){
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
        foreach (ED_Unit* content,*(inside->contents_AlwaysShow)) {
            content->setVisible(false);
        }

        buffer = this->grab(rect());

        foreach (ED_Unit* content,*(inside->contents_AlwaysShow)) {
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
    addAIcon(QFileInfo(path));
}

void MainWindow::addAIcon(QFileInfo qinfo)
{
    QList<FileInfo>infos = getFormFileInfo(qinfo);
    addAIcon(infos);
}

void MainWindow::addAIcon(QList<FileInfo> infos)
{
    QList<QString> nametem;
    foreach (const FileInfo &file, infos)
    {
        qDebug() << file.name << file.type;
        int sizex = 1;
        int sizey = 1;

        ED_Unit *tem = nullptr;
        if (!nametem.contains(file.name))
        {

            if (!file.multi)
            {

                tem = new ED_Block(this, file.icon.pixmap(32), file.name, file.filePath, sizex, sizey);
                nametem.append(file.name);
            }
            else
            {
                switch (file.type)
                {
                case FileInfo::NORM:
                    if (muilt_icon_default_type == 0)
                    {

                        tem = new ED_Block(this, file.icon.pixmap(256), file.name, file.filePath, sizex, sizey);
                        nametem.append(file.name);
                    }
                    break;
                case FileInfo::HORI:
                    if (muilt_icon_default_type == 1)
                    {
                        tem = new ED_HideTextBlock(this, file.icon.pixmap(512), file.name, file.filePath, 2, 1);
                        nametem.append(file.name);
                    }
                    break;
                case FileInfo::VERT:
                    if (muilt_icon_default_type == 2)
                    {
                        tem = new ED_HideTextBlock(this, file.icon.pixmap(512), file.name, file.filePath, 1, 2);
                        nametem.append(file.name);
                    }

                    break;
                }
            }
        }

        if (tem)
        {
            InitAUnit(tem);
            tem->raise();
        }
    }
}

void MainWindow::appendPoints(QPoint p)
{
    if ( drawParamList.count() == 2 ) {
        drawParamList.removeLast(); // 移除最后一个点
    }
    drawParamList.append(p);
}

void MainWindow::updatePer01second()
{
    if (enable_intime_repaint)
        repaint();

    // qDebug()<<rect()<<pos()<<geometry()<<mapToGlobal(QPoint(0,0));
}

void MainWindow::dropEvent(QDropEvent *e)
{
    if(e->mimeData()->hasUrls())//处理期望数据类型
    {
        QList<QUrl> list = e->mimeData()->urls();//获取数据并保存到链表中
        for(int i = 0; i < list.count(); i++)
        {
            addAIcon( list[i].toLocalFile());
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

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    appendPoints(event->pos());
        pls->update();  // 启动paintEvent
    //  mouseReleaseAndControl();  // 在鼠标释放后调用快速定位的功能
    drawParamList.clear();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    appendPoints(event->pos());
    pls->update();
}

void MainWindow::closeEvent(QCloseEvent *event)//关闭窗口会先处理该事件函数
{

    event->accept();
    psh->writeStyleIni();
    writeJson();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // if(event->key()==Qt::Key_Shift){
    //     onShift = true;
    // }
    event->accept();


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
        // ContextMenu::show(QStringList() << "D:/", (void *)winId(), QCursor::pos());
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

void MainWindow::paintEvent(QPaintEvent *ev)
{
    if (!enable_background_transparent)
    {
        QPainter painter(this);
        painter.drawPixmap(rect(), bg);
    }

    if((!showeredVisibal)||(showeredVisibal&&showerAnimations->state()==QAnimationGroup::Running)){
        QPainter painter(this);
        painter.drawPixmap(rect(), buffer);
    }


    Q_UNUSED(ev);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *ev)
{
    setShoweredVisibal(!showeredVisibal);
    pdt->activateWindow();
    Q_UNUSED(ev);
}

void MainWindow::mousePressEvent(QMouseEvent* ev){

    appendPoints(ev->pos());
    printf("mousePressEvent \n");
    pls->raise();

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
    enable_background_transparent = val;
    if (enable_background_blur)
    {
        bgshower->setVisible(!val);
        bgshower->captrued = bg;
    }
    updateBG();
    // qDebug()<<transparent<<val;
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
