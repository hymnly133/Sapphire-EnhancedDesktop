#ifndef SLINEARLAYOUT_H
#define SLINEARLAYOUT_H

#include "slayout.h"
#include "sunit.h"
#include "qwidget.h"
#include <QObject>

class SLinearLayout : public SLayout
{
public:
    explicit SLinearLayout(QWidget *parent = nullptr);
    int nowNum=0;
    //用于维持内部顺序
    QList<SUnit*> list;
    //用于标识方向
    enum Dire{
        Horr = 0,
        Vert = 1
    };
    Dire direction = Horr;

    double longSide(){
        if(direction==Horr)
            return pContainer->width();
        else
            return pContainer->height();
    }
    double shortSide(){
        if(direction==Horr)
            return pContainer->height();
        else
            return pContainer->width();
    }

    double insideSize(){
        return shortSide()*0.9;
    }

    double disToCursor(int posx){
        return abs(pContainer->mapFromGlobal(pContainer->cursor().pos()).x()-posx);
    }

    double fixedDis(){
        if(contents->size())
            return longSide()/contents->size();
        else return longSide();
    }

    void refresh();

    double fixedCX(int ind){
        return fixedDis()*(ind+0.5);
    }

signals:
    // ED_Layout interface
public:
    //采用（x，0）作为索引
    QPoint pos2Ind(int posx, int posy) override;
    QSize ind2Size(int xind, int yind) override;
    QPoint ind2Pos(int xind, int yind) override;

    bool Occupied(int x, int y) override;
    SUnit *ind2Unit(int xind, int yind) override;
    QPoint defaultPutableInd(SUnit *aim) override;
    QPoint clearPutableInd(SUnit *aim) override;

    void updateAfterRemove(SUnit *, int, int) override;
    void updateBeforePut(SUnit *, int, int) override;
    void updateAfterPut(SUnit *, int, int) override;
    void updateBeforePutAnimation(SUnit *, int, int) override;

    void setDirection(Dire aimDirection);


    void load_json(QJsonObject rootObject) override;
    QJsonObject to_json() override;


    void say() override;


};

#endif // SLINEARLAYOUT_H
