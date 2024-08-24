#ifndef SBLOCKLAYOUT_H
#define SBLOCKLAYOUT_H

#include "sunit.h"
#include "qwidget.h"
class SBlockLayout: public SLayout
{
    Q_OBJECT
    struct little_Block {
        int indX;
        int indY;
        double posX()
        {
            return playout->boradXPix() + indX*(playout->spaceXPix() + w());
        };
        double posY()
        {
            return playout->boradYPix() + indY*(playout->spaceYPix() + h());
        };
        double w()
        {
            return playout->W_Block_Clean();
        };
        double h()
        {
            return playout->H_Block_Clean();
        };
        bool occupied;
        SBlockLayout* playout;
        SUnit* content = nullptr;
        int CenterX()
        {
            return posX() + w() / 2;
        }
        int CenterY()
        {
            return posY() + h() / 2;
        }
        QPoint CenterPoint()
        {
            return QPoint(CenterX(), CenterY());
        };
        little_Block(SBlockLayout* playout, int indx, int indy): indX(indx), indY(indy), playout(playout)
        {
        }
    };
public:
    int row;
    int col;

    //表示占总Block_Clean长度的比率
    //如：spaceXR=0.1，表示2000像素长度中有200像素用于分割，若十行则是每行200/9
    double boradXR;
    double boradYR;
    double spaceXR;
    double spaceYR;

    //x行，spacexr，
    //w = WC*(row)+WC*spacexr*(row-1)+WC*boradxr*2
    //w = WC*(row+(row-1)*spacexr+boradxr*2)
    //w = WC*(row(1+spacexr)-spacexr+boradxr*2);

    double W_Block_Clean()
    {
        return W_Container() / (row*(1 + spaceXR) - spaceXR + boradXR * 2);
    };
    double H_Block_Clean()
    {
        return H_Container() / (col*(1 + spaceYR) - spaceYR + boradYR * 2);
    };

    //对应的单个像素值
    double boradXPix()
    {
        return W_Block_Clean() * boradXR;
    }
    double boradYPix()
    {
        return H_Block_Clean() * boradYR;
    }
    double spaceXPix()
    {
        return W_Block_Clean() * spaceXR;
    }
    double spaceYPix()
    {
        return H_Block_Clean() * spaceYR;
    }


    little_Block* blocks[50][50];


    SBlockLayout(SLayoutContainer *father = nullptr, int row = 10, int col = 10, double boradXR = 0.05, double boradYR = 0.05, double spaceXR = 0.1, double spaceYR = 0.1);

    // QPoint pos2Ind(int posx, int posy)override;

    bool Occupied(int x, int y)override;

    //判断一个ED_Unit的左上角放置在该Block中是否合法
    bool OKForUnit(SUnit* aim, int xind, int yind);

    //根据一个Block索引获取对应的ED_Unit指针
    SUnit* ind2Unit(int xind, int yind)override;

    //判断放置一个ED_Unit是否合法

    void load_json(QJsonObject rootObject)override;
    QJsonObject to_json()override;


    // ED_Layout interface
public:
    // QSize areaSize() override;
    QSize unit2Size(SUnit* aim) override;
    QPoint unit2Pos(SUnit* aim) override;
    QPoint defaultPutableInd(SUnit *aim) override;

    // ED_Layout interface
public:
    void updateAfterPut(SUnit *, int, int) override;
    void updateAfterRemove(SUnit *, int, int) override;

    void resize(int sizeX, int sizeY, double boradXR = 0.05, double boradYR = 0.05, double spaceXR = 0.1, double spaceYR = 0.1, bool animated = true);
    // ED_Layout interface
public:
    QPoint clearPutableInd(SUnit *aim) override;

    // ED_Layout interface
public:
    void updateBeforePut(SUnit *, int, int) override;
    void printOccupied();


    // ED_Layout interface
};

#endif // SBLOCKLAYOUT_H
