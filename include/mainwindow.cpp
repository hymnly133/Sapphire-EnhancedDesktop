#include "mainwindow.h"
#include "global.h"
#include "qfuture.h"
#include "qmessagebox.h"
#include "qtconcurrentrun.h"
#include "sbgshower.h"
#include "sdir.h"
#include "sglshower.h"
#include "snotice.h"
#include "sshellfuncunit.h"
#include "sblockcontainer.h"
#include "sfile.h"
#include "sdock.h"
#include"QInputDialog"
#include "seditbox.h"
#include "filefunc.h"
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
#include"QClipboard"
#include"QMimeData"
#include "unitfunc.h"
#include "userfunc.h"
#include <Shlobj.h>
#include <shlwapi.h>
#include <windows.h>
#include<winuser.h>
#include"stylehelper.h"


void MainWindow::setupDesktopMenu()
{
    desktopMenu = new SMenu(this);
    desktopMenu->path = UserDesktopPath;
    desktopMenu->ismain = true;
    // 给当前窗口添加QAction对象
    // SET_ANCTION(act01,刷新,desktopMenu,this,
    //             {refresh();})
    // SAction* act01 = new SAction(tr("刷新"));
    // desktopMenu->addAction(act01);
    // connect(act01, &SAction::triggered, this, [ = ]() {
    //     refresh();
    // });
    SET_ANCTION(act01, tr("刷新"), desktopMenu, this,
    {         refresh(); });

    SET_ANCTION(act1, tr("改变可见"), desktopMenu, this,
    { setShoweredVisibal(!showeredVisibal); })
    act1->setText(tr("改变可见"));
    SET_ANCTION(act3, tr("映射文件"), desktopMenu, this, {
        QFileDialog* fd = new QFileDialog();
        QStringList filePaths = QFileDialog::getOpenFileNames(this, tr("选择文件"), "D:/", nullptr, nullptr, QFileDialog::Options(QFileDialog::DontResolveSymlinks));;
        foreach (const QString& filePath, filePaths)
        {
            addAFile(filePath);
        }
    })
    // SAction *creatNewFileAction = new SAction(desktopMenu);
    // creatNewFileAction->setText(tr("新建"));
    // desktopMenu->addAction(creatNewFileAction);
    // creatNewFileMenu = new SMenu();
    // creatNewFileAction->setMenu(creatNewFileMenu);

    SET_ANCTION_MENU(creatNewFileAction, tr("新建"), desktopMenu, creatNewFileMenu_);
    creatNewFileMenu = creatNewFileMenu_;

    SET_ANCTION(actNewDir, tr("文件夹"), creatNewFileMenu, this, {
        FileHelper::creatNewDir();
    });
    SET_ANCTION(act13, tr("文本文档"), creatNewFileMenu, this, {
        FileHelper::creatNewFile(FileType::txt);
    });
    SET_ANCTION(act14, tr("PPT演示文稿"), creatNewFileMenu, this, {
        FileHelper::creatNewFile(FileType::pptx);
    });
    SET_ANCTION(act15, tr("Word文档"), creatNewFileMenu, this, {
        FileHelper::creatNewFile(FileType::docx);
    });
    SET_ANCTION(act16, tr("空文件"), creatNewFileMenu, this, {
        FileHelper::creatNewFile(FileType::empty);
    });

    SET_ANCTION_MENU(systemSettingMenuAction, tr("系统设置"), desktopMenu, systemSettingMenu);


    SET_ANCTION(act00, tr("主面板"), systemSettingMenu, this, {
        shellrun("ms-settings:");
    });
    SET_ANCTION(act17, tr("显示设置"), systemSettingMenu, this, {
        shellrun("ms-settings:display");
    });
    SET_ANCTION(act18, tr("个性化"), systemSettingMenu, this, {
        shellrun("ms-settings:personalization");
    });
    SET_ANCTION(act21, tr("网络和Internet"), systemSettingMenu, this, {
        shellrun("ms-settings:network-status");
    });
    SET_ANCTION(act19, tr("电源和睡眠"), systemSettingMenu, this, {
        shellrun("ms-settings:powersleep");
    });
    SET_ANCTION(act22, tr("Windows更新"), systemSettingMenu, this, {
        shellrun("ms-settings:windowsupdate");
    });
    SET_ANCTION(act20, tr("系统信息"), systemSettingMenu, this, {
        shellrun("ms-settings:about");
    });
    // desktopMenu->addDirCommands();
    desktopMenu->addDirBGCommands();
    // desktopMenu->addSeparator();
    desktopMenu->addDesktopCommands();


    SET_ANCTION(actToEditMode, tr("编辑模式"), desktopMenu, this, {
        toEditMode();
    })
    SET_ANCTION(act4, tr("退出程序"), desktopMenu, this, {
        SExit();
    })
}


void MainWindow::setupEditMenu()
{
    editMenu = new SMenu(this);
    editMenu->ismain = true;
    SET_ANCTION(act2, tr("切换精简"), editMenu, this, {
        for(SUnit * content : inside->contents)
        {
            content->changeSimpleMode();
        }
    })
    // SAction *creatNewUnitAction = new SAction(editMenu);
    // creatNewUnitAction->setText("新建组件");
    // editMenu->addAction(creatNewUnitAction);
    // SMenu *creatNewUnitMenu = new SMenu();
    // creatNewUnitAction->setMenu(creatNewUnitMenu);

    SET_ANCTION_MENU(creatNewUnitAction, tr("新建组件"), editMenu, creatNewUnitMenu);


#ifdef QT_DEBUG
    SET_ANCTION(act5, tr("获取背景"), editMenu, this, {
        capture();
    })
    SET_ANCTION(actupdate, tr("EndUpdate"), editMenu, this, {
        endUpdate();
    })
#endif
    SET_ANCTION(act6, tr("小型格子"), creatNewUnitMenu, this, {
        auto bc = new SBlockContainer(inside, 2, 2, 2, 2);
    })
    SET_ANCTION(act7, tr("中型格子"), creatNewUnitMenu, this, {
        auto bc = new SBlockContainer(inside, 3, 3, 3, 3);
        // InitAUnit(bc);
    })
    SET_ANCTION(act8, tr("大型格子"), creatNewUnitMenu, this, {
        auto bc = new SBlockContainer(inside, 4, 4, 4, 4);
        // InitAUnit(bc);
    })
    SET_ANCTION(act9, tr("Dock栏"), creatNewUnitMenu, this, {
        auto dock = new SDock(inside);
        // InitAUnit(dock);
    })
    SET_ANCTION(act10, tr("设置箱"), creatNewUnitMenu, this, {
        auto dock = new SEditBox(inside);
        // InitAUnit(dock);
    })
#ifdef QT_DEBUG
    SET_ANCTION(act11, tr("重绘盒"), creatNewUnitMenu, this, {
        auto dock = new RepaintCounterUnit(inside);
        // InitAUnit(dock);
    })
    SET_ANCTION(act151, tr("lowerAndUpdate"), editMenu, this, {
        qDebug() << changeShower->geometry();
        changeShower->lower();
        changeShower->setVisible(!changeShower->isVisible());
        changeShower->updateDisplay();
        changeShower->update();
        // InitAUnit(dock);
    })
#endif
    SET_ANCTION(act12, tr("系统盒子"), creatNewUnitMenu, this, {
        auto dock = new SShellFuncUnit(inside);
        // InitAUnit(dock);
    })
    SET_ANCTION(act14, tr("调整布局"), editMenu, this, {
        resizeForWithDialog((SBlockLayout*)(inside));
    })
    SET_ANCTION(act13, tr("桌面模式"), editMenu, this, {
        toDesktopMode();
    })
#ifdef QT_DEBUG
    SET_ANCTION(actgl, tr("gl"), editMenu, this, {
        auto gl = new SGLShower(inside);
    })
    SET_ANCTION(actdir, tr("文件夹"), editMenu, this, {
        auto gl = new SDir(inside, 3, 3, "D:/Github/dirTest");
    })
#endif
    SET_ANCTION(act4, tr("退出程序"), editMenu, this, {
        SExit();
    })
}


void MainWindow::setupMultiMenu()
{
    multiMenu = new SMenu(this);
    multiMenu->ismain = true;
    // 给当前窗口添加QAction对象
    SET_ANCTION(act01, tr("切换精简"), multiMenu, this,
    {switchSimpleModeG();})
    SET_ANCTION(act02, tr("切换始终显示"), multiMenu, this, {
        swtichAlwayShowG();

    });
    SET_ANCTION(act03, tr("删除"), multiMenu, this, {
        removeG();
    });
}


void MainWindow::setupShower()
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
    connect(this, &MainWindow::showerSize_changed, this, [ = ](QSize val) {
        changeShower->update();
    });
    connect(this, &MainWindow::showerRadius_changed, this, [ = ](int val) {
    });
    changeShower = new roundShower(this);
    changeShower->distri(&showerSize, &showerRadius);
    changeShower->aliment = roundShower::Center;
    changeShower->setVisible(false);
    changeShower->move(0, 0);
    showerSizeAnimation = new QPropertyAnimation(this, "showerSize");
    showerRadiusAnimation = new QPropertyAnimation(this, "showerRadius");
    showerSizeAnimation->setDuration(500);
    showerSizeAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    showerRadiusAnimation->setDuration(500);
    showerRadiusAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    showerAnimations = new QParallelAnimationGroup(this);
    showerAnimations->addAnimation(showerRadiusAnimation);
    showerAnimations->addAnimation(showerSizeAnimation);
    connect(showerAnimations, &QParallelAnimationGroup::finished, this, [ = ]() {
        if (showeredVisibal) {
            inside->setVisible(true);
            changeShower->setVisible(false);
        }
        qDebug() << changeShower->pos() << changeShower->aim_size();
    });
    setShoweredVisibal(true);
}

void MainWindow::setupLayout(int x, int y)
{
    inside = new SBlockLayout(this, x, y);
    inside->isMain = true;
    updateSize();
}

MainWindow::MainWindow(MainWindow *parent, int screenInd)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(appIcon);
    this->screenInd = screenInd;
    qDebug() << "MainWindow Thread" << QThread::currentThread();
    setObjectName("MainWindow" + QString::number(screenInd));


    setFocusPolicy( Qt::StrongFocus );
    setWindowState(Qt::WindowFullScreen);
    setAttribute(Qt::WA_TranslucentBackground);
    // setAcceptDrops(true);
    // qDebug() << ChangeWindowMessageFilterEx((HWND)winId(), WM_DROPFILES, MSGFLT_ALLOW, NULL);
    // qDebug() << ChangeWindowMessageFilterEx((HWND)winId(), 0x0049, MSGFLT_ALLOW, NULL);
    // qDebug() << ChangeWindowMessageFilterEx((HWND)winId(), WM_COPYDATA, MSGFLT_ALLOW, NULL);
    inplace(this);
    positionToScreen(this, screenInd);

    qDebug() << "MainWindow" << screenInd << "Information Fixed:" << rect() << pos() << geometry() << mapToGlobal(QPoint(0, 0)) << mapFromGlobal(QPoint(0, 0));

    preSetup();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::raiseLayers()
{
    // plsB->raise();
    raise();
    pls->raise();
}

void MainWindow::setScale(double scale)
{
    scale = qBound(0.001, scale, 1.0);
    SLayoutContainer::setScale(scale);
    globalScale = scale;
    SEditBox *ed = findChild<SEditBox *>();
    if(ed != nullptr) {
        if(ed->scale_Slider != nullptr) {
            ed->scale_Slider->blockSignals(true);
            ed->scale_Slider->setValue(scale * 100);
            ed->scale_Slider->blockSignals(false);
        }
    }
}

QJsonObject MainWindow::to_json()
{
    QJsonObject rootObject;
    rootObject.insert("ind", screenInd);
    rootObject.insert("content", inside->to_json());
    return rootObject;
}

void MainWindow::load_json(QJsonObject rootObject)
{
    screenInd = rootObject.value("ind").toInt();
    qDebug() << "loading Mainwindow" << screenInd;
    setupLayout(10, 10);
    SLayoutContainer::load_json(rootObject.value("content").toObject());
    endUpdate();
}

QStringList MainWindow::Init(QStringList data)
{
    qDebug() << "initing Mainwindow" << screenInd;
    Init();
    while(!data.empty()) {
        qDebug() << "remmains" << data.size();
        if(!inside->OKForDefaultPut(new SUnit())) {
            break;
        } else {
            qDebug() << "ok";
            addAFile(data[0]);
            data.removeAt(0);
        }
        // inside->printOccupied();
    }
    qDebug() << "Init" << objectName() << "Done,Remainin" << data.size() << "file data";
    return data;
}

void MainWindow::endUpdate()
{
    setTransparent(transparent);
    showerSize = size();
    if(changeShower) {
        changeShower->updateDisplay();
    }
    if(inside != nullptr) {
        SLayoutContainer::endUpdate();
    }
}



void MainWindow::Init(bool final)
{
    qDebug() << "Init Mainwindow" << screenInd;
    QString name;
    if(screenInd == 0) {
        name += "主屏幕";
    } else {
        name += "副屏幕";
    }
    name += "初始化";
    int sizeX = QInputDialog::getInt(nullptr, name, "请输入布局列数(根据屏幕宽度)", 20, 1, 1000, 2);
    int sizeY = QInputDialog::getInt(nullptr, name, "请输入布局行数(根据屏幕高度)", 12, 1, 1000, 2);
    if(!sizeX) {
        sizeX = 10;
    }
    if(!sizeY) {
        sizeY = 10;
    }
    setupLayout(sizeX, sizeY);
    if(screenInd == 0) {
        auto su = new SShellFuncUnit(inside);
        auto eb = new SEditBox(inside);
    }
    //如果只是单纯初始化（如第二屏，没有加载的数据，直接发送信号）
    if(final) {

    }
}

void MainWindow::refresh()
{
    if(enable_refresh_animation) {
        inside->setVisible(false, true);
    }
    endUpdate();
    // if(!showeredVisibal) {
    //     changeShower->setFixedSize(1, 1);
    // }
    // changeShower->raise();
    repaint();
    scanForChange();
    loadInsideAll();
    if(enable_refresh_animation) {
        inside->setVisible(true);
    }
}

void MainWindow::preSetup()
{
    pls = new LayerShower(this, screenInd);
    pls->layer = LayerShower::Upper;
    if(MultiScreen) {
        show();
    }
    pls->startBootAnimationIn();
    updateSize();
    //作为主窗口，调用UserFunc中的setupG进行软件的内容加载
    if(screenInd == 0) {
        connect(pls, &LayerShower::bootAnimationInEnd, this, &setupG);
    }
}

void MainWindow::setup()
{
    bgPicPath = QApplication::applicationDirPath() + QString("/UserBG%1.png").arg(screenInd);
    showerSize = size();


    setupDrop();
    setupShower();
    setupLayout(10, 10);
    setupDesktopMenu();
    setupEditMenu();
    setupMultiMenu();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePer01second())); // slotCountMessage是我们需要执行的响应函数
    // timer->start(50);                                                   // 每隔0.1s
    //检测是否有自选背景
    if(QFile::exists(bgPicPath)) {
        setBackgoundPic(QImage(bgPicPath));
    }
    setTransparent(enable_background_transparent);

    connectTo(always_fill_screen, bool, bool, {
        updateSize();
    });
}

QSize MainWindow::blockSize()
{
    return QSize(((SBlockLayout*)inside)->W_Block_Clean(), ((SBlockLayout*)inside)->H_Block_Clean());
}



void MainWindow::capture()
{
    update();
    buffer = QPixmap();
    buffer = grab(rect());
}


void MainWindow::setShoweredVisibal(bool val)
{
    // changeShower->raise();
    if(val) {
        if(desktopMenu) {
            desktopMenu->showAction(tr("刷新"));
        }
    } else {
        if(desktopMenu) {
            desktopMenu->hideAction(tr("刷新"));
        }

        foreach (SUnit* content, inside->contents) {
            if(content->alwaysShow) {
                content->setVisible(false);
            }
        }
        if(!(showerAnimations->state() == QParallelAnimationGroup::Running)) {
            capture();
        }
        foreach (SUnit* content, inside->contents) {
            content->setVisible(true);
        }
        inside->setVisible(false);
        changeShower->setVisible(true);
        changeShower->raise();
        foreach (SUnit* content, inside->contents) {
            if(content->alwaysShow) {
                content->raise();
            }
        }
    }
    showeredVisibal = val;
    updataShowerAnimation();
}

void MainWindow::setBackgoundPic(QImage image)
{
    if(image.isNull()) {
        SNotice::notice("无法读取文件,设置壁纸失败", "抱歉");
        return;
    }
    if(!onLoading) {
        setTransparent(false);
        image.save(bgPicPath);
    }
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
    QSize displaySize = QSize(image_width * r + 1, image_height * r + 1);
    bg = bg.scaled(displaySize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QSize actualSize = displaySize;
    int off_x = 0;
    int off_y = 0 ;
    if(displaySize.width() >= window_width) {
        actualSize.setWidth(window_width);
        off_x = (displaySize.width() - window_width) / 2;
    }
    if(displaySize.height() >= window_height) {
        actualSize.setHeight(window_height);
        off_y = (displaySize.height() - window_height) / 2;
    }
    bg = bg.copy(off_x, off_y, actualSize.width(), actualSize.height());
}

void MainWindow::updataShowerAnimation()
{
    showerAnimations->stop();
    showerSizeAnimation->setStartValue(showerSize);
    showerSizeAnimation->setEndValue(aim_showerSize());
    showerRadiusAnimation->setStartValue(showerRadius);
    showerRadiusAnimation->setEndValue(aim_showerRadius());
    // qDebug()<<aim_showerSize();
    showerAnimations->start();
}



// bool MainWindow::addAIcon(QFileInfo qinfo, bool notice, QPoint globalPos)
// {
//     return addAIcon(MyFileInfo(qinfo), notice, globalPos);
// }

// bool MainWindow::addAIcon(MyFileInfo info, bool notice, QPoint globalPos)
// {
//     qDebug() << "Mainwindow try to add a info" << info.filePath;
//     if(!inside->OKForDefaultPut(new SFile())) {
//         SNotice::notice("布局无法容纳目标，请调整布局", "布局错误");
//         return false;
//     };
//     SFile *tem = nullptr;
//     tem = new SFile();
//     tem->loadFromMyFI(info, true);
//     // qDebug()<<tem->colorAlpha;
//     if (tem) {
//         if(globalPos == QPoint(-1, -1)) {
//             inside->defaultPut(tem, false);
//         } else {
//             tem->setParent(this);
//             tem->move(mapFromGlobal(globalPos));
//             inside->clearPut(tem, false);
//         }
//         tem->raise();
//         if(notice) {
//             SNotice::notice(QStringList() << tem->filePath, "增添文件", 3000);
//         }
//         return true;
//     }
//     return false;
// }


void MainWindow::appendPoints(QPoint p)
{
    if ( celectPointList.count() == 2 ) {
        celectPointList.removeLast(); // 移除最后一个点
    }
    celectPointList.append(p);
}

void MainWindow::updatePer01second()
{
    if (enable_intime_repaint) {
        repaint();
    }
    // qDebug()<<rect()<<pos()<<geometry()<<mapToGlobal(QPoint(0,0));
}

void MainWindow::finishBootAnimation()
{
    qDebug() << objectName() << "Finish Animation";
}



// void MainWindow::dropEvent(QDropEvent *e)
// {
//     if(e->mimeData()->hasUrls()) { //处理期望数据类型
//         // if()
//         foreach(QString format, e->mimeData()->formats()) {
//             // Retrieving data
//             QByteArray data = e->mimeData()->data(format);
//             qDebug() << format << data;
//         }

//         // return;
//         qDebug() << e->dropAction();
//         QList<QUrl> list = e->mimeData()->urls();//获取数据并保存到链表中
//         for(int i = 0; i < list.count(); i++) {
//             QString path = list[i].toLocalFile();
//             whenDropAFile(path);
//         }
//     } else {
//         e->ignore();
//     }
// }

// void MainWindow::dragEnterEvent(QDragEnterEvent *e)
// {
//     if(e->mimeData()->hasUrls()) { //判断数据类型
//         e->acceptProposedAction();//接收该数据类型拖拽事件
//     } else {
//         e->ignore();//忽略
//     }
// }



void MainWindow::closeEvent(QCloseEvent *event)//关闭窗口会先处理该事件函数
{
    pls->close();
    psh->writeStyleIni();
#ifdef QT_DEBUG
    // QMessageBox::about(NULL, "cs", "closeEvent");
#endif
    event->accept();
    writeContent();
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
    if(event->modifiers() == Qt::ShiftModifier) {
        openSettingWindow();
        // inside->printOccupied();
    } else {
        if(editMode) {
            editMenu->exec(event->globalPos());
        } else {
            desktopMenu->exec(event->globalPos());
        }
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() == Qt::ShiftModifier) {
        qDebug() << event->angleDelta();
        setScale((globalScale + (event->angleDelta().y() / 1000.0)));
    }
}



void MainWindow::focusInEvent(QFocusEvent *event)
{
    pls->raise();
    qDebug() << objectName() << "FoucusIn";
    scanForChange();
    focusin = true;
}

void MainWindow::focusOutEvent(QFocusEvent *event)
{
    qDebug() << objectName() << "FoucusOut";
}



void MainWindow::enterEvent(QEvent *event)
{
    activepmw = this;
    qDebug() << objectName() << "Enter";
}

void MainWindow::leaveEvent(QEvent *event)
{
    qDebug() << objectName() << "Leave" << cursor().pos() << rect() << geometry();
}

void MainWindow::showEvent(QShowEvent *event)
{
    raiseLayers();
}

void MainWindow::paintEvent(QPaintEvent *ev)
{
    if((!showeredVisibal) || (showeredVisibal && showerAnimations->state() == QAnimationGroup::Running)) {
        QPainter painter(this);
        painter.drawPixmap(rect(), buffer);
        qDebug() << "paintBuffer";
    } else if (!transparent) {
        QPainter painter(this);
        painter.drawPixmap(rect(), bg);
        // qDebug()<<"paintBG";
    }
    Q_UNUSED(ev);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
}

void MainWindow::updateAfterPut(SUnit *aim)
{

    if(aim->inherits("SFile")) {
        SFile* sfile = (SFile*)aim;
        //为了避免bug，不移动文件夹
        if(sfile->isDir) {
            return;
        }

        //屏蔽原本就在桌面的文件
        if(inDesktop(sfile->filePath)) {
            return;
        }
        qDebug() << UserDesktopPath + "/" + sfile->fullName();
        sfile->moveToDir(UserDesktopPath);
    }
}

void MainWindow::whenDropAFile(QString &fileName)
{
    QString path = fileName.replace("\\", "/");
    QString newName =  (UserDesktopPath) + "/" + okName(QFileInfo(path).fileName());
    bool removed = QFile::copy(path, newName);


    qDebug() << "move" << newName;
    if(removed) {
        qDebug() << "moved";
        addAFile(newName, true, QCursor::pos());
    } else {
        SNotice::notice("添加失败", "糟糕");
    }
}

void MainWindow::setupDrop()
{
    // void* user32 = LoadLibraryA("user32");
    // FARPROC func = GetProcAddress((HMODULE)user32, "ChangeWindowMessageFilter");
    // qDebug() << (*func)();
    // user32 = LoadLibraryA("user32");
    // func = GetProcAddress((HMODULE)user32, "ChangeWindowMessageFilter");
    // qDebug() << (*func)();
    setAcceptDrops(true);
    ChangeWindowMessageFilter(WM_DROPFILES, 1);
    //    ChangeWindowMessageFilter(WM_COPYDATA, 1);
    //    ChangeWindowMessageFilter(0x0049, 1);
    qDebug() << winId() << effectiveWinId();
    qDebug() << ChangeWindowMessageFilterEx((HWND)effectiveWinId(), WM_DROPFILES, 1, NULL);
    qDebug() << ChangeWindowMessageFilterEx((HWND)effectiveWinId(), WM_COPYDATA, 1, NULL);
    qDebug() << ChangeWindowMessageFilterEx((HWND)effectiveWinId(), 0x0049, 1, NULL);
    DragAcceptFiles((HWND)effectiveWinId(), true);
    qDebug() << GetLastError();
    HRESULT res = RevokeDragDrop((HWND)winId());
    // qDebug() << "res:" << res;
}

void MainWindow::updateSize()
{
    QSize aim = pscs[screenInd]->availableSize();


    if(!MultiScreen) {
        aim = pscs[screenInd]->size();
    }
    setFixedSize(aim);

    if(inside) {
        QSize aim_inside = pscs[screenInd]->availableSize();

        if(always_fill_screen) {
            aim_inside = pscs[screenInd]->size();
        }
        inside->setStandalongRect(QRect(QPoint(0, 0), aim_inside));
    }
    endUpdate();
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *msg = (MSG *)message;
    // qDebug() << eventType;
    if(msg->message == WM_QUERYENDSESSION ) { //Win下的关机消息
        //TODO...
        qDebug() << "User Shutdown";
        SExit();
        return true;
    } else if (eventType == "windows_generic_MSG") {
        PMSG msg = (PMSG) message;
        if(msg->message == 563) {
            // qDebug() << msg->message << msg->hwnd << msg->wParam << msg->lParam << msg->time << msg->pt.x << msg->pt.y;
            UINT file_num = DragQueryFile((HDROP) msg->wParam, 0xFFFFFFFF, NULL, 0);
            qDebug() << "文件数量:" << file_num;
            for(int i = 0; i < (int)file_num; i++) {
                UINT file_name_size = DragQueryFile((HDROP) msg->wParam, i, NULL, 0);
                qDebug() << file_name_size;
                LPWSTR  fn = (LPWSTR)malloc(sizeof(WCHAR) * file_name_size + 1);
                //! https://learn.microsoft.com/zh-cn/windows/win32/api/shellapi/nf-shellapi-dragqueryfilew
                UINT code = DragQueryFileW((HDROP) msg->wParam, i, fn, file_name_size + 1);
                QString filename = QString::fromStdWString(fn);
                free(fn);
                whenDropAFile(filename);
                qDebug() << "get name error:" << code;
            }
            qDebug() << eventType << message << *result;
        }
    }
    return QMainWindow::nativeEvent(eventType, message, result);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        setShoweredVisibal(!showeredVisibal);
        event->accept();
        return;
    }
    event->ignore();
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    loadInsideAll();
    cleanCelect();
    foldG();
    if(event->button() == Qt::LeftButton) {
        appendPoints(event->pos());
    } else if(event->button() == Qt::MiddleButton) {
        switchMode();
    }
    // inside->printOccupied();
    pls->clearTooltip();
    pls->clearInputDialog();
    //首次通知
    if(firstNotice && init) {
        firstNotice = false;
    }
#ifdef QT_DEBUG
    // SToolTip::Tip("样例文本");
    // // SNotice::notice(QStringList()<<"现在Sapphire将会实时更新桌面文件！"<<"你在Sapphire中对图标的操作均会对应到系统文件中！","重要通知!",15000);
    // SNotice::notice(QStringList()<<"infoTestinfoTestinfoTestinfoTestinfoTest","TitleTest");
    // qDebug() << inside->W_Block_Clean() << inside->boradXPix() << inside->spaceXPix();
#endif
    pls->raise();
}



void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        appendPoints(event->pos());
        updateCelect();
        celectPointList.clear();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton) {
        appendPoints(event->pos());
        updateCelect();
    }
}



void MainWindow::onSelectBackground()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择背景文件", "", "Images (*.png *.jpg *.bmp);;Videos (*.mp4 *.avi *.mkv)");
    qDebug() << "Selected file:" << fileName; // 调试输出文件路径
    if (!fileName.isEmpty()) {
        setBackgoundPic(QImage(fileName));
    }
}

void MainWindow::setTransparent(bool val)
{
    if(transparent == val) {
        return;
    }
    transparent = val;
    if (enable_background_blur) {
        bgshower->setVisible(!val);
        bgshower->captrued = bg;
    }
    if(val) {
        setFixedSize(pscs[screenInd]->availableSize());
    } else {
        if(screenNum == 1) {
            setFixedSize(pscs[screenInd]->size());
        } else {
            setFixedSize(pscs[screenInd]->availableSize());
        }
        // setFixedSize(pscs[screenInd]->availableSize());
    }
    showerSize = size();
    if(changeShower) {
        changeShower->updateDisplay();
        changeShower->update();
    }
    if(screenInd == 0) {
        qDebug() << "Settd";
        enable_background_transparent = val;
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
