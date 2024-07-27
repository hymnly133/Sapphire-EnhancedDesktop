#ifndef SNOTICE_H
#define SNOTICE_H

#include "SAnimationRect.h"
#include "qparallelanimationgroup.h"
#include "qpropertyanimation.h"
#include "qtimer.h"
#include "roundshower.h"
#include <QWidget>

class SNotice : public QWidget
{
    Q_OBJECT
public:
    explicit SNotice(QWidget *parent = nullptr);
    QString info;
    QString title;
    roundShower* rs;
    QFont* titleFont;
    QFont* infoFont;
    SAnimationRect* arect;
    QSize titleSize = QSize(0,0);
    QSize infoSize=QSize(0,0);
    QSize aimSize;
    QPoint previousPos;
    QPoint aimPos;
    QTimer* endTimer;
    QPoint pmw2pls;
    int staytime;
    void setTitle(QString Title);
    void setInfo(QStringList info);
    void setStayTime(int time);
    void comeout();
    void end();
    void whenAnimationUpdate();

    static int spaceBetweenNotice;
    static int borad;
    static int boradTitle;
    static int boradInfo;
    static int spaceBetweenTileInfo;
    static QList<SNotice*> noticeList;

    static void notice(QStringList info, QString title ="", int time =3000);
    static int nowOKPosY(SNotice* aim);
    static void updateAllNoticeAnimation();

protected:
    void paintEvent(QPaintEvent *event) override;


    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // SNOTICE_H
