#ifndef SLAYOUTCONTAINER_H
#define SLAYOUTCONTAINER_H

#include "qevent.h"
class SUnit;
class MainWindow;
class SLayout;

class SLayoutContainer
{
public:
    SLayout* inside = nullptr;
    // virtual void InitLayout() = 0;

    SLayoutContainer();

    void clearPut(SUnit* aim, bool animated);
    bool OKForClearPut(SUnit* aim);
    void afterResize(QResizeEvent* event) ;
    void setSimpleMode(bool);
    void setScale(double val);
    void setOpacity(double val);
    void endUpdate();
    void setSLayout(SLayout* aim);
    QJsonObject to_json() ;
    void load_json(QJsonObject rootObject) ;
    void setPMW(MainWindow *pmw) ;
    void updateColor();


    //给容器的各种更新函数

    virtual void updateAfterPut(SUnit* aim);
    virtual void updateBeforePut(SUnit* aim);
    virtual void updateAfterRemove(SUnit* aim);
    virtual void updateBeforeRemove(SUnit* aim);


    virtual bool initAUnit(SUnit* aim, bool notice = true, QPoint globalPos = QPoint(-1, -1));

    // 添加SFile的方法(不创建文件）
    virtual bool addAFile(QString path, bool notice = false, QPoint globalPos = QPoint(-1, -1));
};

#endif // SLAYOUTCONTAINER_H
