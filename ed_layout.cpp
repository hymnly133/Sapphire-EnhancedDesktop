#include "ed_layout.h"
#include "SysFunctions.h"
#include "mainwindow.h"
#include "qdebug.h"
#include<cmath>



ED_Layout::ED_Layout(QWidget *father, int row, int col, int borad_space,int space_x,int space_y) {
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


QPoint ED_Layout::NearestBlockInd(QPoint point){
    return QPoint((point.x()-space)/(W_Block_Clean()+spaceX),(point.y()-space)/(H_Block_Clean()+spaceY));
}
QPoint ED_Layout::NearestBlockInd(int posx,int posy){
    return QPoint((posx-space)/(W_Block_Clean()+spaceX),(posy-space)/(H_Block_Clean()+spaceY));
}

//从Block序号获取中心坐标
QPoint ED_Layout::BlockInd2CenterPoint(QPoint ind){
    return blocks[ind.x()][ind.y()]->CenterPoint();
}
QPoint ED_Layout::BlockInd2CenterPoint(int x,int y){
    return blocks[x][y]->CenterPoint();
}

//从Block序号获取是否占用
bool& ED_Layout::Occupied(QPoint ind){
    if(ind.x()<0||ind.y()<0||ind.x()>=row||ind.y()>=col)
    {
        bool x=false;
        return x;
    }
    return blocks[ind.x()][ind.y()]->occupied;
}
bool& ED_Layout::Occupied(int x,int y){
    if(x<0||y<0||x>=row||y>=col)
    {
        bool x=false;
        return x;
    }
    return blocks[x][y]->occupied;
}

bool ED_Layout::OKForUnit(ED_Unit* aim,int xind,int yind){
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

//将一个ED_Unit放置在Block中，并在双方的变量中纪律以便之后识别
void ED_Layout::put_ED_Unit(ED_Unit* aim,QPoint ind){
    return put_ED_Unit(aim,ind.x(),ind.y());
}

void ED_Layout::put_ED_Unit(ED_Unit* aim,int xind,int yind){
    aim->setParent(pContainer);
    int x = aim->sizeX;
    int y = aim->sizeY;
    for(int i=0;i<x;i++){
        for(int k=0;k<y;k++){
            Occupied(xind+i,yind+k) = true;
            blocks[xind+i][yind+k]->content = aim;
        }
    }
    aim->indX = xind;
    aim->indY = yind;

    aim->setFixedSize(W_Block_Clean()*aim->sizeX+(aim->sizeX-1)*spaceX,H_Block_Clean()*aim->sizeY+(aim->sizeY-1)*spaceY);
    aim->move(blocks[xind][yind]->posX(),blocks[xind][yind]->posY());

    aim->update_after_resize();
    aim->edlayout = this;
    aim->setVisible(true);
    if(isMain){
        aim->showRect = true;
        aim->showLight = true;
    }
    else{
        aim->showRect = false;
        aim->showLight = false;
    }

    pContainer->raise();
    aim->raise();

    contents->push_back(aim);

    qDebug()<<"Put Done,Container Pos:"<<pContainer->pos()<<"Aim geometry "<<aim->geometry()<<"Pos: "<<aim->pos();
    if(enable_background_blur) Update_Region();
    aim->ed_update();
}

void ED_Layout::Update_Region(){
    int countt =0;
    region = QRegion();
    for(ED_Unit* content:*(contents)){
        // qDebug()<< content->isVisible();
        if(content->isVisible()){
            auto tem = content->mapToGlobal(QPoint(0,0));
            region = region.united(QRegion(tem.x(),tem.y(),content->width(),content->height()));
            // qDebug()<<content->geometry();
            countt++;
        }

    }
    qDebug()<<"Region count"<<countt;
}
void ED_Layout::RemoveAUnit(ED_Unit* aim){
    int x = aim->sizeX;
    int y = aim->sizeY;
    int xind = aim->indX;
    int yind = aim->indY ;
    QPoint tempos = aim->mapToGlobal(QPoint(0,0));
    for(int i=0;i<x;i++){
        for(int k=0;k<y;k++){
            Occupied(xind+i,yind+k) = false;
            blocks[xind+i][yind+k]->content = NULL;
        }
    }
    aim->indX = -1;
    aim->indY = -1;
    aim->edlayout = nullptr;
    aim->setParent(pmw);
    aim->move(tempos);
    aim->setEnabled(true);
    aim->setVisible(true);
    aim->raise();
    auto s = std::find(contents->begin(), contents->end(), aim);//第一个参数是array的起始地址，第二个参数是array的结束地址，第三个参数是需要查找的值
    if (s != contents->end())//如果找到，就输出这个元素
    {
        contents->erase(s);
    }
    else//如果没找到
    {
        qDebug() << "not find!";
    }
    Update_Region();
    qDebug()<<"Removed";
}

void ED_Layout::InplaceAUnit(ED_Unit* aim){
    QPoint absolutePos =  aim->mapToGlobal(QPoint(0, 0));
    QPoint relativePos = absolutePos-pContainer->pos();
    QPoint dis = NearestEmptyBlockInd(aim,relativePos);
    // qDebug()<<absolutePos<<relativePos<<dis;
    aim->move(absolutePos);
    put_ED_Unit(aim,dis);
}

void ED_Layout::InitAUnit(ED_Unit* aim){
    default_Put_ED_Unit(aim);
    aim->update_after_resize();
    aim->raise();
}

//根据一个Block索引获取对应的ED_Unit指针
ED_Unit* ED_Layout::getUnitFromBlock(int xind,int yind)
{
    if(Occupied(xind,yind))
    {
        return blocks[xind][yind]->content;
    }
    else
    {
        return nullptr;
    }
    //若无返回空指针
}
ED_Unit* ED_Layout::getUnitFromBlock(QPoint ind)
{
    return getUnitFromBlock(ind.x(),ind.y());
}
//将一个ED_Unit按序号最下且可放置的位置放置

void ED_Layout::default_Put_ED_Unit(ED_Unit* aim)
{
    for(int j=0;j<col;j++)
    {
        for(int i=0;i<row;i++)
        {
            if(OKForUnit(aim,i,j))
            {
                put_ED_Unit(aim,i,j);
                return;
            }
        }
    }
}
QPoint ED_Layout::NearestEmptyBlockInd(ED_Unit* aim,QPoint pos)
{
    return NearestEmptyBlockInd(aim,pos.x(),pos.y());
}

bool ED_Layout::OKforput(ED_Unit*aim)
{
    bool flag=false;
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            if(OKForUnit(aim,i,j))
                flag=true;
        }
    }
    return flag;
}

QPoint ED_Layout::NearestEmptyBlockInd(ED_Unit* aim,int posx,int posy)
{
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


void ED_Layout::setVisible(bool val){
    int countt =0;
    for(ED_Unit* unit:*contents){
        if(val==true || !unit->alwaysShow){
            unit->setVisible(val);
            countt ++;
        }
    }
    visibal = val;
    Update_Region();
    pContainer->update();
    qDebug()<<"setted"<<countt<<" "<<val;
}
bool ED_Layout::Visible(){
    return visibal;
}
void ED_Layout::Update_Positon(){
    for(ED_Unit* aim:*contents){
        aim->setFixedSize(W_Block_Clean()*aim->sizeX+(aim->sizeX-1)*spaceX,H_Block_Clean()*aim->sizeY+(aim->sizeY-1)*spaceY);
        aim->move(blocks[aim->indX][aim->indY]->posX(),blocks[aim->indX][aim->indY]->posY());


    }
}


QJsonObject ED_Layout::to_json()
{
    QJsonObject rootObject;
    rootObject.insert("row",row);
    rootObject.insert("col",col);
    rootObject.insert("space",space);
    rootObject.insert("spaceX",spaceX);
    rootObject.insert("spaceY",spaceY);
    QJsonArray contentArray;
    foreach (ED_Unit* content, *(contents)) {
        contentArray.append(content->to_json());
    }
    rootObject.insert("contents",contentArray);
    return rootObject;
}

void ED_Layout::load_json(QJsonObject rootObject)
{

    for(int i=0;i<row;i++){
        for(int k=0;k<col;k++){
            delete blocks[i][k];
        }
    }
    contents->clear();

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

    QJsonArray contentArray = rootObject.value("contents").toArray();
    foreach (QJsonValue contentValue , (contentArray)) {
        QJsonObject contentObject = contentValue.toObject();
        ED_Unit* unit = from_json(contentObject);
        put_ED_Unit(unit,unit->indX,unit->indY);
    }


}
