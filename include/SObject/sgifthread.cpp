#include "sgifthread.h"
#include "global.h"
#include "qapplication.h"
#include "qmovie.h"
#include "qmutex.h"
#include"mainwindow.h"

SGifThread::SGifThread(QObject *parent, const QString gifpath, QLabel *plabel): QThread{parent}
{
    gifPath = gifpath;
    label = plabel;

    QImageReader reader(gifpath);
    reader.setAutoTransform(true);
    const QImage img = reader.read();
    sourceSize = img.size();
    // connect(activepmw, &)

}

SGifThread::~SGifThread()
{
    requestInterruption();
    gif->stop();
    gif->setCacheMode(QMovie::CacheAll);
    delete gif;
    quit();
    wait();
}

void SGifThread::run()
{
    gif = new QMovie(gifPath);
    label->setMovie(gif);
    connect(gif, &QMovie::frameChanged, this, &SGifThread::gifUpdated);
    gif->start();
    while(!isInterruptionRequested()) {
        if(requirePause) {
            gif->setPaused(true);
            requirePause = false;
        } else if(requireGoOn) {
            gif->setPaused(false);
            requireGoOn = false;
        }
        qApp->processEvents();
    }
}

void SGifThread::endPlay()
{
    requestInterruption();
}

void SGifThread::pause()
{
    requirePause = true;
}

void SGifThread::goOn()
{
    requireGoOn = true;
}

void SGifThread::connectToPmw(MainWindow *mw)
{
    if(pmw == mw) {
        return;
    }
    if(mw == nullptr) {
        return;
    }

    qDebug() << "Disconnection result:" << disconnect(nullptr, &MainWindow::focus_Changed, this, nullptr);
    connect(mw, &MainWindow::focus_Changed, this, [ = ](bool val) {
        if(val) {
            goOn();
        } else {
            pause();
        }
    });
    pmw = mw;
}
