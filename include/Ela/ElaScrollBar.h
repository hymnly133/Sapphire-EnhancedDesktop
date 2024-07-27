#ifndef ELASCROLLBAR_H
#define ELASCROLLBAR_H

#include <QScrollBar>

#include "stdafx.h"

class ElaScrollBarPrivate;
class ELA_EXPORT ElaScrollBar : public QScrollBar
{
    Q_OBJECT
    Q_Q_CREATE(ElaScrollBar)
    Q_PROPERTY_CREATE_Q_H(bool, isAnimation)
public:
    explicit ElaScrollBar(QWidget* parent = nullptr);
    explicit ElaScrollBar(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~ElaScrollBar();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // ELASCROLLBAR_H
