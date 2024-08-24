#include "sscrollarea.h"
#include"SQSS.h"

SScrollArea::SScrollArea(QWidget *parent): QScrollArea(parent)
{
    setStyleSheet(QSS_Scroll());
    setAttribute(Qt::WA_TranslucentBackground);
    // setFrameShape(QFrame::NoFrame);
}
