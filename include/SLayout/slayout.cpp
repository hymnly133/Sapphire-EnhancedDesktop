#include "slayout.h"
#include "SysFunctions.h"
#include "mainwindow.h"
#include "qtimer.h"
#include<algorithm>
#include"QFuture"
#include "scontainer.h"
#include "screenfunc.h"
#include<QtConcurrent/QtConcurrent>


int cmp(const SUnit* a,const SUnit* b)
{
    SUnit* p1=(SUnit*)a;
    SUnit* p2=(SUnit*)b;
    //将a和b强制类型转换后分别存入int型指针变量p1和p2
    return *p1<*p2;
}

void SLayout::setStandalongRect(QRect rect)
{
    useStandaloneRect = true;
    standaloneRect = rect;
}

SLayout::SLayout(QWidget *father)
{
    pContainer = father;
    if(father->inherits("SContainer")){
        if(((SContainer*)father)->pmw!=nullptr){
            pmw = ((SContainer*)father)->pmw;
            qDebug()<<"settedpmw"<<pmw->objectName();
        }
    }
    else{
            pmw = (MainWindow*)father;
    }

}



QPoint SLayout::pos2Ind(QPoint point)
{
    return pos2Ind(point.x(),point.y());
}

QPoint SLayout::ind2CenterPoint(QPoint ind)
{
    return ind2CenterPoint(ind.x(),ind.y());
}

QPoint SLayout::ind2CenterPoint(int x, int y)
{
    QPoint c;
    QPoint pos = ind2Pos(x,y);
    QSize size = ind2Size(x,y);
    c.setX(pos.x()+size.width()/2);
    c.setY(pos.y()+size.height()/2);
    return c;
}

QSize SLayout::ind2Size(QPoint ind)
{
    return ind2Size(ind.x(),ind.y());
}

QPoint SLayout::ind2Pos(QPoint ind)
{
    return ind2Pos(ind.x(),ind.y());
}



bool SLayout::Occupied(QPoint ind)
{
    return Occupied(ind.x(),ind.y());
}

void SLayout::putUnit(SUnit *aim, int xind, int yind, bool animated)
{
    updateBeforePut(aim,xind,yind);

    aim->indX = xind;
    aim->indY = yind;

    aim->layout = this;
    aim->setPMW(pmw);

    qDebug()<<"setted pmw"<<pmw->objectName();


    //setRect
    if(isMain){

    }
    else{
        aim->showLight =true;
        aim->showRect = true;
    }

    contents->push_back(aim);
    if(aim->inherits("SContainer"))
        insideContainers->push_back((SContainer*)aim);


    aim->preSetInLayout(animated);

    updateBeforePutAnimation(aim,xind,yind);
    if(!onLoading)
    writeJson();
    if(enable_background_blur) UpdateRegion();
}

void SLayout::putUnit(SUnit *aim, QPoint ind, bool animated)
{
    putUnit(aim,ind.x(),ind.y(),animated);
}

void SLayout::RemoveAUnit(SUnit *aim)
{
    QPoint tempos = aim->mapTo(pmw,QPoint(0,0));
    int indx = aim->indX;
    int indy = aim->indY;

    updateBeforeRemove(aim,indx,indy);

    aim->indX = -1;
    aim->indY = -1;
    aim->layout = nullptr;
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

    if(aim->inherits("SContainer")){
        insideContainers->removeOne((SContainer*)aim);
    }


    updateAfterRemove(aim,indx,indy);
}

void SLayout::clearPut(SUnit *aim,bool animated)
{
    if(OKForClearPut(aim)){
        putUnit(aim,clearPutableInd(aim),animated);
    }
    else{
        qDebug()<<"No Area";
    }
}
bool SLayout::OKForClearPut(SUnit *aim)
{
    if(clearPutableInd(aim)==QPoint(-1,-1)) return false;
    else return true;
}

bool SLayout::OKForDefaultPut(SUnit *aim)
{
    if(defaultPutableInd(aim)==QPoint(-1,-1)) return false;
    else return true;
}

SUnit *SLayout::ind2Unit(QPoint ind)
{
    return ind2Unit(ind.x(),ind.y());
}

void SLayout::defaultPut(SUnit *aim, bool animated)
{
    QPoint dis = defaultPutableInd(aim);
    putUnit(aim,dis,animated);
}

void SLayout::setVisible(bool val, bool force)
{
    int countt =0;
    if(force){
        foreach(SUnit* unit,*contents){

            unit->setVisible(val);
            countt ++;

        }
    }
    else{
        foreach (SUnit* unit,*contents) {
            if(!val&&unit->alwaysShow) continue;
            unit->setVisible(val);
            countt ++;
        }
    }

    visibal = val;
    UpdateRegion();
    pContainer->update();
    qDebug()<<"setted"<<countt<<" "<<val;
}

void SLayout::UpdateRegion()
{
    int countt =0;
    region = QRegion();
    for(SUnit* content:*(contents)){
        if(content->isVisible()){
            auto tem = content->mapToGlobal(QPoint(0,0));
            region = region.united(QRegion(tem.x(),tem.y(),content->width(),content->height()));
            countt++;
        }

    }
    qDebug()<<"Region count"<<countt;
}

void SLayout::UpdateContentPositon(bool animated)
{
    foreach (SUnit* content, *contents) {
        content->updateInLayout(animated);
    }
}

void SLayout::updateAfterPut(SUnit *, int, int)
{

}

void SLayout::updateBeforePutAnimation(SUnit *, int, int)
{

}

void SLayout::updateAfterRemove(SUnit *, int, int)
{

}

void SLayout::updateBeforePut(SUnit *, int, int)
{

}

void SLayout::updateBeforeRemove(SUnit *, int, int)
{

}

void SLayout::load_json(QJsonObject rootObject)
{
    contents->clear();
    insideContainers->clear();

    QJsonArray contentsArray = rootObject.value("contents").toArray();
    QVector<SUnit*> tem;
    if(rootObject.contains("useStandaloneRect")) useStandaloneRect = rootObject.value("useStandaloneRect").toBool();
    if(useStandaloneRect){
        if(rootObject.contains("x"))
        standaloneRect.setX(rootObject.value("x").toInt());
        if(rootObject.contains("y"))
        standaloneRect.setY(rootObject.value("y").toInt());
        if(rootObject.contains("w"))
        standaloneRect.setWidth(rootObject.value("w").toInt());
        if(rootObject.contains("h"))
        standaloneRect.setHeight(rootObject.value("h").toInt());
    }

    foreach (QJsonValue contentValue , (contentsArray)) {
        QJsonObject contentObject = contentValue.toObject();
        SUnit* unit = from_json(contentObject,this);
        if(unit==nullptr) continue;
        if(unit->indX==-1){
            defaultPut(unit,false);
        }
        else
        putUnit(unit,unit->indX,unit->indY,false);
    }

    // std::sort(tem.begin(),tem.end(),cmp);
    // foreach (SUnit* aim , tem) {
    //     putUnit(aim,aim->indX,aim->indY,false);
    // }
}

QJsonObject SLayout::to_json()
{
    QJsonObject rootObject;
    QJsonArray contentsArray;

    foreach (SUnit* content, *(contents)) {
        contentsArray.append(content->to_json());
    }

    rootObject.insert("contents",contentsArray);
    rootObject.insert("useStandaloneRect",useStandaloneRect);
    if(useStandaloneRect){
        rootObject.insert("x",standaloneRect.x());
        rootObject.insert("y",standaloneRect.y());
        rootObject.insert("w",standaloneRect.width());
        rootObject.insert("h",standaloneRect.height());
    }

    return rootObject;
}

void SLayout::say()
{

}
