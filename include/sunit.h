#ifndef SUNIT_H
#define SUNIT_H
#include "qgraphicseffect.h"
#include "qmenu.h"
#include "qparallelanimationgroup.h"
#include "qpropertyanimation.h"
#include "roundshower.h"
#include "style.h"
#include <QWidget>
#include<qlabel.h>
#include <QVariant>
#include<qopenglwidget.h>
#include<qopenglfunctions.h>
#include"slayout.h"

class MainWindow;

//桌面组件的所有基类
class SUnit : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int colorAlpha MEMBER colorAlpha NOTIFY colorAlpha_changed )
    Q_PROPERTY(QColor mainColor MEMBER mainColor NOTIFY mainColor_changed WRITE setMainColor)
    Q_PROPERTY(double scale MEMBER scale NOTIFY scale_changed WRITE setScale)
    Q_PROPERTY(double scaleFix MEMBER scaleFix NOTIFY scaleFix_changed WRITE setScaleFix)

    Q_PROPERTY(QPoint nowPos MEMBER nowPos NOTIFY nowPos_changed)
    Q_PROPERTY(QSize nowSize MEMBER nowSize NOTIFY nowSize_changed)
    Q_PROPERTY(double nowPadRatio MEMBER nowPadRatio NOTIFY nowPadRatio_changed)
    Q_PROPERTY(double nowMainColorRatio MEMBER nowMainColorRatio NOTIFY nowMainColorRatio_changed)
public:
    //用于区分容器（即将弃用）
    enum STYPE {Unit,Container};
    STYPE type = Unit;

    //所在的布局管理器
    SLayout* layout = nullptr;

    //右键菜单
    QMenu* myMenu;

    //指向当前的MainWindow
    MainWindow* pmw = nullptr;

    //用于动画的指定和更新
    QPoint nowPos;
    QSize nowSize;


    //边框显示比率Alpha
    double nowPadRatio = 1;

    double nowMainColorRatio = aim_mainColorRatio();

    //各种属性将要进入的值
    int aim_colorAlpha();
    double aim_scaleFix();
    double aim_padRatio();
    QPoint aim_pos;
    QSize aim_size;
    double aim_mainColorRatio();

    //pos/size的缓存（在！layout时使用）
    QPoint my_pos_tem;
    QSize my_size_tem;

    //长聚焦计时器
    QTimer* longFocusTimer;

    //各种当前属性
    bool alwaysShow = false;
    bool simpleMode = false;
    bool onLongFocus = false;
    bool preLongFocus = false;
    bool requireLongFocusOutDeltaTime = false;
    bool moving = false;
    bool premove = false;
    bool showRect = true;
    bool showLight = true;
    bool showSide = true;
    bool onmouse = false;
    bool deepColor = false;
    bool dark = false;
    double scaleFix = 1.0;
    double scale = 1.0;

    //移动时使用 与鼠标相对距离
    QPoint relativeP;

    //主颜色 不关注透明度
    QColor mainColor;

    //组件主颜色与主题颜色混合 不关注透明度
    QColor displayColor(){
        QColor sysrgb =  GetWindowsThemeColor();
        QColor res = QColor(
            mainColor.red()*nowMainColorRatio + sysrgb.red()*(1.0-nowMainColorRatio),
            mainColor.green()*nowMainColorRatio + sysrgb.green()*(1.0-nowMainColorRatio),
            mainColor.blue()*nowMainColorRatio + sysrgb.blue()*(1.0-nowMainColorRatio)
        );
        return res;
    }

    //加入alpha的dispalycolor
    QColor displayColor_Alphaed();

    //动画
    QParallelAnimationGroup * focusAnimations;
    QPropertyAnimation* alphaAnimation;
    QPropertyAnimation* scaleFixAnimation;
    QPropertyAnimation* padRatioAnimation;
    QPropertyAnimation* mainColorRatioAnimation;

    QParallelAnimationGroup * positionAnimations;
    QPropertyAnimation* posAnimation;
    QPropertyAnimation* sizeAnimation;


    QParallelAnimationGroup * longFocusAnimations;

    //移动至基于layout->pcontainer的坐标
    void edmove(QPoint dis);

    //基于layout->pcontainer的坐标
    QPoint edpos();

    //以格数计的大小
    int sizeX = 1;
    int sizeY = 1;


    int colorAlpha = unfocused_alpha;

    int indX = -1;
    int indY = -1;

    //圆角显示器
    roundShower* rs;
    QGraphicsDropShadowEffect* shadow_main_color;


    explicit SUnit(SLayout* dis = nullptr,int sizex =1,int sizey =1);

    SUnit(const SUnit &other);

    bool operator<(const SUnit& another) const;

    //从layout脱离
    void removeFromLayout();

    virtual void setupMenu();


    // virtual void update_after_resize();
    //重定向之后的双击事件
    virtual void double_click_action(QMouseEvent* event);
    //重定向之后的单击事件（未启用
    virtual void single_click_action();
    //重定向之后的移动事件（未启用
    virtual void mouse_move_action();
    //重定向之后的释放事件（未启用
    virtual void mouse_release_action();
    //重定向之后的释放事件（未启用
    virtual void mouse_enter_action();
    //重定向之后的离开事件（未启用
    virtual void mouse_leave_action();

    //设置格数大小
    virtual bool setBlockSize(int w,int h);

    //重定向之后的菜单事件
    virtual void onContextMenu(QContextMenuEvent* event);
    //重定向之后的Shift菜单事件
    virtual void onShiftContextMenu(QContextMenuEvent* event);

    //作为处理器时处理其他SUnit的事件
    virtual void onProcessAnother(SUnit* another);

    //精简模式更改时触发一次
    virtual void onSimpleModeChange(bool val);

    //scale*scaleFix更改时触发（包括动画更改）
    virtual void onScaleChange(double val);

    //mainColor更改时触发
    virtual void onMainColorChange(QColor val);


    //长聚焦动画更新时
    virtual void whenLongFocusAnimationChange();
    //聚焦动画更新时
    virtual void whenFocusAnimationChange();

    //作为各种操作结束时（如加载/动画）的更新
    virtual void endUpdate();

    virtual QJsonObject to_json();
    virtual void load_json(QJsonObject rootObject);

    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;


    //移除
    virtual void Remove();

    //Resize之后触发
    virtual void afterResize(QResizeEvent* event);

    //直接切换
    void changeSimpleMode();


    void setMainColor(QColor color);
    virtual void setSimpleMode(bool);
    virtual void setLongFocus(bool);
    virtual void preSetLongFocus(bool);
    virtual void setScale(double val);
    virtual void setScaleFix(double val);
    virtual void setAlwaysShow(bool val);
    virtual void setPMW(MainWindow* pmw);


    //切换屏幕
    void tryToSwitch(QMouseEvent *event);

    //更新各种情况下的动画

    virtual void updateLongFocusAnimation();
    virtual void updataFocusAnimation();
    virtual void updatePositionAnimation();

    //用于定位和resize的统一接口
    virtual QPoint MyPos();
    virtual QSize MySize();

    //被拖出时调用
    virtual void onDragedOut(QMouseEvent *event);

    //更新layout并将要触发positionAnimation时调用
    virtual void preSetInLayout(bool animated);

    //positionAnimation结束时调用
    virtual void setInLayout(bool animated);

    //在layout中由layout调用以匹配layout->pContainer的变化（不包括放置进入layout）
    virtual void updateInLayout(bool animated = true);

    //动画move
    virtual void moveto(QPoint edpos,QSize size);

public slots:
    //用于接受计时器
    void setInLayoutAniSlot();
    void longFocusTimeoutSlot();



public: signals:

    //未启用
    void mainColor_changed(QColor);

    //以下全部为动画更新的数值
    void colorAlpha_changed(int);
    void scale_changed(double);
    void scaleFix_changed(double);
    void nowPos_changed(QPoint);
    void nowSize_changed(QSize);
    void nowPadRatio_changed(double);
    void nowMainColorRatio_changed(double);


};
inline  double SUnit:: aim_mainColorRatio(){
    if(onmouse)return focused_color_ratio;
    else return unfocused_color_ratio;
}


inline int SUnit::aim_colorAlpha(){
    if(onmouse){
        if(deepColor) return focused_alpha_deep;
        else return focused_alpha;
    }
    else{
        if(deepColor) return unfocused_alpha_deep;
        else return unfocused_alpha;
    }
}

inline double SUnit::aim_scaleFix(){
    if(onmouse){
        return scale_fix_ratio;
    }
    else{
        return 1.0;
    }
}





inline SUnit::SUnit(const SUnit &other)
    :SUnit(other.layout,other.sizeX,other.sizeY)
{

}

inline bool SUnit::operator<(const SUnit &another) const{
    if(indX!=another.indX) return indX<another.indX;
    else return indY<another.indY;
}


inline QPoint SUnit::MyPos(){
    if(layout!=nullptr){
        my_pos_tem = layout->ind2Pos(indX,indY);
    }
    else{
        qDebug()<<"Use Temp Pos"<<my_pos_tem;
    }
    return my_pos_tem;
}

inline QSize SUnit::MySize(){
    if(layout!=nullptr){
        my_size_tem = layout->ind2Size(indX,indY);
    }
    else{
        qDebug()<<"Use Temp Size"<<my_size_tem;
    }
    return my_size_tem;
}
Q_DECLARE_METATYPE(SUnit);

#endif // SUNIT_H
