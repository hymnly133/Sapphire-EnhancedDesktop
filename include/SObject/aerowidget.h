#ifndef AEROWIDGET_H
#define AEROWIDGET_H

#include <QWidget>
#include <QWidget>
#include <QtWin>
#include <qdialog.h>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QEvent>
#include <QMouseEvent>
#include <qscreen.h>
#include <QHBoxLayout>//水平
#include <QVBoxLayout>//垂直
#include <qspinbox.h>

class AeroWidget : public QWidget
{
    Q_OBJECT
public:
    QWidget* _parent;
    explicit AeroWidget(QWidget *parent = nullptr);
public:
    void setParent(QWidget* p);//设置父类
    void setAlpha(int v);//设置透明度
    void paintEvent(QPaintEvent *ev);//绘图时间,在此函数中搞毛玻璃背景
    QColor bgColor;
private slots:
    void valueChanged_Slot(int v);//更新透明度
};

//重要
enum AccentState {
    ACCENT_DISABLED = 0,
    ACCENT_ENABLE_GRADIENT = 1,
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
    ACCENT_ENABLE_BLURBEHIND = 3,
    ACCENT_INVALID_STATE = 4
};
struct AccentPolicy {
    AccentState AccentState;
    int AccentFlags;
    int GradientColor;
    int AnimationId;
};
enum WindowCompositionAttribute {
    WCA_UNDEFINED = 0,
    WCA_NCRENDERING_ENABLED = 1,
    WCA_NCRENDERING_POLICY = 2,
    WCA_TRANSITIONS_FORCEDISABLED = 3,
    WCA_ALLOW_NCPAINT = 4,
    WCA_CAPTION_BUTTON_BOUNDS = 5,
    WCA_NONCLIENT_RTL_LAYOUT = 6,
    WCA_FORCE_ICONIC_REPRESENTATION = 7,
    WCA_EXTENDED_FRAME_BOUNDS = 8,
    WCA_HAS_ICONIC_BITMAP = 9,
    WCA_THEME_ATTRIBUTES = 10,
    WCA_NCRENDERING_EXILED = 11,
    WCA_NCADORNMENTINFO = 12,
    WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
    WCA_VIDEO_OVERLAY_ACTIVE = 14,
    WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
    WCA_DISALLOW_PEEK = 16,
    WCA_CLOAK = 17,
    WCA_CLOAKED = 18,
    WCA_ACCENT_POLICY = 19,
    WCA_FREEZE_REPRESENTATION = 20,
    WCA_EVER_UNCLOAKED = 21,
    WCA_VISUAL_OWNER = 22,
    WCA_LAST = 23
};
struct WindowCompositionAttributeData {
    WindowCompositionAttribute Attribute;
    int * Data;
    int SizeOfData;
};
typedef int* (*pfun)(HWND hwnd, WindowCompositionAttributeData *data);

//下面的宏其实是为了方便写绘图事件处理窗口内的模糊的代码,
//使用方式为 AERO(主窗口指针(本类中的_parent),bgColor)
#define AERO(t,bgColor) static bool v = false;\
if (v) return;\
    QPainter painter(t);\
    painter.setRenderHint(QPainter::Antialiasing);\
    painter.setPen(Qt::NoPen);\
    painter.setBrush(bgColor);\
    painter.drawRoundedRect(rect(), 0, 0);\
    v = true;

#endif // AEROWIDGET_H
