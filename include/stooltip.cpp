#include "stooltip.h"
#include "SysFunctions.h"
#include "qpainter.h"
#include "screenfunc.h"


#define SET_ANIMATION(ANIMATION,NAME,TIME)\
ANIMATION = new QPropertyAnimation(this,#NAME);\
    ANIMATION->setDuration(TIME);\
    ANIMATION->setEasingCurve(QEasingCurve::InOutQuad);\
    animations->addAnimation(ANIMATION);

SToolTip::SToolTip(QWidget *parent)
    : QWidget{parent}
{
    rs = new roundShower(this);
    font = new QFont("MiSans", 10, QFont::Bold);   //字体，大小，粗体，斜体
    font->setCapitalization(QFont::Capitalize);   //设置字母大小写
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_InputMethodTransparent);
    setWindowFlags(Qt::WindowTransparentForInput|Qt::NoDropShadowWindowHint);
    animations = new QParallelAnimationGroup(this);

    int animationTime =200;
    SET_ANIMATION(radiusAnimation,nowRadius,animationTime);
    SET_ANIMATION(alphaAnimation,nowAlpha,animationTime);
    SET_ANIMATION(sizeAnimation,nowSize,animationTime);

    posAnimation = new QPropertyAnimation(this,"nowPos");
    posAnimation->setDuration(animationTime);
    posAnimation->setEasingCurve(QEasingCurve::InOutQuad);\


    connect(this,&SToolTip::nowAlpha_changed,this,[=](int val){
        whenAnimationChange();
    });
    connect(this,&SToolTip::nowRadius_changed,this,[=](int val){
        whenAnimationChange();
    });
    connect(this,&SToolTip::nowSize_changed,this,[=](QSize val){
        whenAnimationChange();
    });
    connect(this,&SToolTip::nowPos_changed,this,[=](QPoint val){
        whenAnimationChange();
    });
    // connect(animations,&)
    rs->distriRadius(&nowRadius);
    rs->raise();
}

void SToolTip::setInfo(QString info)
{
    this->info = info;
    QFontMetrics fm(*font);
    QRect rec = fm.boundingRect(info);
    aimSize = QSize(rec.width()+20,rec.height()+20);//这个就获得了字符串所占的像素宽度
}

void SToolTip::comeout()
{
    alphaAnimation->setStartValue(0);
    alphaAnimation->setEndValue(230);

    sizeAnimation->setStartValue(QSize(0,aimSize.height()));
    sizeAnimation->setEndValue(aimSize);

    radiusAnimation->setStartValue(0);
    radiusAnimation->setEndValue(10);

    animations->start();
    setVisible(true);
    show();
}

void SToolTip::end()
{
    if(onEnd) return;
    onEnd = true;
    previousPos = pos();
    nowPos = pos();
    aimPos = geometry().topRight();
    animations->stop();

    if(enable_tooltip_right_animation){
        animations->addAnimation(posAnimation);

        posAnimation->setStartValue(pos());
        posAnimation->setEndValue(aimPos);

        animations->removeAnimation(sizeAnimation);
    }
    else{

        sizeAnimation->setStartValue(size());
        sizeAnimation->setEndValue(QSize(0,aimSize.height()));
    }


    alphaAnimation->setStartValue(nowAlpha);
    alphaAnimation->setEndValue(0);

    radiusAnimation->setStartValue(10);
    radiusAnimation->setEndValue(5);


    connect(animations,&QParallelAnimationGroup::finished,this,&SToolTip::whenEndAnimationEnd);

    animations->start();
}

void SToolTip::Tip(QString info)
{
    Tip(pls->mapFromGlobal(QCursor::pos()),info);
}

void SToolTip::Tip(QPoint pos, QString info)
{
    //ls pos
    SToolTip* Tip = new SToolTip(pls);
    Tip->move(pos+QPoint(13,8));
    Tip->setInfo(info);
    Tip->comeout();
}

void SToolTip::Tip(QWidget *aim, QPoint pos, QString info)
{
    Tip(mapToLS(aim,pos),info);
}

void SToolTip::whenAnimationChange()
{

    if(onEnd&&enable_tooltip_right_animation){
        move(nowPos);
        setFixedWidth(aimPos.x()-nowPos.x());
    }
    else{
               setFixedSize(nowSize);
    }
    update();


}

void SToolTip::whenEndAnimationEnd()
{
    deleteLater();
}

void SToolTip::paintEvent(QPaintEvent *event)
{
    QColor tem = GetWindowsThemeColor();

    QPainter painter(this);
    tem.setAlpha(nowAlpha);
    painter.setBrush(tem);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());


    tem = QColor("black");
    tem.setAlpha(nowAlpha);
    painter.setPen(tem);

    //    font.setUnderline(true); //设置下划线
    //    font.setOverline(true); //设置上划线
    // font.setLetterSpacing(QFont::AbsoluteSpacing, 10); //设置字符间的间距
    painter.setFont(*font);
    QRect temsize = QRect((previousPos-nowPos).x(),0,aimSize.width(),aimSize.height());
    painter.drawText(temsize, Qt::AlignCenter, info);//字体水平居中
}

void SToolTip::resizeEvent(QResizeEvent *event)
{
    rs->updateDisplay();
    QWidget::resizeEvent(event);
}




