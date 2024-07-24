#ifndef SLAYOUT_H
#define SLAYOUT_H

#include "qjsonobject.h"
#include "qregion.h"
#include <QObject>
class SUnit;
class SContainer;
class MainWindow;
void writeJson();

class SLayout
{

public:
    // virtual QSize areaSize() =0;

    //可见
    bool visibal;
    //区分主容器
    bool isMain = false;
    //用于自定义显示区域（false使用pContainer->size)
    bool useStandaloneRect  = false;
    void setStandalongRect(QRect rect);
    QRect standaloneRect = QRect(1,1,1,1);

    MainWindow* pmw;

    //当前区域（用于显示桌面模糊，未启用
    QRegion region;

    //容器
    QWidget* pContainer;

    //内部管理，分为三个
    QVector<SUnit*>* contents = new QVector<SUnit*>;
    QVector<SUnit*>* contents_AlwaysShow = new QVector<SUnit*>;

    //contents差contents_AlwaysShow
    QVector<SUnit*>* contents_Show = new QVector<SUnit*>;


    SLayout(QWidget* father);

    // 从坐标获得最近的Block序号
    QPoint pos2Ind(QPoint point);
    virtual QPoint pos2Ind(int posx,int posy) = 0;


    //从Block序号获取中心坐标
    virtual QPoint ind2CenterPoint(QPoint ind);
    virtual QPoint ind2CenterPoint(int x,int y);

    //从序号获得大小
    virtual QSize ind2Size(QPoint ind);
    virtual QSize ind2Size(int xind,int yind) = 0;

    //从序号获得pos
    virtual QPoint ind2Pos(QPoint ind);
    virtual QPoint ind2Pos(int xind,int yind) = 0;

    //从Block序号获取是否占用
    bool Occupied(QPoint ind);
    virtual bool Occupied(int x,int y) =0;

    //将一个Unit放置在Block中，并在双方的变量中记录以便之后识别，是放置的主函数
    void putUnit(SUnit* aim,int xind,int yind,bool animated);
    void putUnit(SUnit* aim,QPoint ind,bool animated);


    //将一个Unit从坐标体系中脱离
    void RemoveAUnit(SUnit* aim);


    //将一个Unit放入Layou有两种方法，default和clear
    //default用于初始化
    //clear用于定位放置

    //对应的序号获取方法，不能放置返回（-1，-1）
    virtual QPoint defaultPutableInd(SUnit* aim)=0;
    virtual QPoint clearPutableInd(SUnit* aim)=0;

    //对应的放置方法
    void clearPut(SUnit* aim,bool animated);
    void defaultPut(SUnit* aim,bool animated);

    //判断一个Unit能否防止进入
    bool OKForClearPut(SUnit* aim);
    bool OKForDefaultPut(SUnit* aim);

    //根据一个Block索引获取对应的ED_Unit指针
    virtual SUnit* ind2Unit(int xind,int yind) =0;
    SUnit* ind2Unit(QPoint ind);

    //设置可见性
    virtual void setVisible(bool val, bool force = false);

    //更新region
    void UpdateRegion();
    //更新内部组件位置，默认调用SUnit::updateInLayout
    void UpdateContentPositon(bool animated =true);

    //开始放置前调用（content与unit::layout未更新）
    virtual void updateBeforePut(SUnit*,int,int);

    //开始放置后，动画开始前调用（content与unit::layout已更新）
    virtual void updateBeforePutAnimation(SUnit*,int,int);

    //开始放置后，动画开始前调用（content与unit::layout已更新）
    virtual void updateAfterPut(SUnit*,int,int);

    virtual void updateBeforeRemove(SUnit*,int,int);
    virtual void updateAfterRemove(SUnit*,int,int);
    virtual void load_json(QJsonObject rootObject);
    virtual QJsonObject to_json();

    void saveLayout(){
        writeJson();
    }

    virtual void say();
};



#endif // SLAYOUT_H
