#include "sblocklayout.h"
#include "SysFunctions.h"
#include "screenfunc.h"

SBlockLayout::SBlockLayout(QWidget *father, int row, int col, int borad_space,int space_x,int space_y):SLayout(father) {
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

QPoint SBlockLayout::pos2Ind(int posx,int posy){
    int tem1 =W_Block_Clean();
    int tem2 = H_Block_Clean();
    return QPoint((posx-space)/(tem1+spaceX),(posy-space)/(tem2+spaceY));
}

//从Block序号获取中心坐标


QPoint SBlockLayout::defaultPutableInd(SUnit *aim)
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
    qDebug()<<"Unable to defaultput";
    return QPoint(-1,-1);
}

void SBlockLayout::updateAfterPut(SUnit * aim, int indx, int indy)
{
}

void SBlockLayout::updateAfterRemove(SUnit *aim, int indx, int indy)
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

QPoint SBlockLayout::clearPutableInd(SUnit *aim)
{
    auto pos = pContainer->mapFrom(aim->parentWidget(),aim->pos());
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

void SBlockLayout::updateBeforePut(SUnit * aim, int indx, int indy)
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



QSize SBlockLayout::ind2Size(int xind, int yind)
{
    auto aim = ind2Unit(xind,yind);
    int w = W_Block_Clean()*aim->sizeX+(aim->sizeX-1)*spaceX;
    int h = H_Block_Clean()*aim->sizeY+(aim->sizeY-1)*spaceY;

    return QSize(w,h);
}
QPoint SBlockLayout::ind2Pos(int xind, int yind)
{
    return QPoint(blocks[xind][yind]->posX(),blocks[xind][yind]->posY());
}

//从Block序号获取是否占用
bool SBlockLayout::Occupied(int x,int y){
    if(x<0||y<0||x>=row||y>=col)
    {
        return false;
    }
    return blocks[x][y]->occupied;
}

bool SBlockLayout::OKForUnit(SUnit* aim,int xind,int yind){
    int x = aim->sizeX;
    int y = aim->sizeY;
    if(x+xind>row)
        return false;
    if(y+yind>col)
        return false;
    for(int i=0;i<x;i++){
        for(int k=0;k<y;k++){
            if(Occupied(xind+i,yind+k)) return false;
        }
    }
    return true;
}

SUnit *SBlockLayout::ind2Unit(int xind, int yind)
{
    if(xind>=row||xind<0) return nullptr;
    else if (yind>=col||yind<0) return nullptr;
    return blocks[xind][yind]->content;
}


QJsonObject SBlockLayout::to_json()
{
    QJsonObject rootObject = SLayout::to_json();
    rootObject.insert("row",row);
    rootObject.insert("col",col);
    rootObject.insert("space",space);
    rootObject.insert("spaceX",spaceX);
    rootObject.insert("spaceY",spaceY);
    return rootObject;
}

void SBlockLayout::load_json(QJsonObject rootObject)
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

    SLayout::load_json(rootObject);
}
