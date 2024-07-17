#ifndef ED_BLOCKLAYOUT_H
#define ED_BLOCKLAYOUT_H

#include "ed_unit.h"
#include "qwidget.h"
class ED_BlockLayout:public ED_Layout
{
    struct little_Block{
        int indX;
        int indY;
        int posX(){
            return playout->space+indX*(playout->spaceX+w());
        };
        int posY(){
            return playout->space+indY*(playout->spaceY+h());
        };
        int w(){
            return playout->W_Block_Clean();
        };
        int h(){
            return playout->H_Block_Clean();
        };
        bool occupied;
        ED_BlockLayout* playout;
        ED_Unit* content;
        int CenterX(){
            return posX() +w()/2;
        }
        int CenterY(){
            return posY() +h()/2;
        }
        QPoint CenterPoint(){
            return QPoint(CenterX(),CenterY());
        };
        little_Block(ED_BlockLayout* playout,int indx,int indy):indX(indx),indY(indy),playout(playout){
        }
    };
public:
    int row;
    int col;
    int W_Container(){
        return pContainer->width()-2*space;
    };
    int H_Container(){
        return pContainer->height()-2*space;
    };
    int W_Block_Clean(){
        return (W_Container()-(row-1)*spaceX)/row;
    };
    int H_Block_Clean(){
        return (H_Container()-(col-1)*spaceY)/col;
    };
    int space;
    int spaceX;
    int spaceY;

    little_Block* blocks[50][50];


    ED_BlockLayout(QWidget *father,int row, int col,int borad_space,int space_x,int space_y);

    QPoint pos2Ind(int posx,int posy)override;

    bool Occupied(int x,int y)override;

    //判断一个ED_Unit的左上角放置在该Block中是否合法
    bool OKForUnit(ED_Unit* aim,int xind,int yind);

    //根据一个Block索引获取对应的ED_Unit指针
    ED_Unit* ind2Unit(int xind,int yind)override;

    //判断放置一个ED_Unit是否合法

    void load_json(QJsonObject rootObject)override;
    QJsonObject to_json()override;


    // ED_Layout interface
public:
    // QSize areaSize() override;
    QPoint ind2CenterPoint(int x, int y) override;
    QSize ind2Size(int xind, int yind) override;
    QPoint defaultPutableInd(ED_Unit *aim) override;

    // ED_Layout interface
public:
    void updateAfterPut(ED_Unit *, int, int) override;
    void updateAfterRemove(ED_Unit *, int, int) override;

    // ED_Layout interface
public:
    QPoint clearPutableInd(ED_Unit *aim) override;

    // ED_Layout interface
public:
    void updateBeforePut(ED_Unit *, int, int) override;


    // ED_Layout interface
public:
    QPoint ind2Pos(int xind, int yind) override;
};

#endif // ED_BLOCKLAYOUT_H
