#ifndef SMULTIFUNC_H
#define SMULTIFUNC_H

#include <QWidget>
#include "SysFunctions.h"
#include "sfileinfo.h"
#include "slabel.h"
#include "sunit.h"
#include "picturebox.h"
#include "qboxlayout.h"
#include "qlabel.h"
#include <QVBoxLayout>

class SMultiFunc : public SUnit
{
    Q_OBJECT;
    Q_PROPERTY(double nowDefaultScale MEMBER nowDefaultScale NOTIFY nowDefaultScale_changed)
public:
    SLabel* lb ;
    QPixmap pix;
    QString pixPath;
    void updateDefaultScale();
    double nowDefaultScale = 0.5;
    bool fullShow = false;
    bool requireMulti = false;
    bool isProcessor = false;
    bool showNameTip = true;
    QString name;
    PictureBox* gv ;

    QVBoxLayout* vl;
    QGraphicsDropShadowEffect* pix_shadow;

    explicit SMultiFunc(SLayout* dis = nullptr, int sizex = 1, int sizey = 1);

    // explicit SMultiFunc(QWidget *parent, QPixmap image, QString _name, int sizex=1, int sizey=1);
    // explicit SMultiFunc(QWidget *parent,QString path,int sizeX = 1,int sizeY = 1);

    QPropertyAnimation* defaultScaleAnimation;

    SMultiFunc(const SMultiFunc& other): SMultiFunc(other.layout, other.sizeX, other.sizeY) {};

    void afterResize(QResizeEvent* event) override;
    void mouse_enter_action() override;
    void mouse_leave_action()override;
    void onSimpleModeChange(bool) override;
    void onScaleChange(double) override;
    virtual void setIconFromPath(QString iconPath, bool save);
    virtual void setGIF(QString gifPath);
    virtual void setPix(QPixmap pixmap);
    virtual void setName(QString);

    void paintEvent(QPaintEvent *event) override;

    QJsonObject to_json() override;
    void load_json(QJsonObject rootObject) override;
    void onMainColorChange(QColor val) override;


    virtual void endUpdate() override;
    virtual void setLongFocus(bool val) override;

    //将要设置/取消设置为Processor的逻辑处理函数和外部调用函数
    virtual void preSetProcessor(bool val);

    //设置/取消设置为Processor的内部处理函数
    virtual void setProcessor(bool val);




    virtual void processorTip();

    virtual void processAnother(SUnit *another) override;

    void processFile(SFile *sfile);
    virtual void processFile(SFileInfo *sfileInfo);
    void processFile(QString path);

    virtual void setupEditMenu() override;




    // QWidget interface
    void setFullShow(bool val);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    // ED_Unit interface
public:
    void onDragedOut() override;
    void preSetInLayout(bool animated) override;


public:
signals:
    void nowDefaultScale_changed(double val);

signals:

    // SUnit interface
public slots:
    void updateColor() override;

    // SUnit interface
public:
    void whenFocusAnimationChange() override;



    // SUnit interface
public:
    void setInLayout(bool animated) override;
};


Q_DECLARE_METATYPE(SMultiFunc)


#endif // SMULTIFUNC_H
