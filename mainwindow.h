#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ed_bgshower.h"
#include "ed_blocklayout.h"
#include <QMainWindow>
#include <mousehook.h>
#include <QPushButton>  // 添加QPushButton头文件
#include "layershower.h"
#include "qdesktopwidget.h"
#include "qfileinfo.h"
#include "qparallelanimationgroup.h"
#include "weather.h"
#include "hitokoto.h"
#include<QLinkedList>
#include"ed_unit.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct FileInfo;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QSize showerSize MEMBER showerSize NOTIFY showerSize_changed)
    Q_PROPERTY(int showerRadius MEMBER showerRadius NOTIFY showerRadius_changed)
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;
    ED_BGShower* bgshower = nullptr;

    ED_BlockLayout* inside;
    QPixmap bg;
    QPixmap buffer;
    roundShower* changeShower;
    QSize showerSize;
    QList<QPoint> drawParamList;
    bool showeredVisibal = false;
    int showerRadius = 500;

    QPropertyAnimation* showerSizeAnimation;
    QPropertyAnimation* showerRadiusAnimation;
    QParallelAnimationGroup * showerAnimations;

    void InitAUnit(ED_Unit* aim, bool animated=false);
    void paintEvent(QPaintEvent * ev) override;
    void mouseDoubleClickEvent(QMouseEvent* ev) override;
    void mousePressEvent(QMouseEvent* ev) override;


    QPushButton *selectBackgroundButton;  // 新增：选择背景按钮
    void setTransparent(bool val);
    void setBlur(bool val);
    void ed_update();
    void InitDesktop();
    void capture();
    void updateBG();
    void setShoweredVisibal(bool val);

    QSize aim_showerSize(){
        if(showeredVisibal) return size();
        else return QSize(0,0);
    }

    int aim_showerRadius(){
        if(showeredVisibal) return 0;
        else return 500;
    }

    void updata_animation();

    void addAIcon(QString path);
    void addAIcon(QFileInfo info);
    void addAIcon(QList<FileInfo> infos);
    void appendPoints(QPoint p);

private:
    void setupActions();
    void setupUnits();

public slots:;
    void setScale(double Scale);
    void onSelectBackground();  // 新增：选择背景文件槽函数
    void updatePer01second();

public: signals:
    void showerSize_changed(QSize);
    void showerRadius_changed(int);



    // QWidget interface
protected:
    void dropEvent(QDropEvent *event) override;



    // QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;

    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};
extern MainWindow* pmw;
extern ED_Unit* pMovingUnit;
extern QDesktopWidget* pdt;
extern LayerShower* pls;
extern bool debug;
#endif // MAINWINDOW_H
