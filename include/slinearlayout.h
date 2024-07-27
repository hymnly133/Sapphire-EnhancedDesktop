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
    QList<SUnit*> list;
    int insideHeight(){
        // qDebug()<<pContainer->height();
        return pContainer->height()*0.9;
    }

    double disToCursor(int posx){
        return abs(pContainer->mapFromGlobal(pContainer->cursor().pos()).x()-posx);
    }

    double fixedDis(){
        if(contents->size())
            return ((double)pContainer->width())/contents->size();
        else return pContainer->width();
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


    // ED_Layout interface
public:
    void updateAfterRemove(SUnit *, int, int) override;
    void updateBeforePut(SUnit *, int, int) override;



    // ED_Layout interface
public:
    void load_json(QJsonObject rootObject) override;

    // ED_Layout interface
public:
    void updateAfterPut(SUnit *, int, int) override;

    // ED_Layout interface
public:
    void updateBeforePutAnimation(SUnit *, int, int) override;



    // SLayout interface
public:
    void say() override;

    // SLayout interface
public:
    QJsonObject to_json() override;
};

#endif // SLINEARLAYOUT_H
