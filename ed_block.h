#ifndef ED_BLOCK_H
#define ED_BLOCK_H

#include "SysFunctions.h"
#include "ed_unit.h"
#include "filepreviewwidget.h"
#include "picturebox.h"
#include "qboxlayout.h"
#include "qlabel.h"
#include <QWidget>
//新增
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>

class ED_Block : public ED_Unit
{
    Q_OBJECT;

public:
    QLabel* lb ;
    QPixmap iconmap;
    QString filePath;
    double default_scale = 0.5;
    static int default_size;
    QString name;
    PictureBox* gv ;
    QVBoxLayout* vl;
    QGraphicsDropShadowEffect* icon_shadow;
    QGraphicsDropShadowEffect* text_shadow;
    explicit ED_Block():ED_Block(nullptr,1,1){};
    explicit ED_Block(QWidget *parent, int sizex =1, int sizey=1);
    explicit ED_Block(QWidget *parent, QPixmap image, QString _name, QString _cmd, int sizex=1, int sizey=1);
    explicit ED_Block(QWidget *parent,QString path,int sizeX = 1,int sizeY = 1);

    ED_Block(const ED_Block& other):ED_Block(other.parentWidget(),other.iconmap,other.name,other.filePath,other.sizeX,other.sizeY){};

    void afterResize(QResizeEvent* event) override;
    void mouse_enter_action() override;
    void mouse_leave_action()override;
    void whenSimpleModeChange(bool) override;
    void whenScaleChange(double) override;


    void unzip(QString filepath);

    void double_click_action() override;
    void paintEvent(QPaintEvent *event) override;
    // FilePreviewWidget *previewWidget;
    QVBoxLayout *layout; // 新增 QVBoxLayout 成员
    QJsonObject to_json() override;
    void load_json(QJsonObject rootObject) override;
    void whenMainColorChange(QColor val) override;
    virtual void ed_update() override;
};

Q_DECLARE_METATYPE(ED_Block)

#endif // ED_BLOCK_H
