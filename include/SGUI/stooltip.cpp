#include "stooltip.h"
#include "SysFunctions.h"
#include "global.h"
#include "mainwindow.h"
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
    arect = new SAnimationRect(this);
    rs->distriRadius(&arect->nowRadius);
    rs->raise();

    setAttribute(Qt::WA_TransparentForMouseEvents);
    // setAttribute(Qt::WA_InputMethodTransparent);
    setWindowFlags(Qt::WindowTransparentForInput | Qt::NoDropShadowWindowHint);
    // connect(animations,&)

    connect(arect, &SAnimationRect::finishedFinal, this, [ = ] {
        deleteLater();
    });
    connect(arect, &SAnimationRect::animationUpdating, this, [ = ]() {
        move(relaPos + arect->nowPos);
        setFixedSize(arect->nowSize);
        update();
    });
}

void SToolTip::setInfo(QString info)
{
    QFontMetrics fm(qApp->font());
    QRect rec = fm.boundingRect(info);
    this->info = info;

    //加边框
    aimSize = rec.size() + QSize(20, 20);

    //这个就获得了字符串所占的像素宽度
}

void SToolTip::comeout()
{
    QPoint tem = relaPos;
    //相对于pmw的坐标
    tem = activepmw->mapFromGlobal(activepmw->pls->mapToGlobal(tem));


    qDebug() << "tip comeout" << info;
    if(tem.x() + aimSize.width() > activepmw->width()) {
        //左侧显示
        left = true;
    }


    int aimX = 0;
    int aimY = 0;
    if(left) {
        aimX -= aimSize.width();
    }

    if(up) {
        aimY = -aimSize.height() - 5;
    } else {
        aimY = 8;
    }

    if(shift) {
        if(left) {
            aimX -= 13;
        } else {
            aimX += 13;
        }
    }


    aimPos = QPoint(aimX, aimY);



    arect->setStartValue(QPoint(0, aimPos.y()),
                         QSize(0, aimSize.height()),
                         0, 0);

    arect->setEndValue(aimPos,
                       QSize(aimSize.width(), aimSize.height()),
                       230, 10);


    arect->start();
    setVisible(true);
    emit startToShow();
    show();
}

void SToolTip::end()
{
    arect->stop();


    arect->setEndValue(QPoint(0, aimPos.y()),
                       QSize(0, aimSize.height()),
                       0, 0);
    arect->setFinal();



    arect->start();
    emit startToEnd();
}

SToolTip* SToolTip::tip(QString info, QPoint pos, bool shift, bool up)
{
    if(!enable_tooltip) {
        return nullptr;
    }
    SToolTip* Tip = new SToolTip(activepmw->pls);
    if(pos == NO_POS) {
        qDebug() << QCursor::pos();
        // qDebug() << QCursor::pos();

        Tip->relaPos = activepmw->pls->mapFromGlobal(QCursor::pos());
    } else {
        Tip->relaPos = activepmw->pls->mapFromGlobal(pos);
    }
    Tip->shift = shift;
    Tip->up = up;
    qDebug() << Tip->relaPos;
    Tip->move(Tip->relaPos);
    Tip->setInfo(info);
    Tip->comeout();
    return Tip;
}


void SToolTip::paintEvent(QPaintEvent *event)
{
    QColor tem = themeColor();
    // qDebug()<<mapToGlobal(QPoint(0,0));
    QPainter painter(this);
    tem.setAlpha(arect->nowAlpha);
    painter.setBrush(tem);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());


    tem = text_color;
    tem.setAlpha(arect->nowAlpha);
    painter.setPen(tem);
    painter.setFont(qApp->font());

    QRect temsize = QRect(0, 0, aimSize.width(), aimSize.height());
    if(left) {

        int delta = -arect->nowPos.x() - arect->nowSize.width();
        QPoint plsstill = (relaPos + aimPos);
        // temsize.setX(mapFromParent(plsstill).x());
        temsize = QRect(mapFromParent(plsstill).x() + 13 - delta, 0, aimSize.width(), aimSize.height());
        // temsize.setX(0);
    }
    // qDebug()<<temsize;
    painter.drawText(temsize, Qt::AlignCenter, info);//字体水平居中
}

void SToolTip::resizeEvent(QResizeEvent *event)
{
    rs->updateDisplay();
    QWidget::resizeEvent(event);
}




