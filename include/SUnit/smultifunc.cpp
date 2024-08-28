#include "smultifunc.h"
#include"QProcess"
#include "SQSS.h"
#include "filefunc.h"
#include "guifunc.h"
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
#include "qmovie.h"
#include "qpainter.h"
#include"QGraphicsDropShadowEffect"
#include"QFileDialog"
#include "sfile.h"
#include "slabel.h"
#include"stooltip.h"
#include"global.h"
#include "stylehelper.h"
#include "unitfunc.h"
#include"snotice.h"



SMultiFunc::SMultiFunc(SLayout *dis, int sizex, int sizey): SUnit(dis, sizex, sizey)
{
    name = "empty";

    defaultScaleAnimation = new QPropertyAnimation(this, "nowDefaultScale");
    defaultScaleAnimation->setDuration(200);
    defaultScaleAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    // 初始化内部组件
    vl = new QVBoxLayout(this);
    vl->setContentsMargins(0, 3, 0, 0);
    vl->addSpacing(0);

    gv = new PictureBox(this);
    lb = new SLabel(this);
    // lb->adjustSize();

    // 显示图标
    // double defaultRatio = (double)default_size/image.size().width();
    pix = QPixmap(1, 1);

    gv->setBackground(QBrush (QColor(0, 0, 0, 0)));
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
    lb->setFixedWidth(width() - 5);




    pix_shadow = new QGraphicsDropShadowEffect;
    pix_shadow->setColor(applyAlpha(displayColor(), icon_shadow_alpha));
    pix_shadow->setBlurRadius(icon_shadow_blur_radius);   // 模糊半径
    pix_shadow->setOffset(0, 0);     // 偏移量
    connectTo(icon_shadow_blur_radius, int, int, {
        pix_shadow->setBlurRadius(value);
    })
    gv->setGraphicsEffect(pix_shadow);


    connect(this, &SMultiFunc::nowDefaultScale_changed, this, [ = ](double val) {
        whenFocusAnimationChange();
        // gv->setScale(scaleFix*scale*nowDefaultScale);
    });
}

void SMultiFunc::updateDefaultScale()
{
    double aimScale;

    if(simpleMode) {
        // qDebug()<<"SimpleModeFix";
        aimScale =  0.60;
    } else {
        aimScale =  0.5;
        // qDebug()<<"NoSimpleModeFix";

    }
    if(layout != nullptr && !(layout->isMain)) {
        aimScale *= 1.1;
    } else {

    }


    defaultScaleAnimation->stop();
    defaultScaleAnimation->setStartValue(nowDefaultScale);
    defaultScaleAnimation->setEndValue(aimScale);
    defaultScaleAnimation->start();
}


void SMultiFunc::afterResize(QResizeEvent* event)
{
    SUnit::afterResize(event);
    lb->setFixedWidth(qBound(10, width() - 5, 9999));
    setName(name);
    gv->updateDispaly();
}

void SMultiFunc::mouse_enter_action()
{
    // qDebug() << lb->opacity;
}

void SMultiFunc::mouse_leave_action()
{
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
    rootObject.insert("name", name);
    rootObject.insert("pixPath", pixPath);
    rootObject.insert("fullShow", fullShow);
    return rootObject;
}

void SMultiFunc::load_json(QJsonObject rootObject)
{
    SUnit::load_json(rootObject);
    pixPath = rootObject.value("pixPath").toString();
    if(pixPath != "") {
        if(QFile::exists(pixPath)) {
            SMultiFunc::setIconFromPath(pixPath, false);
        } else {
            SNotice::notice(pixPath, tr("预加载图片丢失！"));
            pixPath = "";
        }
    }

    if(rootObject.contains("name")) {
        setName(rootObject.value("name").toString());
    }
    setFullShow(rootObject.value("fullShow").toBool());

}

void SMultiFunc::onMainColorChange(QColor val)
{
    SUnit::onMainColorChange(val);

    pix_shadow->setColor(applyAlpha(displayColor(), icon_shadow_alpha));
    pix_shadow->update();
}

void SMultiFunc::endUpdate()
{
    pix_shadow->setColor(applyAlpha( displayColor(), icon_shadow_alpha));
    pix_shadow->update();
    SUnit::endUpdate();
    gv->requireRefresh = true;
    gv->updateDispaly();
    lb->setOpacity(1.0);
}

void SMultiFunc::setLongFocus(bool val)
{
    SUnit::setLongFocus(val);
    preSetProcessor(val);
    if(val)
        if(!isProcessor && showNameTip) {
            //没有设置为Processor,只是单纯的长聚焦的话
            SToolTip::tip(name);
        }
}

void SMultiFunc::preSetProcessor(bool val)
{
    //取消为即时操作

    if(!val) {
        setProcessor(val);
    } else {
        if(!isEnabled()) {
            return;
        }
        if(!pCelectedUnits.empty() && moving_global && !pCelectedUnits.contains(this)) {
            if(numCelected == 1 || (numCelected >= 1 && requireMulti)) {
                setProcessor(true);
            }
        } else {
            if(processor != nullptr && processor == this) {
                setProcessor(false);
            }
        }
    }


}

void SMultiFunc::setProcessor(bool val)
{
    if(val == isProcessor) {
        return;
    }

    if(val) {
        qDebug() << "Processor set:" << name;
        isProcessor = true;
        processor = this;
        processorTip();
    } else {
        qDebug() << "Processor unset:" << objectName();
        processor = nullptr;
        isProcessor = false;
    }
}

void SMultiFunc::processorTip()
{

}

void SMultiFunc::processAnother(SUnit *another)
{
    if(another->inherits("SFile")) {
        processFile(((SFile*)another));
    }
}

void SMultiFunc::processFile(SFile *sfile)
{
    processFile((SFileInfo*)sfile);
}

void SMultiFunc::processFile(SFileInfo *sfileInfo)
{

}

void SMultiFunc::processFile(QString path)
{
    processFile(new SFileInfo(nullptr, path));
}


void SMultiFunc::setupEditMenu()
{
    SUnit::setupEditMenu();
    SET_ANCTION(act1, tr("选择图标"), editMenu, this, {
        QString tem =  QFileDialog::getOpenFileName(nullptr, tr("选择一个文件"), "D:/");
        if(!tem.isEmpty())
        {
            setIconFromPath(tem, true);
            writeContent();
            // gv->updateDispaly();
        }
    })

    SET_ANCTION(act2, tr("切换铺满"), editMenu, this, {
        switchFullShowG(this);
    })

}

void SMultiFunc::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << objectName() << "DragEnter";
    processorTip();
    if(event->mimeData()->hasUrls()) { //判断数据类型
        event->acceptProposedAction();//接收该数据类型拖拽事件
    } else {
        event->ignore();//忽略
    }
}

void SMultiFunc::dragLeaveEvent(QDragLeaveEvent *event)
{
    qDebug() << objectName() << "DragLeave";
    pmw->pls->clearTooltip();
}

void SMultiFunc::dropEvent(QDropEvent *event)
{
    qDebug() << objectName() << "Drop";
    if(event->mimeData()->hasUrls()) { //处理期望数据类型
        QList<QUrl> list = event->mimeData()->urls();//获取数据并保存到链表中
        for(int i = 0; i < list.count(); i++) {
            processFile(list[i].toLocalFile());
        }
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void SMultiFunc::setFullShow(bool val)
{
    fullShow = val;
    if(fullShow || simpleMode) {
        lb->setVisible(false);

        vl->setContentsMargins(0, 0, 0, 0);
    } else {
        lb->setVisible(true);
        vl->setContentsMargins(0, 3, 0, 0);
    }


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


void SMultiFunc::updateColor()
{
    if(gv->type == PictureBox::pic) {
        setMainColor(pixmapMainColor(pix));
    } else {
        setMainColor(pixmapMainColor(gv->gifThread->gif->currentPixmap()));
    }
}

void SMultiFunc::whenFocusAnimationChange()
{
    pix_shadow->setColor(applyAlpha(displayColor(), icon_shadow_alpha));
    pix_shadow->update();
    SUnit::whenFocusAnimationChange();
}

void SMultiFunc::onSimpleModeChange(bool val)
{

    updateDefaultScale();
    if(fullShow || simpleMode) {
        lb->setVisible(false);
        vl->setContentsMargins(0, 0, 0, 0);
    } else {
        vl->setContentsMargins(0, 3, 0, 0);
        lb->setVisible(true);
    }
}

void SMultiFunc::onScaleChange(double val)
{

    if(fullShow) {
        gv->setScale(1.0 * scaleFix);
    } else {
        gv->setScale(val*nowDefaultScale);
    }
}

void SMultiFunc::setIconFromPath(QString iconPath, bool save)
{
    if(save) {
        this->pixPath = iconPath;
    }
    if(use_pic_as_icon && QFileInfo(pixPath).suffix().toLower() == "gif") {
        setGIF(iconPath);
    } else {
        setPix(path2Icon(iconPath));
    }

}

void SMultiFunc::setGIF(QString gifPath)
{
    // pix = pixmap;
    gv->setGIF(gifPath);
    connect(gv->gifThread, &QThread::started, this, [ = ]() {
        enableFocusScaleAnimation = false;
        updateColor();

        onScaleChange(scale*scaleFix);
        gv->updateDispaly();
        update();
    });

}

void SMultiFunc::setPix(QPixmap pixmap)
{
    enableFocusScaleAnimation = true;
    pix = pixmap;
    gv->follow(&pix);

    updateColor();

    onScaleChange(scale*scaleFix);
    gv->updateDispaly();
    update();
}

void SMultiFunc::setName(QString sname)
{
    if(sname == "") {
        return;
    }
    name = sname;
    setObjectName("SMultiFunc-" + name);
    lb->setText(elidedLineText(lb, 3, name));
}


