#include "mainwindow.h"
#include "ed_bgshower.h"
#include "ed_container.h"
#include "ed_block.h"
#include "ed_dock.h"
#include "ed_editbox.h"
#include "ed_hidetextblock.h"
#include "qgraphicseffect.h"
#include "qpainter.h"
#include "ui_mainwindow.h"
#include "SysFunctions.h"
#include <QMouseEvent>
#include <QDebug>
#include <QWidget>
#include <QFileDialog>
#include <qtimer.h>
#include "QScreen"
#include "QThread"
#include"QStyle"
ED_Unit *pMovingUnit = nullptr;
QDesktopWidget* pdt;

void MainWindow::setupActions()
{
    // 只要将某个QAction添加给对应的窗口, 这个action就是这个窗口右键菜单中的一个菜单项了
    // 在窗口中点击鼠标右键, 就可以显示这个菜单
    setContextMenuPolicy(Qt::ActionsContextMenu);
    // 给当前窗口添加QAction对象
    QAction *act1 = new QAction("改变可见");
    this->addAction(act1);
    connect(act1, &QAction::triggered, this, [=]()
            { edlayout->setVisible(!edlayout->Visible()); });

    QAction *act2 = new QAction("改变复杂度");
    this->addAction(act2);

    connect(act2, &QAction::triggered, this, [=]()
            {
                for(ED_Unit* content:*(edlayout->contents)){
                    content->changeSimpleMode();
                } });

    QAction *act3 = new QAction("新增文件");
    this->addAction(act3);
    connect(act3, &QAction::triggered, this, [=]()
    {

        QStringList filePaths =QFileDialog::getOpenFileNames(this, QStringLiteral("选择文件"));;

        foreach (const QString& filePath, filePaths) {
            QFileInfo qinfo(filePath);
            QList<FileInfo>infos = getFormFileInfo(qinfo);
            foreach (const FileInfo& info, infos) {
                auto tem = new ED_Block(this, info.icon.pixmap(256), info.name, info.filePath, 1, 1);
                edlayout->InitAUnit(tem);

                repaintAround(tem);
            }
        }


    });

    QAction *act4 = new QAction("退出程序");
    this->addAction(act4);
    connect(act4, &QAction::triggered, this, [=]()
            { close(); });

    QAction *act5 = new QAction("获取背景");
    this->addAction(act5);
    connect(act5, &QAction::triggered, this, [=]()
            {
                setVisible(false);
                QThread::msleep(200);
                QScreen *screen = QGuiApplication::primaryScreen();
                bgshower->captrued = screen->grabWindow(0);
                bgshower->cap = true;
                setVisible(true); });

    QAction *act6 = new QAction("新建小型格子");
    this->addAction(act6);
    connect(act6, &QAction::triggered, this, [=]()
            {
                auto bc = new ED_Container(this,2,2,2,2,5,10,10);
                InitAUnit(bc); });
    QAction *act7 = new QAction("新建中型格子");
    this->addAction(act7);
    connect(act7, &QAction::triggered, this, [=]()
            {
                auto bc = new ED_Container(this,3,3,3,3,5,15,15);
                InitAUnit(bc); });

    QAction *act8 = new QAction("新建大型格子");
    this->addAction(act8);
    connect(act8, &QAction::triggered, this, [=]()
            {
                auto bc = new ED_Container(this,4,4,4,4,5,20,20);
                InitAUnit(bc); });

    QAction *act9 = new QAction("新建dock栏");
    this->addAction(act9);
    connect(act9, &QAction::triggered, this, [=]()
            {
                auto dock = new ED_Dock(this,6,1,4);
                InitAUnit(dock); });
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

    edlayout = new ED_Layout(this, 20, 12, 5, 10, 10);
    edlayout->isMain = true;
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
    edlayout->Update_Region();
    update();
    // bgshower->update();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), weatherwidget(nullptr)
{
    pmw = this;
    pdt = QApplication::desktop();
    ui->setupUi(this);
    setWindowState(Qt::WindowFullScreen);
        // mainwindow->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    // setWindowFlags(Qt::FramelessWindowHint|Qt::Window);

    setAttribute(Qt::WA_TranslucentBackground);


    Init(this);

    setFixedSize(QGuiApplication::primaryScreen()->availableSize());
    move(0,0);
    qDebug()<<rect()<<pos()<<geometry()<<mapToGlobal(QPoint(0,0)) ;
    move(-geometry().x(),-geometry().y());
    // auto tem = -mapToGlobal(QPoint(0,0));
    // move(tem);
    setupUnits();

    setupActions();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePer01second())); // slotCountMessage是我们需要执行的响应函数
    timer->start(50);                                                   // 每隔0.1s
}

void MainWindow::InitAUnit(ED_Unit *aim)
{
    // connect(aim, &ED_Unit::sendSelf, this, &MainWindow::getObject);
    edlayout->InitAUnit(aim);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setScale(double scale)
{
    foreach(ED_Unit *content , *(edlayout->contents))
    {
        content->setScale(scale);
    }
}

void MainWindow::ed_update()
{
    foreach(ED_Unit *content , *(edlayout->contents))
    {
        content->ed_update();
    }
}

void MainWindow::InitDesktop()
{
    // 获取图标
    QList<FileInfo> iconns = scanalldesktopfiles();
    QList<QString> nametem;
    foreach (const FileInfo &file, iconns)
    {
        qDebug() << file.name << file.type;
        int sizex = 1;
        int sizey = 1;

        ED_Unit *tem = nullptr;
        if (!nametem.contains(file.name))
        {

            if (!file.multi)
            {

                tem = new ED_Block(this, file.icon.pixmap(256), file.name, file.filePath, sizex, sizey);
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

    auto eb = new ED_EditBox(this);
    InitAUnit(eb);

    weatherwidget = new Weather(this, 4, 2);
    InitAUnit(weatherwidget);
    htkt = new Hitokoto(this, 2, 1);
    InitAUnit(htkt);
}


void MainWindow::updatePer01second()
{
    if (enable_intime_repaint)
        repaint();

    // qDebug()<<rect()<<pos()<<geometry()<<mapToGlobal(QPoint(0,0));
}

void MainWindow::paintEvent(QPaintEvent *ev)
{
    QPainter painter0(this);
    painter0.fillRect(rect(),QColor(1,1,1,1));
    if (!enable_background_transparent)
    {
        QPainter painter(this);
        painter.drawPixmap(rect(), bg);
    }
    Q_UNUSED(ev);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *ev)
{
    edlayout->setVisible(!edlayout->Visible());
    pdt->activateWindow();
    Q_UNUSED(ev);
}

void MainWindow::mousePressEvent(QMouseEvent* ev){
    pdt->activateWindow();
    Q_UNUSED(ev);
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
    // qDebug()<<transparent<<val;
}

void MainWindow::setBlur(bool val)
{
    enable_background_blur = val;

    bgshower->setEnabled(val);
    bgshower->setVisible(val);
    bgshower->lower();

    // qDebug()<<transparent<<val;
}
