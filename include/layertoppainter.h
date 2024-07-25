#ifndef LAYERTOPPAINTER_H
#define LAYERTOPPAINTER_H

#include "layershower.h"
#include <QWidget>

class LayerTopPainter : public LayerShower
{
    Q_OBJECT
public:
    explicit LayerTopPainter(MainWindow * parent , int screenId);

signals:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // LAYERTOPPAINTER_H
