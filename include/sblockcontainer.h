#ifndef SBLOCKCONTAINER_H
#define SBLOCKCONTAINER_H

#include <QWidget>
#include "sblocklayout.h"
#include "scontainer.h"

class SBlockContainer : public SContainer
{
    Q_OBJECT
    Q_PROPERTY(double outSizeFix MEMBER outSizeFix NOTIFY outSizeFix_changed);
public:
    explicit SBlockContainer(SLayout* dis = nullptr,int outsizex = 3,int outsizey = 3,int row=3,int cal =3,int space = 2, int spaceX= 4,int spaceY=4);

    SBlockContainer(const SBlockContainer &other);
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
    QPoint MyPos() override{
        QPoint tem = QPoint(SUnit::MyPos().x()-(1.0*SUnit::MySize().width()*(outSizeFix-1)/2),SUnit::MyPos().y()-(1.0*SUnit::MySize().height()*(outSizeFix-1)/2));
        return refine(tem,MySize());
    };
    QSize MySize() override{
        return SUnit::MySize()*outSizeFix;
    };

    // ED_Unit interface
public:
    void preSetLongFocus(bool) override;


    // ED_Unit interface
public:
    void setLongFocus(bool) override;
};

Q_DECLARE_METATYPE(SBlockContainer)

#endif // SBLOCKCONTAINER_H
