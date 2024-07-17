#include "ed_dock.h"
#include "ed_linearlayout.h"
#include"ed_unit.h"
#include "qdebug.h"
#include"QPainterPath"
#include"QPainter"
#include"QJsonObject"
ED_Dock::ED_Dock(QWidget *parent,int outSizeX,int outSizeY)
    : ED_Container(parent,outSizeX,outSizeY)
{
    alwaysShow = true;
    setMainColor(QColor(79,98,124));
    inside = new ED_LinearLayout(this);
}


void ED_Dock::paintEvent(QPaintEvent *event){
    // paintRect(this,QColor(0,0,155,aim_Alpha));


    QPainter painter(this);
    QLinearGradient linearGradient(QPoint(0,0),QPoint(width(),0));

    auto tem = mainColor_Alphaed();

    int count=0;
    ED_LinearLayout* inside_ = (ED_LinearLayout*) inside;
    for(int i=0;i<inside_->num();i++){
        // for(int i =0;i<10;i++){
        //     if(inside_->blocks[i]==nullptr) qDebug()<<i<<"No";
        //     else qDebug()<<i<<"Yes";
        // }
        count++;
        auto temm = inside->ind2Unit(i,0)->mainColor;
        float ratio = 1.0*(inside->ind2CenterPoint(i,0).x())/width();
        temm.setAlpha(colorAlpha);
        linearGradient.setColorAt(ratio, temm);
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
