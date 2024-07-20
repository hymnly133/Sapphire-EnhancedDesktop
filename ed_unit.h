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
    Q_PROPERTY(int colorAlpha MEMBER colorAlpha NOTIFY alpha_changed )
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
    double nowPadRatio;

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


    int sizeX = 1;
    int sizeY = 1;
    int ind;
    int aim_alpha(){
        if(onmouse){
            if(deepColor) return active_alpha_deep;
            else return active_alpha;
        }
        else{
            if(deepColor) return sleep_alpha_deep;
            else return sleep_alpha;
        }
    }
    double aim_scaleFix(){
        if(onmouse){
            return scale_fix_ratio;
        }
        else{
            return 1.0;
        }
    }
    double aim_padRatio(){
        if(layout==nullptr) return 1.0;
        else{
            if(layout->isMain) return 1.0;
            else{
                if(onmouse) return 1.0;
                else return 0.0;
            }
        }
    }

    int colorAlpha;
    bool alwaysShow = false;
    bool simpleMode = false;

    int indX = -1;
    int indY = -1;

    roundShower* rs;
    QGraphicsDropShadowEffect* shadow_main_color;

    explicit ED_Unit();
    explicit ED_Unit(QWidget *parent,int sizex,int sizey);
    ED_Unit(const ED_Unit &other)
        :ED_Unit(other.parentWidget(),other.sizeX,other.sizeY)
    {

    }

    bool operator<(const ED_Unit& another) const{
        if(indX!=another.indX) return indX<another.indX;
        else return indY<another.indY;
    }

    void removeFromLayout();

    virtual void setupMenu();


    // virtual void update_after_resize();
    virtual void double_click_action();
    virtual void single_click_action();
    virtual void mouse_move_action();
    virtual void mouse_release_action();
    virtual void mouse_enter_action();
    virtual void mouse_leave_action();
    virtual void setBlockSize(int w,int h);

    virtual void onContextMenu(QContextMenuEvent* event);
    virtual void onShiftContextMenu(QContextMenuEvent* event);


    virtual void whenSimpleModeChange(bool val);
    virtual void whenScaleChange(double val);
    virtual void whenMainColorChange(QColor val);
    virtual void afterResize(QResizeEvent* event);


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
    void changeSimpleMode();

    void setMainColor(QColor color);
    virtual void setSimpleMode(bool);
    virtual void setScale(double val);
    virtual void setScaleFix(double val);
    virtual void setAlwaysShow(bool val);
    virtual void setPMW(MainWindow* pmw);
    QColor mainColor_Alphaed();


    void updataFocusAnimation();
    void updatePositionAnimation();

    QPoint MyPos_Centual(){
        return layout->ind2Pos_Centual(indX,indY);
    }
    QPoint MyPos(){
        return layout->ind2Pos(indX,indY);
    }
    QSize MySize(){
        return layout->ind2Size(indX,indY);
    }
    virtual void whenDragedOut();
    virtual void preSetInLayout(bool animated);
    virtual void setInLayout(bool animated);
    virtual void updateInLayout();
    virtual void moveto(QPoint pos,QSize size);


public: signals:
    void alpha_changed(int);
    void mainColor_changed(QColor);
    void scale_changed(double);
    void scaleFix_changed(double);
    void nowPos_changed(QPoint);
    void nowSize_changed(QSize);
    void nowPadRatio_changed(double);

    // QOpenGLWidget interface


    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;

    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};
Q_DECLARE_METATYPE(ED_Unit);

#endif // ED_UNIT_H
