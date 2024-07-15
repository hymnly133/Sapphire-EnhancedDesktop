#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ed_bgshower.h"
#include "ed_layout.h"
#include <QMainWindow>
#include <mousehook.h>
#include <QPushButton>  // 添加QPushButton头文件
#include "qdesktopwidget.h"
#include "weather.h"
#include "hitokoto.h"
#include"ed_unit.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;
    ED_BGShower* bgshower = nullptr;
    bool moving = false;

    ED_Layout* inside;
    QPixmap bg;
    Weather *weatherwidget;
    Hitokoto *htkt;

    void InitAUnit(ED_Unit* aim);
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

private:
    void setupActions();
    void setupUnits();

public slots:;
    void setScale(double Scale);
    void onSelectBackground();  // 新增：选择背景文件槽函数
    void updatePer01second();


};
extern MainWindow* pmw;
extern ED_Unit* pMovingUnit;
extern QDesktopWidget* pdt;
#endif // MAINWINDOW_H
