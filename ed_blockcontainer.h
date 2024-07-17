#ifndef ED_BLOCKCONTAINER_H
#define ED_BLOCKCONTAINER_H

#include <QWidget>
#include "ed_container.h"

class ED_BlockContainer : public ED_Container
{
    Q_OBJECT
public:
    explicit ED_BlockContainer():ED_BlockContainer(nullptr,1,1,1,1,1){}
    explicit ED_BlockContainer(QWidget *parent,int sizeX,int sizeY,int row,int cal,int space,int spaceX =0,int spaceY = 0);
    explicit ED_BlockContainer(QWidget *parent, QJsonObject rootObject);
    ED_BlockContainer(const ED_BlockContainer &other)
    :ED_BlockContainer(other.parentWidget(),other.sizeX,other.sizeY,other.row,other.col,other.space,other.spaceX,other.spaceY)
    {}
    int row,col,space,spaceX,spaceY;
    // virtual void InitLayout();
    void Say();



protected:
    void paintEvent(QPaintEvent *event);

    // ED_Unit interface
public:
    QJsonObject to_json() ;
    void load_json(QJsonObject rootObject) ;
};
Q_DECLARE_METATYPE(ED_BlockContainer)

#endif // ED_BLOCKCONTAINER_H
