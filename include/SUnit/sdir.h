#ifndef SDIR_H
#define SDIR_H

#include <QObject>
#include "sblockcontainer.h"
#include "sfile.h"
#include"SContainer.h"
class SFlowlayout;
class SDir : public SFile, public SLayoutContainer
{
    Q_OBJECT
public:
    explicit SDir(SLayout* dis = nullptr, int sizex = 1, int sizey = 1, QString filePath = "");

    SFlowlayout* inside_f;
    bool isFold = false;
    bool isExpand = true;
    SAnimationRect* ar_fold;
    SAnimationRect* ar_expand;
    // SAnimationRect ar;
    // SUnit interface
    void afterResize(QResizeEvent* event) override;
    void setSimpleMode(bool)override;
    void setScale(double val)override;
    void endUpdate()override;
    QJsonObject to_json() override;
    void load_json(QJsonObject rootObject) override;
    void setPMW(MainWindow *pmw) override;



    void updateAfterPut(SUnit* aim) override;
    void updateAfterRemove(SUnit* aim) override;
    void updateBeforePut(SUnit *aim) override;


    void setFold(bool val);
    void updateFoldAnimation();

    void setExpand(bool val);
    void updateExpandAnimation();

    void preSetLongFocus(bool)override;


    void single_click_action(QMouseEvent* event) override;
signals:

    // SUnit interface
public slots:
    void updateColor() override;
    void whenFoldAnimationUpdate();
    void whenExpandAnimationUpdate();

    // SUnit interface
public:
    void remove() override;


    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event) override;

    // SUnit interface
public:
    QPoint MyPos() override
    {
        QPoint centerPos = layout->unit2CenterPoint(this);
        return QPoint(centerPos.x() - MySize().width() / 2, centerPos.y() - MySize().height() / 2);
        // return refine(tem, MySize());
    };
    QSize MySize() override
    {
        aim_expandSize();
        // previousSize;
        SUnit::MySize();
        double expandRatio = ar_expand->nowRatio;

        // if(usePreviousSize) {
        //     if(isExpand) {
        //         return  previousSize* (1 - expandRatio) +  aim_expandSize() * (ar_expand->nowRatio);
        //     } else {
        //         return  SUnit::MySize() * (1 - expandRatio) +  aim_expandSize() * (ar_expand->nowRatio);
        //     }
        // } else {
        return  SUnit::MySize() * (1 - expandRatio) +  aim_expandSize() * (ar_expand->nowRatio);
        // }
    };
    QSize aim_expandSize();

    // SLayoutContainer interface

};
Q_DECLARE_METATYPE(SDir);



#endif // SDIR_H
