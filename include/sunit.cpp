#include "sunit.h"
#include "mainwindow.h"
#include "qaction.h"
#include "qapplication.h"
#include "qdebug.h"
#include "qevent.h"
#include"SysFunctions.h"
#include"sblockcontainer.h"
#include "qgraphicseffect.h"
#include "qtimer.h"
#include "screenfunc.h"
#include "style.h"
#include "sfile.h"
#include"QPropertyAnimation"
#include"stooltip.h"
#include <cmath>
#include "QBitmap"

#define SET_ANCTION(NAME,TEXT,FUCTION)\
QAction *NAME = new QAction(#TEXT);\
    myMenu->addAction(NAME);\
    connect(NAME, &QAction::triggered, this, [=]()FUCTION);

#define SET_ANIMATION(ANIMATION,NAME,GROUP,TIME)\
ANIMATION = new QPropertyAnimation(this,#NAME);\
    ANIMATION->setDuration(TIME);\
    ANIMATION->setEasingCurve(QEasingCurve::InSine);\
    GROUP->addAnimation(ANIMATION);

void SUnit::edmove(QPoint dis){
    QPoint red = dis;
    if(layout!=nullptr)
    if(parentWidget()!=layout->pContainer){
            red = parentWidget()->mapFromGlobal(layout->pContainer->mapToGlobal(dis));
    }
    move(red);
}

QPoint SUnit::edpos()
{
    return layout->pContainer->mapFromGlobal(mapToGlobal(QPoint(0,0)));
}



SUnit::SUnit(SLayout *dis, int sizex, int sizey):QWidget(nullptr)
{
    setObjectName(metaObject()->className());
    sizeX = sizex;
    sizeY = sizey;

    if(dis!= nullptr)
        layout = dis;
    if(dis!= nullptr)
        setPMW(dis->pmw);


    shadow_main_color = new QGraphicsDropShadowEffect;
    shadow_main_color->setBlurRadius(unit_shadow_blur_radius);   //模糊半径
    shadow_main_color->setOffset(0);      //偏移量
    shadow_main_color->setEnabled(true);

    setGraphicsEffect(shadow_main_color);

    setMainColor(winThemeColor());

    // colorAlpha = aim_colorAlpha();
    // nowPadRatio = aim_padRatio();

    focusAnimations = new QParallelAnimationGroup(this);
    positionAnimations = new QParallelAnimationGroup(this);
    longFocusAnimations = new QParallelAnimationGroup(this);

    SET_ANIMATION(alphaAnimation,colorAlpha,focusAnimations,focus_animation_time);

    SET_ANIMATION(scaleFixAnimation,scaleFix,focusAnimations,focus_animation_time);

    SET_ANIMATION(padRatioAnimation,nowPadRatio,focusAnimations,focus_animation_time);

    SET_ANIMATION(mainColorRatioAnimation,nowMainColorRatio,focusAnimations,focus_animation_time);

    SET_ANIMATION(sizeAnimation,nowSize,positionAnimations,position_animation_time);

    SET_ANIMATION(posAnimation,nowPos,positionAnimations,position_animation_time);

    longFocusTimer = new QTimer(this);
    connect(longFocusTimer, SIGNAL(timeout()), this, SLOT(longFocusTimeoutSlot()));

    // setMouseTracking(true);
    connect(this,&SUnit::colorAlpha_changed,this,[=](int val){
        // qDebug()<<val;
        whenFocusAnimationChange();
    });

    connect(this,&SUnit::nowMainColorRatio_changed,this,[=](double val){
        // qDebug()<<val;
        whenFocusAnimationChange();
    });

    connect(this,&SUnit::scale_changed,this,[=](double val){
        onScaleChange(val*scaleFix);
    });

    connect(this,&SUnit::scaleFix_changed,this,[=](double val){
        // qDebug()<<val;
        whenFocusAnimationChange();

    });

    connect(this,&SUnit::nowPos_changed,this,[=](QPoint val){
        edmove(val);
    });

    connect(this,&SUnit::nowSize_changed,this,[=](QSize val){
        setFixedSize(val);
    });

    connect(this,&SUnit::nowPadRatio_changed,this,[=](double val){
        // qDebug()<<val;
        whenFocusAnimationChange();
    });

    connect(this,&SUnit::mainColor_changed,this,&SUnit::onMainColorChange);

    connect(focusAnimations,&QParallelAnimationGroup::finished,this,[=](){
    nowPadRatio = aim_padRatio();
    scaleFix = aim_scaleFix();
    colorAlpha = aim_colorAlpha();
    update();
    });

    rs = new roundShower(this);
    rs->raise();
    setupMenu();

    if(dis!= nullptr)
    dis->defaultPut(this,false);
};




void SUnit::single_click_action(){
    //最终单击执行
}

void SUnit::double_click_action(QMouseEvent *event){
    //最终双击执行
}

void SUnit::mouse_enter_action(){
    if(pmw==nullptr)
        qDebug()<<"null";
    else
        qDebug()<<"have";

}

void SUnit::mouse_leave_action(){
}

void SUnit::mouse_move_action(){
    //最终移动执行


}

void SUnit::mouse_release_action(){
    //最终释放执行

}

void SUnit::removeFromLayout(){
    if(layout!=nullptr)
    layout->RemoveAUnit(this);
}



void SUnit::setupMenu()
{
    myMenu = new SMenu(this);

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

    SET_ANCTION(act5,切换精简,{
        changeSimpleMode();
    });

    SET_ANCTION(act6,切换始终显示,{
        setAlwaysShow(!alwaysShow);
    });


    SET_ANCTION(act7,删除,{
        Remove();
    });


}

void SUnit::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        grabMouse();
        single_click_action();
        premove = true;
        relativeP = event->pos();
        if(!pCelectedUnits.contains(this))
            cleanCelect();
        setCelect(true);
    }
    event->accept();
    qDebug()<<objectName()<<"press"<<event->pos()<<event->globalPos()<<mapTo(pmw,event->pos());
}

void SUnit::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<<objectName()<<"Release"<<event->pos()<<event->globalPos()<<mapTo(pmw,event->pos());
    releaseMouse();
    mouse_release_action();
    if(moving){
        releaseCelect();
    }
    event->accept();
}

void SUnit::mouseDoubleClickEvent(QMouseEvent *event)
{
    double_click_action(event);
}

void SUnit::mouseMoveEvent(QMouseEvent *event)
{
    mouse_move_action();
    event->accept();
    if (moving)
    {
        //processor
        // QPoint ind = pmw->inside->SLayout::pos2Ind(pmwPos);
        // SUnit* aim = pmw->inside->SLayout::ind2Unit(ind);
        // if(aim!=nullptr){
        //     aim->preSetLongFocus(true);
        // }
        // foreach (SUnit* tem, *(pmw->inside->contents)) {
        //     if(tem!=aim &&tem->preLongFocus){
        //         tem->preSetLongFocus(false);
        //     }
        // }
        moveCelect();
        // tryToSwitch(event);
    }
    else if(premove){
        auto tem = event->pos();
        int dis =sqrt ((tem.x()-relativeP.x())*(tem.x()-relativeP.x())+(tem.y()-relativeP.y())*(tem.y()-relativeP.y()));
        if(dis>=2){
            dragOut();
        }
    }
}

void SUnit::enterEvent(QEvent *event){
    qDebug()<<objectName()<<"enter"<<(layout != nullptr);

    onFocus = true;
    mouse_enter_action();
    updateFocusAnimation();
    event->accept();
    if(layout!=nullptr)
        if(!layout->isMain){
            layout->pContainer->update();
            layout->pContainer->repaint();
        }
    if(!moving)
    preSetLongFocus(true);
    update();
}

void SUnit::leaveEvent(QEvent *event){
    qDebug()<<objectName()<<"leave";
    onFocus = false  ;
    preSetLongFocus(false);
    mouse_leave_action();
    updateFocusAnimation();
    if(layout!=nullptr)
        if(!(layout->isMain)){
            layout->pContainer->update();
            layout->pContainer->repaint();
        }

}

void SUnit::updateInLayout(bool animated)
{
    //当Layout改变大小等调用（不包含初次放置）
    if(animated)
        moveto(MyPos(),MySize());
    else{
        move(MyPos());
        setFixedSize(MySize());
    }
}

void SUnit::moveto(QPoint edpos, QSize size)
{
    //传入相对于Layou-pContainer的坐标

    aim_pos = edpos;
    aim_size = size;

    updatePositionAnimation();
    // updataFocusAnimation();
}

void SUnit::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    rs->updateDisplay();
    afterResize(event);
}

void SUnit::contextMenuEvent(QContextMenuEvent *event)
{
    if(event->modifiers() == Qt::ShiftModifier)
        onShiftContextMenu(event);
    else
        onContextMenu(event);
}

bool SUnit::setBlockSize(int w,int h){
    if(w<=0)return false;
    if(h<=0)return false;
    SLayout* tem_layout;
    if(layout!=nullptr){
        SUnit temu(nullptr,w,h);
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

void SUnit::onContextMenu(QContextMenuEvent *event)
{
    myMenu->exec(event->globalPos());
}

void SUnit::onShiftContextMenu(QContextMenuEvent *event)
{

}

void SUnit::onProcessAnother(SUnit *another)
{

}

void SUnit::setSimpleMode(bool val){
    simpleMode = val;
    onSimpleModeChange(val);
}

void SUnit::setLongFocus(bool val)
{
    qDebug()<<"Long Focus"<<val;
    onLongFocus = val;
    if(!val){
        activepmw->pls->clearTooltip();
    }
    updateLongFocusAnimation();
}

void SUnit::preSetLongFocus(bool val)
{
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
            else{
                if(requireLongFocusOutDeltaTime)
                longFocusTimer->start(long_focus_out_delta_time);
                else
                    longFocusTimeoutSlot();
            }

        }

    }
    preLongFocus = val;
}

void SUnit::changeSimpleMode(){
    setSimpleMode(!simpleMode);
}

void SUnit::setMainColor(QColor val)
{
    val.setAlpha(255);
    mainColor = val;


    auto tem = mainColor;
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

    // onMainColorChange(val);
    emit mainColor_changed(val);
}

void SUnit::onSimpleModeChange(bool val){}

void SUnit::onScaleChange(double val){}

void SUnit::onCelectChange(double val)
{
    updateFocusAnimation();
}

void SUnit::setScale(double val){
    scale = val;
    emit scale_changed(val);
}

void SUnit::setScaleFix(double val)
{
    scaleFix = val;
    emit scaleFix_changed(val);
}

void SUnit::setAlwaysShow(bool val)
{
    alwaysShow = val;
}

void SUnit::setPMW(MainWindow *pmw)
{
    this->pmw = pmw;
}

QColor SUnit::displayColor_Alphaed()
{
    QColor _displayColor = displayColor();
    //使用可能在动画中的alpha
    _displayColor.setAlpha(colorAlpha);
    return _displayColor;
}

void SUnit::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    paintSide(this,QColor("green"));
    paintRect(this,displayColor_Alphaed());
    paintLight(this,displayColor());
}

void SUnit::wheelEvent(QWheelEvent *event)
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

void SUnit::Remove()
{
    removeFromLayout();
    deleteLater();
}

void SUnit::endUpdate(){
    //当初始化完成之后调用以更新状态
    colorAlpha = aim_colorAlpha();
    nowPadRatio = aim_padRatio();
    scaleFix = aim_scaleFix();
    nowMainColorRatio = aim_mainColorRatio();
    if(layout!=nullptr){
        updateInLayout(false);
    }
    rs->updateDisplay();
    rs->raise();
    update();
}


void SUnit::tryToSwitch(QMouseEvent *event)
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
                setFocus();
                grabMouse();

            }
        }
    }
}

void SUnit::updateLongFocusAnimation()
{

}

void SUnit::updateFocusAnimation()
{
    focusAnimations->stop();

    // qDebug()<<"colorAlpha Update: now:"<<colorAlpha<<"aim"<<aim_colorAlpha();
    alphaAnimation->setStartValue(colorAlpha);
    alphaAnimation->setEndValue(aim_colorAlpha());

    // qDebug()<<"nowPadRatio Update: now:"<<nowPadRatio<<"aim"<<aim_padRatio();
    padRatioAnimation->setStartValue(nowPadRatio);
    padRatioAnimation->setEndValue(aim_padRatio());

    // qDebug()<<"nowMainColorRatio Update: now:"<<nowMainColorRatio<<"aim"<<aim_mainColorRatio();
    mainColorRatioAnimation->setStartValue(nowMainColorRatio);
    mainColorRatioAnimation->setEndValue(aim_mainColorRatio());

    // qDebug()<<"scaleFix Update: now:"<<scaleFix<<"aim"<<aim_scaleFix();
    scaleFixAnimation->setStartValue(scaleFix);
    scaleFixAnimation->setEndValue(aim_scaleFix());

    focusAnimations->start();
}

void SUnit::updatePositionAnimation()
{
    positionAnimations->stop();
    posAnimation->setStartValue(edpos());
    posAnimation->setEndValue(aim_pos);

    sizeAnimation->setStartValue(size());
    sizeAnimation->setEndValue(aim_size);
    positionAnimations->start();
}

void SUnit::onDragedOut()
{
    relativeP = mapFromGlobal(QCursor::pos());
    moving = true;
    preSetLongFocus(false);
    pmw->pls->clearTooltip();
    QPoint usedp = mapTo(pmw,QPoint(0,0));
    positionAnimations->stop();
    if(layout)
        if(!layout->isMain)layout->pContainer->update();
    if(layout)
        removeFromLayout();
    move(usedp);
}

void SUnit::preSetInLayout(bool animated)
{
    raise();
    if(animated){
        moveto(MyPos(),MySize());
        connect(positionAnimations,&QParallelAnimationGroup::finished,this,&::SUnit::setInLayoutAniSlot,Qt::UniqueConnection);
        connect(positionAnimations,&QParallelAnimationGroup::currentLoopChanged,this,[=](){
            disconnect(positionAnimations,SIGNAL(finished()), 0, 0);
        });
    }
    else{
        edmove(MyPos());
        setFixedSize(MySize());
        setInLayout(animated);
    }

}

void SUnit::setInLayoutAniSlot()
{
    setInLayout(true);
}

void SUnit::longFocusTimeoutSlot()
{
    longFocusTimer->stop();
    setLongFocus(preLongFocus);

}


void SUnit::setInLayout(bool animated)
{
    QPoint tem = edpos();
    // qDebug()<<tem<<dis;
    setParent(layout->pContainer);
    edmove(tem);
    setVisible(true);
    layout->updateAfterPut(this,indX,indY);
    update();

}

QJsonObject SUnit::to_json(){
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

void SUnit::load_json(QJsonObject rootObject)
{
    type = (SUnit::STYPE)rootObject.value("type").toInt();
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
}

void SUnit::onMainColorChange(QColor val){

}

void SUnit::afterResize(QResizeEvent* event)
{

}

void SUnit::whenLongFocusAnimationChange()
{

}

void SUnit::whenFocusAnimationChange()
{
    onScaleChange(scale*scaleFix);
    if(layout && layout->pContainer->inherits("SDock")){
        layout->pContainer->update();
    }
    else{
        update();
    }
}

double SUnit::aim_padRatio(){
    double val =0.5;
    if(layout==nullptr) val= 1.0;
    else{
        if(layout->isMain) val = 1.0;
        else{
            if(onFocus) val = 1.0;
            else val =  0.0;
        }
    }

    return val;
}

void SUnit::setCelect(bool newOnCelected, bool disableAnimation )
{
    if(newOnCelected){
        if(!pCelectedUnits.contains(this))
            pCelectedUnits.append(this);
    }
    else{
        if(pCelectedUnits.contains(this))
            pCelectedUnits.removeOne(this);
    }
    onCelect = newOnCelected;
    if(pCelectedUnits.size()>4||disableAnimation)
    endUpdate();
    else
    updateFocusAnimation();

}
