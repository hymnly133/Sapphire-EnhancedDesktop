#ifndef ED_BLOCK_H
#define ED_BLOCK_H

#include "SysFunctions.h"
#include "ed_unit.h"
#include "picturebox.h"
#include "qboxlayout.h"
#include "qlabel.h"
#include <QWidget>
//新增
#include <QVBoxLayout>

class ED_Block : public ED_Unit
{
    Q_OBJECT;
    Q_PROPERTY(double nowDefaultScale MEMBER nowDefaultScale NOTIFY nowDefaultScale_changed)
public:
    QLabel* lb ;
    QPixmap iconmap;
    QString filePath;
    bool requireIcon = false;
    bool isDir = false;
    QString iconPath;
    void updateDefaultScale();
    double nowDefaultScale = 0.5;
    QString name;
    PictureBox* gv ;

    QVBoxLayout* vl;
    QGraphicsDropShadowEffect* icon_shadow;
    QGraphicsDropShadowEffect* text_shadow;
    explicit ED_Block():ED_Block(nullptr,1,1){};
    explicit ED_Block(QWidget *parent, int sizex =1, int sizey=1);
    explicit ED_Block(QWidget *parent, QPixmap image, QString _name, QString _cmd, int sizex=1, int sizey=1);
    explicit ED_Block(QWidget *parent,QString path,int sizeX = 1,int sizeY = 1);

    QPropertyAnimation* defaultScaleAnimation;

    ED_Block(const ED_Block& other):ED_Block(other.parentWidget(),other.iconmap,other.name,other.filePath,other.sizeX,other.sizeY){};

    void afterResize(QResizeEvent* event) override;
    void mouse_enter_action() override;
    void mouse_leave_action()override;
    void onSimpleModeChange(bool) override;
    void whenScaleChange(double) override;
    void setIcon(QString iconPath);




    void loadFromPath(QString filepath);
    virtual void loadFromMyFI(MyFileInfo info);
    void double_click_action() override;
    void paintEvent(QPaintEvent *event) override;

    QJsonObject to_json() override;
    void load_json(QJsonObject rootObject) override;
    void whenMainColorChange(QColor val) override;
    virtual void ed_update() override;
    virtual void setLongFocus(bool val) override;



    void tip();


    // ED_Unit interface
public:
    void onShiftContextMenu(QContextMenuEvent *event) override;

    // ED_Unit interface
public:
    void onProcessAnother(ED_Unit *another) override;
    bool ProcessPath(QString path);

    // QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    // ED_Unit interface
public:
    void onDragedOut(QMouseEvent *event) override;
    void preSetInLayout(bool animated) override;

public:signals:
    void nowDefaultScale_changed(double val);
};

Q_DECLARE_METATYPE(ED_Block)

#endif // ED_BLOCK_H
