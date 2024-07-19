#include "ed_block.h"
#include "SysFunctions.h"
#include "QWidget"
#include"QProcess"
#include "qboxlayout.h"
#include"SysFunctions.h"
#include "qevent.h"
#include "qfileinfo.h"
#include "qlabel.h"
#include"QDebug"
#include"QUrl"
#include"QDesktopServices"
#include"QTextCodec"
#include "qpainter.h"
#include"QGraphicsDropShadowEffect"
#include"ContextMenu/contextmenu.h"
#include"QFileDialog"

#define SET_ANCTION(NAME,TEXT,FUCTION)\
QAction *NAME = new QAction(#TEXT);\
    myMenu->addAction(NAME);\
    connect(NAME, &QAction::triggered, this, [=]()FUCTION);


int ED_Block::default_size = 48;
ED_Block::ED_Block(QWidget *parent, int sizex, int sizey):ED_Unit(parent, sizex, sizey){
    filePath = "empty";
    name = "empty";

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
    setScale(1.0);

    SET_ANCTION(act1,选择图标,{
        QString tem =  QFileDialog::getOpenFileName(pmw,tr("open a file."),"D:/");
        if(!tem.isEmpty()){
            setIcon(tem);
            writeJson();
        }
    })
}

ED_Block::ED_Block(QWidget *parent, QPixmap image, QString _name, QString filepath, int sizex, int sizey)
    : ED_Block(parent, sizex, sizey)
{
    filePath = filepath;
    name = _name;
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
    ED_Unit::double_click_action();
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
    whenScaleChange(scale*scaleFix);
}

void ED_Block::onShiftContextMenu(QContextMenuEvent *event)
{
    qDebug()<<filePath;
    ContextMenu::show(QStringList() << filePath, (void *)pmw->winId(), event->globalPos());
}
void ED_Block::whenSimpleModeChange(bool val){
    lb->setVisible(!val);
}

void ED_Block::whenScaleChange(double val){
    gv->setScale(val*default_scale);
}

void ED_Block::setIcon(QString iconPath)
{
    iconmap = path2Icon(iconPath)[0];
    requireIcon = true;
    this->iconPath = iconPath;
}

void ED_Block::loadFromMyFI(MyFileInfo info){
    filePath = info.filePath;
    name = info.name;
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
