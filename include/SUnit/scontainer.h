#ifndef SCONTAINER_H
#define SCONTAINER_H

#include "sunit.h"
#include <QWidget>
class SUnit;

class SContainer : public SUnit
{
    Q_OBJECT
public:

    explicit SContainer(SLayout *dis = nullptr, int sizex=3, int sizey=3);

    SContainer(const SContainer &other)
        :SContainer(other.layout,other.sizeX,other.sizeY)
    {

    }
    SLayout* inside = nullptr;
    // virtual void InitLayout();
    void clearPut(SUnit* aim,bool animated);
    bool OKForClearPut(SUnit* aim);
    void afterResize(QResizeEvent* event) override;
    void setSimpleMode(bool)override;
    void setScale(double val)override;
    void endUpdate()override;

    virtual void Say();
    // ED_Unit interface
public:
    QJsonObject to_json()override ;
    void load_json(QJsonObject rootObject)override ;
signals:

    // ED_Unit interface
public:
    void setPMW(MainWindow *pmw) override;

};


Q_DECLARE_METATYPE(SContainer)
#endif // SCONTAINER_H
