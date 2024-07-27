#ifndef LAYERMOUSEGETTER_H
#define LAYERMOUSEGETTER_H

#include "layershower.h"
#include "qmenu.h"
#include <QWidget>
class MainWindow;
class LayerMouseGetter:public QWidget
{
public:
    QMenu* myMenu;
    explicit LayerMouseGetter(MainWindow * parent , int screenId);
    int screenId;
    MainWindow* pmw;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;



    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // LAYERMOUSEGETTER_H
