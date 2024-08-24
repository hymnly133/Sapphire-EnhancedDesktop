#ifndef SQSS_H
#define SQSS_H

#include"QString"
#include "qcolor.h"
#include "qwidget.h"
enum STYLEGUI {
    Label = 0,
    CheckBox = 1,
    Menu = 2
};

QString rgb2qss(QColor color);

QString Process(QString resource);
void linkToStyle(QWidget* aim, STYLEGUI gui);
QString QSS_SMenu();
QString QSS_Label();
QString QSS_CheckBox();
QString QSS_Scroll();

#endif // SQSS_H
