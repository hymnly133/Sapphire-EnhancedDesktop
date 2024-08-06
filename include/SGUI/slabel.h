#ifndef SLABEL_H
#define SLABEL_H

#include <QLabel>
#include <QObject>
#include <QWidget>

class SLabel : public QLabel
{
public:
    using QLabel::QLabel;
    SLabel(QWidget* parent = nullptr);
};

#endif // SLABEL_H
