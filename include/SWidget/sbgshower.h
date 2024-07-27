#ifndef SBGSHOWER_H
#define SBGSHOWER_H
#include <QWidget>
#include <QtWin>

class MainWindow;

class SBGShower : public QWidget
{
    Q_OBJECT
public:

    explicit SBGShower(QWidget *parent = nullptr);
    QRegion updateMask();
    MainWindow* pmw;
    QPixmap captrued;
    bool cap = false;
public:
    void paintEvent(QPaintEvent * ev) override;

};
#endif // SBGSHOWER_H
