#include "ed_unit.h"
#include "mainwindow.h"
#include "qaction.h"
#include "qapplication.h"
#include "qdebug.h"
#include "qevent.h"
#include"SysFunctions.h"
#include"ed_blockcontainer.h"
#include "qgraphicseffect.h"
#include "qtimer.h"
#include "screenfunc.h"
#include "style.h"
#include"QPropertyAnimation"
#include <cmath>
#include "QBitmap"



void ED_Unit::edmove(QPoint dis){
    qDebug()<<"ED_MOVE from"<<dis;
    QPoint red = dis;
    if(parentWidget()!=layout->pContainer){
        red = parentWidget()->mapFrom(pmw,layout->pContainer->mapTo(pmw,dis));
    }
    qDebug()<<"redirected::"<<red<<"Global"<<parentWidget()->mapTo(pmw,red);
    move(red);
}

QPoint ED_Unit::edpos()
{
    // if(parentWidget()!=layout->pContainer){
    //     layout->pContainer->mapFrom(pmw,parentWidget()->mapTo(pmw,pos()));
    // }
    // else{
    //     return pos();
    // }
    return layout->pContainer->mapFrom(pmw,mapTo(pmw,QPoint(0,0)));
}

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

    colorAlpha = aim_colorAlpha();

    focusAnimations = new QParallelAnimationGroup(this);
    positionAnimations = new QParallelAnimationGroup(this);
    longFocusAnimations = new QParallelAnimationGroup(this);

    SET_ANIMATION(alphaAnimation,colorAlpha,focusAnimations,focus_animation_time);

    SET_ANIMATION(scaleFixAnimation,scaleFix,focusAnimations,focus_animation_time);

    SET_ANIMATION(padRatioAnimation,nowPadRatio,focusAnimations,focus_animation_time);

    SET_ANIMATION(sizeAnimation,nowSize,positionAnimations,position_animation_time);

    SET_ANIMATION(posAnimation,nowPos,positionAnimations,position_animation_time);

    longFocusTimer = new QTimer(this);
    connect(longFocusTimer, SIGNAL(timeout()), this, SLOT(longFocusTimeoutSlot()));

    // setMouseTracking(true);
    connect(this,&ED_Unit::colorAlpha_changed,this,[=](int val){
        whenFocusAnimationChange();
    });

    connect(this,&ED_Unit::scale_changed,this,[=](double val){
        whenScaleChange(val*scaleFix);
    });

    connect(this,&ED_Unit::scaleFix_changed,this,[=](double val){
        whenFocusAnimationChange();
    });

    connect(this,&ED_Unit::nowPos_changed,this,[=](QPoint val){
        edmove(val);
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

#define SET_ANCTION(NAME,TEXT,FUCTION)\
QAction *NAME = new QAction(#TEXT);\
    myMenu->addAction(NAME);\
    connect(NAME, &QAction::triggered, this, [=]()FUCTION);

void ED_Unit::setupMenu()
{
    myMenu = new QMenu(this);

    SET_ANCTION(act1,加宽,{
        setBlockSize(sizeX+1,sizeY);
    });

    SET_ANCTION(act2,加高,{
        setBlockSize(sizeX,sizeY+1);
    });

    SET_ANCTION(act3,减宽,{
        if(sizeX>=2)
            setBlockSize(sizeX-1,sizeY);
    });

    SET_ANCTION(act4,减高,{
        if(sizeY>=2)
            setBlockSize(sizeX,sizeY-1);
    });

    SET_ANCTION(act5,切换复杂度,{
        changeSimpleMode();
    });

    SET_ANCTION(act6,切换始终显示,{
        setAlwaysShow(!alwaysShow);
    });


    SET_ANCTION(act7,删除,{
        removeFromLayout();
        deleteLater();
    });


}

void ED_Unit::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        grabMouse();
        single_click_action();
        premove = true;
        relativeP = event->pos();
    }
    event->accept();
    qDebug()<<objectName()<<"press"<<event->pos()<<event->globalPos()<<mapTo(pmw,event->pos());
}

void ED_Unit::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<<objectName()<<"Release"<<event->pos()<<event->globalPos()<<mapTo(pmw,event->pos());
    releaseMouse();
    mouse_release_action();
    if(moving){
        //首先检查是否拖到文件夹
        ED_Layout* mwlayout = pmw->inside;
        auto pp = QPoint(pos().x()+width()/2,pos().y()+height()/2);
        qDebug()<<"Detecting Pos:"<<pp;
        QPoint point = mwlayout->pos2Ind(pp);
        qDebug()<<"Find Ins:"<<point;

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
    event->accept();
    if (moving)
    {

        QPoint pmwPos = mapTo(pmw,event->pos());
        move(pmwPos-relativeP);
        QPoint ind = pmw->inside->ED_Layout::pos2Ind(pmwPos);
        ED_Unit* aim = pmw->inside->ED_Layout::ind2Unit(ind);
        if(aim!=nullptr){
            aim->setPreLongFocus(true);
        }
        foreach (ED_Unit* tem, *(pmw->inside->contents)) {
            if(tem!=aim &&tem->preLongFocus){
                tem->setPreLongFocus(false);
            }
        }
        update();

        tryToSwitch(event);
    }
    else if(premove){
        auto tem = event->pos();
        int dis =sqrt ((tem.x()-relativeP.x())*(tem.x()-relativeP.x())+(tem.y()-relativeP.y())*(tem.y()-relativeP.y()));
        if(dis>=2){
            whenDragedOut(event);
        }
    }
}

void ED_Unit::enterEvent(QEvent *event){
    onmouse = true  ;
    raise();
    mouse_enter_action();
    updataFocusAnimation();
    event->accept();
    if(layout!=nullptr)
        if(!layout->isMain){
            layout->pContainer->update();
        }
    setPreLongFocus(true);

    update();
}

void ED_Unit::leaveEvent(QEvent *event){
    onmouse = false  ;
    setPreLongFocus(false);
    // qDebug()<<type;
    mouse_leave_action();
    updataFocusAnimation();
    if(layout!=nullptr)
        if(!(layout->isMain)){
            layout->pContainer->update();
        }

}

void ED_Unit::updateInLayout(bool animated)
{
    //当Layout改变大小等调用（不包含初次放置）
    if(animated)
        moveto(MyPos_Centual(),MySize());
    else{
        move(MyPos_Centual());
        setFixedSize(MySize());
    }
}

void ED_Unit::moveto(QPoint pos, QSize size)
{
    //传入相对于Layou-pContainer的坐标

    aim_pos = pos;
    aim_size = size;

    updatePositionAnimation();
    // updataFocusAnimation();
}

void ED_Unit::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    rs->updateDisplay();
    afterResize(event);
}

void ED_Unit::contextMenuEvent(QContextMenuEvent *event)
{
    if(event->modifiers() == Qt::ShiftModifier)
        onShiftContextMenu(event);
    else
        onContextMenu(event);
}

bool ED_Unit::setBlockSize(int w,int h){
    if(w<=0)return false;
    if(h<=0)return false;
    ED_Layout* tem_layout;
    if(layout!=nullptr){
        ED_Unit temu(pmw,w,h);
        tem_layout = layout;
        temu.setParent(pmw);
        temu.setPMW(pmw);
        bool res = false;
        temu.move(mapTo(pmw,QPoint(0,0)));
        removeFromLayout();
        if(tem_layout->OKForClearPut(&temu)){
            sizeX = w;
            sizeY = h;
            res = true;
        }
        tem_layout->clearPut(this,true);
        return res;
    }
    else{
        sizeX = w;
        sizeY = h;
        return true;
    }
}

void ED_Unit::onContextMenu(QContextMenuEvent *event)
{
    myMenu->exec(event->globalPos());
}

void ED_Unit::onShiftContextMenu(QContextMenuEvent *event)
{

}

void ED_Unit::setSimpleMode(bool val){
    simpleMode = val;
    whenSimpleModeChange(val);
}

void ED_Unit::setLongFocus(bool val)
{
    qDebug()<<"Long Focus"<<val;
    onLongFocus = val;
    updateLongFocusAnimation();
}

void ED_Unit::setPreLongFocus(bool val)
{
    qDebug()<<"pre set"<<val<<"now focus"<<onLongFocus<<"now pre"<<preLongFocus;

    if(val == onLongFocus){
        longFocusTimer->stop();
    }
    else{
        if(val == preLongFocus)return;
        else{

            longFocusTimer->stop();
            preLongFocus = val;
            if(val)
            longFocusTimer->start(long_focus_in_delta_time);
            else
            longFocusTimer->start(long_focus_out_delta_time);
        }

    }
    preLongFocus = val;
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

void ED_Unit::setPMW(MainWindow *pmw)
{
    this->pmw = pmw;
}

void ED_Unit::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    paintSide(this,QColor("green"));
    paintRect(this,mainColor_Alphaed());
    paintLight(this,mainColor);
}

void ED_Unit::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() ==Qt::ShiftModifier){
        int tem = event->angleDelta().y();
        qDebug()<<event->angleDelta();
        if(tem>0){
            if(!setBlockSize(sizeX+1,sizeY+1)){
                if(!setBlockSize(sizeX,sizeY+1)){
                    setBlockSize(sizeX+1,sizeY);
                }
            }
        }
        else{
            if(!setBlockSize(sizeX-1,sizeY-1)){
                if(!setBlockSize(sizeX,sizeY-1)){
                    setBlockSize(sizeX-1,sizeY);
                }
            }
        }
        event->accept();
    }
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

void ED_Unit::tryToSwitch(QMouseEvent *event)
{
    if(screenNum>1){
        //多屏切换
        qDebug()<<"try to scan for switch pmw";
        foreach(auto pmw,pmws){
            if(pmw->geometry().contains(cursor().pos()) && pmw!=this->pmw){
                qDebug()<<"ScreenChange! "<<pmw->objectName()<<"Should Be The Aim";

                auto globalPos = mapToGlobal(pos());
                auto aimPos = pmw->mapFromGlobal(globalPos);
                releaseMouse();
                setParent(pmw);
                setPMW(pmw);
                move(aimPos);
                setVisible(true);
                setEnabled(true);
                pmw->setFocus();
                qDebug()<<pmw->objectName()<<"Acitive:"<<pmw->isActiveWindow();
                pmw->raise();
                raise();
                pls->raise();
                setFocus();
                grabMouse();

            }
        }
    }
}

void ED_Unit::updateLongFocusAnimation()
{

}

void ED_Unit::updataFocusAnimation()
{
    focusAnimations->stop();

    alphaAnimation->setStartValue(colorAlpha);
    alphaAnimation->setEndValue(aim_colorAlpha());

    padRatioAnimation->setStartValue(nowPadRatio);
    padRatioAnimation->setEndValue(aim_padRatio());

    scaleFixAnimation->setStartValue(scaleFix);
    scaleFixAnimation->setEndValue(aim_scaleFix());

    focusAnimations->start();
}

void ED_Unit::updatePositionAnimation()
{
    positionAnimations->stop();
    posAnimation->setStartValue(edpos());
    posAnimation->setEndValue(aim_pos);

    sizeAnimation->setStartValue(size());
    sizeAnimation->setEndValue(aim_size);
    positionAnimations->start();
}

void ED_Unit::whenDragedOut(QMouseEvent *event)
{
    setPreLongFocus(false);
    QPoint usedp = mapTo(pmw,QPoint(0,0));
    positionAnimations->stop();
    if(!layout->isMain)layout->pContainer->update();
    if(layout)
        removeFromLayout();
    move(usedp);
    pMovingUnit = this;
    moving = true;

}

void ED_Unit::preSetInLayout(bool animated)
{
    raise();
    pmw->setFrozen(false);
    if(animated){
        moveto(MyPos_Centual(),MySize());
        connect(positionAnimations,&QParallelAnimationGroup::finished,this,&::ED_Unit::setInLayoutAniSlot,Qt::UniqueConnection);
        connect(positionAnimations,&QParallelAnimationGroup::currentLoopChanged,this,[=](){
            qDebug()<<"CurrentLoop Changed";
            disconnect(positionAnimations,SIGNAL(finished()), 0, 0);
        });
    }
    else{
        move(layout->pContainer->mapTo(pmw,MyPos_Centual()));
        setFixedSize(MySize());
        setInLayout(animated);
    }

}

void ED_Unit::setInLayoutAniSlot()
{
    qDebug()<<"anilayout called";
    setInLayout(true);
}

void ED_Unit::longFocusTimeoutSlot()
{
    qDebug()<<"long focus timer out";
    longFocusTimer->stop();

    setLongFocus(preLongFocus);

}

void ED_Unit::setInLayout(bool animated)
{
    QPoint tem = edpos();
    // qDebug()<<tem<<dis;
    nowPadRatio = aim_padRatio();
    qDebug()<<"before"<<edpos();
    setParent(layout->pContainer);
    qDebug()<<"after"<<edpos();

    edmove(tem);
    setVisible(true);
    if(!layout->isMain)
    parentWidget()->update();
    update();

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

void ED_Unit::whenLongFocusAnimationChange()
{

}

void ED_Unit::whenFocusAnimationChange()
{
    whenScaleChange(scale*scaleFix);
    update();
}
