#include "ed_layout.h"
#include "SysFunctions.h"
#include "mainwindow.h"
#include "qtimer.h"
#include<algorithm>
#include"QFuture"
#include "screenfunc.h"
#include<QtConcurrent/QtConcurrent>


int cmp(const ED_Unit* a,const ED_Unit* b)
{
    ED_Unit* p1=(ED_Unit*)a;
    ED_Unit* p2=(ED_Unit*)b;
    //将a和b强制类型转换后分别存入int型指针变量p1和p2
    return *p1<*p2;

}

ED_Layout::ED_Layout(QWidget *father)
{
    pContainer = father;
}

QPoint ED_Layout::pos2Ind(QPoint point)
{
    return pos2Ind(point.x(),point.y());
}

QPoint ED_Layout::ind2CenterPoint(QPoint ind)
{
    return ind2CenterPoint(ind.x(),ind.y());
}

QSize ED_Layout::ind2Size(QPoint ind)
{
    return ind2Size(ind.x(),ind.y());
}

QPoint ED_Layout::ind2Pos(QPoint ind)
{
    return ind2Pos(ind.x(),ind.y());
}

QPoint ED_Layout::ind2Pos_Centual(QPoint ind)
{
    return ind2Pos_Centual(ind.x(),ind.y());
}

QPoint ED_Layout::ind2Pos_Centual(int xind, int yind)
{
    QSize size = ind2Size(xind,yind);
    return ind2CenterPoint(xind,yind)-QPoint(size.width(),size.height())/2;
}

bool ED_Layout::Occupied(QPoint ind)
{
    return Occupied(ind.x(),ind.y());
}

void ED_Layout::putUnit(ED_Unit *aim, int xind, int yind, bool animated)
{
    updateBeforePut(aim,xind,yind);

    aim->indX = xind;
    aim->indY = yind;

    aim->layout = this;
    aim->setPMW(pmw);


    //setRect
    if(isMain){

    }
    else{
        aim->showLight =true;
        aim->showRect = true;
    }

    contents->push_back(aim);
    if(aim->alwaysShow){
        contents_AlwaysShow->push_back(aim);
    }
    else{
        contents_Show->push_back(aim);
    }


    aim->preSetInLayout(animated);

    updateBeforePutAnimation(aim,xind,yind);

    if(enable_background_blur) UpdateRegion();

    if(animated){
        QTimer *pTimer = new QTimer(pContainer);
        pTimer->singleShot(position_animation_time*2,[=](){
            if(aim->layout!=nullptr){
                aim->setInLayout(true);
                updateAfterPut(aim,xind,yind);
                qDebug()<<"timer setted end";
            }
            // qDebug()<<"timer end";
        });
    }
    else{
        aim->setInLayout(false);
        updateAfterPut(aim,xind,yind);
    }

    QFuture<void> future = QtConcurrent::run(writeJson);
}

void ED_Layout::putUnit(ED_Unit *aim, QPoint ind, bool animated)
{
    putUnit(aim,ind.x(),ind.y(),animated);
}

void ED_Layout::RemoveAUnit(ED_Unit *aim)
{
    QPoint tempos = aim->mapTo(pmw,QPoint(0,0));
    int indx = aim->indX;
    int indy = aim->indY;

    updateBeforeRemove(aim,indx,indy);

    aim->indX = -1;
    aim->indY = -1;
    aim->layout = nullptr;
    aim->setParent(pmws[screenInd(aim)]);
    aim->move(tempos);
    aim->setEnabled(true);
    aim->setVisible(true);
    pls->raise();
    aim->raise();
    auto s = std::find(contents->begin(), contents->end(), aim);//第一个参数是array的起始地址，第二个参数是array的结束地址，第三个参数是需要查找的值
    if (s != contents->end())//如果找到，就输出这个元素
    {
        contents->erase(s);
    }
    if(aim->alwaysShow){
        auto s = std::find(contents_AlwaysShow->begin(), contents_AlwaysShow->end(), aim);//第一个参数是array的起始地址，第二个参数是array的结束地址，第三个参数是需要查找的值
        if (s != contents_AlwaysShow->end())//如果找到，就输出这个元素
        {
            contents_AlwaysShow->erase(s);
        }
    }
    else{
        auto s = std::find(contents_Show->begin(), contents_Show->end(), aim);//第一个参数是array的起始地址，第二个参数是array的结束地址，第三个参数是需要查找的值
        if (s != contents_Show->end())//如果找到，就输出这个元素
        {
            contents_Show->erase(s);
        }
    }

    updateAfterRemove(aim,indx,indy);

    // UpdateRegion();
}

void ED_Layout::clearPut(ED_Unit *aim,bool animated)
{
    if(OKForClearPut(aim)){
        putUnit(aim,clearPutableInd(aim),animated);
    }
    else{
        qDebug()<<"No Area";
    }
}
bool ED_Layout::OKForClearPut(ED_Unit *aim)
{
    if(clearPutableInd(aim)==QPoint(-1,-1)) return false;
    else return true;
}

bool ED_Layout::OKForDefaultPut(ED_Unit *aim)
{
    if(defaultPutableInd(aim)==QPoint(-1,-1)) return false;
    else return true;
}

ED_Unit *ED_Layout::ind2Unit(QPoint ind)
{
    return ind2Unit(ind.x(),ind.y());
}

void ED_Layout::defaultPut(ED_Unit *aim, bool animated)
{
    QPoint dis = defaultPutableInd(aim);
    putUnit(aim,dis,animated);
}

void ED_Layout::setVisible(bool val, bool force)
{
    int countt =0;
    if(force){
        foreach(ED_Unit* unit,*contents){

            unit->setVisible(val);
            countt ++;

        }
    }
    else{
        foreach (ED_Unit* unit,*contents_Show) {
            unit->setVisible(val);
            countt ++;
        }

        if(!val){
            foreach (ED_Unit* unit,*contents_AlwaysShow) {
                unit->raise();
            }
        }
    }

    visibal = val;
    UpdateRegion();
    pContainer->update();
    qDebug()<<"setted"<<countt<<" "<<val;
}

void ED_Layout::UpdateRegion()
{
    int countt =0;
    region = QRegion();
    for(ED_Unit* content:*(contents)){
        if(content->isVisible()){
            auto tem = content->mapToGlobal(QPoint(0,0));
            region = region.united(QRegion(tem.x(),tem.y(),content->width(),content->height()));
            countt++;
        }

    }
    qDebug()<<"Region count"<<countt;
}

void ED_Layout::UpdateContentPositon()
{
    foreach (ED_Unit* content, *contents) {
        content->updateInLayout();
    }
}

void ED_Layout::updateAfterPut(ED_Unit *, int, int)
{

}

void ED_Layout::updateBeforePutAnimation(ED_Unit *, int, int)
{

}

void ED_Layout::updateAfterRemove(ED_Unit *, int, int)
{

}

void ED_Layout::updateBeforePut(ED_Unit *, int, int)
{

}

void ED_Layout::updateBeforeRemove(ED_Unit *, int, int)
{

}

void ED_Layout::load_json(QJsonObject rootObject)
{
    contents->clear();
    contents_AlwaysShow->clear();
    contents_Show->clear();

    QJsonArray contentsArray = rootObject.value("contents").toArray();
    QVector<ED_Unit*> tem;
    foreach (QJsonValue contentValue , (contentsArray)) {
        QJsonObject contentObject = contentValue.toObject();
        ED_Unit* unit = from_json(contentObject,pmw);
        tem.append(unit);
    }
    std::sort(tem.begin(),tem.end(),cmp);
    foreach (ED_Unit* aim , tem) {
        putUnit(aim,aim->indX,aim->indY,false);
    }
}

QJsonObject ED_Layout::to_json()
{
    QJsonObject rootObject;
    QJsonArray contentsArray;
    // std::sort(contents->begin(),contents->end());

    foreach (ED_Unit* content, *(contents)) {
        contentsArray.append(content->to_json());
    }
    rootObject.insert("contents",contentsArray);
    return rootObject;
}
