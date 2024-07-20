#ifndef FROZENTHREAD_H
#define FROZENTHREAD_H

#include "mainwindow.h"
#include "qthread.h"
class FrozenThread: public QThread
{
    Q_OBJECT
public:
    FrozenThread(MainWindow* pmw);
    MainWindow* pmw;
protected:
    void run();
signals:
    void CaptrueDone();
};

#endif // FROZENTHREAD_H
