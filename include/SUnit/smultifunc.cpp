#include "smultifunc.h"
#include"QProcess"
#include "filefunc.h"
#include "mainwindow.h"
#include "qboxlayout.h"
#include"SysFunctions.h"
#include "qevent.h"
#include "qlabel.h"
#include"QDebug"
#include"QUrl"
#include"QDesktopServices"
#include"QTextCodec"
#include "qmimedata.h"
#include "qpainter.h"
#include"QGraphicsDropShadowEffect"
#include"QFileDialog"
#include "sfile.h"
#include"stooltip.h"
#include"global.h"
#include "unitfunc.h"

#define SET_ANCTION(NAME,TEXT,FUCTION)\
QAction *NAME = new QAction(#TEXT);\
    myMenu->addAction(NAME);\
    connect(NAME, &QAction::triggered, this, [=]()FUCTION);



SMultiFunc::SMultiFunc(SLayout *dis,int sizex,int sizey):SUnit(dis,sizex,sizey)
{
    name = "empty";

    defaultScaleAnimation = new QPropertyAnimation(this,"nowDefaultScale");
    defaultScaleAnimation->setDuration(200);
    defaultScaleAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    // 初始化内部组件
    vl = new QVBoxLayout(this);
    vl->setContentsMargins(0, 3, 0, 0);
    vl->addSpacing(0);

    gv = new PictureBox(this);
    lb = new QLabel(this);
    // lb->adjustSize();

    // 显示图标
    // double defaultRatio = (double)default_size/image.size().width();
    pix=QPixmap(1,1);

    (( QGraphicsDropShadowEffect*)graphicsEffect())->setColor(QColor(255,0,0,255));

    gv->setBackground(QBrush (QColor(0,0,0,0)));
    gv->setVisible(true);
    // vl->setAlignment(Qt::AlignHCenter);

    // 添加布局
    vl->addStretch();
    vl->addWidget(gv);
    vl->setAlignment(gv, Qt::AlignCenter);
    // vl->setAlignment(gv, Qt::AlignVCenter);

    vl->addWidget(lb);
    vl->setAlignment(lb, Qt::AlignCenter);
    vl->addStretch();

    // 显示名字
    lb->setAlignment(Qt::AlignCenter);
    lb->setFont(QFont(qApp->font().family(), 10, 40));
    lb->setFixedWidth(width() - 5);
    lb->setText(elidedLineText(lb, 3, name));


    auto tem = displayColor();
    tem.setAlpha(icon_shadow_alpha);

    pix_shadow = new QGraphicsDropShadowEffect;
    pix_shadow->setColor(tem);
    pix_shadow->setBlurRadius(icon_shadow_blur_radius);   // 模糊半径
    pix_shadow->setOffset(0,0);      // 偏移量
    gv->setGraphicsEffect(pix_shadow);

    text_shadow = new QGraphicsDropShadowEffect;
    text_shadow->setColor(tem);
    text_shadow->setBlurRadius(10);   // 模糊半径
    text_shadow->setOffset(10);      // 偏移量
    text_shadow->setEnabled(enable_text_shadow);
    lb->setGraphicsEffect(text_shadow);


    SET_ANCTION(act1,选择图标,{
        QString tem =  QFileDialog::getOpenFileName(nullptr,tr("open a file."),"D:/");
        if(!tem.isEmpty()){
            setPix(tem,true);
            writeJson();
            gv->updateDispaly();
        }
    })

    SET_ANCTION(act2,切换铺满,{
        switchFullShowG(this);
    })

    connect(this,&SMultiFunc::nowDefaultScale_changed,this,[=](double val){
        whenFocusAnimationChange();
        // gv->setScale(scaleFix*scale*nowDefaultScale);
    });
}

void SMultiFunc::updateDefaultScale()
{
    double aimScale;

    if(simpleMode){
        // qDebug()<<"SimpleModeFix";
        aimScale =  0.60;
    }
    else{
        aimScale =  0.5;
        // qDebug()<<"NoSimpleModeFix";

    }
    if(layout!=nullptr&&!(layout->isMain)){
        aimScale*=1.1;
    }
    else{

    }


    defaultScaleAnimation->stop();
    defaultScaleAnimation->setStartValue(nowDefaultScale);
    defaultScaleAnimation->setEndValue(aimScale);
    defaultScaleAnimation->start();
}


void SMultiFunc::afterResize(QResizeEvent* event){
    SUnit::afterResize(event);
    lb->setFixedWidth(qBound(10, width()-5,9999));
    lb->setText(elidedLineText(lb,4,name));
    gv->updateDispaly();
}

void SMultiFunc::mouse_enter_action(){

}

void SMultiFunc::mouse_leave_action(){
    // 最终移动执行

}

void SMultiFunc::paintEvent(QPaintEvent *event)
{
    // qDebug()<<"painter";
    SUnit::paintEvent(event);
    // paintRect(this,displayColor_Alphaed());
}

QJsonObject SMultiFunc::to_json()
{
    QJsonObject rootObject = SUnit::to_json();
    rootObject.insert("name",name);
    rootObject.insert("pixPath",pixPath);
    rootObject.insert("fullShow",fullShow);
    return rootObject;
}

void SMultiFunc::load_json(QJsonObject rootObject)
{
    SUnit::load_json(rootObject);
    pixPath = rootObject.value("pixPath").toString();
    if(pixPath!="")
        SMultiFunc::setPix(path2Icon(pixPath)[0]);

    if(rootObject.contains("name")) setName(rootObject.value("name").toString());
    setFullShow(rootObject.value("fullShow").toBool());

}

void SMultiFunc::onMainColorChange(QColor val)
{
    SUnit::onMainColorChange(val);

    pix_shadow->setColor(val);
    text_shadow->setColor(val);

    pix_shadow->update();
    text_shadow->update();

    // if(dark){
    //     lb->setStyleSheet("color:white;");
    // }
    // else{
    //     lb->setStyleSheet("color:black;");
    // }
}

void SMultiFunc::endUpdate()
{
    SUnit::endUpdate();
    gv->requireRefresh = true;
    gv->updateDispaly();
    // onScaleChange(scale*scaleFix);
}

void SMultiFunc::setLongFocus(bool val)
{
    SUnit::setLongFocus(val);
    bool set = false;

    if(val&& !pCelectedUnits.empty()&&moving_global &&!pCelectedUnits.contains(this)){
        processor = this;
        onCelectedProcessor(true);
        set = true;
        qDebug()<<"set Processor"<<name;
    }
    else{
        if(processor!=nullptr){
            if(processor==this){
                qDebug()<<"release Processor"<<objectName();
                onCelectedProcessor(false);
                processor = nullptr;
            }
            else{

            }
        }
    }
    if(val){
        if(set)
            processorTip();
        else
            SToolTip::Tip(name);
    }

}

void SMultiFunc::processorTip()
{

}

void SMultiFunc::onProcessAnother(SUnit *another)
{
    if(another->inherits("SFile")){
        bool removed = ProcessPath(((SFile*)another)->filePath);
        if(removed){
            another->removeFromLayout();
            another->deleteLater();
        }
    }
}

bool SMultiFunc::ProcessPath(QString path)
{
    return true;
}

void SMultiFunc::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug()<<objectName()<<"DragEnter";
    processorTip();
    if(event->mimeData()->hasUrls())//判断数据类型
    {
        event->acceptProposedAction();//接收该数据类型拖拽事件
    }
    else
    {
        event->ignore();//忽略
    }
}

void SMultiFunc::dragLeaveEvent(QDragLeaveEvent *event)
{
    qDebug()<<objectName()<<"DragLeave";
    pmw->pls->clearTooltip();
}

void SMultiFunc::dropEvent(QDropEvent *event)
{
    qDebug()<<objectName()<<"Drop";
    if(event->mimeData()->hasUrls())//处理期望数据类型
    {
        QList<QUrl> list = event->mimeData()->urls();//获取数据并保存到链表中
        for(int i = 0; i < list.count(); i++)
        {
            ProcessPath(list[i].toLocalFile());
        }
    }
    else
    {
        event->ignore();
    }
}

void SMultiFunc::setFullShow(bool val)
{
    fullShow = val;
    if(val){
        vl->setContentsMargins(0, 0, 0, 0);
    }
    else{
        vl->setContentsMargins(0, 3, 0, 0);
    }
    if(fullShow||simpleMode)
        lb->setVisible(false);
    else
        lb->setVisible(true);


    gv->maxFill = val;
    onScaleChange(scale*scaleFix);
}

void SMultiFunc::onDragedOut()
{
    SUnit::onDragedOut();
    updateDefaultScale();
}



void SMultiFunc::preSetInLayout(bool animated)
{
    SUnit::preSetInLayout(animated);
    updateDefaultScale();
}

void SMultiFunc::onCelectedProcessor(bool val)
{

}

void SMultiFunc::onSimpleModeChange(bool val){

    updateDefaultScale();
    if(fullShow||simpleMode)
        lb->setVisible(false);
    else
        lb->setVisible(true);
}

void SMultiFunc::onScaleChange(double val){

    if(fullShow)
        gv->setScale(1.0*scaleFix);
    else
        gv->setScale(val*nowDefaultScale);
}

void SMultiFunc::setPix(QString pixPath,bool save)
{
    if(save)
    this->pixPath = pixPath;
    setPix(path2Icon(pixPath)[0]);
}

void SMultiFunc::setPix(QPixmap pixmap)
{
    pix = pixmap;

    setMainColor(pixmapMainColor(pixmap));

    onScaleChange(scale*scaleFix);
    gv->follow(&pix);
    gv->updateDispaly();
    update();
}

void SMultiFunc::setName(QString sname)
{
    if(sname == "")return;
    name = sname;
    setObjectName("SMultiFunc-"+name);
    lb->setText(sname);
}


