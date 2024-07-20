#include "frozenthread.h"
#include "mainwindow.h"

FrozenThread::FrozenThread(MainWindow *pmw)
{
    this->pmw = pmw;
    connect(this,SIGNAL(CaptrueDone()),pmw,SLOT(whenFrozenThreadDone()));
}

void FrozenThread::run()
{
    qDebug()<<"startCapture";
    pmw->capture();
    qDebug()<<"Capture Done";
    emit CaptrueDone();
}
