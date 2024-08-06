#ifndef SCOLORVIEW_H
#define SCOLORVIEW_H

#include <QWidget>

class SColorView : public QWidget
{
    Q_OBJECT
public:
    explicit SColorView(QWidget *parent = nullptr,QColor aim = nullptr);

    QColor now;

signals:
    void colorChanged(QColor);
    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
};

#endif // SCOLORVIEW_H
