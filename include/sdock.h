#ifndef SDOCK_H
#define SDOCK_H

#include "sblockcontainer.h"
#include "sunit.h"
#include <QWidget>

class SDock : public SContainer
{
    Q_OBJECT
public:
    explicit SDock(SLayout *dis = nullptr, int outSizeX=5, int outSizeY=1);

    void paintEvent(QPaintEvent *event);
signals:

    // ED_Unit interface
public:
    void endUpdate() override;
};
Q_DECLARE_METATYPE(SDock);
#endif // SDOCK_H
