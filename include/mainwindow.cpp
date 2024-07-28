#include "mainwindow.h"
#include "global.h"
#include "sbgshower.h"
#include "snotice.h"
#include "sshellfuncunit.h"
#include "sblockcontainer.h"
#include "sfile.h"
#include "sdock.h"
#include"QInputDialog"
#include "seditbox.h"
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
#include"QClipboard"
#include"QMimeData"
#include "unitfunc.h"
#include "userfunc.h"
#include <Shlobj.h>
#include <shlwapi.h>
#include <windows.h>
#include"stooltip.h"
#define SET_ANCTION(NAME,TEXT,MENU,FUCTION)\
QAction *NAME = new QAction(#TEXT);\
MENU->addAction(NAME);\
connect(NAME, &QAction::triggered, this, [=]()FUCTION);

void MainWindow::setupActions()
{
    // 只要将某个QAction添加给对应的窗口, 这个action就是这个窗口右键菜单中的一个菜单项了
    // 在窗口中点击鼠标右键, 就可以显示这个菜单
    myMenu = new SMenu(pls);
    // 给当前窗口添加QAction对象

    SET_ANCTION(act1,改变可见,myMenu,
    { setShoweredVisibal(!showeredVisibal); })

    SET_ANCTION(act2,切换精简,myMenu,
    {
    for(SUnit* content:*(inside->contents)){
        content->changeSimpleMode();
    }})


    SET_ANCTION(act3,映射文件,myMenu,
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

    QAction *creatNewUnitAction = new QAction(myMenu);
    creatNewUnitAction->setText("新建组件");
    myMenu->addAction(creatNewUnitAction);

    SMenu *creatNewUnitMenu = new SMenu();
    creatNewUnitAction->setMenu(creatNewUnitMenu);


    #ifdef QT_DEBUG

    SET_ANCTION(act5,获取背景,myMenu,{
        capture();
    })

    SET_ANCTION(actupdate,EndUpdate,myMenu,{
        endUpdate();
    })

    #endif


    SET_ANCTION(act6,小型格子,creatNewUnitMenu,{
        auto bc = new SBlockContainer(inside,2,2,2,2,5,10,10);
    })



    SET_ANCTION(act7,中型格子,creatNewUnitMenu,{
        auto bc = new SBlockContainer(inside,3,3,3,3,5,15,15);
        // InitAUnit(bc);
    })


    SET_ANCTION(act8,大型格子,creatNewUnitMenu,{
        auto bc = new SBlockContainer(inside,4,4,4,4,5,20,20);
        // InitAUnit(bc);
    })


    SET_ANCTION(act9,Dock栏,creatNewUnitMenu,{
        auto dock = new SDock(inside);
        // InitAUnit(dock);
    })


    SET_ANCTION(act10,设置箱,creatNewUnitMenu,{
        auto dock = new SEditBox(inside);
        // InitAUnit(dock);
    })


#ifdef QT_DEBUG

    SET_ANCTION(act11,重绘盒,creatNewUnitMenu,{
        auto dock = new RepaintCounterUnit(inside);
        // InitAUnit(dock);
    })
    SET_ANCTION(act151,lowerAndUpdate,myMenu,{
        qDebug()<<changeShower->geometry();
        changeShower->lower();
        changeShower->setVisible(!changeShower->isVisible());
        changeShower->updateDisplay();
        changeShower->update();
        // InitAUnit(dock);
    })
#endif

    SET_ANCTION(act12,系统盒子,creatNewUnitMenu,{
        auto dock = new SShellFuncUnit(inside);
        // InitAUnit(dock);
    })


    QAction *creatNewFileAction = new QAction(myMenu);
    creatNewFileAction->setText("新建文件");
    myMenu->addAction(creatNewFileAction);

    SMenu *creatNewFileMenu = new SMenu();
    creatNewFileAction->setMenu(creatNewFileMenu);

    SET_ANCTION(act13,文本文档,creatNewFileMenu,{
        fileCreator::creatNewFile(FileType::txt);
                                                   });
    SET_ANCTION(act14,PPT演示文稿,creatNewFileMenu,{
        fileCreator::creatNewFile(FileType::pptx);
    });
    SET_ANCTION(act15,Word文档,creatNewFileMenu,{
        fileCreator::creatNewFile(FileType::docx);
    });
    SET_ANCTION(act16,空文件,creatNewFileMenu,{
        fileCreator::creatNewFile(FileType::empty);
    });

    QAction *systemSettingMenuAction = new QAction(myMenu);
    systemSettingMenuAction->setText("系统设置");
    myMenu->addAction(systemSettingMenuAction);

    connect(systemSettingMenuAction,&QAction::triggered,this,[=](){
        qDebug()<<"triggered";
            shellrun("ms-settings:");
    });

    SMenu *systemSettingMenu = new SMenu();
    systemSettingMenuAction->setMenu(systemSettingMenu);
    SET_ANCTION(act00,主面板,systemSettingMenu,{
        shellrun("ms-settings:");
    });
    SET_ANCTION(act17,显示设置,systemSettingMenu,{
        shellrun("ms-settings:display");
    });
    SET_ANCTION(act18,个性化,systemSettingMenu,{
        shellrun("ms-settings:personalization");
    });
    SET_ANCTION(act21,网络和Internet,systemSettingMenu,{
        shellrun("ms-settings:network-status");
    });
    SET_ANCTION(act19,电源和睡眠,systemSettingMenu,{
        shellrun("ms-settings:powersleep");
    });
    SET_ANCTION(act22,Windows更新,systemSettingMenu,{
        shellrun("ms-settings:windowsupdate");
    });
    SET_ANCTION(act20,系统信息,systemSettingMenu,{
        shellrun("ms-settings:about");
    });
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
    bgPicPath = QApplication::applicationDirPath()+QString("/UserBG%1.png").arg(screenInd);
    this->screenInd = screenInd;
    setFocusPolicy( Qt::StrongFocus );
    setWindowState(Qt::WindowFullScreen);
    setAttribute(Qt::WA_TranslucentBackground);
    // setWindowFlags(Qt::FramelessWindowHint);
    setAcceptDrops(true);
    inplace(this);
    positionToScreen(this,screenInd);
    //shift:Windows主屏幕位于全局的位置;
    if(QFile::exists(bgPicPath)){
        setFixedSize(pscs[screenInd]->size());
    }

    showerSize = size();

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
    // timer->start(50);                                                   // 每隔0.1s


    connect(this,&MainWindow::showerSize_changed,this,[=](QSize val){
        changeShower->update();
    });

    connect(this,&MainWindow::showerRadius_changed,this,[=](int val){
    });

    changeShower = new roundShower(this);
    changeShower->distri(&showerSize,&showerRadius);
    changeShower->aliment = roundShower::Center;
    changeShower->setVisible(false);
    changeShower->move(0,0);

    showerSizeAnimation = new QPropertyAnimation(this,"showerSize");
    showerRadiusAnimation = new QPropertyAnimation(this,"showerRadius");
    showerSizeAnimation->setDuration(500);
    showerSizeAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    showerRadiusAnimation->setDuration(500);
    showerRadiusAnimation->setEasingCurve(QEasingCurve::InOutCubic);

    showerAnimations = new QParallelAnimationGroup(this);
    showerAnimations->addAnimation(showerRadiusAnimation);
    showerAnimations->addAnimation(showerSizeAnimation);


    connect(showerAnimations,&QParallelAnimationGroup::finished,this,[=](){
        if (showeredVisibal){
            inside->setVisible(true);
            changeShower->setVisible(false);
        }
    });


    setShoweredVisibal(true);


    //检测是否有自选背景
    if(QFile::exists(bgPicPath)){

        setBackgoundPic(QImage(bgPicPath));
        setTransparent(false);
    }
    else{
        setTransparent(true);
    }
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
    setTransparent(transparent);
    showerSize = size();
    if(changeShower)
    changeShower->updateDisplay();

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


void MainWindow::setShoweredVisibal(bool val){
    // changeShower->raise();

    if(val){

    }
    else{
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
        changeShower->setVisible(true);
        changeShower->raise();
    }


    showeredVisibal = val;
    updata_animation();
}

void MainWindow::setBackgoundPic(QImage image)
{
    if(image.isNull()){
        SNotice::notice("无法读取文件,设置壁纸失败","抱歉");
        return;
    }
    setTransparent(false);
    image.save(bgPicPath);
    bg = QPixmap::fromImage(image);

    double window_width, window_height;
    double image_width, image_height;
    double r1, r2, r;

    window_width = width();
    window_height = height();

    image_width = bg.width();
    image_height = bg.height();

    r1 = window_width / image_width;
    r2 = window_height / image_height;

    r = qMax(r1, r2);
    QSize displaySize =QSize(image_width * r +1, image_height * r +1);
    bg = bg.scaled(displaySize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QSize actualSize = displaySize;
    int off_x = 0;
    int off_y = 0 ;


    if(displaySize.width()>=window_width){
        actualSize.setWidth(window_width);
        off_x = -(displaySize.width()-window_width)/2;
    }

    if(displaySize.height()>=window_height){
        actualSize.setHeight(window_height);
        off_y = -(displaySize.height()-window_height)/2;
    }

    bg = bg.copy(off_x,off_y,actualSize.width(),actualSize.height());

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

bool MainWindow::addAIcon(QString path,bool notice, QPoint globalPos)
{
    return addAIcon(path2MyFI(path),notice,globalPos);
}

bool MainWindow::addAIcon(QFileInfo qinfo , bool notice, QPoint globalPos)
{
    return addAIcon(MyFileInfo(qinfo),notice,globalPos);
}

bool MainWindow::addAIcon(MyFileInfo info,bool notice, QPoint globalPos)
{

    qDebug()<<"Mainwindow try to add a info"<<info.filePath;
    if(!inside->OKForDefaultPut(new SFile())){
        SNotice::notice("布局无法容纳目标，请调整布局","布局错误");
        return false;
    };

    SFile *tem = nullptr;
    tem = new SFile();

    tem->loadFromMyFI(info,true);

    // qDebug()<<tem->colorAlpha;

    if (tem)
    {
        if(globalPos==QPoint(-1,-1)){
            inside->defaultPut(tem,false);
        }
        else{
            tem->setParent(this);
            tem->move(mapFromGlobal(globalPos));
            inside->clearPut(tem,false);
        }
        tem->raise();
        if(notice)
        SNotice::notice(QStringList()<<tem->filePath,"增添文件",3000);
        return true;
    }

    return false;

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
    checkForKey(event);
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
        StyleSettingWindow* k = new StyleSettingWindow;
        k->show();
        inside->printOccupied();
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
    qDebug()<<QCursor::pos();
    // QProcess process;
    // process.startDetached()
    scanForChange();
    focusInPoint  = QCursor::pos();
    focusin = true;
}

void MainWindow::focusOutEvent(QFocusEvent *event)
{
    qDebug()<<objectName()<<"FoucusOut";
}



void MainWindow::enterEvent(QEvent *event)
{
    activepmw = this;
    qDebug()<<objectName()<<"Enter";

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
    if((!showeredVisibal)||(showeredVisibal&&showerAnimations->state()==QAnimationGroup::Running)){
        QPainter painter(this);



        painter.drawPixmap(rect(), buffer);
        qDebug()<<"paintBuffer";
    }
    else if (!transparent)
    {
        QPainter painter(this);
        painter.drawPixmap(rect(), bg);
        qDebug()<<"paintBG";
    }

    Q_UNUSED(ev);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{

}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *ev)
{
    setShoweredVisibal(!showeredVisibal);
    Q_UNUSED(ev);
}

void MainWindow::mousePressEvent(QMouseEvent* event){
    cleanCelect();

    appendPoints(event->pos());
    // inside->printOccupied();
    pls->clearTooltip();
    pls->clearInputDialog();
    if(firstNotice&&init){
        firstNotice = false;
    }

#ifdef QT_DEBUG
    // SToolTip::Tip("样例文本");
    // // SNotice::notice(QStringList()<<"现在Sapphire将会实时更新桌面文件！"<<"你在Sapphire中对图标的操作均会对应到系统文件中！","重要通知!",15000);
    // SNotice::notice(QStringList()<<"infoTestinfoTestinfoTestinfoTestinfoTest","TitleTest");
#endif
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
        setBackgoundPic(QImage(fileName));
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
        setFixedSize(pscs[screenInd]->size());
    }
    showerSize = size();
    if(changeShower){
        changeShower->updateDisplay();
        changeShower->update();
    }
    update();
    // updateBG();
}

void MainWindow::setBlur(bool val)
{
    enable_background_blur = val;
    bgshower->setEnabled(val);
    bgshower->setVisible(val);
    bgshower->lower();
    // updateBG();
    // qDebug()<<transparent<<val;
}
