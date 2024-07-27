#ifndef STOOLTIP_H
#define STOOLTIP_H

#include "SAnimationRect.h"
#include "roundshower.h"
#include <QWidget>

class SToolTip : public QWidget
{
    Q_OBJECT

public:
    QString info;
    roundShower* rs;
    SAnimationRect* arect;

    QFont* font;
    bool left = false;

    QSize aimSize;
    QPoint previousPos;
    QPoint aimPos;
    explicit SToolTip(QWidget *parent = nullptr);
    void setInfo(QString info);
    void comeout();
    void end();

    static void Tip(QString info);
    static void Tip(QPoint plspos, QString info);
    static void Tip(QWidget* aim,QPoint pos, QString info);


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // STOOLTIP_H
