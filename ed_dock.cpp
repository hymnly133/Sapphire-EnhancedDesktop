#include "ed_dock.h"
#include"ed_unit.h"
#include "qdebug.h"
#include"QPainterPath"
#include"QPainter"
#include"QJsonObject"
ED_Dock::ED_Dock(QWidget *parent,int outSizeX,int outSizeY,int inSize)
    : ED_Container(parent,outSizeX,outSizeY,inSize,1,2,80,10)
{
    alwaysShow = true;
    setMainColor(QColor(79,98,124));
}

void ED_Dock::InplaceAUnit(ED_Unit* aim){
    aim->setBlockSize(1,1);
    edlayout->InplaceAUnit(aim);
}

bool ED_Dock::OKforput(ED_Unit* aim){
    ED_Unit tem(nullptr,1,1);
    qDebug()<<edlayout->col;
    return edlayout->OKforput(&tem);
}

void ED_Dock::paintEvent(QPaintEvent *event){
    // paintRect(this,QColor(0,0,155,aim_Alpha));


    QPainter painter(this);
    QLinearGradient linearGradient(QPoint(0,0),QPoint(width(),0));

    auto tem = mainColor_Alphaed();

    int count=0;
    for(int i=0;i<row;i++){
        if(edlayout->Occupied(i,0)){
            count++;
            auto temm = edlayout->getUnitFromBlock(i,0)->mainColor;
            float ratio = 1.0*edlayout->blocks[i][0]->CenterX()/width();
            // qDebug()<<"found" << i<<ratio<<edlayout->blocks[i][0]->CenterX()<<width();
            temm.setAlpha(colorAlpha);
            linearGradient.setColorAt(ratio, temm);
        }
    }


    if(count){
        linearGradient.setColorAt(0, QColor(255,255,255,0));
        linearGradient.setColorAt(1, QColor(255,255,255,0));
    }
    else{
        linearGradient.setColorAt(0, tem);
        linearGradient.setColorAt(1, tem);
    }


    // 使用线性渐变创建一个画刷，用来填充

    QBrush brush(linearGradient);
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    // x, y, w, h
    painter.drawRect(rect());

    // paintLight(this,mainColor);

}
