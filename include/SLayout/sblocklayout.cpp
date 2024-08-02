#include "sblocklayout.h"
#include "SysFunctions.h"
#include "screenfunc.h"
#include"SNotice.h"


SBlockLayout::SBlockLayout(QWidget *father, int row, int col, double boradXR, double boradYR, double spaceXR, double spaceYR):SLayout(father)
{
    this->row = row;
    this->col = col;
    this->boradXR = boradXR;
    this->boradYR = boradYR;
    this->spaceXR = spaceXR;
    this->spaceYR = spaceYR;
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
    double tem1 =W_Block_Clean();
    double tem2 = H_Block_Clean();
    return QPoint((posx-boradXPix())/(tem1+spaceXPix()),(posy-boradXPix())/(tem2+spaceYPix()));
}

//从Block序号获取中心坐标
//从Block序号获取中心坐标


QPoint SBlockLayout::defaultPutableInd(SUnit *aim)
{
    // qDebug()<<aim->sizeX<<aim->sizeY;
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

void SBlockLayout::resize(int sizeX, int sizeY, double boradXR, double boradYR, double spaceXR, double spaceYR, bool animated)
{
    this->boradXR = boradXR;
    this->boradYR = boradYR;
    this->spaceXR = spaceXR;
    this->spaceYR = spaceYR;

    if(sizeX<=row&&sizeY<=col){
        //缩小
        qDebug()<<"缩小";

        QList<SUnit* > temp;
        for(int i=sizeX;i<row;i++){
            for(int j=0;j<this->col;j++){
                if(blocks[i][j]->occupied){
                    temp.append(blocks[i][j]->content);
                    blocks[i][j]->content->removeFromLayout();
                }
            }
        }
        this->row = sizeX;

        for(int i=sizeY;i<col;i++){
            for(int j=0;j<this->row;j++){
                if(blocks[j][i]->occupied){
                    temp.append(blocks[j][i]->content);
                    blocks[j][i]->content->removeFromLayout();
                }
            }
        }
        this->col = sizeY;

        foreach (SUnit* unit, temp) {
            clearPut(unit,true);
        }
        UpdateContentPositon();
        return;
    }
    else if(sizeX>=row&&sizeY>=col){
        //增大
        qDebug()<<"增大";
        for(int i=this->row;i<sizeX;i++){
            for(int j=0;j<this->col;j++){
                blocks[i][j] = new little_Block(this,i,j);
                blocks[i][j]->occupied = false;
            }
        }
        this->row = sizeX;

        for(int i=this->col;i<sizeY;i++){
            for(int j=0;j<this->row;j++){
                blocks[j][i] = new little_Block(this,j,i);
                blocks[j][i]->occupied = false;
            }
        }
        this->col = sizeY;

        UpdateContentPositon(animated);
        return;
    }
    else{
        //分成先缩小在增大
        resize(qMin(row,sizeX),qMin(col,sizeY));
        resize(qMax(row,sizeX),qMax(col,sizeY));
    }
    saveLayout();
    SNotice::notice(QStringList()<<"列数："+QString::number(sizeX)<<"行数："+QString::number(sizeY),"重布局",5000);
}

QPoint SBlockLayout::clearPutableInd(SUnit *aim)
{
    auto pos = pContainer->mapFromGlobal(aim->mapToGlobal(QPoint(0,0)));
    double posx = pos.x();
    double posy = pos.y();
    double mindeltaw=W_Container();
    double mindeltah=H_Container();
    double bpw,bph;
    bpw=bph=-1;
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            double deltaw=abs(posx-boradXPix()-i*(W_Block_Clean()+spaceXPix()));
            double deltah=abs(posy-boradYPix()-j*(H_Block_Clean()+spaceYPix()));
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

void SBlockLayout::printOccupied()
{
    for(int i=0;i<col;i++){
        QString buf;
        for(int j=0;j<row;j++){
            buf.append(QString::number(blocks[j][i]->occupied)).append(" ");
        }
        qDebug()<<buf;
    }
}



QSize SBlockLayout::ind2Size(int xind, int yind)
{
    auto aim = ind2Unit(xind,yind);
    double w = W_Block_Clean()*aim->sizeX+(aim->sizeX-1)*spaceXPix();
    double h = H_Block_Clean()*aim->sizeY+(aim->sizeY-1)*spaceYPix();

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
    // rootObject.insert("space",space);

    rootObject.insert("boradXR",boradXR);
    rootObject.insert("boradYR",boradYR);
    rootObject.insert("spaceXR",spaceXR);
    rootObject.insert("spaceYR",spaceYR);
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

    if(rootObject.contains("space")){
        //旧数据，进行转化
    }
    else{
        boradXR = rootObject.value("boradXR").toDouble();
        boradYR = rootObject.value("boradYR").toDouble();
        spaceXR = rootObject.value("spaceXR").toDouble();
        spaceYR = rootObject.value("spaceYR").toDouble();
    }




    for(int i=0;i<row;i++){
        for(int k=0;k<col;k++){
            blocks[i][k] = new little_Block(this,i,k);
            blocks[i][k]->occupied = false;
        }
    }

    SLayout::load_json(rootObject);
}
