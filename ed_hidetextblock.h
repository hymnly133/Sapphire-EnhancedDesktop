#ifndef ED_HIDETEXTBLOCK_H
#define ED_HIDETEXTBLOCK_H

#include "ed_block.h"
class ED_HideTextBlock:public ED_Block
{
    Q_OBJECT

public:
    // using ED_Block::ED_Block;
    explicit ED_HideTextBlock():ED_HideTextBlock(nullptr,1,1){};
    explicit ED_HideTextBlock(QWidget *parent, int sizex = 1, int sizey = 1);
    explicit ED_HideTextBlock(QWidget *parent, QPixmap image, QString _name, QString filepath, int sizex, int sizey);
    ED_HideTextBlock(const ED_HideTextBlock& other):ED_HideTextBlock(other.parentWidget(),other.iconmap,other.name,other.filePath,other.sizeX,other.sizeY){};
    void whenSimpleModeChange(bool val) override;
    void whenScaleChange(double) override;
    // void ed_update() override;




    // ED_Block interface
public:
    void loadFromMyFI(MyFileInfo info) override;
};
Q_DECLARE_METATYPE(ED_HideTextBlock)
#endif // ED_HIDETEXTBLOCK_H
