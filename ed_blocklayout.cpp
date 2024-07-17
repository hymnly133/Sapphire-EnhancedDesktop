#include "ed_blocklayout.h"
#include "qdebug.h"



ED_BlockLayout::ED_BlockLayout(QWidget *father, int row, int col, int borad_space,int space_x,int space_y):ED_Layout(father) {
    this->row = row;
    this->col = col;
    this->space = borad_space;
    this->spaceX = space_x;
    this->spaceY = space_y;
    this->pContainer = father;
    for(int i=0;i<row;i++){
        for(int k=0;k<col;k++){
            blocks[i][k] = new little_Block(this,i,k);
            blocks[i][k]->occupied = false;
        }
    }
    visibal = true;
}

QPoint ED_BlockLayout::pos2Ind(int posx,int posy){
    return QPoint((posx-space)/(W_Block_Clean()+spaceX),(posy-space)/(H_Block_Clean()+spaceY));
}

//从Block序号获取中心坐标

QPoint ED_BlockLayout::ind2CenterPoint(int x,int y){
    auto aim = ind2Unit(x,y);
    QPoint size(W_Block_Clean()*aim->sizeX+(aim->sizeX-1)*spaceX,H_Block_Clean()*aim->sizeY+(aim->sizeY-1)*spaceY);
    QPoint pos(blocks[x][y]->posX(),blocks[x][y]->posY());
    return (pos+size/2);
}

QSize ED_BlockLayout::ind2Size(int xind, int yind)
{
    auto aim = ind2Unit(xind,yind);
    int w = W_Block_Clean()*aim->sizeX+(aim->sizeX-1)*spaceX;
    int h = H_Block_Clean()*aim->sizeY+(aim->sizeY-1)*spaceY;
    return QSize(w,h);
}

QPoint ED_BlockLayout::defaultPutableInd(ED_Unit *aim)
{
    for(int j=0;j<col;j++)
    {
        for(int i=0;i<row;i++)
        {
            if(OKForUnit(aim,i,j))
            {
                return QPoint(i,j);
            }
        }
    }
    return QPoint(-1,-1);
}

void ED_BlockLayout::updateAfterPut(ED_Unit * aim, int indx, int indy)
{
}

void ED_BlockLayout::updateAfterRemove(ED_Unit *aim, int indx, int indy)
{
    int x = aim->sizeX;
    int y = aim->sizeY;
    for(int i=0;i<x;i++){
        for(int k=0;k<y;k++){
            blocks[indx+i][indy+k]->occupied = false;
            blocks[indx+i][indy+k]->content = nullptr;
        }
    }
}

QPoint ED_BlockLayout::clearPutableInd(ED_Unit *aim)
{
    auto pos = pContainer->mapFromGlobal(aim->pos());
    int posx = pos.x();
    int posy = pos.y();
    int mindeltaw=W_Container();
    int mindeltah=H_Container();
    int bpw,bph;
    bpw=bph=-1;
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            int deltaw=abs(posx-space-i*(W_Block_Clean()+spaceX));
            int deltah=abs(posy-space-j*(H_Block_Clean()+spaceY));
            if((deltaw+deltah<mindeltaw+mindeltah)&&(OKForUnit(aim,i,j)))
            {
                mindeltaw=deltaw;
                mindeltah=deltah;
                bpw=i;
                bph=j;
            }
        }
    }
    return QPoint(bpw,bph);

}

void ED_BlockLayout::updateBeforePut(ED_Unit * aim, int indx, int indy)
{
    int x = aim->sizeX;
    int y = aim->sizeY;
    for(int i=0;i<x;i++){
        for(int k=0;k<y;k++){
            blocks[indx+i][indy+k]->occupied = true;
            blocks[indx+i][indy+k]->content = aim;
        }
    }
}

QPoint ED_BlockLayout::ind2Pos(int xind, int yind)
{
    return QPoint(blocks[xind][yind]->posX(),blocks[xind][yind]->posY());
}

//从Block序号获取是否占用
bool ED_BlockLayout::Occupied(int x,int y){
    if(x<0||y<0||x>=row||y>=col)
    {
        return false;
    }
    return blocks[x][y]->occupied;
}

bool ED_BlockLayout::OKForUnit(ED_Unit* aim,int xind,int yind){
    int x = aim->sizeX;
    int y = aim->sizeY;
    if(x+xind>row)return false;
    if(y+yind>col) return false;
    for(int i=0;i<x;i++){
        for(int k=0;k<y;k++){
            if(Occupied(xind+i,yind+k)) return false;
        }
    }
    return true;
}

ED_Unit *ED_BlockLayout::ind2Unit(int xind, int yind)
{
    return blocks[xind][yind]->content;
}

//将一个ED_Unit放置在Block中，并在双方的变量中纪律以便之后识别

// void ED_BlockLayout::put_ED_Unit(ED_Unit* aim,int xind,int yind){
//     aim->setParent(pContainer);
//     int x = aim->sizeX;
//     int y = aim->sizeY;
//     for(int i=0;i<x;i++){
//         for(int k=0;k<y;k++){
//             Occupied(xind+i,yind+k) = true;
//             blocks[xind+i][yind+k]->content = aim;
//         }
//     }
//     aim->indX = xind;
//     aim->indY = yind;

//     aim->setFixedSize(W_Block_Clean()*aim->sizeX+(aim->sizeX-1)*spaceX,H_Block_Clean()*aim->sizeY+(aim->sizeY-1)*spaceY);
//     aim->move(blocks[xind][yind]->posX(),blocks[xind][yind]->posY());

//     aim->update_after_resize();
//     aim->edlayout = this;
//     aim->setVisible(true);
//     if(isMain){
//         aim->showRect = true;
//         aim->showLight = true;
//     }
//     else{
//         aim->showRect = false;
//         aim->showLight = false;
//     }

//     pContainer->raise();
//     aim->raise();

//     contents->push_back(aim);
//     if(aim->alwaysShow){
//         contents_AlwaysShow->push_back(aim);
//     }
//     else{
//         contents_Show->push_back(aim);
//     }

//     qDebug()<<"Put Done,Container Pos:"<<pContainer->pos()<<"Aim geometry "<<aim->geometry()<<"Pos: "<<aim->pos();
//     if(enable_background_blur) Update_Region();
//     // aim->ed_update();
// }


//将一个ED_Unit按序号最下且可放置的位置放置


QJsonObject ED_BlockLayout::to_json()
{
    QJsonObject rootObject = ED_Layout::to_json();
    rootObject.insert("row",row);
    rootObject.insert("col",col);
    rootObject.insert("space",space);
    rootObject.insert("spaceX",spaceX);
    rootObject.insert("spaceY",spaceY);
    return rootObject;
}

void ED_BlockLayout::load_json(QJsonObject rootObject)
{

    for(int i=0;i<row;i++){
        for(int k=0;k<col;k++){
            delete blocks[i][k];
        }
    }

    row = rootObject.value("row").toInt();
    col = rootObject.value("col").toInt();
    space = rootObject.value("space").toInt();
    spaceX = rootObject.value("spaceX").toInt();
    spaceY = rootObject.value("spaceY").toInt();


    for(int i=0;i<row;i++){
        for(int k=0;k<col;k++){
            blocks[i][k] = new little_Block(this,i,k);
            blocks[i][k]->occupied = false;
        }
    }

    ED_Layout::load_json(rootObject);
}
