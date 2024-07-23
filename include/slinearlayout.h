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
    int num(){
        return contents->size();
    }
    int maxSize = 10;
    int insideHeight(){
        return pContainer->height()*0.9;
    }
    struct littleBlock{
        int ind;
        SUnit* content;
        littleBlock(int ind,SUnit* content):ind(ind),content(content){};
        void setInd(int ind){
            this->ind = ind;
            content->indX = ind;
        }
    };

    littleBlock* blocks[100];
    double disToCursor(int posx){
        return abs(pContainer->mapFromGlobal(pContainer->cursor().pos()).x()-posx);
    }

    double fixedDis(){
        if(num())
            return ((double)pContainer->width())/num();
        else return 1;
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
    QPoint ind2CenterPoint(int x, int y) override;
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
};

#endif // SLINEARLAYOUT_H
