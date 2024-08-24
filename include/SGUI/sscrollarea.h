#ifndef SSCROLLAREA_H
#define SSCROLLAREA_H

#include <QObject>
#include <QScrollArea>
#include <QWidget>

class SScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    SScrollArea(QWidget* parent);
};

#endif // SSCROLLAREA_H
