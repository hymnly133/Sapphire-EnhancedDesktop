#ifndef ED_DOCK_H
#define ED_DOCK_H

#include "ed_container.h"
#include "ed_unit.h"
#include <QWidget>

class ED_Dock : public ED_Container
{
    Q_OBJECT
public:
    using ED_Container::ED_Container;
    ED_Dock(QWidget *parent,int outSizeX,int outSizeY,int inSize);
    virtual void InplaceAUnit(ED_Unit* aim) override;
    virtual bool OKforput(ED_Unit* aim) override;

    void paintEvent(QPaintEvent *event);
signals:
};
Q_DECLARE_METATYPE(ED_Dock);
#endif // ED_DOCK_H
