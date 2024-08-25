#ifndef SGIFTHREAD_H
#define SGIFTHREAD_H

#include "qlabel.h"
#include <QObject>
#include <QThread>

class SGifThread : public QThread
{
    Q_OBJECT
public:
    explicit SGifThread(QObject *parent = nullptr);
public:
    SGifThread(QObject *parent, const QString gifpath, QLabel* plabel);
    QString gifPath;
    QLabel *label;
    bool canRun = true;
    QMovie *gif = nullptr;
    QSize sourceSize = QSize(1, 1);
    void run();


public slots:
    void endPlay();
};

#endif // SGIFTHREAD_H
