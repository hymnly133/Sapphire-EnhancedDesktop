#include "sdock.h"
#include "slinearlayout.h"
#include"sunit.h"
#include "qdebug.h"
#include"QPainterPath"
#include"QPainter"
#include"QJsonObject"

#define SET_ANCTION(NAME,TEXT,FUCTION)\
QAction *NAME = new QAction(#TEXT);\
    myMenu->addAction(NAME);\
    connect(NAME, &QAction::triggered, this, [=]()FUCTION);



SDock::SDock(SLayout *dis, int outSizeX, int outSizeY): SContainer(dis,outSizeX,outSizeY)
{
    alwaysShow = true;
    setMainColor(GetWindowsThemeColor());
    inside = new SLinearLayout(this);
    SET_ANCTION(accc,"ED_Update",{
        endUpdate();
    })
}

void SDock::paintEvent(QPaintEvent *event){
    // paintRect(this,QColor(0,0,155,aim_Alpha));


    QPainter painter(this);
    QLinearGradient linearGradient(QPoint(0,0),QPoint(width(),0));

    auto tem = displayColor_Alphaed();

    int count=0;
    SLinearLayout* inside_ = (SLinearLayout*) inside;
    for(int i=0;i<inside_->num();i++){
        // for(int i =0;i<10;i++){
        //     if(inside_->blocks[i]==nullptr) qDebug()<<i<<"No";
        //     else qDebug()<<i<<"Yes";
        // }
        count++;
        auto temm = inside->ind2Unit(i,0)->displayColor();
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


}

void SDock::endUpdate()
{
    SContainer::endUpdate();
    qDebug()<<"ed_update called";
    inside->UpdateContentPositon(false);
}
