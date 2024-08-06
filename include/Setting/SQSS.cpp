#include"SQSS.h"
#include"QString"
#include "global.h"
#include "qcolor.h"
#include "style.h"
#include"stylehelper.h"


QString label_qss = R"(
    color: [text_color];

)";
QString checkbox_qss = R"(
    color: [text_color];
)";


QString menu_qss = R"(
QMenu{
border: none;
padding:5px;
background-color:transparent;
}
QMenu::item:text {
    color: [text_color];
    padding-left:15px;
    padding-right:15px;
    padding-top:10px;
    padding-bottom:10px;
}
QMenu::item:selected{
    color: [highlight_color];
    background-color: [background_color];
    border-radius:10px;
}
QMenu::separator{
height:1px;
background:#bbbbbb;
margin:5px;
    margin-left:10px;
    margin-right:10px;
}

    )";
QString rgb2qss(QColor color){
    QString res = "rgba(R,G,B,A)";
    res.replace("R",QString::number(color.red()));
    res.replace("G",QString::number(color.green()));
    res.replace("B",QString::number(color.blue()));
    res.replace("A",QString::number(color.alpha()));
    return res;
}

QString Process(QString resource){
    resource.replace("[background_color]",rgb2qss(background_color));
    resource.replace("[highlight_color]",rgb2qss(highlight_color));
    resource.replace("[theme_color]",rgb2qss(themeColor()));
    resource.replace("[text_color]",rgb2qss(text_color));
    return resource;
}


void linkToStyle(QWidget* aim,STYLEGUI gui)
{
    switch (gui) {

    case Label:
        aim->setStyleSheet(QSS_Label());
        QObject::connect(psh,&StyleHelper::colorChanged,aim,[=](){
        aim->setStyleSheet(QSS_Label());
        });
        break;
    case CheckBox:
        aim->setStyleSheet(QSS_CheckBox());
        QObject::connect(psh,&StyleHelper::colorChanged,aim,[=](){
            aim->setStyleSheet(QSS_CheckBox());
        });
        break;
    case Menu:
        aim->setStyleSheet(QSS_SMenu());
        QObject::connect(psh,&StyleHelper::colorChanged,aim,[=](){
            aim->setStyleSheet(QSS_SMenu());
        });
        break;
    }
}

QString QSS_SMenu(){
    return Process(menu_qss);
}


QString QSS_Label(){
    return Process(label_qss);
}

QString QSS_CheckBox(){

    return Process(checkbox_qss);
}
