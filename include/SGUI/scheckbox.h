#ifndef SCHECKBOX_H
#define SCHECKBOX_H

#include <QCheckBox>
#include <QObject>

class SCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    SCheckBox(QWidget* parent = nullptr);
    SCheckBox(QString text,QWidget* parent = nullptr);
};

#endif // SCHECKBOX_H
