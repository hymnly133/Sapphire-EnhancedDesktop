#ifndef SDOCK_H
#define SDOCK_H

#include "sblockcontainer.h"
#include "sunit.h"
#include <QWidget>

class SDock : public SContainer
{
    Q_OBJECT
public:
    explicit SDock(SLayout *dis = nullptr, int outSizeX = 5, int outSizeY = 1);
    void paintEvent(QPaintEvent *event);
    void mouse_enter_action();

    //根据linear的特点进行特化大小改变
    bool onBigger() override;
    bool onSmaller() override;

    //尝试改变方向
    bool switchDirection();
    bool onSwitch  = false;
    bool useContentColor = true;
    QMap<SUnit*, int> map;




public:
    void endUpdate() override;

signals:
    // ED_Unit interface

public slots:
    void switchFinishedSlot();

    // SUnit interface
public:
    void setupEditMenu() override;
    QJsonObject to_json() override;
    void load_json(QJsonObject rootObject) override;
};
Q_DECLARE_METATYPE(SDock);
#endif // SDOCK_H
