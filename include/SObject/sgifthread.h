#ifndef SGIFTHREAD_H
#define SGIFTHREAD_H

#include "qlabel.h"
#include <QObject>
#include <QThread>
class MainWindow;
class SGifThread : public QThread
{
    Q_OBJECT
public:
    explicit SGifThread(QObject *parent = nullptr);
public:
    SGifThread(QObject *parent, const QString gifpath, QLabel* plabel);
    ~SGifThread();
    QString gifPath;
    QLabel *label;
    bool canRun = true;
    QMovie *gif = nullptr;
    QSize sourceSize = QSize(1, 1);
    void run();
    MainWindow* pmw;
    bool requirePause = false;
    bool requireGoOn = false;
signals:
    void gifUpdated();

public slots:
    void endPlay();
    void pause();
    void goOn();
    void connectToPmw(MainWindow* mw);
};

#endif // SGIFTHREAD_H
