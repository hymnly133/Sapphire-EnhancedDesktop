#ifndef ED_UNIT_H
#define ED_UNIT_H
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
#include"ed_layout.h"


class MainWindow;
class ED_Unit : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int colorAlpha MEMBER colorAlpha NOTIFY colorAlpha_changed )
    Q_PROPERTY(QColor mainColor MEMBER mainColor NOTIFY mainColor_changed WRITE setMainColor)
    Q_PROPERTY(double scale MEMBER scale NOTIFY scale_changed WRITE setScale)
    Q_PROPERTY(double scaleFix MEMBER scaleFix NOTIFY scaleFix_changed WRITE setScaleFix)
    Q_PROPERTY(QPoint nowPos MEMBER nowPos NOTIFY nowPos_changed)
    Q_PROPERTY(QSize nowSize MEMBER nowSize NOTIFY nowSize_changed)
    Q_PROPERTY(double nowPadRatio MEMBER nowPadRatio NOTIFY nowPadRatio_changed)
public:
    enum ED_TYPE {Unit,Container};
    ED_TYPE type = Unit;
    ED_Layout* layout = nullptr;

    QMenu* myMenu;
    MainWindow* pmw;
    QPoint nowPos;
    QPoint aim_pos;
    QSize nowSize;
    QSize aim_size;

    QPoint my_pos_tem;
    QPoint my_pos_centual_tem;
    QSize my_size_tem;
    double nowPadRatio;
    QTimer* longFocusTimer;

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
    QPoint relativeP;
    QColor mainColor;

    QParallelAnimationGroup * positionAnimations;
    QPropertyAnimation* alphaAnimation;
    QPropertyAnimation* scaleFixAnimation;
    QPropertyAnimation* padRatioAnimation;

    QParallelAnimationGroup * focusAnimations;
    QPropertyAnimation* posAnimation;
    QPropertyAnimation* sizeAnimation;

    QParallelAnimationGroup * longFocusAnimations;

    void edmove(QPoint dis);
    QPoint edpos();
    int sizeX = 1;
    int sizeY = 1;
    int ind;
    int aim_colorAlpha();
    double aim_scaleFix();
    double aim_padRatio();

    int colorAlpha = sleep_alpha;
    bool alwaysShow = false;
    bool simpleMode = false;

    int indX = -1;
    int indY = -1;

    roundShower* rs;
    QGraphicsDropShadowEffect* shadow_main_color;

    explicit ED_Unit();
    explicit ED_Unit(QWidget *parent,int sizex,int sizey);
    ED_Unit(const ED_Unit &other);

    bool operator<(const ED_Unit& another) const;

    void removeFromLayout();

    virtual void setupMenu();


    // virtual void update_after_resize();
    virtual void double_click_action();
    virtual void single_click_action();
    virtual void mouse_move_action();
    virtual void mouse_release_action();
    virtual void mouse_enter_action();
    virtual void mouse_leave_action();
    virtual bool setBlockSize(int w,int h);

    virtual void onContextMenu(QContextMenuEvent* event);
    virtual void onShiftContextMenu(QContextMenuEvent* event);
    virtual void onProcessAnother(ED_Unit* another);

    virtual void whenSimpleModeChange(bool val);
    virtual void whenScaleChange(double val);
    virtual void whenMainColorChange(QColor val);



    virtual void whenLongFocusAnimationChange();
    virtual void whenFocusAnimationChange();


    virtual void ed_update();

    virtual QJsonObject to_json();
    virtual void load_json(QJsonObject rootObject);

    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *) override;                      //进入QWidget瞬间事件
    void leaveEvent(QEvent *) override;                      //离开QWidget瞬间事件
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

    virtual void afterResize(QResizeEvent* event);

    void changeSimpleMode();

    void setMainColor(QColor color);
    virtual void setSimpleMode(bool);
    virtual void setLongFocus(bool);
    virtual void preSetLongFocus(bool);
    virtual void setScale(double val);
    virtual void setScaleFix(double val);
    virtual void setAlwaysShow(bool val);
    virtual void setPMW(MainWindow* pmw);

    QColor mainColor_Alphaed();

    void tryToSwitch(QMouseEvent *event);

    virtual void updateLongFocusAnimation();
    virtual void updataFocusAnimation();
    virtual void updatePositionAnimation();

    virtual QPoint MyPos_Centual();
    virtual QPoint MyPos();
    virtual QSize MySize();
    virtual void onDragedOut(QMouseEvent *event);
    virtual void preSetInLayout(bool animated);
public slots:
    void setInLayoutAniSlot();
    void longFocusTimeoutSlot();

    virtual void setInLayout(bool animated);
    virtual void updateInLayout(bool animated = true);
    virtual void moveto(QPoint pos,QSize size);


public: signals:
    void colorAlpha_changed(int);
    void mainColor_changed(QColor);
    void scale_changed(double);
    void scaleFix_changed(double);
    void nowPos_changed(QPoint);
    void nowSize_changed(QSize);
    void nowPadRatio_changed(double);

    // QOpenGLWidget interface


};

inline int ED_Unit::aim_colorAlpha(){
    if(onmouse){
        if(deepColor) return active_alpha_deep;
        else return active_alpha;
    }
    else{
        if(deepColor) return sleep_alpha_deep;
        else return sleep_alpha;
    }
}

inline double ED_Unit::aim_scaleFix(){
    if(onmouse){
        return scale_fix_ratio;
    }
    else{
        return 1.0;
    }
}

inline double ED_Unit::aim_padRatio(){
    if(layout==nullptr) return 1.0;
    else{
        if(layout->isMain) return 1.0;
        else{
            if(onmouse) return 1.0;
            else return 0.0;
        }
    }
}

inline ED_Unit::ED_Unit(const ED_Unit &other)
    :ED_Unit(other.parentWidget(),other.sizeX,other.sizeY)
{

}

inline bool ED_Unit::operator<(const ED_Unit &another) const{
    if(indX!=another.indX) return indX<another.indX;
    else return indY<another.indY;
}

inline QPoint ED_Unit::MyPos_Centual(){
    if(layout!=nullptr){
        my_pos_centual_tem = layout->ind2Pos_Centual(indX,indY);
    }
    return my_pos_centual_tem;

}

inline QPoint ED_Unit::MyPos(){
    if(layout!=nullptr){
        my_pos_tem = layout->ind2Pos(indX,indY);
    }
    return my_pos_tem;
}

inline QSize ED_Unit::MySize(){
    if(layout!=nullptr){
        my_size_tem = layout->ind2Size(indX,indY);
    }
    return my_size_tem;
}
Q_DECLARE_METATYPE(ED_Unit);

#endif // ED_UNIT_H
