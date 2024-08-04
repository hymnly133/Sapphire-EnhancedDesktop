#ifndef SFIELDSWIDGET_H
#define SFIELDSWIDGET_H

#include "qlabel.h"
#include <QObject>
#include <QVBoxLayout>
#include <QMap>
class SFieldsWidget : public QWidget
{
    Q_OBJECT
public:
    //map<parentField,contentLayout>
    SFieldsWidget(QWidget* parent);
    QVBoxLayout* nowLayout = nullptr;



    QMap<QStringList,QList<QWidget*>*> fieldContent;


    QMap<QStringList,QWidget*> fieldWidgets;
    QMap<QStringList,QLayout*> smallLayouts;
    QMap<QStringList,QLayout*> bigLayouts;


    QMap<QStringList,QLabel*> labels;
    void add(QStringList parentfields, QWidget* contentLayout, bool small);
    void raise(QStringList aimfield);

};

#endif // SFIELDSWIDGET_H
