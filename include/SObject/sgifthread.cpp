#include "sgifthread.h"
#include "qapplication.h"
#include "qmovie.h"
#include "qmutex.h"


SGifThread::SGifThread(QObject *parent, const QString gifpath, QLabel *plabel): QThread{parent}
{
    gifPath = gifpath;
    label = plabel;

    QImageReader reader(gifpath);
    reader.setAutoTransform(true);
    const QImage img = reader.read();
    sourceSize = img.size();

}

SGifThread::~SGifThread()
{
    requestInterruption();
    gif->stop();
    delete gif;
    quit();
    wait();
}

void SGifThread::run()
{
    gif = new QMovie(gifPath);
    label->setMovie(gif);
    // gif->setScaledSize(QSize(400, 300));
    // gif->setSpeed(500);
    gif->start();
    while(!isInterruptionRequested()) {
        qApp->processEvents();
    }
}

void SGifThread::endPlay()
{
    canRun = false;
}
