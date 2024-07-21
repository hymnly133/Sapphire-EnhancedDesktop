#include "ed_block.h"
#include "SysFunctions.h"
#include "QWidget"
#include"QProcess"
#include "filefunc.h"
#include "qboxlayout.h"
#include"SysFunctions.h"
#include "qevent.h"
#include "qfileinfo.h"
#include "qlabel.h"
#include"QDebug"
#include"QUrl"
#include"QDesktopServices"
#include"QTextCodec"
#include "qmimedata.h"
#include "qpainter.h"
#include"QGraphicsDropShadowEffect"
#include"ContextMenu/contextmenu.h"
#include"QFileDialog"
#include"edtooltip.h"

#define SET_ANCTION(NAME,TEXT,FUCTION)\
QAction *NAME = new QAction(#TEXT);\
    myMenu->addAction(NAME);\
    connect(NAME, &QAction::triggered, this, [=]()FUCTION);


void ED_Block::updateDefaultScale()
{
    double aimScale;
    if(layout!=nullptr&&!(layout->isMain)){
        aimScale =  0.65;
    }
    else{
        aimScale =  0.5;
    }

    defaultScaleAnimation->stop();
    defaultScaleAnimation->setStartValue(nowDefaultScale);
    defaultScaleAnimation->setEndValue(aimScale);
    defaultScaleAnimation->start();
}

ED_Block::ED_Block(QWidget *parent, int sizex, int sizey):ED_Unit(parent, sizex, sizey){
    filePath = "empty";
    name = "empty";

    defaultScaleAnimation = new QPropertyAnimation(this,"nowDefaultScale");
    defaultScaleAnimation->setDuration(200);
    defaultScaleAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    // 初始化内部组件
    vl = new QVBoxLayout(this);
    vl->setContentsMargins(0, 5, 0, 5);
    vl->addSpacing(0);

    gv = new PictureBox(this);
    lb = new QLabel(this);
    // lb->adjustSize();

    // 显示图标
    // double defaultRatio = (double)default_size/image.size().width();
    iconmap=QPixmap(1,1);
    setMainColor(pixmapMainColor(iconmap,sleep_color_ratio));

    (( QGraphicsDropShadowEffect*)graphicsEffect())->setColor(mainColor);
    gv->setBackground(QBrush (QColor(0,0,0,0)));
    gv->setVisible(true);
    // vl->setAlignment(Qt::AlignHCenter);

    // 添加布局
    // vl->addStretch();
    vl->addWidget(gv);
    vl->setAlignment(gv, Qt::AlignCenter);
    // vl->setAlignment(gv, Qt::AlignVCenter);

    vl->addWidget(lb);
    vl->setAlignment(lb, Qt::AlignCenter);
    // vl->addStretch();

    // 显示名字
    lb->setAlignment(Qt::AlignCenter);
    lb->setFont(QFont("MiSans", 10, 40));
    lb->setFixedWidth(width() - 5);
    lb->setText(elidedLineText(lb, 3, name));


    auto tem = mainColor;
    tem.setAlpha(icon_shadow_alpha);

    icon_shadow = new QGraphicsDropShadowEffect;
    icon_shadow->setColor(tem);
    icon_shadow->setBlurRadius(10);   // 模糊半径
    icon_shadow->setOffset(10);      // 偏移量
    lb->setGraphicsEffect(icon_shadow);

    text_shadow = new QGraphicsDropShadowEffect;
    text_shadow->setColor(tem);
    text_shadow->setBlurRadius(icon_shadow_blur_radius);   // 模糊半径
    text_shadow->setOffset(0, 0);      // 偏移量
    gv->setGraphicsEffect(text_shadow);

    SET_ANCTION(act1,选择图标,{
        QString tem =  QFileDialog::getOpenFileName(pls,tr("open a file."),"D:/");
        if(!tem.isEmpty()){
            setIcon(tem);
            writeJson();
        }
    })


    connect(this,&ED_Block::nowDefaultScale_changed,this,[=](){
        update();
        gv->setScale(scaleFix*scale*nowDefaultScale);
    });

    ed_update();

}

ED_Block::ED_Block(QWidget *parent, QPixmap image, QString _name, QString filepath, int sizex, int sizey)
    : ED_Block(parent, sizex, sizey)
{
    setAcceptDrops(true);
    filePath = filepath;
    if(QFileInfo(filepath).isDir()) isDir = true;
    name = _name;
    setObjectName("EDBlock-"+name);
    // 初始化内部组件
    // 显示图标
    iconmap=image;
    setMainColor(pixmapMainColor(iconmap,sleep_color_ratio));

    ((QGraphicsDropShadowEffect*)graphicsEffect())->setColor(mainColor);

    gv->follow(&iconmap);

    lb->setText(elidedLineText(lb, 3, name));

    auto tem = mainColor;
    tem.setAlpha(icon_shadow_alpha);
    icon_shadow->setColor(tem);
    text_shadow->setColor(tem);

}

ED_Block::ED_Block(QWidget *parent, QString path, int sizeX, int sizeY):ED_Block(parent,sizeX,sizeY)
{
    loadFromPath(path);
}
//---------------------------------------------------

void ED_Block::afterResize(QResizeEvent* event){
    ED_Unit::afterResize(event);
    lb->setFixedWidth(qBound(10, width()-5,9999));
    lb->setText(elidedLineText(lb,4,name));
    gv->updateDispaly();
}

void ED_Block::mouse_enter_action(){
    ED_Unit::mouse_enter_action();
    mainColor = pixmapMainColor(iconmap, active_color_ratio);
    qDebug()<<rs->size();
    // 文件预览
    QFileInfo fileInfo(filePath); // 去掉 "file:///"
    qDebug() << "File info: " << fileInfo.filePath()<<scale<<gv->m_scale<<gv->displaySize<<gv->actualSize; // 调试信息
    // if (fileInfo.isFile()) {
    //     if (fileInfo.suffix() == "txt") {
    //         QFile file(fileInfo.filePath());
    //         if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //             QTextStream in(&file);
    //             in.setCodec("UTF-8"); // 设置为UTF-8编码
    //             QString content = in.readAll();
    //             file.close();

    //             // 如果UTF-8读取失败，尝试其他编码
    //             if (content.contains(QChar(0xFFFD))) { // 检查是否有替换字符
    //                 file.open(QIODevice::ReadOnly | QIODevice::Text);
    //                 in.setCodec("GBK"); // 尝试GBK编码
    //                 content = in.readAll();
    //                 file.close();
    //             }

    //             // 限制预览内容的长度
    //             if (content.length() > 1000) {
    //                 content = content.left(1000) + "\n...";
    //             }

    //             previewWidget->setText(content);
    //             previewWidget->move(mapToGlobal(QPoint(0, height())));
    //             previewWidget->show();
    //             qDebug() << "Text file preview shown"; // 调试信息
    //         }
    //     } else if (fileInfo.suffix() == "png") {
    //         QImage image(fileInfo.filePath());
    //         previewWidget->setImage(image);
    //         previewWidget->move(mapToGlobal(QPoint(0, height())));
    //         previewWidget->show();
    //         qDebug() << "Image file preview shown"; // 调试信息
    //     }
    // }
}

void ED_Block::mouse_leave_action(){
    // 最终移动执行
    ED_Unit::mouse_leave_action();

    mainColor = pixmapMainColor(iconmap, sleep_color_ratio);
    // 文件预览
    // previewWidget->hide();
    // 隐藏视频预览
}

void ED_Block::double_click_action(){
    //最终双击执行
    QString cmd= QString("file:///")+filePath;
    qDebug("cmd = %s",qPrintable(cmd));
    QDesktopServices::openUrl(QUrl(cmd));
}

void ED_Block::paintEvent(QPaintEvent *event)
{
    // qDebug()<<"painter";
    ED_Unit::paintEvent(event);
    QColor alphaed = mainColor_Alphaed();
    paintRect(this,alphaed);
}

QJsonObject ED_Block::to_json()
{
    QJsonObject rootObject = ED_Unit::to_json();
    rootObject.insert("path",filePath);
    rootObject.insert("requireIcon",requireIcon);
    rootObject.insert("iconPath",iconPath);
    return rootObject;
}

void ED_Block::load_json(QJsonObject rootObject)
{
    ED_Unit::load_json(rootObject);
    loadFromPath(rootObject.value("path").toString());
    requireIcon = rootObject.value("requireIcon").toBool();
    if(requireIcon){
        setIcon(rootObject.value("iconPath").toString());
    }
}

void ED_Block::whenMainColorChange(QColor val)
{
    ED_Unit::whenMainColorChange(val);
    if(dark){
        lb->setStyleSheet("color:white;");
    }
    else{
        lb->setStyleSheet("color:black;");
    }
}

void ED_Block::ed_update()
{
    ED_Unit::ed_update();
    whenScaleChange(scale);
}

void ED_Block::setLongFocus(bool val)
{
    ED_Unit::setLongFocus(val);
    bool set = false;

    if(val&&pMovingUnit!=nullptr&&pMovingUnit!=this){
        pmw->processor = this;
        set = true;
        qDebug()<<"set Processor"<<filePath;
    }
    else{
        if(pmw->processor!=nullptr){
            if(pmw->processor==this){
                qDebug()<<"release Processor"<<filePath;
                pmw->processor = nullptr;
            }
            else{

            }
        }
    }
    if(val){
        if(set)
            tip();
        else
            EDToolTip::Tip(name);
    }

}

void ED_Block::tip()
{
    if(isDir)
        EDToolTip::Tip("移动至“"+name+"”");
    else
        EDToolTip::Tip("用“"+name+"”打开");;
}

void ED_Block::onShiftContextMenu(QContextMenuEvent *event)
{
    qDebug()<<filePath;
    ContextMenu::show(QStringList() << filePath, (void *)pls->winId(), event->globalPos());
}

void ED_Block::onProcessAnother(ED_Unit *another)
{
    if(another->inherits("ED_Block")){
        if(ProcessPath(((ED_Block*)another)->filePath)){
            another->removeFromLayout();
            another->deleteLater();
        };
    }
    if(pmw->processor!=nullptr){
        pmw->processor = nullptr;
    }
}

bool ED_Block::ProcessPath(QString path)
{
    qDebug()<<path<<filePath;
    bool removed = false;
    if(isDir){
        QString newName =  filePath+"/"+QFileInfo(path).fileName();
        removed = QFile::rename(path,newName);
        qDebug()<<"new name"<<newName;

    }
    else{
        QProcess* CalcPro = new QProcess(this);;
        CalcPro->start("cmd",QStringList()<<"/c"<<filePath<<path);
    }
    return removed;
}

void ED_Block::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug()<<objectName()<<"DragEnter";
    tip();
    if(event->mimeData()->hasUrls())//判断数据类型
    {
        event->acceptProposedAction();//接收该数据类型拖拽事件
    }
    else
    {
        event->ignore();//忽略
    }
}

void ED_Block::dragLeaveEvent(QDragLeaveEvent *event)
{
    qDebug()<<objectName()<<"DragLeave";
    pls->Clear();
}

void ED_Block::dropEvent(QDropEvent *event)
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

void ED_Block::onDragedOut(QMouseEvent *event)
{
    ED_Unit::onDragedOut(event);
    updateDefaultScale();
}

void ED_Block::preSetInLayout(bool animated)
{
    ED_Unit::preSetInLayout(animated);
    updateDefaultScale();
}

void ED_Block::whenSimpleModeChange(bool val){
    lb->setVisible(!val);
}

void ED_Block::whenScaleChange(double val){
    gv->setScale(val*nowDefaultScale);
}

void ED_Block::setIcon(QString iconPath)
{
    iconmap = path2Icon(iconPath)[0];
    requireIcon = true;
    this->iconPath = iconPath;
}

void ED_Block::loadFromMyFI(MyFileInfo info){
    filePath = info.filePath;
    if(QFileInfo(filePath).isDir()) isDir = true;
    name = info.name;
    setObjectName("EDBlock-"+name);
    iconmap=info.icons[0];
    requireIcon = false;


    setMainColor(pixmapMainColor(iconmap,sleep_color_ratio));

    ((QGraphicsDropShadowEffect*)graphicsEffect())->setColor(mainColor);
    gv->follow(&iconmap);
    lb->setText(elidedLineText(lb, 3, name));

    auto tem = mainColor;
    tem.setAlpha(icon_shadow_alpha);
    icon_shadow->setColor(tem);
    text_shadow->setColor(tem);
}

void ED_Block::loadFromPath(QString filepath)
{
    qDebug()<<"Loading form path:"<<filepath;
    MyFileInfo info = path2MyFI(filepath);
    loadFromMyFI(info);
}
