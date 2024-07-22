#ifndef ED_DOCK_H
#define ED_DOCK_H

#include "ed_blockcontainer.h"
#include "ed_unit.h"
#include <QWidget>

class ED_Dock : public ED_Container
{
    Q_OBJECT
public:
    explicit ED_Dock(QWidget *parent = nullptr, int outSizeX=5, int outSizeY=1);
    explicit ED_Dock(QWidget *parent, QJsonObject rootObject);
    ED_Dock(const ED_BlockContainer &other)
        :ED_Dock(other.parentWidget())
    {}





    void paintEvent(QPaintEvent *event);
signals:

    // ED_Unit interface
public:
    void ed_update() override;
};
Q_DECLARE_METATYPE(ED_Dock);
#endif // ED_DOCK_H
