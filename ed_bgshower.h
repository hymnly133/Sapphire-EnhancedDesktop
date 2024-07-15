#ifndef ED_BGSHOWER_H
#define ED_BGSHOWER_H

#include <QWidget>
#include <QtWin>

class ED_BGShower : public QWidget
{
    Q_OBJECT
public:
    explicit ED_BGShower(QWidget *parent = nullptr);
    QRegion updateMask();
    QPixmap captrued;
    bool cap = false;
public:
    void paintEvent(QPaintEvent * ev) override;

};
#endif // ED_BGSHOWER_H
