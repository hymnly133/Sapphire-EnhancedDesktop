#ifndef STRAYICON_H
#define STRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>

class STrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit STrayIcon(QObject *parent = nullptr);

};

#endif // STRAYICON_H
