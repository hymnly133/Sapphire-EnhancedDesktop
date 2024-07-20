#include "mainwindow.h"
#include "ContextMenu/shellmenuitem.h"
#include "ed_bgshower.h"
#include"ContextMenu/desktopmenu.h"
#include "ed_blockcontainer.h"
#include "ed_block.h"
#include "ed_dock.h"
#include"QInputDialog"
#include "ed_editbox.h"
#include "ed_hidetextblock.h"
#include "filefunc.h"
#include "frozenthread.h"
#include "qgraphicseffect.h"
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

    foreach(auto pmw,pmws){
            pmw->close();
        }
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

#ifdef QT_DEBUG
    SET_ANCTION(act11,新建重绘盒,{
        auto dock = new RepaintCounterUnit(this);
        InitAUnit(dock);
    })
#endif
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


    setBlur(enable_background_blur);


    // 初始化一些
    bg = QPixmap(":/images/background");

    setVisible(true);
    inside->UpdateRegion();
    update();
    // bgshower->update();
}

void MainWindow::setupLayout(int x, int y)
{
    inside = new ED_BlockLayout(this, x, y, 5, 10, 10);
    inside->isMain = true;
    inside->pmw =this;
}

MainWindow::MainWindow(QWidget *parent, int screenInd)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    this->screenInd = screenInd;
    setObjectName("MainWindow"+QString::number(screenInd));
    ui->setupUi(this);
    setWindowState(Qt::WindowFullScreen);
    setAttribute(Qt::WA_TranslucentBackground);
    setAcceptDrops(true);
    inplace(this);

    setFixedSize(pscs[screenInd]->availableSize());
    move(pscs[screenInd]->geometry().x(),pscs[screenInd]->geometry().y());

    qDebug()<<"MainWindow"<<screenInd<<"Information:"<<rect()<<pos()<<geometry()<<mapToGlobal(QPoint(0,0))<<mapFromGlobal(QPoint(0,0)) ;
    auto geopos = geometry().topLeft();
    move(2*pos()-geopos);

    qDebug()<<"MainWindow"<<screenInd<<"Information Fixed:"<<rect()<<pos()<<geometry()<<mapToGlobal(QPoint(0,0))<<mapToGlobal(QPoint(0,0))<<mapFromGlobal(QPoint(0,0)) ;
    setupLayout(10,10);
    setupUnits();
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

    setShoweredVisibal(true);
    ed_update();
    updateBG();
}

void MainWindow::InitAUnit(ED_Unit *aim,bool animated)
{
    aim->setPMW(this);
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

QJsonObject MainWindow::to_json(){
    QJsonObject rootObject;
    rootObject.insert("ind",screenInd);
    rootObject.insert("content",inside->to_json());
    return rootObject;
}

void MainWindow::load_json(QJsonObject rootObject)
{
    qDebug()<<"MainWindow Loading json";
    screenInd = rootObject.value("ind").toInt();
    setupLayout(10,10);
    inside->load_json(rootObject.value("content").toObject());
}

void MainWindow::ed_update()
{
    foreach(ED_Unit *content , *(inside->contents))
    {
        content->ed_update();
    }
}

void MainWindow::setFrozen(bool val)
{

    if(val){
        FrozenThread* pft = new FrozenThread(this);
        pft->start();
    }
    else{
        isfrozen = val;
        inside->setVisible(true);
    }
}

void MainWindow::Init()
{
    // 获取图标
    qDebug()<<"Init Mainwindow"<<screenInd;
    QString name;
    if(screenInd==0) name+="主屏幕";
    else name+= "副屏幕";

    name+="初始化";
    int sizeX = QInputDialog::getInt(nullptr,name,"请输入布局宽度(根据屏幕大小)",20,1,1000,2);
    int sizeY = QInputDialog::getInt(nullptr,name,"请输入布局高度(根据屏幕大小)",12,1,1000,2);
    if(!sizeX) sizeX=10;
    if(!sizeY) sizeY=10;

    setupLayout(sizeX,sizeY);

    if(screenInd==0){
        qDebug()<<"Scaning";
        QVector<MyFileInfo> iconns = scanalldesktopfiles();
        foreach (MyFileInfo content, iconns) {
            addAIcon(content);
        }
        auto eb = new ED_EditBox(this);
        InitAUnit(eb);
    }
    qDebug()<<"Init Done";
}

void MainWindow::capture()
{
    update();
    buffer = QPixmap();
    buffer = grab(rect());
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

        capture();

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
    addAIcon(path2MyFI(path));
}

void MainWindow::addAIcon(QFileInfo qinfo)
{
    addAIcon(MyFileInfo(qinfo));
}

void MainWindow::addAIcon(MyFileInfo info)
{

    qDebug() <<"Adding to Desktop"<< info.name << info.type;

    ED_Block *tem = nullptr;


    if (info.type==MyFileInfo::SINGLE)
    {
        tem = new ED_Block(this);
        tem->loadFromMyFI(info);
    }
    else
    {
        switch (default_steam_icon_type)
        {
        case 0:
                tem = new ED_Block(this, 1, 1);
                tem->loadFromMyFI(info);

            break;
        case 1:

                tem = new ED_HideTextBlock(this,1,2);
                tem->loadFromMyFI(info);
            break;
        case 2:

                tem = new ED_HideTextBlock(this,2,1);
                tem->loadFromMyFI(info);
            break;
        }
    }


    if (tem)
    {
        InitAUnit(tem);
        tem->raise();
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

void MainWindow::whenFrozenThreadDone()
{
    isfrozen = true;
    inside->setVisible(false,true);
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
    appendPoints(event->globalPos());
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
        StyleSettingWindow* k = new StyleSettingWindow();
        k->show();
        // QMenu menu;
        // static bool isSwitch = false;
        // ShellMenuItems items;
        // if(isSwitch)
        //     items = DesktopMenu::computerShellItems();
        // else
        //     items = DesktopMenu::desktopMenuItems();
        // isSwitch = !isSwitch;

        // foreach(auto item, items)
        // {
        //     ShellMenuItems childItems;

        //     DesktopMenu::shellSubMenuItems(item, childItems, true);
        //     if(childItems.isEmpty())
        //     {
        //         menu.addAction(item.icon, item.caption, this, [=](){
        //             item.exec();
        //         });
        //     }
        //     else
        //     {
        //         QMenu *subMenu = new QMenu(item.caption);
        //         foreach(auto childItem, childItems)
        //         {
        //             subMenu->addAction(childItem.icon, childItem.caption, this, [=](){
        //                 QPoint p = QCursor::pos();
        //                 childItem.pItem->contextMenu((void *)winId(), p.x(), p.y());
        //             });
        //         }
        //         QAction* action = subMenu->menuAction();
        //         action->setIcon(item.icon);
        //         menu.addMenu(subMenu);
        //     }
        // }
        // menu.exec(QCursor::pos());
        // ContextMenu::show(QStringList()<<toLinuxPath(*UserDesktopPath),(HWND*)winId(),event->globalPos());
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
    qDebug()<<objectName()<<"FoucusIn";
}

void MainWindow::focusOutEvent(QFocusEvent *event)
{
    qDebug()<<objectName()<<"FoucusOut";
}

void MainWindow::enterEvent(QEvent *event)
{
    qDebug()<<objectName()<<"Enter";
    raise();
    pls->raise();
}

void MainWindow::leaveEvent(QEvent *event)
{
    qDebug()<<objectName()<<"Leave"<<cursor().pos()<<rect()<<geometry();
    if(screenNum>1){
        //多屏切换
        foreach(auto pmw,pmws){
            if(pmw->geometry().contains(cursor().pos()) && pmw!=this){
                qDebug()<<"ScreenChange! "<<pmw->objectName()<<"Should Be The Aim";
                if(pMovingUnit!=nullptr){
                    auto globalPos = mapToGlobal(pMovingUnit->pos());
                    auto aimPos = pmw->mapFromGlobal(globalPos);
                    pMovingUnit->releaseMouse();
                    pMovingUnit->setParent(pmw);
                    pMovingUnit->setPMW(pmw);
                    pMovingUnit->move(aimPos);
                    pMovingUnit->setVisible(true);
                    pMovingUnit->setEnabled(true);
                    pmw->setFocus();
                    qDebug()<<pmw->objectName()<<"Acitive:"<<pmw->isActiveWindow();
                    pmw->raise();
                    pMovingUnit->raise();
                    pls->raise();
                    pMovingUnit->setFocus();
                    pMovingUnit->grabMouse();
                }
            }
        }

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
    else if(isfrozen){
        QPainter painter(this);
        // painter.fillRect(rect(),QColor(0,0,0,0));
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

void MainWindow::mousePressEvent(QMouseEvent* event){

    appendPoints(event->globalPos());
    qDebug()<<objectName()<<"press"<<event->pos()<<event->globalPos()<<mapTo(this,event->pos());
    raise();
    pls->raise();
    // foreach(auto content,*(inside->contents)){
    //     qDebug()<<content->pos()<<content->parentWidget()->objectName()<<content->isVisible();
    // }
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
