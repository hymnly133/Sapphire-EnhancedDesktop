#ifndef LAYERBACKGROUND_H
#define LAYERBACKGROUND_H

#include <QWidget>

class MainWindow;
class LayerBackground : public QWidget
{
    Q_OBJECT
public:
    explicit LayerBackground(QWidget *parent,int screenInd);
    MainWindow* pmw;
    int screenInd;

    void Clear();
signals:
};

#endif // LAYERBACKGROUND_H
