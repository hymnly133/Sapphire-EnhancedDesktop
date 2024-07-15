#ifndef ROUNDSHOWER_H
#define ROUNDSHOWER_H

#include <QWidget>

class roundShower : public QWidget
{
    Q_OBJECT
public:
    explicit roundShower(QWidget *parent = nullptr);

signals:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // ROUNDSHOWER_H
