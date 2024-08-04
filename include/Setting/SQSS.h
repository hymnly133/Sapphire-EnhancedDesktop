#ifndef SQSS_H
#define SQSS_H

#include <string>
#include"QString"
#include "SysFunctions.h"
#include "qcolor.h"
#include "style.h"
using namespace std;
// color:black;

string menu_qss = R"(
QMenu{
border: none;
padding:5px;
background-color:transparent;
}
QMenu::item:selected{
    color: #1aa3ff;
    background-color: #e5f5ff;
    border-radius:10px;

}
QMenu::separator{
height:1px;
background:#bbbbbb;
margin:5px;
    margin-left:10px;
    margin-right:10px;
}
QMenu::item:text {
    padding-left:15px;
    padding-right:15px;
    padding-top:10px;
    padding-bottom:10px;
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
    QColor temcolor = winThemeColor();
    QColor temcolorAlpha =temcolor;
    temcolor.setAlpha(240);
    resource.replace("[themeColorAlpha]",rgb2qss(temcolorAlpha));
    resource.replace("[themeColor]",rgb2qss(temcolor));
    return resource;
}

QString QSS_SMenu(){
    QString temp = QString::fromStdString(menu_qss);
    return Process(temp);
}



#endif // SQSS_H
