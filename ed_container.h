#ifndef ED_CONTAINER_H
#define ED_CONTAINER_H

#include "ed_unit.h"
#include <QWidget>

class ED_Container : public ED_Unit
{
    Q_OBJECT
public:

    explicit ED_Container(QWidget *parent = nullptr,int sizeX=3,int sizeY=3);
    explicit ED_Container(QWidget *parent, QJsonObject rootObject);
    ED_Container(const ED_Container &other)
        :ED_Container(other.parentWidget(),other.sizeX,other.sizeY)
    {

    }
    ED_Layout* inside;
    // virtual void InitLayout();
    void clearPut(ED_Unit* aim,bool animated);
    bool OKForClearPut(ED_Unit* aim);
    void afterResize(QResizeEvent* event) override;
    void setSimpleMode(bool)override;
    void setScale(double val)override;
    void ed_update()override;


    // ED_Unit interface
public:
    QJsonObject to_json()override ;
    void load_json(QJsonObject rootObject)override ;
signals:

    // ED_Unit interface
public:
    void setPMW(MainWindow *pmw) override;
};

#endif // ED_CONTAINER_H
