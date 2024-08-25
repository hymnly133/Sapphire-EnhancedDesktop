#ifndef SLAYOUT_H
#define SLAYOUT_H

#include "qjsonobject.h"
#include "qregion.h"
#include "qwidget.h"
#include "slayoutcontainer.h"
#include <QObject>
class SUnit;
class SContainer;
class MainWindow;
void writeContent();

class SLayout: public QObject
{
    Q_OBJECT
public:

    //可见
    bool visibal;

    //区分主容器
    bool isMain = false;

    bool contentResizable = true;

    //可用(放置和拖动),只会影响两种判断
    bool enable = true;

    //用于自定义显示区域（false使用pContainer->size)
    bool useStandaloneRect  = false;
    void setStandalongRect(QRect rect);

    virtual int W_Container()
    {
        if(useStandaloneRect) {
            return standaloneRect.width();
        } else {
            return pContainerW->width();
        }
    };

    virtual int H_Container()
    {
        if(useStandaloneRect) {
            return standaloneRect.height();
        } else {
            return pContainerW->height();
        }
    };

    QRect standaloneRect = QRect(1, 1, 1, 1);

    MainWindow* pmw;

    //当前区域（用于显示桌面模糊，未启用
    QRegion region;

    //容器(实际上是一个对象，只是为了方便代码编译等，给与两种形式)
    SLayoutContainer* pContainer = nullptr;
    //实际容器Widget
    QWidget* pContainerW = nullptr;

    //pContainer的Widget指针
    QWidget* pContainerS = nullptr;

    //内部管理
    QList<SUnit*> contents;
    QList<SLayout*> insideLayouts;



    SLayout(SLayoutContainer* father = nullptr);

    // 从坐标获得最近的Block序号
    // QPoint pos2Ind(QPoint point);
    // virtual QPoint pos2Ind(int posx, int posy) = 0;


    //从Block序号获取中心坐标
    // virtual QPoint ind2CenterPoint(QPoint ind);
    virtual QPoint unit2CenterPoint(SUnit* aim);

    //从unit获得大小
    virtual QSize unit2Size(SUnit* aim) = 0;
    //从unit获得坐标
    virtual QPoint unit2Pos(SUnit* aim) = 0;

    // virtual QSize unit2Size(QPoint ind);
    //从序号获得pos
    // virtual QPoint unit2Pos(QPoint ind);

    //从Block序号获取是否占用
    bool Occupied(QPoint ind);
    virtual bool Occupied(int x, int y) = 0;

    //将一个Unit放置在Block中，并在双方的变量中记录以便之后识别，是放置的主函数
    void putUnit(SUnit* aim, int xind, int yind, bool animated);
    void putUnit(SUnit* aim, QPoint ind, bool animated);


    //将一个Unit从坐标体系中脱离
    void RemoveAUnit(SUnit* aim);


    //将一个Unit放入Layou有两种方法，default和clear
    //default用于初始化
    //clear用于定位放置

    //对应的序号获取方法，不能放置返回（-1，-1）
    virtual QPoint defaultPutableInd(SUnit* aim) = 0;
    virtual QPoint clearPutableInd(SUnit* aim) = 0;

    //对应的放置方法
    void clearPut(SUnit* aim, bool animated);
    void defaultPut(SUnit* aim, bool animated);

    //判断一个Unit能否放置进入
    bool OKForClearPut(SUnit* aim);
    bool OKForDefaultPut(SUnit* aim, bool force = false);

    //根据一个Block索引获取对应的ED_Unit指针
    virtual SUnit* ind2Unit(int xind, int yind) = 0;
    SUnit* ind2Unit(QPoint ind);

    //从相对pcontainerW的坐标获取unit
    SUnit* pos2Unit(QPoint pos);

    //设置可见性
    virtual void setVisible(bool val, bool force = false);

    //更新region
    void UpdateRegion();
    //更新内部组件位置，默认调用SUnit::updateInLayout
    virtual void UpdateContentPositon(bool animated = true);

    //开始放置前调用（content与unit::layout未更新）
    virtual void updateBeforePut(SUnit*, int, int);

    //开始放置后，动画开始前调用（content与unit::layout已更新）
    virtual void updateBeforePutAnimation(SUnit*, int, int);

    //开始放置后，动画开始前调用（content与unit::layout已更新）
    virtual void updateAfterPut(SUnit*, int, int);

    virtual void updateBeforeRemove(SUnit*, int, int);
    virtual void updateAfterRemove(SUnit*, int, int);
    virtual void load_json(QJsonObject rootObject);
    virtual QJsonObject to_json();

    void saveLayout()
    {
        writeContent();
    }

    virtual void setEnable(bool val);

    virtual void afterResize();
    virtual void say();
};



#endif // SLAYOUT_H
