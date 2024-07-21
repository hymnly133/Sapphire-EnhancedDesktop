#ifndef SCREENFUNC_H
#define SCREENFUNC_H
#include "SysFunctions.h"
#include "qwidget.h"

extern QPoint Shift_Global;

int screenInd(QWidget* aim);

int cursorScreenInd();

QPoint mapToLS(QWidget* aim,QPoint dis);
#endif // SCREENFUNC_H
