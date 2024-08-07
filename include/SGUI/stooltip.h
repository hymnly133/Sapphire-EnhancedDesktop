#ifndef STOOLTIP_H
#define STOOLTIP_H

#include "SAnimationRect.h"
#include "global.h"
#include "roundshower.h"
#include <QWidget>

class SToolTip : public QWidget
{
    Q_OBJECT

public:
    QString info;
    roundShower* rs;
    SAnimationRect* arect;

    //是否左侧显示
    bool left = false;
    //是否漂移
    bool shift = true;
    //是否在上方显示
    bool up = false;
    QSize aimSize;
    QPoint previousPos;
    QPoint aimPos;
    explicit SToolTip(QWidget *parent = nullptr);
    void setInfo(QString info);
    void comeout();
    void end();

    static SToolTip *tip(QString info,QPoint pos = NO_POS,bool shift = true,bool up = false);


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;

public:signals:
    void startToShow();
    void startToEnd();
};

#endif // STOOLTIP_H
