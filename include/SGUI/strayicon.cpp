#include "strayicon.h"
#include "global.h"
#include"smenu.h"
STrayIcon::STrayIcon(QObject *parent)
    : QSystemTrayIcon{parent}
{
    setIcon(QIcon(":/appIcon/Sapphire.ico"));
    setToolTip("Sapphire");
    setContextMenu(trayMenu);
    show();
}
