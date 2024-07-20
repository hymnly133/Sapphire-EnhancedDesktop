#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ed_bgshower.h"
#include "ed_blocklayout.h"
#include <QMainWindow>
#include <mousehook.h>
#include <QPushButton>  // 添加QPushButton头文件
#include "qfileinfo.h"
#include "qparallelanimationgroup.h"
#include<QLinkedList>
#include"ed_unit.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct MyFileInfo;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QSize showerSize MEMBER showerSize NOTIFY showerSize_changed)
    Q_PROPERTY(int showerRadius MEMBER showerRadius NOTIFY showerRadius_changed)
public:
    MainWindow(QWidget *parent = nullptr,int screenInd=0);
    ~MainWindow();
    Ui::MainWindow *ui;
    ED_BGShower* bgshower = nullptr;
    bool onShift = false;
    QMenu* myMenu;
    ED_BlockLayout* inside;
    QPixmap bg;
    QPixmap buffer;
    bool isfrozen;
    roundShower* changeShower;
    QSize showerSize;
    QList<QPoint> drawParamList;
    bool showeredVisibal = false;
    int showerRadius = 500;
    double globalScale =1;
    QPropertyAnimation* showerSizeAnimation;
    QPropertyAnimation* showerRadiusAnimation;
    QParallelAnimationGroup * showerAnimations;
    int screenInd;
    void InitAUnit(ED_Unit* aim, bool animated=false);
    void paintEvent(QPaintEvent * ev) override;



    QPushButton *selectBackgroundButton;  // 新增：选择背景按钮
    void setTransparent(bool val);
    void setBlur(bool val);
    void ed_update();
    void setFrozen(bool val);
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
    void addAIcon(MyFileInfo info);
    void appendPoints(QPoint p);
    QJsonObject to_json();
    void load_json(QJsonObject rootObject);


    void Init();

private:
    void setupActions();
    void setupUnits();
    void setupLayout(int x,int y);

public slots:;
    void setScale(double Scale);
    void onSelectBackground();  // 新增：选择背景文件槽函数
    void updatePer01second();
    void whenFrozenThreadDone();

public: signals:
    void showerSize_changed(QSize);
    void showerRadius_changed(int);



    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent* ev) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent* ev) override;


    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;

    void closeEvent( QCloseEvent * event ) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    // QWidget interface
protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

#endif // MAINWINDOW_H
