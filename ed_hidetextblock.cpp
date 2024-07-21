#include "ed_hidetextblock.h"
#include"QDesktopServices"
#include "SysFunctions.h"
#include "filefunc.h"

ED_HideTextBlock::ED_HideTextBlock(QWidget *parent, int sizex, int sizey) : ED_Block(parent, sizex, sizey)
{
    deepColor =true;
    lb->setVisible(false);
    vl->setMargin(0);
    gv->requireFill = true;
    gv->maxFill = true;
    gv->setScale(1.0);
}
ED_HideTextBlock::ED_HideTextBlock(QWidget *parent, QPixmap image, QString _name, QString filepath, int sizex, int sizey):ED_HideTextBlock(parent,sizex,sizey){
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

void ED_HideTextBlock::whenSimpleModeChange(bool val)
{

}

void ED_HideTextBlock::whenScaleChange(double val)
{
    // qDebug()<<"HSCalled";
    gv->setScale(1.0*scaleFix);
}

void ED_HideTextBlock::loadFromMyFI(MyFileInfo info)
{
    ED_Block::loadFromMyFI(info);
    if(info.okForAim()){
        iconmap = info.icons[default_steam_icon_type];
    }
    else{
        iconmap = info.icons[0];
    }
    gv->follow(&iconmap);
}

// void ED_HideTextBlock::ed_update()
// {
//     ED_Unit::ed_update();
//     // gv->setScale(1.0);
// }
