#include "snotice.h"
#include "SysFunctions.h"
#include "global.h"
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
    titleFont = new QFont(qApp->font().family(), 15, QFont::Bold);   //字体，大小，粗体，斜体
    titleFont->setCapitalization(QFont::Capitalize);   //设置字母大小写

    endTimer = new QTimer(this);
    endTimer->setSingleShot(true);

    infoFont = new QFont(qApp->font().family(), 10, QFont::Bold);   //字体，大小，粗体，斜体
    // infoFont->setCapitalization(QFont::Capitalize);   //设置字母大小写


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
{   if(onLoading) return;
    if(nowOKPosY(this)>=activepmw->height()*0.4){
        endOne();
    }
    noticeList.append(this);
    aimSize = QSize(qMax(titleSize.width(),infoSize.width())+borad*2,titleSize.height()+infoSize.height()+borad*2+spaceBetweenTileInfo);

    arect->setStartValue(QPoint(0,0),
                         QSize(0,0),
                         0,0);

    arect->setEndValue(QPoint(-aimSize.width()/2,0),
                       QSize(aimSize.width(),aimSize.height()),
                       230,unit_radius);


    QPoint shift = activepmw->pls->mapFromGlobal(activepmw->mapToGlobal(QPoint(0,0)));
    move(QPoint((activepmw->width()-aimSize.width())/2,nowOKPosY(this))+shift);
    arect->start();
    endTimer->start(staytime);
    setEnabled(true);
    setVisible(true);
    raise();
}

void SNotice::end()
{
    onEnd = true;
    arect->stop();

    arect->setEndValue(QPoint(0,0),
                       QSize(0,0),
                       0,0);
    arect->setFinal();

    arect->start();
}

void SNotice::whenAnimationUpdate()
{

    // qDebug()<<pos()<<size();
    QPoint shift = activepmw->pls->mapFromGlobal(activepmw->mapToGlobal(QPoint(0,0)));
    move(QPoint(activepmw->width()/2,nowOKPosY(this))+shift+arect->nowPos);
    setFixedSize(arect->nowSize);
    update();
}

void SNotice::endOne()
{
    if(noticeList.empty()) return;
    foreach (SNotice* notice, noticeList) {
        if(!notice->onEnd){
            notice->end();
            return;
        }
    }
}



void SNotice::notice(QStringList info, QString title, int time)
{
    SNotice* notice;
    if(activepmw)
    notice = new SNotice(activepmw->pls);
    else
    notice = new SNotice(nullptr);

    notice->setInfo(info);
    notice->setTitle(title);
    notice->setStayTime(time);
    notice->comeout();
}

void SNotice::notice(QString info, QString title, int time)
{
    notice(QStringList()<<info,title,time);
}

//从MainWindow顶端到将要显示通知的顶端的距离
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
    QColor tem = themeColor();

    QPainter painter(this);
    tem.setAlpha(arect->nowAlpha);
    painter.setBrush(tem);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());


    tem = text_color;
    tem.setAlpha(arect->nowAlpha);
    painter.setPen(tem);

    //    font.setUnderline(true); //设置下划线
    //    font.setOverline(true); //设置上划线
    // font.setLetterSpacing(QFont::AbsoluteSpacing, 10); //设置字符间的间距
    int Xshift =  mapFromGlobal(activepmw->mapToGlobal(QPoint(activepmw->width()/2,0))).x();

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
