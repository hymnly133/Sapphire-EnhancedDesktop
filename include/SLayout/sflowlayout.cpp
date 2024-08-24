#include "sflowlayout.h"
#include "global.h"
#include "guifunc.h"
#include "sscrollarea.h"
#include "sunit.h"
#include "mainwindow.h"

SFlowlayout::SFlowlayout(SLayoutContainer *father, int maxRow, double boradXR, double boradYR, double spaceXR, double spaceYR): SLayout(father)
{
    putWidget = new QWidget(pContainerS);
    putWidget->move(0, 0);
    putWidget->show();

    pContainerW = putWidget;
    this->maxRow = maxRow;
    this->boradXR = boradXR;
    this->boradYR = boradYR;
    this->spaceXR = spaceXR;
    this->spaceYR = spaceYR;
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 50; j++) {
            blocks[i][j] = littleBlock(i, j, this);
        }
    }
    visibal = true;
    contentResizable = false;
    scrollValueAnimation = new QPropertyAnimation(this, "nowScrollValue");
    scrollValueAnimation->setDuration(500);
    scrollValueAnimation->setEasingCurve(QEasingCurve::OutQuad);
    scrollValueAnimation->setStartValue(0);
    connect(this, &SFlowlayout::nowScollValue_Changed, this, &SFlowlayout::whenScrollAnimationUpdate);
}

void SFlowlayout::refresh()
{
    for(int i = 0; i < list.size(); i++) {
        list[i]->indX = i;
    }

    int nowRow = row();
    int nowCol = col() + 1;

    //清理旧数据
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 50; j++) {
            if(blocks[i][j].occupied()) {
                blocks[i][j].remove();
            }
        }
    }

    //尽可能靠前放置
    int nowInd = 0;
    QList<SUnit* > newList;
    for(int j = 0; j < nowCol; j++) {
        for(int i = 0; i < nowRow; i++) {
            if(!blocks[i][j].occupied()) {
                //尝试靠前寻找一个unit来放置
                foreach (SUnit* unit, list) {
                    if(okForUnit(unit, i, j)) {
                        putAt(unit, i, j, nowInd);
                        nowInd++;
                        newList.append(unit);
                        list.removeOne(unit);
                        break;
                    }
                }
            }

        }
    }

    list = newList;


    QList<SUnit*>reverseList = list;
    //更新放置表
    // qDebug() << reverseList;
    std::reverse(reverseList.begin(), reverseList.end());
    // qDebug() << "reversed" << reverseList;

    for(int j = 0; j < 50; j++) {
        for(int i = 0; i < 10; i++) {
            putableMap[i][j] = list.size();
        }
    }
    foreach (SUnit* unit, reverseList) {
        int remainLines = unit->sizeY;
        for(int j = 0; j < nowCol; j++) {
            bool find = false;
            for(int i = 0; i < nowRow; i++) {
                if(blocks[i][j].content != unit) {
                    if(!find) {
                        putableMap[i][j] = unit->indX;
                    } else {
                        break;
                    }
                } else {
                    find = true;
                    putableMap[i][j] = unit->indX;
                }
            }
            if(find) {
                remainLines--;
                if(remainLines <= 0) {
                    break;
                }
            }
        }
    }

    // printPutableMap();


}
bool SFlowlayout::okForUnit(SUnit *aim, int x, int y)
{
    for(int i = 0; i < aim->sizeX; i++) {
        for(int j = 0; j < aim->sizeY; j++) {
            if(blocks[x + i][y + j].occupied()) {
                return false;
            } else if(x + i >= row()) {
                return false;
            }
        }
    }
    return true;
}

void SFlowlayout::putAt(SUnit *aim, int x, int y, int ind)
{
    // qDebug() << x  << y  << aim->objectName() << ind;
    for(int i = 0; i < aim->sizeX; i++) {
        for(int j = 0; j < aim->sizeY; j++) {
            blocks[x + i][y + j].insert(aim, ind);
        }
    }
}

int SFlowlayout::occupiedSize()
{
    int res = 0;
    foreach (SUnit* unit, contents) {
        res += unit->sizeX*unit->sizeY;
    }
    return res;
}

void SFlowlayout::printPutableMap()
{
    QStringList pt;
    int nowRow = row();
    int nowCol = col();
    for(int j = 0; j < nowCol; j++) {
        for(int i = 0; i < nowRow; i++) {
            pt << QString::number(putableMap[i][ j]);
        }
        qDebug() << pt.join(" ");
        pt.clear();
    }
    // qDebug(QStringList)


    foreach(SUnit* unit, list) {
        qDebug() << unit->objectName() << unit->indX << unit2block[unit]->ind << unit2block[unit]->indX << unit2block[unit]->indY;
    }
}

QSize SFlowlayout::unit2Size(SUnit* aim)
{
    double w = W_Block_Clean() * aim->sizeX + (aim->sizeX - 1) * spaceXPix();
    double h = H_Block_Clean() * aim->sizeY + (aim->sizeY - 1) * spaceYPix();
    return QSize(w, h);
}

QPoint SFlowlayout::unit2Pos(SUnit* aim)
{
    if(unit2block.contains(aim)) {
        int x = unit2block[aim]->indX;
        int y = unit2block[aim]->indY;
        return QPoint(blocks[x][y].posX(), blocks[x][y].posY());
    } else {
        return QPoint(0, 0);
    }
}

bool SFlowlayout::Occupied(int x, int y)
{
    return false;
}

QPoint SFlowlayout::defaultPutableInd(SUnit *aim)
{
    return QPoint(list.size(), 0);
}

QPoint SFlowlayout::clearPutableInd(SUnit *aim)
{
    auto pos = pContainerW->mapFromGlobal(aim->mapToGlobal(QPoint(0, 0)));
    double posx = pos.x();
    double posy = pos.y();
    double mindeltaw = W_Container();
    double mindeltah = H_Container();
    int bpw, bph;
    bpw = bph = -1;
    int nowRow = row();
    int nowCol = col() + 1;
    // int bx = boradXPix();
    // int wx = W_Block_Clean();
    // int hx = H_Block_Clean();
    // int sx = spaceXPix();

    for(int i = 0; i < nowRow; i++) {
        for(int j = 0; j < nowCol; j++) {
            double deltaw = abs(posx - boradXPix() - i*(W_Block_Clean() + spaceXPix()));
            double deltah = abs(posy - boradYPix() - j*(H_Block_Clean() + spaceYPix()));
            if((deltaw + deltah < mindeltaw + mindeltah)) {
                mindeltaw = deltaw;
                mindeltah = deltah;
                bpw = i;
                bph = j;
            }
        }
    }
    QSize t = pContainerW->size();
    return QPoint(putableMap[bpw][bph], 0);
}

SUnit *SFlowlayout::ind2Unit(int xind, int yind)
{
    return list[xind];
}

void SFlowlayout::updateAfterRemove(SUnit *aim, int x, int y)
{
    list.removeAt(x);
    refresh();
    UpdateContentPositon();
}

void SFlowlayout::scroll(int delta)
{
    aimScroolValue += delta;
    updateScrollAnimation();
}

void SFlowlayout::scrollTo(int to)
{
    aimScroolValue = to;
    updateScrollAnimation();
}

void SFlowlayout::updateScrollAnimation()
{
    scrollValueAnimation->stop();
    scrollValueAnimation->setStartValue(nowScrollValue);
    scrollValueAnimation->setEndValue(aimScroolValue);
    scrollValueAnimation->start();
}

void SFlowlayout::afterResize()
{
    // putWidget->lower();
    // putWidget->setFixedSize(pContainerS->width());
    putWidget->setFixedSize(pContainerS->width(), 99999);
    // updateScrollAnimation();
}

void SFlowlayout::UpdateContentPositon(bool animated)
{
    foreach (SUnit* content, contents) {
        if(unit2block.contains(content)) {
            content->updateInLayout(animated);
        }
    }
}

void SFlowlayout::whenScrollAnimationUpdate()
{

    putWidget->move(0, -nowScrollValue);
    // qDebug() << putWidget->geometry();
}

int SFlowlayout::W_Container()
{
    if(useStandaloneRect) {
        return standaloneRect.width();
    } else {
        return pContainerS->width();
    }
}

int SFlowlayout::H_Container()
{
    if(useStandaloneRect) {
        return standaloneRect.width();
    } else {
        return pContainerS->width();
    }
}

void SFlowlayout::updateBeforePut(SUnit *aim, int x, int y )
{
    //将block队列位置调整对（ind仍为错）
    // qDebug()<<"Put"<<x;
    aim->setBlockSize(1, 1);
    // aim->resizable = false;
    list.insert(x, aim);
    refresh();
}

void SFlowlayout::updateAfterPut(SUnit *aim, int x, int y)
{
    refresh();
    UpdateContentPositon(false);
}

void SFlowlayout::updateBeforePutAnimation(SUnit * aim, int x, int y)
{

    refresh();
    UpdateContentPositon(!onLoading);
}
