#include "ed_hidetextblock.h"
#include"QDesktopServices"
#include "SysFunctions.h"

ED_HideTextBlock::ED_HideTextBlock(QWidget *parent, int sizey, int sizex) : ED_Block(parent, sizex, sizey)
{
    deepColor =true;
    lb->setVisible(false);
    setScale(1.0);
    vl->setMargin(0);
}
ED_HideTextBlock::ED_HideTextBlock(QWidget *parent, QPixmap image, QString _name, QString filepath, int sizex, int sizey):ED_HideTextBlock(parent,sizex,sizey){
    filePath = filepath;
    name = _name;

    // 初始化内部组件
    // 显示图标
    iconmap=image;
    setMainColor(pixmapMainColor(iconmap,sleep_color_ratio));

    ((QGraphicsDropShadowEffect*)graphicsEffect())->setColor(mainColor);

    gv->setImage(image);


    lb->setText(elidedLineText(lb, 3, name));

    auto tem = mainColor;
    tem.setAlpha(icon_shadow_alpha);
    icon_shadow->setColor(tem);
    text_shadow->setColor(tem);
}

void ED_HideTextBlock::whenSimpleModeChange(bool val)
{

}

void ED_HideTextBlock::whenScaleChange(double val)
{
    // qDebug()<<"HSCalled";
    gv->setScale(1.0*scaleFix);
}

// void ED_HideTextBlock::ed_update()
// {
//     ED_Unit::ed_update();
//     // gv->setScale(1.0);
// }
