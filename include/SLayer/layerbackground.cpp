#include "layerbackground.h"
#include "SysFunctions.h"

LayerBackground::LayerBackground(QWidget *parent, int screenInd)
    : QWidget{parent}
{
    setObjectName("MainWindow" + QString::number(screenInd));

    this->screenInd = screenInd;
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setWindowState(Qt::WindowFullScreen);
    setAttribute(Qt::WA_TranslucentBackground);

    setAcceptDrops(true);
    inplace(this);
    // positionToScreen(this,screenInd);
    setVisible(true);
    show();
}
