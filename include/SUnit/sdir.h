#ifndef SDIR_H
#define SDIR_H

#include <QObject>
#include "guifunc.h"
#include "qjsonarray.h"
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
    bool moveFile = true;
    SAnimationRect* ar_fold;
    SAnimationRect* ar_expand;
    // SAnimationRect ar;
    // SUnit interface
    QJsonArray waitedToLoad;
    QStringList newfiles;
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

    // void preSetLongFocus(bool)override;
    void setFocus(bool ) override;
    void setLongFocus(bool)override;
    void setProcessor(bool) override;



    virtual void loadFromMyFI(const MyFileInfo& info, bool init) override;

    //预加载
    void scanDir();

    //加载
    virtual void startToLoad() override;

    QStringList jsonFiles();

    void single_click_action(QMouseEvent* event) override;
    void remove() override;
    QPoint MyPos() override;;
    QSize MySize() override;;
    QSize aim_expandSize();
    void onDragedOut() override;
    void processorTip() override;
    void processFile(SFileInfo *sfileInfo) override;




signals:

    // SUnit interface
public slots:
    void updateColor() override;
    void whenFoldAnimationUpdate();
    void whenExpandAnimationUpdate();


protected:
    void wheelEvent(QWheelEvent *event) override;



    // SLayoutContainer interface
public:
    bool checkType(SUnit *unit) override;

private:
    void setInsideUnexpand();
};

inline QPoint SDir::MyPos()
{
    if(!outOfParent_actual) {
        return SFile::MyPos();
    }
    QPoint centerPos = layout->unit2CenterPoint(this);
    QSize aimSize = MySize();
    centerPos =  refineRect(layout->pContainerW->mapToGlobal(centerPos), aimSize, pmw);
    centerPos = layout->pContainerW->mapFromGlobal(centerPos);

    return QPoint(centerPos.x() - aimSize.width() / 2, centerPos.y() - aimSize.height() / 2);
    // return refine(tem, MySize());
}

inline QSize SDir::MySize()
{
    if(!outOfParent_actual) {
        return SFile::MySize();
    }
    aim_expandSize();
    SUnit::MySize();
    double expandRatio = ar_expand->nowRatio;
    return  SUnit::MySize() * (1 - expandRatio) +  aim_expandSize() * (ar_expand->nowRatio);
}
Q_DECLARE_METATYPE(SDir);



#endif // SDIR_H
