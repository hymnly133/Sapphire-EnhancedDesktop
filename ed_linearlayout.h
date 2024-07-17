#ifndef ED_LINEARLAYOUT_H
#define ED_LINEARLAYOUT_H

#include "ed_layout.h"
#include "ed_unit.h"
#include "qwidget.h"
#include <QObject>

class ED_LinearLayout : public ED_Layout
{
public:
    explicit ED_LinearLayout(QWidget *parent = nullptr);
    int num(){
        return contents->size();
    }
    int maxSize = 10;
    struct littleBlock{
        int ind;
        ED_Unit* content;
        littleBlock(int ind,ED_Unit* content):ind(ind),content(content){};
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
    ED_Unit *ind2Unit(int xind, int yind) override;
    QPoint defaultPutableInd(ED_Unit *aim) override;
    QPoint clearPutableInd(ED_Unit *aim) override;


    // ED_Layout interface
public:
    void updateAfterRemove(ED_Unit *, int, int) override;
    void updateBeforePut(ED_Unit *, int, int) override;



    // ED_Layout interface
public:
    void load_json(QJsonObject rootObject) override;

    // ED_Layout interface
public:
    void updateAfterPut(ED_Unit *, int, int) override;

    // ED_Layout interface
public:
    void updateBeforePutAnimation(ED_Unit *, int, int) override;
};

#endif // ED_LINEARLAYOUT_H
