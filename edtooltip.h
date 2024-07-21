#ifndef EDTOOLTIP_H
#define EDTOOLTIP_H

#include "mainwindow.h"
#include "qparallelanimationgroup.h"
#include "qpropertyanimation.h"
#include "roundshower.h"
#include <QWidget>

class EDToolTip : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int nowRadius MEMBER nowRadius NOTIFY nowRadius_changed)
    Q_PROPERTY(QSize nowSize MEMBER nowSize NOTIFY nowSize_changed)
    Q_PROPERTY(int nowAlpha MEMBER nowAlpha NOTIFY nowAlpha_changed)
    Q_PROPERTY(QPoint nowPos MEMBER nowPos NOTIFY nowPos_changed)

public:
    QString info;
    roundShower* rs;
    QFont* font;
    int nowRadius=0;
    QSize nowSize = QSize(0,0);
    int nowAlpha=255;
    QSize aimSize;
    QPoint nowPos = QPoint(0,0);
    QPoint aimPos = QPoint(0,0);
    QPropertyAnimation* radiusAnimation;
    QPropertyAnimation* sizeAnimation;
    QPropertyAnimation* alphaAnimation;
    QPropertyAnimation* posAnimation;

    QParallelAnimationGroup* animations;
    bool onEnd = false;
    QPoint previousPos;
    explicit EDToolTip(QWidget *parent = nullptr);
    void setInfo(QString info);
    void comeout();
    void end();

    static void Tip(QString info);
    static void Tip(QPoint plspos, QString info);
    static void Tip(QWidget* aim,QPoint pos, QString info);


signals:
    void nowRadius_changed(int val);
    void nowAlpha_changed(int val);
    void nowSize_changed(QSize val);
    void nowPos_changed(QPoint val);

public slots:
    void whenAnimationChange();
    void whenEndAnimationEnd();




    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // EDTOOLTIP_H
