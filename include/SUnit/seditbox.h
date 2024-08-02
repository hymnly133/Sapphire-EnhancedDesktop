#ifndef SEDITBOX_H
#define SEDITBOX_H

#include "sunit.h"
#include "qboxlayout.h"
#include "qslider.h"
#include <QWidget>

class SEditBox : public SUnit
{
    Q_OBJECT
public:
    explicit SEditBox(SLayout* dis = nullptr,int sizex = 3,int sizey = 2);
    QSlider* scale_Slider;
    QHBoxLayout* settings;
    QVBoxLayout* overall;
    QVBoxLayout* paint;
    QVBoxLayout* scale;
    QVBoxLayout* tem;



signals:

    // SUnit interface
public:
    void setPMW(MainWindow *pmw) override;
};
Q_DECLARE_METATYPE(SEditBox)
#endif // SEDITBOX_H
