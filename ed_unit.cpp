#include "ed_unit.h"
#include "mainwindow.h"
#include "qaction.h"
#include "qdebug.h"
#include "qevent.h"
#include"SysFunctions.h"
#include"ed_layout.h"
#include"ed_container.h"
#include "qgraphicseffect.h"
#include "qpainter.h"
#include "style.h"
#include"QPropertyAnimation"
#include <cmath>
#include "QBitmap"

ED_Unit::ED_Unit(): ED_Unit(nullptr,1,1){

};

ED_Unit::ED_Unit(QWidget *parent,int sizex,int sizey): QWidget{parent}
{
    sizeX = sizex;
    sizeY = sizey;
    shadow_main_color = new QGraphicsDropShadowEffect;
    shadow_main_color->setBlurRadius(unit_shadow_blur_radius);   //模糊半径
    shadow_main_color->setOffset(0);      //偏移量
    shadow_main_color->setEnabled(true);
    setGraphicsEffect(shadow_main_color);

    setMainColor(QColor(88,119,144));

    colorAlpha = aim_alpha();

    alphaAnimation = new QPropertyAnimation(this,"colorAlpha");
    scaleFixAnimation = new QPropertyAnimation(this,"scaleFix");

    connect(this,&ED_Unit::alpha_changed,this,[=](int val){
        update();
    });

    connect(this,&ED_Unit::scale_changed,this,[=](double val){
        whenScaleChange(val*scaleFix);
    });

    connect(this,&ED_Unit::scaleFix_changed,this,[=](double val){
        whenScaleChange(val*scale);
    });

    connect(this,&ED_Unit::mainColor_changed,this,[=](QColor val){
        whenMainColorChange(val);
    });

    setupMenu();
}


void ED_Unit::single_click_action(){
    //最终单击执行
}


void ED_Unit::double_click_action(){
    //最终双击执行
}

void ED_Unit::mouse_enter_action(){

}
void ED_Unit::mouse_leave_action(){
}

void ED_Unit::mouse_move_action(){
    //最终移动执行


}

void ED_Unit::mouse_release_action(){
    //最终释放执行

}

void ED_Unit::removeFromLayout(){
    edlayout->RemoveAUnit(this);
}

void ED_Unit::setupMenu()
{

    setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction* act1  = new QAction("加宽");
    this->addAction(act1);
    connect(act1, &QAction::triggered, this, [=]()
            {
                setBlockSize(sizeX+1,sizeY);
            });

    QAction* act3  = new QAction("减宽");
    this->addAction(act3);
    connect(act3, &QAction::triggered, this, [=]()
            {
                if(sizeX>=2)
                    setBlockSize(sizeX-1,sizeY);
            });

    QAction* act2  = new QAction("加高");
    this->addAction(act2);
    connect(act2, &QAction::triggered, this, [=]()
            {
                setBlockSize(sizeX,sizeY+1);
            });



    QAction* act4  = new QAction("减高");
    this->addAction(act4);
    connect(act4, &QAction::triggered, this, [=]()
            {
                if(sizeY>=2)
                    setBlockSize(sizeX,sizeY-1);
            });



    QAction* act5  = new QAction("切换复杂度");
    this->addAction(act5);
    connect(act5, &QAction::triggered, this, [=]()
            {
                changeSimpleMode();
            });


    QAction* act6  = new QAction("删除");
    this->addAction(act6);
    connect(act6, &QAction::triggered, this, [=]()
            {
                removeFromLayout();
                deleteLater();
            });
}

void ED_Unit::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<to_json();
    if(event->button() == Qt::LeftButton){
        grabMouse();
        single_click_action();
        premove = true;
        relativeP = event->pos();
    }

}

void ED_Unit::mouseReleaseEvent(QMouseEvent *event)
{

    releaseMouse();
    mouse_release_action();
    if(moving){
        //首先检查是否拖到文件夹
        ED_Layout* mwlayout = pmw->edlayout;
        QPoint point = mwlayout->NearestBlockInd(pos().x()+width()/2,pos().y()+height()/2);
        if(!(point.x()<0||point.y()<0||point.x()>=mwlayout->row||point.y()>=mwlayout->col))
        {
            if(mwlayout->Occupied(point)){
                if(mwlayout->getUnitFromBlock(point)->type == ED_Unit::Container){
                    qDebug()<<"Container";
                    ED_Container*  c = (ED_Container*)mwlayout->getUnitFromBlock(point);
                    if(c->OKforput(this)){
                        c->InplaceAUnit(this);
                        c->raise();
                        moving = false;
                        return;
                    }
                }
            }
        }
        // 放置
        pMovingUnit = nullptr;
        mwlayout->InplaceAUnit(this);
        moving = false;
    }
    premove = false;
    // repaintAround(this);

}

void ED_Unit::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    double_click_action();
}

void ED_Unit::mouseMoveEvent(QMouseEvent *event)
{
    mouse_move_action();

    if (moving)
    {
        move(cursor().pos()-relativeP);
    }
    else if(premove){
        auto tem = mapFromGlobal(cursor().pos());
        int dis =sqrt ((tem.x()-relativeP.x())*(tem.x()-relativeP.x())+(tem.y()-relativeP.y())*(tem.y()-relativeP.y()));
        qDebug()<<(dis)<<event->pos()<<relativeP;
        if(dis>=2){
            QPoint usedp = mapToGlobal(QPoint(0,0));
            if(edlayout)
                removeFromLayout();
            move(usedp);
            pMovingUnit = this;
            moving = true;
        }
    }
}

void ED_Unit::updata_animation(){
    alphaAnimation->stop();
    alphaAnimation->setStartValue(colorAlpha);
    alphaAnimation->setEndValue(aim_alpha());
    alphaAnimation->setDuration(100);
    alphaAnimation->start();


    scaleFixAnimation->stop();
    scaleFixAnimation->setStartValue(scaleFix);
    scaleFixAnimation->setEndValue(aim_scaleFix());
    scaleFixAnimation->setDuration(100);
    scaleFixAnimation->start();
}

void ED_Unit::enterEvent(QEvent *event){
    onmouse = true  ;
    mouse_enter_action();
    updata_animation();
    event->accept();

}
void ED_Unit::leaveEvent(QEvent *event){
    onmouse = false  ;
    mouse_leave_action();
    updata_animation();

}

void ED_Unit::setBlockSize(int w,int h){
    ED_Layout* tem = nullptr;
    if(edlayout){
        tem = edlayout;
        removeFromLayout();
        ED_Unit temu(nullptr,w,h);
        if(tem->OKforput(&temu)){
            sizeX = w;
            sizeY = h;
        }
        tem->InplaceAUnit(this);
    }
    else{
        sizeX = w;
        sizeY = h;
    }
}

void ED_Unit::update_after_resize(){
}

void ED_Unit::setSimpleMode(bool val){
    simpleMode = val;
    whenSimpleModeChange(val);
}

void ED_Unit::changeSimpleMode(){
    setSimpleMode(!simpleMode);
}

void ED_Unit::setMainColor(QColor val)
{
    mainColor = val;
    auto tem = val;
    tem.setAlpha(unit_shadow_alpha);
    shadow_main_color->setColor(tem);
    shadow_main_color->setEnabled(true);
    shadow_main_color->update();

    if (val.red()*.299+val.green()*.587+val.blue()*.114<80){
        dark = true;
    }
    else{
        dark = false;
    }
    emit mainColor_changed(val);
}

void ED_Unit::whenSimpleModeChange(bool val){}

void ED_Unit::whenScaleChange(double val){}

void ED_Unit::setScale(double val){
    scale = val;
    emit scale_changed(val);
}

void ED_Unit::setScaleFix(double val)
{
    scaleFix = val;
    emit scaleFix_changed(val);
}

void ED_Unit::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    paintSide(this,QColor("green"));
    paintRect(this,mainColor_Alphaed());
    paintLight(this,mainColor);
}


void ED_Unit::ed_update(){

    QBitmap bmp(this->size());

    bmp.fill();

    QPainter p(&bmp);

    p.setPen(Qt::NoPen);

    p.setBrush(Qt::black);

    p.drawRoundedRect(bmp.rect(),unit_radius,unit_radius);

    setMask(bmp);

    update();
}

QColor ED_Unit::mainColor_Alphaed(){
    QColor tem = mainColor;
    tem.setAlpha(colorAlpha);
    return tem;
}

QJsonObject ED_Unit::to_json(){
    QJsonObject rootObject;
    rootObject.insert("Class",metaObject()->className());
    rootObject.insert("type",type);
    rootObject.insert("sizeX",sizeX);
    rootObject.insert("sizeY",sizeY);
    rootObject.insert("indX",indX);
    rootObject.insert("indY",indY);
    rootObject.insert("alwaysShow",alwaysShow);
    rootObject.insert("simpleMode",simpleMode);
    rootObject.insert("dark",deepColor);
    rootObject.insert("showRect",showRect);
    rootObject.insert("showLight",showLight);
    rootObject.insert("showSide",showSide);
    return rootObject;
}

void ED_Unit::load_json(QJsonObject rootObject)
{
    ED_Unit* tem = nullptr;
    type = (ED_Unit::ED_TYPE)rootObject.value("type").toInt();
    sizeX = rootObject.value("sizeX").toInt();
    sizeY = rootObject.value("sizeY").toInt();
    indX = rootObject.value("indX").toInt();
    indY = rootObject.value("indY").toInt();
    alwaysShow = rootObject.value("alwaysShow").toBool();
    simpleMode = rootObject.value("simpleMode").toBool();
    deepColor = rootObject.value("dark").toBool();
    showRect = rootObject.value("showRect").toBool();
    showLight = rootObject.value("showLight").toBool();
    showSide = rootObject.value("showSide").toBool();
    whenSimpleModeChange(simpleMode);
}

void ED_Unit::whenMainColorChange(QColor val){

}
