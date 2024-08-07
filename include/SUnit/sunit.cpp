#include "sunit.h"
#include "global.h"
#include "guifunc.h"
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
#include "unitfunc.h"
#include"stylehelper.h"

#define SET_ANIMATION(ANIMATION,NAME,GROUP,TIME)\
ANIMATION = new QPropertyAnimation(this,#NAME);\
    ANIMATION->setDuration(TIME);\
    ANIMATION->setEasingCurve(QEasingCurve::InOutQuad);\
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
    connect(psh->intVal("unit_shadow_blur_radius"),&intVal::valueChanged,this,[=](int value){
        shadow_main_color->setBlurRadius(value);
        shadow_main_color->update();
    });
    connect(psh->intVal("unit_shadow_alpha"),&intVal::valueChanged,this,[=](int value){
        shadow_main_color->setColor(applyAlpha( displayColor(),unit_shadow_alpha));
        shadow_main_color->update();
    });
    setGraphicsEffect(shadow_main_color);
    updateColor();
    connect(psh,&StyleHelper::colorChanged,this,&SUnit::updateColor);

    // colorAlpha = aim_colorAlpha();
    // nowPadRatio = aim_padRatio();
    // linear = new SLinerAnimation(this);
    // connect(linear,&SLinerAnimation::progressChanged,this,[=](double progress){
    //     whenFocusAnimationChange();
    // });

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

    // connectTo(always_simple_mode,bool,bool,{
    //     setSimpleMode(value);
    //                             })


    connect(this,&SUnit::mainColor_changed,this,&SUnit::onMainColorChange);

    connect(focusAnimations,&QParallelAnimationGroup::finished,this,[=](){
    nowPadRatio = aim_padRatio();
    scaleFix = aim_scaleFix();
    colorAlpha = aim_colorAlpha();
    update();
    });

    rs = new roundShower(this);
    rs->raise();

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
    updateFocusAnimation();
}



void SUnit::setupEditMenu()
{
    editMenu = new SMenu(this);
    editMenu->ismain = true;

    connect(editMenu,&QMenu::aboutToShow,this,[=](){
        onContextMenuShowing = true;
    });

    connect(editMenu,&QMenu::aboutToHide,this,[=](){
        onContextMenuShowing = false;
    });

    SET_ANCTION(act1,tr("加宽"),editMenu,this,{
        setBlockSize(sizeX+1,sizeY);
    });

    SET_ANCTION(act2,tr("加高"),editMenu,this,{
        setBlockSize(sizeX,sizeY+1);
    });

    SET_ANCTION(act3,tr("减宽"),editMenu,this,{
        if(sizeX>=2)
            setBlockSize(sizeX-1,sizeY);
    });

    SET_ANCTION(act4,tr("减高"),editMenu,this,{
        if(sizeY>=2)
            setBlockSize(sizeX,sizeY-1);
    });

    SET_ANCTION(act5,tr("切换精简"),editMenu,this,{
        setSimpleMode(!simpleMode);
    });

    SET_ANCTION(act6,tr("切换始终显示"),editMenu,this,{
        // swtichAlwayShowG(this);

        setAlwaysShow(!alwaysShow);
    });


    SET_ANCTION(act7,tr("删除"),editMenu,this,{
        remove();
    });
}

void SUnit::setupDesktopMenu()
{
    desktopMenu = new SMenu(this);
    desktopMenu->ismain = true;
    connect(desktopMenu,&QMenu::aboutToShow,this,[=](){
        onContextMenuShowing = true;
    });

    connect(desktopMenu,&QMenu::aboutToHide,this,[=](){
        onContextMenuShowing = false;
    });
}

void SUnit::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<objectName()<<"press"<<event->pos()<<event->globalPos()<<mapTo(pmw,event->pos());
    if(event->button() == Qt::LeftButton){
        grabMouse();
        single_click_action();
        premove = true;
        relativeP = event->pos();
        if(!pCelectedUnits.contains(this))
            cleanCelect(this);
        setCelect(true);
        event->accept();
    }
    else if(event->button()==Qt::MiddleButton){
        if(!editMode){
            if(unset){
                setupEditMenu();
                setupDesktopMenu();
                unset = false;
            }
            editMenu->exec(event->globalPos());
        }
    }
}

void SUnit::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<<objectName()<<"Release"<<event->pos()<<event->globalPos()<<mapTo(pmw,event->pos());
    releaseMouse();
    mouse_release_action();
    if(moving){
        releaseCelect(this);
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
        moveCelect(this);
    }
    else if(premove){
        auto tem = event->pos();
        qDebug()<<tem;
        int dis =sqrt ((tem.x()-relativeP.x())*(tem.x()-relativeP.x())+(tem.y()-relativeP.y())*(tem.y()-relativeP.y()));
        if(dis>=2){
            dragOutG(this,event);
        }
    }
}

void SUnit::enterEvent(QEvent *event){
    qDebug()<<objectName()<<"enter"<<(layout != nullptr);

    onFocus = true;
    pFocusedUnit = this;
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
    pFocusedUnit = nullptr;
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
        tem_layout->clearPut(this,res);
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
    if(unset){
        setupEditMenu();
        setupDesktopMenu();
        unset = false;
    }
    requireContexMenu(event,this);
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
    if(!val&&!onContextMenuShowing){
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
    if(mainColor==val) return;

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

void SUnit::onSwitch(MainWindow *aimpmw)
{
    auto globalPos = mapToGlobal(QPoint(0,0));
    auto aimPos = aimpmw->mapFromGlobal(globalPos);
    releaseMouse();
    setParent(aimpmw);
    setPMW(aimpmw);
    move(aimPos);
    setVisible(true);
    setEnabled(true);
    aimpmw->setFocus();
    // qDebug()<<pmw->objectName()<<"Acitive:"<<pmw->isActiveWindow();
    aimpmw->raise();
    raise();
    grabMouse();
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
        qDebug()<<"called"<<parentWidget();
        int tem = event->angleDelta().y();
        qDebug()<<event->angleDelta();
        bool res = false;
        if(tem>0){
            res = onBigger();
        }
        else{
            res = onSmaller();
        }
        if(res){
            event->accept();
            qDebug()<<"eventAccepted";
        }
        else{
            qDebug()<<"eventIgnored";
            qDebug()<<"nowParent"<<parentWidget();
            event->ignore();
        }
    }
}

void SUnit::remove()
{
    removeFromLayout();
    if(pCelectedUnits.contains(this)){
        pCelectedUnits.removeOne(this);
    }
    if(pFocusedUnit&&pFocusedUnit==this){
        pFocusedUnit = nullptr;
    }
    deleteLater();
}

void SUnit::endUpdate(){
    //当初始化完成之后调用以更新状态
    setSimpleMode(simpleMode);
    colorAlpha = aim_colorAlpha();
    nowPadRatio = aim_padRatio();
    scaleFix = aim_scaleFix();
    nowMainColorRatio = aim_mainColorRatio();
    shadow_main_color->setColor(applyAlpha( displayColor(),unit_shadow_alpha));
    shadow_main_color->update();
    if(layout!=nullptr){
        updateInLayout(false);
    }
    updateColor();
    rs->updateDisplay();
    rs->raise();
    update();

}




void SUnit::updateLongFocusAnimation()
{

}

void SUnit::updateFocusAnimation()
{
    focusAnimations->stop();

    // qDebug()<<"colorAlpha Update: now:"<<colorAlpha<<"aim"<<aim_colorAlpha();
    alphaAnimation->setStartValue(colorAlpha);
    alphaAnimation->setDuration(focus_animation_time);
    alphaAnimation->setEndValue(aim_colorAlpha());

    // qDebug()<<"nowPadRatio Update: now:"<<nowPadRatio<<"aim"<<aim_padRatio();
    padRatioAnimation->setStartValue(nowPadRatio);
    padRatioAnimation->setDuration(focus_animation_time);
    padRatioAnimation->setEndValue(aim_padRatio());

    // qDebug()<<"nowMainColorRatio Update: now:"<<nowMainColorRatio<<"aim"<<aim_mainColorRatio();
    mainColorRatioAnimation->setStartValue(nowMainColorRatio);
    mainColorRatioAnimation->setDuration(focus_animation_time);
    mainColorRatioAnimation->setEndValue(aim_mainColorRatio());

    // qDebug()<<"scaleFix Update: now:"<<scaleFix<<"aim"<<aim_scaleFix();
    scaleFixAnimation->setStartValue(scaleFix);
    scaleFixAnimation->setDuration(focus_animation_time);
    scaleFixAnimation->setEndValue(aim_scaleFix());

    focusAnimations->start();
    // linear->stop();
    // linear->setTime(focus_animation_time);
    // linear->start();
}

void SUnit::updatePositionAnimation()
{
    positionAnimations->stop();
    posAnimation->setStartValue(edpos());
    posAnimation->setDuration(position_animation_time);
    posAnimation->setEndValue(aim_pos);

    sizeAnimation->setStartValue(size());
    sizeAnimation->setDuration(position_animation_time);
    sizeAnimation->setEndValue(aim_size);
    positionAnimations->start();
}

void SUnit::onDragedOut()
{
    if(!premove)
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
        updateFocusAnimation();
        connect(positionAnimations,&QParallelAnimationGroup::finished,this,&::SUnit::setInLayoutAniSlot,Qt::UniqueConnection);
        connect(positionAnimations,&QParallelAnimationGroup::currentLoopChanged,this,[=](){
            disconnect(positionAnimations,SIGNAL(finished()), 0, 0);
        });
    }
    else{
        endUpdate();
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

void SUnit::updateColor()
{
    setMainColor(themeColor());
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

bool SUnit::onBigger()
{
    if(!setBlockSize(sizeX+1,sizeY+1)){
        if(!setBlockSize(sizeX,sizeY+1)){
            return setBlockSize(sizeX+1,sizeY);
        }
    }

    return true;
}

bool SUnit::onSmaller()
{
    if(!setBlockSize(sizeX-1,sizeY-1)){
        if(!setBlockSize(sizeX,sizeY-1)){
            return setBlockSize(sizeX-1,sizeY);
        }
    }
    return true;
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
    shadow_main_color->setColor(applyAlpha( displayColor(),unit_shadow_alpha));
    shadow_main_color->update();
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
        premove = false;

    }
    onCelect = newOnCelected;
    if(pCelectedUnits.size()>4||disableAnimation)
    endUpdate();
    else
    updateFocusAnimation();
}
