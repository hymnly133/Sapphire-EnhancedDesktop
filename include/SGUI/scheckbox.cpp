#include "scheckbox.h"
#include "SQSS.h"
#include"global.h"
#include "stylehelper.h"


SCheckBox::SCheckBox(QWidget *parent):QCheckBox(parent)
{
    linkToStyle(this,CheckBox);
}

SCheckBox::SCheckBox(QString text, QWidget *parent):SCheckBox(parent)
{
    setText(text);
}
