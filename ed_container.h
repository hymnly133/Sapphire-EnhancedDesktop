#ifndef ED_CONTAINER_H
#define ED_CONTAINER_H

#include <QWidget>
#include"ed_layout.h"

class ED_Container : public ED_Unit
{
    Q_OBJECT
public:
    explicit ED_Container():ED_Container(nullptr,1,1,1,1,1){}
    explicit ED_Container(QWidget *parent,int sizeX,int sizeY,int row,int cal,int space,int spaceX =0,int spaceY = 0);
    explicit ED_Container(QWidget *parent, QJsonObject rootObject);
    ED_Container(const ED_Container &other)
    :ED_Container(other.parentWidget(),other.sizeX,other.sizeY,other.row,other.col,other.space,other.spaceX,other.spaceY)
    {

    }
    ED_Layout* edlayout;
    int row,col,space,spaceX,spaceY;
    // virtual void InitLayout();
    void Say();
    virtual void InplaceAUnit(ED_Unit* aim);
    virtual bool OKforput(ED_Unit* aim);
    virtual void update_after_resize();
    virtual void setSimpleMode(bool);
    virtual void setScale(double val);
    virtual void ed_update();



protected:
    void paintEvent(QPaintEvent *event);

    // ED_Unit interface
public:
    QJsonObject to_json() ;
    void load_json(QJsonObject rootObject) ;
};
Q_DECLARE_METATYPE(ED_Container)

#endif // ED_CONTAINER_H
