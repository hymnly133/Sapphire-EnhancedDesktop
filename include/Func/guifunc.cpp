#include "guifunc.h"
#include "global.h"
#include "qpainter.h"
#include "qpainterpath.h"
#include "screenfunc.h"
#include "sunit.h"

void paintRect(QWidget* aim,QColor color){
    bool another = true;

    if(aim->inherits("SUnit")) {
        color.setAlpha(color.alpha()*((SUnit*)aim)->nowPadRatio);
        another = ((SUnit*)aim)->showRect;
    }
    if(ShowRect&(another)){
        QPainter painter(aim);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(color);
        painter.drawRect(aim->rect());;
    }
}

void paintLight(QWidget* aim,QColor color){
    // qDebug()<<"Light color"<<color;
    bool another = true;
    bool choosen = false;
    int aim_alpha_start = light_alpha_start;
    int aim_alpha_end= light_alpha_end;
    if(aim->inherits("SUnit")) {
        aim_alpha_end*=((SUnit*)aim)->nowPadRatio;
        aim_alpha_start+=((SUnit*)aim)->nowPadRatio;
        // qDebug()<<color.alpha();
        another = ((SUnit*)aim)->showLight;
    }


    if(ShowLight&(another)){
        auto pos =aim->mapFromGlobal(aim->cursor().pos());
        QRadialGradient* radialGradient;
        if(enable_light_track&&choosen){
            radialGradient = new QRadialGradient(aim->width()/2 , aim->height()/2, rectLen(aim->width(),aim->height())/2,pos.x() ,pos.y());
        }
        else{
            radialGradient = new QRadialGradient(aim->width()/2 , aim->height()/2, rectLen(aim->width(),aim->height())/2,0, aim->height());
        }
        //创建了一个QRadialGradient对象实例，参数分别为中心坐标，半径长度和焦点坐标,如果需要对称那么中心坐标和焦点坐标要一致
        QPainter painter(aim);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        color.setAlpha(aim_alpha_start);
        if(aim->inherits("SUnit")) {
            color.setAlpha(color.alpha()*((SUnit*)aim)->nowPadRatio);
        }
        radialGradient->setColorAt(0,color);
        color.setAlpha(aim_alpha_end);
        if(aim->inherits("SUnit")) {
            color.setAlpha(color.alpha()*((SUnit*)aim)->nowPadRatio);
            // qDebug()<<color.alpha();
        }
        radialGradient->setColorAt(1.0,color);
        painter.setBrush(QBrush(*radialGradient));
        painter.drawRect(aim->rect());//在相应的坐标画出来
    }
}

void paintSide(QWidget* aim,QColor color){
    bool another = true;
    if(aim->inherits("SUnit")){

        another = ((SUnit*)aim)->showSide;
        // 恢复默认混合模式，绘制边框，如果没有则不用
        if(ShowSide&&another){
            QPainter painter(aim);
            QPainterPath path;
            //这里圆角区域需要根据dpi、size调整
            path.addRoundedRect(QRectF(aim->rect()).adjusted(1.5, 1.5, -1.5, -1.5), unit_radius, unit_radius);
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            painter.setPen(QPen(QColor(0xCA64EA), 1.0));
            painter.drawPath(path);
        }
    }
    else if(ShowSide){
        QPainter p(aim);
        p.setPen(color); //设置画笔记颜色
        p.drawRect(0, 0, aim->width() -1, aim->height() -1); //绘制边框
    }
}

QPixmap resizeToRect(QPixmap source){
    qDebug()<<"Resizing from"<<source.size();
    QImage image = source.toImage();
    qDebug()<<"image"<<image.size();
    int width = source.width();
    int height = source.height();


    int start = 0;
    bool find = false;
    for(start = 0;start<=width-2&&!find;start++){
        //横向扫描
        for(int j=start;j<=width-1;j++){
            QColor pix = image.pixelColor(QPoint(j,start));
            if(pix.alpha()!=0){
                find = true;
                break;
            }
        }
        //竖向扫描
        for(int j=start;j<=height-1;j++){
            QColor pix = image.pixelColor(QPoint(start,j));
            if(pix.alpha()!=0){
                find = true;
                break;
            }
        }
    }

    int end=qMax(width,height)-1;
    find = false;
    for(;end>=start &&!find;end--){
        //横向扫描
        for(int j=0;j<=end;j++){
            QColor pix = image.pixelColor(QPoint(j,end));
            if(pix.alpha()!=0){
                find = true;
                break;
            }
        }
        //竖向扫描
        for(int j=0;j<=end;j++){
            QColor pix = image.pixelColor(QPoint(end,j));
            if(pix.alpha()!=0){
                find = true;
                break;
            }
        }
    }

    int f_start = qBound(0,start-1,qMin(width-1,height-1));
    int f_end = qBound(0,end,qMin(width-1,height-1));
    QPixmap res = source.copy(f_start,f_start,f_end+2-f_start,f_end+2-f_start);
    // qDebug()<<"Start"<<start<<"End"<<end<<"Result Size"<<res.size();
    return res;
}


void repaintAround(QWidget* aim){
    auto tem = aim->geometry();
    auto rrect = QRect(tem.x()-100,tem.y()-100,tem.width()+200,tem.height()+200);
    pmws[screenInd(aim)]->repaint(rrect);
}
