#ifndef ED_BLOCKCONTAINER_H
#define ED_BLOCKCONTAINER_H

#include <QWidget>
#include "ed_container.h"

class ED_BlockContainer : public ED_Container
{
    Q_OBJECT
    Q_PROPERTY(double outSizeFix MEMBER outSizeFix NOTIFY outSizeFix_changed);
public:
    explicit ED_BlockContainer():ED_BlockContainer(nullptr,1,1,1,1,1){}
    explicit ED_BlockContainer(QWidget *parent,int sizeX,int sizeY,int row,int cal,int space,int spaceX =0,int spaceY = 0);
    explicit ED_BlockContainer(QWidget *parent, QJsonObject rootObject);
    ED_BlockContainer(const ED_BlockContainer &other)
    :ED_BlockContainer(other.parentWidget(),other.sizeX,other.sizeY,1,1,1)
    {}
    // virtual void InitLayout();
    void Say();
    bool enableLongFocusEffect = 0;
    double outSizeFix = 1;
    double aim_outSizeFix(){
        if(onLongFocus){
            return long_focus_container_fix_ratio;
        }
        else{
            return 1.0;
        }
    }
    QPropertyAnimation* outSizeAnimation;

protected:
    void paintEvent(QPaintEvent *event);

signals:
    void outSizeFix_changed(double);

public slots:
    // ED_Unit interface
public:
    QJsonObject to_json() ;
    void load_json(QJsonObject rootObject) ;


    // ED_Unit interface
public:
    void updateLongFocusAnimation() override;


    void whenLongFocusAnimationChange() override;
public:
    QPoint refine(QPoint dis,QSize size){
        if(dis.x()<0) dis.setX(0);
        if(dis.y()<0) dis.setY(0);
        if(layout!=nullptr){
            if(dis.x()+size.width()>layout->pContainer->width()) dis.setX(layout->pContainer->width()-size.width());
            if(dis.y()+size.height()>layout->pContainer->height()) dis.setY(layout->pContainer->height()-size.height());
        }
        return dis;
    }
    QPoint MyPos_Centual() override{
        QPoint tem = QPoint(ED_Unit::MyPos_Centual().x()-(1.0*ED_Unit::MySize().width()*(outSizeFix-1)/2),ED_Unit::MyPos_Centual().y()-(1.0*ED_Unit::MySize().height()*(outSizeFix-1)/2));
        return refine(tem,MySize());
    };
    QPoint MyPos() override{
        QPoint tem = QPoint(ED_Unit::MyPos().x()-(1.0*ED_Unit::MySize().width()*(outSizeFix-1)/2),ED_Unit::MyPos().y()-(1.0*ED_Unit::MySize().height()*(outSizeFix-1)/2));
        return refine(tem,MySize());
    };
    QSize MySize() override{
        return ED_Unit::MySize()*outSizeFix;
    };

    // ED_Unit interface
public:
    void setPreLongFocus(bool) override;
};
Q_DECLARE_METATYPE(ED_BlockContainer)

#endif // ED_BLOCKCONTAINER_H
