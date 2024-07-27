#ifndef ROUNDSHOWER_H
#define ROUNDSHOWER_H

#include "style.h"
#include <QWidget>

class roundShower : public QWidget
{
    Q_OBJECT
public:
    explicit roundShower(QWidget *parent = nullptr);
    QSize* pSize;
    int* pRadius = nullptr;
    bool follow = true;

    void distri(QSize* sizedis,int* radiusdis);
    void distriRadius(int* radiusdis);

    QSize aim_size(){
        if(follow) return parentWidget()->size();
        else return *pSize;
    }

    void updateDisplay();

    int aim_radius(){
        if(pRadius==nullptr) return unit_radius;
        else return *pRadius;
    }
signals:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // ROUNDSHOWER_H
