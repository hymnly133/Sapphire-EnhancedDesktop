#include "scontainer.h"
#include "global.h"
#include"mainwindow.h"
#include "unitfunc.h"
SContainer::SContainer(SLayout *dis, int sizex, int sizey): SUnit(dis, sizex, sizey)
{
    type = Container;
    setMainColor(background_color);
}
void SContainer::setSimpleMode(bool val)
{
    SUnit::setSimpleMode(val);
    SLayoutContainer::setSimpleMode(val);
}

void SContainer::setScale(double val)
{
    SUnit::setScale(val);
    SLayoutContainer::setScale(val);
}

void SContainer::setFocus(bool val)
{
    SUnit::setFocus(val);
    if(val) {
        loadInsideAll();
    }
}

void SContainer::endUpdate()
{
    SUnit::endUpdate();
    SLayoutContainer::endUpdate();
}

void SContainer::Say()
{

}


void SContainer::afterResize(QResizeEvent *event)
{
    SUnit::afterResize(event);
    SLayoutContainer::afterResize(event);

}

QJsonObject SContainer::to_json()
{
    QJsonObject rootObject = SUnit::to_json();
    QJsonObject contentObject = SLayoutContainer::to_json();
    rootObject.insert("content", contentObject);
    return rootObject;
}

void SContainer::load_json(QJsonObject rootObject)
{
    SUnit::load_json(rootObject);
    SLayoutContainer::load_json(rootObject.value("content").toObject());
}

void SContainer::setPMW(MainWindow *pmw)
{
    SUnit::setPMW(pmw);
    SLayoutContainer::setPMW(pmw);
}

void SContainer::updateColor()
{
    setMainColor(background_color);
    SLayoutContainer::updateColor();
}

void SContainer::remove()
{
    QList<SUnit*> con;
    foreach (auto content, inside->contents) {
        content->removeFromLayout();
        con.push_back(content);
    }
    SUnit::remove();
    foreach (auto content, con) {
        activepmw->inside->clearPut(content, true);
    }
}

void SContainer::preSetLongFocus(bool val)
{
    if(val) {
        loadInsideAll();
    }
}

void SContainer::single_click_action(QMouseEvent *event)
{
    foldG();
}

void SContainer::updateAfterPut(SUnit *aim)
{
    if(layout) {
        layout->pContainer->updateAfterPut(aim);
    }
}

