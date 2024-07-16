#ifndef LAYERSHOWER_H
#define LAYERSHOWER_H

#include <QWidget>

class LayerShower : public QWidget
{
    Q_OBJECT
public:
    explicit LayerShower(QWidget *parent = nullptr);

signals:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // LAYERSHOWER_H
