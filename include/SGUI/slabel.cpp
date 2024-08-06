#include "slabel.h"
#include "SQSS.h"
#include"global.h"
#include "stylehelper.h"

SLabel::SLabel(QWidget* parent):QLabel(parent){
    setStyleSheet(QSS_Label());
    connectTo(text_color,color,QColor,{setStyleSheet(QSS_Label());});
}
