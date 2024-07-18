#ifndef ED_EDITBOX_H
#define ED_EDITBOX_H

#include "ed_unit.h"
#include "qboxlayout.h"
#include "qslider.h"
#include <QWidget>

class ED_EditBox : public ED_Unit
{
    Q_OBJECT
public:
    explicit ED_EditBox(QWidget *parent = nullptr);
    QSlider* scale_Slider;
    QHBoxLayout* settings;
    QVBoxLayout* overall;
    QVBoxLayout* paint;
    QVBoxLayout* scale;
    QVBoxLayout* tem;
signals:
};
Q_DECLARE_METATYPE(ED_EditBox)
#endif // ED_EDITBOX_H
