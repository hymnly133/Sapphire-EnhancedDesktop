#ifndef SLABEL_H
#define SLABEL_H

#include "qgraphicseffect.h"
#include <QLabel>
#include <QObject>
#include <QWidget>

class SLabel : public QLabel
{
public:
    using QLabel::QLabel;
    SLabel(QWidget* parent = nullptr);
    QGraphicsOpacityEffect* opacityEffect;
    double opacity = 1.0;
    void setOpacity(double val);
};

#endif // SLABEL_H
