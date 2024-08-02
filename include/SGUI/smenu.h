#ifndef SMENU_H
#define SMENU_H

#include "SAnimationRect.h"
#include <QWidget>
#include <qmenu.h>

class SMenu:public QMenu
{
    Q_OBJECT
public:
    SMenu(QWidget* parent = nullptr);
    SAnimationRect* arect;
    roundShower* rs;
    QSize aimSize;
    QPoint previousPos;
    QPoint aimPos;
    bool ismain = false;
    bool firstShow = true;

    void init();
    void raiseAction(QAction* action);
    void exec(QPoint pos,bool multi = false);
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
};

#endif // SMENU_H
