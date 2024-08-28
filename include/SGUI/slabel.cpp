#include "slabel.h"
#include "SQSS.h"
#include"global.h"
#include "stylehelper.h"
SLabel::SLabel(QWidget* parent): QLabel(parent)
{
    linkToStyle(this, Label);
    // setStyleSheet(QSS_Label());
    // connectTo(text_color,color,QColor,{setStyleSheet(QSS_Label());});
    setFont(qApp->font());
    connect(psh, &StyleHelper::fontChanged, this, [ = ]() {
        setFont(qApp->font());
    });
    opacityEffect = new QGraphicsOpacityEffect;
    opacityEffect->setEnabled(true);
    setOpacity(1.0);
    setGraphicsEffect(opacityEffect);
}

void SLabel::setOpacity(double val)
{
    val = qBound(0.0, val, 0.999);
    if(opacity == val) {
        return;
    }


    // qDebug() << val;
    opacity = val;
    opacityEffect->setOpacity(val);
    opacityEffect->update();
    update();
}
