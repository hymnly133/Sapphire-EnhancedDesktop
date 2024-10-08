﻿#ifndef SCONTAINER_H
#define SCONTAINER_H

#include "slayoutcontainer.h"
#include "sunit.h"
#include <QWidget>
class SUnit;

//放置在桌面（理应只出现在桌面上）的格子
class SContainer : public SUnit, public SLayoutContainer
{
    Q_OBJECT
public:

    explicit SContainer(SLayout *dis = nullptr, int sizex = 3, int sizey = 3);

    SContainer(const SContainer &other)
        : SContainer(other.layout, other.sizeX, other.sizeY)
    {

    }

    virtual void Say();
    void afterResize(QResizeEvent* event) override;
    void setSimpleMode(bool)override;
    void setScale(double val)override;
    virtual void setFocus(bool val) override;


    void endUpdate()override;
    QJsonObject to_json() override;
    void load_json(QJsonObject rootObject) override;
    void setPMW(MainWindow *pmw) override;

signals:

    // SUnit interface
public slots:
    void updateColor() override;

    // SUnit interface
public:
    void remove() override;

    // SUnit interface
public:
    void preSetLongFocus(bool) override;

    // SUnit interface
public:
    void single_click_action(QMouseEvent *event) override;

    // SLayoutContainer interface
public:
    void updateAfterPut(SUnit *aim) override;
};


Q_DECLARE_METATYPE(SContainer)
#endif // SCONTAINER_H
