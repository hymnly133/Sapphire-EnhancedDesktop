#ifndef ELATOOLBUTTON_H
#define ELATOOLBUTTON_H

#include <QToolButton>

#include "Def.h"
#include "stdafx.h"
class ElaMenu;
class ElaToolButtonPrivate;
class ELA_EXPORT ElaToolButton : public QToolButton
{
    Q_OBJECT
    Q_Q_CREATE(ElaToolButton)
public:
    explicit ElaToolButton(QWidget* parent = nullptr);
    ~ElaToolButton();
    void setMenu(ElaMenu* menu);
    void setElaIcon(ElaIconType icon);
};

#endif // ELATOOLBUTTON_H
