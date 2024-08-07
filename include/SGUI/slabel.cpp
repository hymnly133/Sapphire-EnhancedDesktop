#include "slabel.h"
#include "SQSS.h"
#include"global.h"
#include "stylehelper.h"

SLabel::SLabel(QWidget* parent):QLabel(parent){
    linkToStyle(this,Label);
    // setStyleSheet(QSS_Label());
    // connectTo(text_color,color,QColor,{setStyleSheet(QSS_Label());});
    connect(psh,&StyleHelper::fontChanged,this,[=](){
        setFont(qApp->font());
    });
}
