#ifndef SBLOCKLAYOUT_H
#define SBLOCKLAYOUT_H

#include "sunit.h"
#include "qwidget.h"
class SBlockLayout:public SLayout
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
        SBlockLayout* playout;
        SUnit* content = nullptr;
        int CenterX(){
            return posX() +w()/2;
        }
        int CenterY(){
            return posY() +h()/2;
        }
        QPoint CenterPoint(){
            return QPoint(CenterX(),CenterY());
        };
        little_Block(SBlockLayout* playout,int indx,int indy):indX(indx),indY(indy),playout(playout){
        }
    };
public:
    int row;
    int col;
    int W_Container(){
        if(useStandaloneRect){
            return standaloneRect.width()-2*space;
        }
        else{
            return pContainer->width()-2*space;
        }
    };
    int H_Container(){
        if(useStandaloneRect){
            return standaloneRect.height()-2*space;
        }
        else{
            return pContainer->height()-2*space;
        }
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


    SBlockLayout(QWidget *father,int row =10, int col=10,int borad_space=5,int space_x=5,int space_y=5);

    QPoint pos2Ind(int posx,int posy)override;

    bool Occupied(int x,int y)override;

    //判断一个ED_Unit的左上角放置在该Block中是否合法
    bool OKForUnit(SUnit* aim,int xind,int yind);

    //根据一个Block索引获取对应的ED_Unit指针
    SUnit* ind2Unit(int xind,int yind)override;

    //判断放置一个ED_Unit是否合法

    void load_json(QJsonObject rootObject)override;
    QJsonObject to_json()override;


    // ED_Layout interface
public:
    // QSize areaSize() override;
    QSize ind2Size(int xind, int yind) override;
    QPoint ind2Pos(int xind, int yind) override;
    QPoint defaultPutableInd(SUnit *aim) override;

    // ED_Layout interface
public:
    void updateAfterPut(SUnit *, int, int) override;
    void updateAfterRemove(SUnit *, int, int) override;

    // ED_Layout interface
public:
    QPoint clearPutableInd(SUnit *aim) override;

    // ED_Layout interface
public:
    void updateBeforePut(SUnit *, int, int) override;


    // ED_Layout interface
};

#endif // SBLOCKLAYOUT_H
