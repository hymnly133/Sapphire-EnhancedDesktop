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

    //将一个ED_Unit放置在Block中，并在双方的变量中记录以便之后识别
    void putUnit(SUnit* aim,int xind,int yind,bool animated);
    void putUnit(SUnit* aim,QPoint ind,bool animated);


    //将一个Unit从坐标体系中脱离
    void RemoveAUnit(SUnit* aim);
    //将一个Unit加入到坐标体系中
    void clearPut(SUnit* aim,bool animated);
    virtual void defaultPut(SUnit* aim,bool animated);

    //判断一个ED_Unit的左上角放置在该Block中是否合法

    bool OKForClearPut(SUnit* aim);
    bool OKForDefaultPut(SUnit* aim);

    //根据一个Block索引获取对应的ED_Unit指针
    virtual SUnit* ind2Unit(int xind,int yind) =0;
    SUnit* ind2Unit(QPoint ind);

    virtual QPoint defaultPutableInd(SUnit* aim)=0;
    virtual QPoint clearPutableInd(SUnit* aim)=0;


    //将一个ED_Unit按序号最下且可放置的位置放置

    virtual void setVisible(bool val, bool force = false);

    void UpdateRegion();
    void UpdateContentPositon(bool animated =true);


    virtual void updateAfterPut(SUnit*,int,int);
    virtual void updateBeforePutAnimation(SUnit*,int,int);
    virtual void updateAfterRemove(SUnit*,int,int);
    virtual void updateBeforePut(SUnit*,int,int);
    virtual void updateBeforeRemove(SUnit*,int,int);

    virtual void load_json(QJsonObject rootObject);
    virtual QJsonObject to_json();

    void saveLayout(){
        writeJson();
    }

    virtual void say();
};



#endif // SLAYOUT_H
