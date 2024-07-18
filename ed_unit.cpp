#include "ed_unit.h"
#include "mainwindow.h"
#include "qaction.h"
#include "qdebug.h"
#include "qevent.h"
#include"SysFunctions.h"
#include"ed_blockcontainer.h"
#include "qgraphicseffect.h"
#include "style.h"
#include"QPropertyAnimation"
#include <cmath>
#include "QBitmap"

ED_Unit::ED_Unit(): ED_Unit(nullptr,1,1){

};

#define SET_ANIMATION(ANIMATION,NAME,GROUP,TIME)\
ANIMATION = new QPropertyAnimation(this,#NAME);\
ANIMATION->setDuration(TIME);\
ANIMATION->setEasingCurve(QEasingCurve::InSine);\
GROUP->addAnimation(ANIMATION);


ED_Unit::ED_Unit(QWidget *parent,int sizex,int sizey): QWidget{parent}
{
    sizeX = sizex;
    sizeY = sizey;
    shadow_main_color = new QGraphicsDropShadowEffect;
    shadow_main_color->setBlurRadius(unit_shadow_blur_radius);   //模糊半径
    shadow_main_color->setOffset(0);      //偏移量
    shadow_main_color->setEnabled(true);
    setGraphicsEffect(shadow_main_color);

    setMainColor(GetWindowsThemeColor());

    colorAlpha = aim_alpha();

    focusAnimations = new QParallelAnimationGroup(this);
    positionAnimations = new QParallelAnimationGroup(this);

    SET_ANIMATION(alphaAnimation,colorAlpha,focusAnimations,100);

    SET_ANIMATION(scaleFixAnimation,scaleFix,focusAnimations,100);

    SET_ANIMATION(padRatioAnimation,nowPadRatio,focusAnimations,100);

    SET_ANIMATION(sizeAnimation,nowSize,positionAnimations,position_animation_time);

    SET_ANIMATION(posAnimation,nowPos,positionAnimations,position_animation_time);



    // setMouseTracking(true);

    connect(this,&ED_Unit::alpha_changed,this,[=](int val){
        update();
        // qDebug()<<nowPadRatio;
    });

    connect(this,&ED_Unit::scale_changed,this,[=](double val){
        whenScaleChange(val*scaleFix);
    });

    connect(this,&ED_Unit::scaleFix_changed,this,[=](double val){
        whenScaleChange(val*scale);
    });

    connect(this,&ED_Unit::nowPos_changed,this,[=](QPoint val){
        move(val);
    });

    connect(this,&ED_Unit::nowSize_changed,this,[=](QSize val){
        setFixedSize(val);
    });

    rs = new roundShower(this);
    rs->raise();
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
    layout->RemoveAUnit(this);
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

    QAction* act7  = new QAction("切换始终显示");
    this->addAction(act7);
    connect(act7, &QAction::triggered, this, [=]()
            {
        setAlwaysShow(!alwaysShow);
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
    event->accept();


}

void ED_Unit::mouseReleaseEvent(QMouseEvent *event)
{
    releaseMouse();
    mouse_release_action();
    if(moving){
        //首先检查是否拖到文件夹
        ED_Layout* mwlayout = pmw->inside;
        QPoint point = mwlayout->pos2Ind(pos().x()+width()/2,pos().y()+height()/2);

        if(mwlayout->Occupied(point)){
            if(mwlayout->ind2Unit(point)->type == ED_Unit::Container){
                qDebug()<<"Container";
                ED_BlockContainer*  c = (ED_BlockContainer*)mwlayout->ind2Unit(point);
                if(c->OKForClearPut(this)){
                    c->clearPut(this,true);
                    moving = false;
                    return;
                }
            }
        }
        // 放置
        pMovingUnit = nullptr;
        mwlayout->clearPut(this,true);
        moving = false;
    }
    premove = false;
    // repaintAround(this);
    event->accept();

}

void ED_Unit::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    double_click_action();
}

void ED_Unit::mouseMoveEvent(QMouseEvent *event)
{
    mouse_move_action();
    // update();

    if (moving)
    {
        move(cursor().pos()-relativeP);
    }
    else if(premove){
        auto tem = mapFromGlobal(cursor().pos());
        int dis =sqrt ((tem.x()-relativeP.x())*(tem.x()-relativeP.x())+(tem.y()-relativeP.y())*(tem.y()-relativeP.y()));
        // qDebug()<<(dis)<<event->pos()<<relativeP;
        if(dis>=2){
            QPoint usedp = mapToGlobal(QPoint(0,0));
            positionAnimations->stop();
            if(layout)
                removeFromLayout();
            move(usedp);
            pMovingUnit = this;
            moving = true;
        }
    }
    event->accept();
}

void ED_Unit::updateInLayout()
{
    //当Layout改变大小等调用（不包含初次放置）
    moveto(MyPos_Centual(),MySize());
}

void ED_Unit::moveto(QPoint pos, QSize size)
{
    aim_pos = pos;
    aim_size = (size);
    updatePositionAnimation();
    // updataFocusAnimation();
}

void ED_Unit::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    rs->updateDisplay();
    afterResize(event);
}

void ED_Unit::enterEvent(QEvent *event){
    onmouse = true  ;
    mouse_enter_action();
    updataFocusAnimation();
    event->accept();
    if(layout!=nullptr)
    if(!layout->isMain){
        layout->pContainer->update();
    }
    update();
}

void ED_Unit::leaveEvent(QEvent *event){
    onmouse = false  ;
    // qDebug()<<type;
    mouse_leave_action();
    updataFocusAnimation();
    if(layout!=nullptr)
    if(!(layout->isMain)){
        layout->pContainer->update();
    }
    premove = false;
    moving = false;
}

void ED_Unit::setBlockSize(int w,int h){
    ED_Layout* tem = nullptr;
    if(layout){
        tem = layout;
        removeFromLayout();
        ED_Unit temu(nullptr,w,h);
        if(tem->OKForClearPut(&temu)){
            sizeX = w;
            sizeY = h;
        }
        tem->clearPut(this,true);
    }
    else{
        sizeX = w;
        sizeY = h;
    }
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

void ED_Unit::setAlwaysShow(bool val)
{
    alwaysShow = val;
    if(layout!=nullptr){
        if(val){
            auto s = std::find(layout->contents_Show->begin(), layout->contents_Show->end(), this);//第一个参数是array的起始地址，第二个参数是array的结束地址，第三个参数是需要查找的值
            if (s != layout->contents_Show->end())//如果找到，就输出这个元素
            {
                layout->contents_Show->erase(s);
            }
            else//如果没找到
            {
                qDebug() << "not find!";
            }

            layout->contents_AlwaysShow->push_back(this);
        }
        else{

            auto s = std::find(layout->contents_AlwaysShow->begin(), layout->contents_AlwaysShow->end(), this);//第一个参数是array的起始地址，第二个参数是array的结束地址，第三个参数是需要查找的值
            if (s != layout->contents_AlwaysShow->end())//如果找到，就输出这个元素
            {
                layout->contents_AlwaysShow->erase(s);
            }
            else//如果没找到
            {
                qDebug() << "not find!";
            }

            layout->contents_Show->push_back(this);
        }
    }
}

void ED_Unit::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    paintSide(this,QColor("green"));
    paintRect(this,mainColor_Alphaed());
    paintLight(this,mainColor);
}

void ED_Unit::ed_update(){
    rs->updateDisplay();
    rs->raise();
    update();
}

QColor ED_Unit::mainColor_Alphaed(){
    QColor tem = mainColor;
    tem.setAlpha(colorAlpha);
    return tem;
}

void ED_Unit::updataFocusAnimation()
{
    focusAnimations->stop();

    alphaAnimation->setStartValue(colorAlpha);
    alphaAnimation->setEndValue(aim_alpha());

    padRatioAnimation->setStartValue(nowPadRatio);
    padRatioAnimation->setEndValue(aim_padRatio());

    scaleFixAnimation->setStartValue(scaleFix);
    scaleFixAnimation->setEndValue(aim_scaleFix());

    focusAnimations->start();
}

void ED_Unit::updatePositionAnimation()
{
    positionAnimations->stop();
    posAnimation->setStartValue(pos());
    posAnimation->setEndValue(aim_pos);

    sizeAnimation->setStartValue(size());
    sizeAnimation->setEndValue(aim_size);
    positionAnimations->start();
}

void ED_Unit::preSetInLayout(bool animated)
{
    raise();

    if(animated){
        moveto(layout->pContainer->mapToGlobal(MyPos_Centual()),MySize());
    }
    else{
        move(layout->pContainer->mapToGlobal(MyPos_Centual()));
        setFixedSize(MySize());
    }

}

void ED_Unit::setInLayout(bool animated)
{
    QPoint tem = mapToGlobal(QPoint(0,0));
    QPoint dis = layout->pContainer->mapFromGlobal(tem);
    // qDebug()<<tem<<dis;
    nowPadRatio = aim_padRatio();
    setParent(layout->pContainer);
    setVisible(true);
    raise();
    move(dis);
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

void ED_Unit::afterResize(QResizeEvent* event)
{

}