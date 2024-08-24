#ifndef SFLOWLAYOUT_H
#define SFLOWLAYOUT_H

#include "qmath.h"
#include "qpropertyanimation.h"
#include "qscrollarea.h"
#include "slayout.h"

class SFlowlayout : public SLayout
{
    Q_OBJECT
    Q_PROPERTY(int nowScrollValue MEMBER nowScrollValue NOTIFY nowScollValue_Changed)
public:
    struct littleBlock {
        int indX;
        int indY;
        int ind = -1;
        SFlowlayout* playout = nullptr;
        SUnit* content = nullptr;
        littleBlock(int x = -1, int y = -1, SFlowlayout* playout = nullptr): indX(x), indY(y), playout(playout) {};

        void remove()
        {
            if(playout->unit2block.contains(content)) {
                playout->unit2block.remove(content);
            }
            content = nullptr;
            ind = -1;
        }
        void insert(SUnit* unit, int ind)
        {
            if(!playout->unit2block.contains(unit)) {
                playout->unit2block.insert(unit, this);
                qDebug() << "inserted";
            }
            content = unit;
            this->ind = ind;
        }
        bool occupied()
        {
            return content != nullptr;
        }
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
    };
    int putableMap[10][50] = {};
    //采用(x,0)作为序号
    SFlowlayout(SLayoutContainer *father = nullptr, int maxRow = 5, double boradXR = 0.05, double boradYR = 0.05, double spaceXR = 0.1, double spaceYR = 0.1);
    QWidget* putWidget;

    int maxRow;
    QList<SUnit*> list;
    QMap<SUnit*, littleBlock*> unit2block;
    QPropertyAnimation* scrollValueAnimation;
    littleBlock blocks[10][50];
    void refresh();
    int row();

    int col();

    int nowScrollValue = 0;
    int aimScroolValue = 0;

    //当前情况（静态）是否有空位放aim
    bool okForUnit(SUnit* aim, int x, int y);

    void putAt(SUnit*aim, int x, int y, int ind);

    int occupiedSize();

    void printPutableMap();

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

    double W_Block_Clean();
    double H_Block_Clean();
    int W_Container() override;
    int H_Container() override;


    //对应的单个像素值
    double boradXPix();
    double boradYPix();
    double spaceXPix();
    double spaceYPix();

    // SLayout interface
public:
    // QPoint pos2Ind(int posx, int posy) override;
    QSize unit2Size(SUnit* aim) override;
    QPoint unit2Pos(SUnit* aim) override;
    bool Occupied(int x, int y) override;
    QPoint defaultPutableInd(SUnit *aim) override;
    QPoint clearPutableInd(SUnit *aim) override;
    SUnit *ind2Unit(int xind, int yind) override;



    // SLayout interface
public:
    void updateBeforePut(SUnit *, int, int) override;
    void updateBeforePutAnimation(SUnit *, int, int) override;
    void updateAfterPut(SUnit *, int, int) override;
    // void updateBeforeRemove(SUnit *, int, int) override;
    void updateAfterRemove(SUnit *, int, int) override;

    void scroll(int delta);
    void scrollTo(int to);
    void updateScrollAnimation();


    // SLayout interface
public:
    void afterResize() override;

    // SLayout interface
public:
    void UpdateContentPositon(bool animated = true) override;

public:
signals:
    void nowScollValue_Changed(int value);

public slots:
    void whenScrollAnimationUpdate();

};

inline int SFlowlayout::row()
{
    int res = 2 ;
    res = int(qSqrt(occupiedSize()));
    if(res * res != occupiedSize()) {
        res += 1;
    }
    return  qBound(2, res, maxRow);
}

inline int SFlowlayout::col()
{
    int res = occupiedSize() / row() + 1;

    if(occupiedSize() % row() == 0) {
        res -= 1;
    }
    res = qMax(res, row());

    return qBound(2, res, 999);
}

inline double SFlowlayout::W_Block_Clean()
{

    return W_Container() / (row() * (1 + spaceXR) - spaceXR + boradXR * 2);
}

inline double SFlowlayout::H_Block_Clean()
{
    int x = H_Container();
    int y = qMin(col(), maxRow);
    return H_Container() / (y * (1 + spaceYR) - spaceYR + boradYR * 2);
}

inline double SFlowlayout::boradXPix()
{
    return W_Block_Clean() * boradXR;
}

inline double SFlowlayout::boradYPix()
{
    return H_Block_Clean() * boradYR;
}

inline double SFlowlayout::spaceXPix()
{
    return W_Block_Clean() * spaceXR;
}

inline double SFlowlayout::spaceYPix()
{
    return H_Block_Clean() * spaceYR;
}

#endif // SFLOWLAYOUT_H
