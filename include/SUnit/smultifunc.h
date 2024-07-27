#ifndef SMULTIFUNC_H
#define SMULTIFUNC_H

#include <QWidget>
#include "SysFunctions.h"
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
    QLabel* lb ;
    QPixmap pix;
    QString pixPath;
    void updateDefaultScale();
    double nowDefaultScale = 0.5;
    bool fullShow = false;
    QString name;
    PictureBox* gv ;

    QVBoxLayout* vl;
    QGraphicsDropShadowEffect* pix_shadow;
    QGraphicsDropShadowEffect* text_shadow;

    explicit SMultiFunc(SLayout* dis = nullptr, int sizex =1, int sizey =1);

    // explicit SMultiFunc(QWidget *parent, QPixmap image, QString _name, int sizex=1, int sizey=1);
    // explicit SMultiFunc(QWidget *parent,QString path,int sizeX = 1,int sizeY = 1);

    QPropertyAnimation* defaultScaleAnimation;

    SMultiFunc(const SMultiFunc& other):SMultiFunc(other.layout,other.sizeX,other.sizeY){};

    void afterResize(QResizeEvent* event) override;
    void mouse_enter_action() override;
    void mouse_leave_action()override;
    void onSimpleModeChange(bool) override;
    void onScaleChange(double) override;
    virtual void setPix(QString pixPath, bool save);
    virtual void setPix(QPixmap pixmap);
    virtual void setName(QString);

    void paintEvent(QPaintEvent *event) override;

    QJsonObject to_json() override;
    void load_json(QJsonObject rootObject) override;
    void onMainColorChange(QColor val) override;


    virtual void endUpdate() override;
    virtual void setLongFocus(bool val) override;
    virtual void processorTip();

    virtual void onProcessAnother(SUnit *another) override;
    virtual bool ProcessPath(QString path);

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

    virtual void onCelectedProcessor(bool val);

public:signals:
    void nowDefaultScale_changed(double val);

signals:
};


Q_DECLARE_METATYPE(SMultiFunc)


#endif // SMULTIFUNC_H
