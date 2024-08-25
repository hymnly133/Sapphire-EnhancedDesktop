#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "layerbackground.h"
#include "layermousegetter.h"
#include "layertoppainter.h"
#include "sbgshower.h"

#include "sblocklayout.h"
#include <QMainWindow>
#include <QPushButton>  // 添加QPushButton头文件
#include "qfileinfo.h"
#include "qparallelanimationgroup.h"
#include<QLinkedList>
#include "slayoutcontainer.h"
#include "smenu.h"
#include "smultifunc.h"
#include "sunit.h"
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE
class SFile;
class MainWindow : public QMainWindow, public SLayoutContainer
{
    Q_OBJECT
    Q_PROPERTY(QSize showerSize MEMBER showerSize NOTIFY showerSize_changed)
    Q_PROPERTY(int showerRadius MEMBER showerRadius NOTIFY showerRadius_changed)
public:
    MainWindow(MainWindow *parent = nullptr, int screenInd = 0);
    ~MainWindow();

    QString bgPicPath;

    //用于消除从外部程序回到Sapphire时的判断飘逸
    QPoint focusInPoint;
    bool focusin = false;
    LayerBackground* plsBG = nullptr;
    //顶层，用于绘制顶层特效
    LayerShower* pls;
    //中间层，用于放置图标
    // LayerMouseGetter* plsM;
    //底层，用于绘制底层特效
    LayerShower* plsB;

    void raiseLayers();

    Ui::MainWindow *ui;
    //用于显示背景模糊（未启用
    SBGShower* bgshower = nullptr;
    //标识shift（未启用
    bool onShift = false;

    //右键菜单
    //桌面模式的菜单
    SMenu* desktopMenu = nullptr;
    //编辑模式的菜单
    SMenu* editMenu = nullptr;
    //多组件框选的菜单
    SMenu* multiMenu = nullptr;
    //新建文件的菜单
    SMenu* creatNewFileMenu = nullptr;

    //标识背景透视
    bool transparent = true;
    //显示自选背景
    QPixmap bg;
    //储存缓存
    QPixmap buffer;
    //用于绘制框选
    QList<QPoint> celectPointList;
    void appendPoints(QPoint p);

    //用于双击隐藏
    roundShower* changeShower = nullptr;
    QSize showerSize;

    bool showeredVisibal = false;
    int showerRadius = 500;
    double globalScale = 1;
    QPropertyAnimation* showerSizeAnimation;
    QPropertyAnimation* showerRadiusAnimation;
    QParallelAnimationGroup * showerAnimations;
    QSize aim_showerSize();

    int aim_showerRadius();

    //屏幕序号
    int screenInd;


    QPushButton *selectBackgroundButton;  // 选择背景按钮

    //设置背景透视
    void setTransparent(bool val);
    //设置模糊（未启用
    void setBlur(bool val);
    //调用SUnit::endUpdate
    void endUpdate();
    //捕获当前屏幕
    void capture();
    // //更新模糊显示器，未启用
    // void updateBG();
    //设置双击隐藏
    void setShoweredVisibal(bool val);

    void setBackgoundPic(QImage image);

    void updataShowerAnimation();

    // bool addAFile(QString path, bool notice = false, QPoint globalPos = QPoint(-1, -1));


    QJsonObject to_json();
    void load_json(QJsonObject rootObject);

    //初始化并加载data
    QStringList Init(QStringList data);
    //初始化
    void Init(bool final = false);

    //刷新
    void refresh();




    QSize blockSize();


    //刚创建到启动加载动画的阶段(以迅速创建出加载动画为目标)
    void preSetup();

    //主要加载函数(在加载动画中进行的初始化和内容加载)
    void setup();
private:

    //设置编辑模式菜单
    void setupEditMenu();
    //设置编辑模式菜单
    void setupDesktopMenu();
    //设置多文件选中菜单
    void setupMultiMenu();
    //设置一些层
    void setupShower();
    //设置布局
    void setupLayout(int x, int y);
    //设置拖拽事件
    void setupDrop();

public slots:
    //设置全局scale，（待重构
    void setScale(double Scale);
    void onSelectBackground();  // 新增：选择背景文件槽函数
    //即时更新
    void updatePer01second();

    //加载完成后的处理
    void finishBootAnimation();

    //设置窗口和布局大小
    void updateSize();
public:
signals:
    void showerSize_changed(QSize);
    void showerRadius_changed(int);



    // QWidget interface
protected:
    //qt原生事件函数

    void mouseDoubleClickEvent(QMouseEvent* ev) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent* ev) override;


    // void dropEvent(QDropEvent *event) override;
    // void dragEnterEvent(QDragEnterEvent *event) override;

    void closeEvent( QCloseEvent * event ) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void showEvent(QShowEvent *event) override;

    void paintEvent(QPaintEvent * ev) override;


    void resizeEvent(QResizeEvent *event) override;

    void updateAfterPut(SUnit* aim) override;

    void whenDropAFile(QString& fileName);


    // QWidget interface
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
};

inline QSize MainWindow::aim_showerSize()
{
    if(showeredVisibal) {
        return size();
    } else {
        return QSize(0, 0);
    }
}

inline int MainWindow::aim_showerRadius()
{
    if(showeredVisibal) {
        return 0;
    } else {
        return 500;
    }
}

#endif // MAINWINDOW_H
