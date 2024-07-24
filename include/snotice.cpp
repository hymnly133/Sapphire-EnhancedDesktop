#include "snotice.h"
#include "SysFunctions.h"
#include "mainwindow.h"
#include "qpainter.h"
#define SET_ANIMATION(ANIMATION,NAME,TIME)\
ANIMATION = new QPropertyAnimation(this,#NAME);\
    ANIMATION->setDuration(TIME);\
    ANIMATION->setEasingCurve(QEasingCurve::InOutQuad);\
    animations->addAnimation(ANIMATION);

QList<SNotice*> SNotice::noticeList;
int SNotice::spaceBetweenNotice  = 10;
int SNotice::borad  = 5;
int SNotice::boradTitle  = 10;
int SNotice::boradInfo  = 20;
int SNotice::spaceBetweenTileInfo  = 5;
SNotice::SNotice(QWidget *parent)
    : QWidget{parent}
{
    rs = new roundShower(this);
    titleFont = new QFont("MiSans", 20, QFont::Bold);   //字体，大小，粗体，斜体
    titleFont->setCapitalization(QFont::Capitalize);   //设置字母大小写

    endTimer = new QTimer(this);
    endTimer->setSingleShot(true);

    infoFont = new QFont("MiSans", 10, QFont::Bold);   //字体，大小，粗体，斜体
    infoFont->setCapitalization(QFont::Capitalize);   //设置字母大小写


    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_InputMethodTransparent);
    setWindowFlags(Qt::WindowTransparentForInput|Qt::NoDropShadowWindowHint);
    arect = new SAnimationRect(this);
    // connect(animations,&)
    rs->distriRadius(&arect->nowRadius);
    rs->raise();
    connect(arect,&SAnimationRect::whenEndAnimationEnd,this,[=]{
        noticeList.removeOne(this);
        deleteLater();
        updateAllNoticeAnimation();
    });
    connect(arect,&SAnimationRect::animationUpdating,this,[=](QPoint pos,QSize size,int,int){

        updateAllNoticeAnimation();
        // update();
        // qDebug()<<this->pos();
        // setVisible(true);
    });

    connect(endTimer,&QTimer::timeout,this,[=](){
        end();
    });
    setVisible(false);


}


void SNotice::setTitle(QString Title)
{
    title = Title;
    QFontMetrics fm(*titleFont);
    titleSize = fm.boundingRect(Title).size()+QSize(boradTitle*2,boradTitle*2);
}

void SNotice::setInfo(QStringList info)
{
    this->info = info.join("\n");
    int aimwidth = 0;
    int aimheight = 0;
    QFontMetrics fm(*infoFont);
    foreach(QString k ,info){
        infoSize = fm.boundingRect(k).size();
        aimwidth = qMax(aimwidth,infoSize.width());
        aimheight+=infoSize.height();
    }
    infoSize= QSize(aimwidth,aimheight)+QSize(boradInfo*2,boradInfo*2);
}

void SNotice::setStayTime(int time)
{
    staytime = time;
}

void SNotice::comeout()
{
    pmw2pls = pls->mapFromGlobal(pmws[0]->mapToGlobal(QPoint(0,0)));
    noticeList.append(this);
    aimSize = QSize(qMax(titleSize.width(),infoSize.width())+borad*2,titleSize.height()+infoSize.height()+borad*2+spaceBetweenTileInfo);

    arect->setStartValue(QPoint(0,0),
                         QSize(0,0),
                         0,0);

    arect->setEndValue(QPoint(-aimSize.width()/2,0),
                       QSize(aimSize.width(),aimSize.height()),
                       230,unit_radius);


    move(pmw2pls+QPoint((pmws[0]->width()-aimSize.width())/2,nowOKPosY(this)));
    arect->start();
    endTimer->start(staytime);
    setEnabled(true);
    setVisible(true);
    raise();
}

void SNotice::end()
{
    arect->stop();

    arect->setEndValue(QPoint(0,0),
                       QSize(0,0),
                       0,0);
    arect->setFinal();

    arect->start();
}

void SNotice::whenAnimationUpdate()
{
    pmw2pls = pls->mapFromGlobal(pmws[0]->mapToGlobal(QPoint(0,0)));
    // qDebug()<<pos()<<size();
    move(pmw2pls+QPoint(pmws[0]->width()/2,nowOKPosY(this))+arect->nowPos);
    setFixedSize(arect->nowSize);
    update();
}

void SNotice::notice(QStringList info, QString title, int time)
{
    SNotice* notice = new SNotice(pls);
    notice->setInfo(info);
    notice->setTitle(title);
    notice->setStayTime(time);
    notice->comeout();
}

int SNotice::nowOKPosY(SNotice *aim)
{
    int res = spaceBetweenNotice;
    foreach (SNotice* notice, noticeList) {
        if(notice!=aim){
            res+=notice->height();
            res+=spaceBetweenNotice;
        }
        else{
            break;
        }
    }
    return res;
}

void SNotice::updateAllNoticeAnimation()
{
    foreach (SNotice* notice, noticeList) {
        notice->whenAnimationUpdate();
    }
}

void SNotice::paintEvent(QPaintEvent *event)
{
    QColor tem = GetWindowsThemeColor();

    QPainter painter(this);
    tem.setAlpha(arect->nowAlpha);
    painter.setBrush(tem);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());


    tem = QColor("black");
    tem.setAlpha(arect->nowAlpha);
    painter.setPen(tem);

    //    font.setUnderline(true); //设置下划线
    //    font.setOverline(true); //设置上划线
    // font.setLetterSpacing(QFont::AbsoluteSpacing, 10); //设置字符间的间距
    int Xshift =  mapFromGlobal(pmws[0]->mapToGlobal(QPoint(pmws[0]->width()/2,0))).x();

    painter.setFont(*titleFont);
    QRect titleRect = QRect(-aimSize.width()/2+borad+Xshift,borad,titleSize.width(),titleSize.height());
    painter.drawText(titleRect, Qt::AlignCenter, title);//字体水平居中


    painter.setFont(*infoFont);
    QRect infoRect = QRect(-aimSize.width()/2+borad+Xshift , borad+titleSize.height(),infoSize.width(),infoSize.height());
    painter.drawText(infoRect, Qt::AlignCenter, info);//字体水平居中
}

void SNotice::resizeEvent(QResizeEvent *event)
{
    rs->updateDisplay();
    QWidget::resizeEvent(event);
}
