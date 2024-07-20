#ifndef DESKTOPMENU_H
#define DESKTOPMENU_H
#include "ContextMenu/shellmenuitem.h"

class DesktopMenu
{
public:
    static ShellMenuItems computerShellItems();
    static ShellMenuItems desktopMenuItems();
    static void shellSubMenuItems(ShellMenuItem const& item,
                                  ShellMenuItems & menuItems,
                                  bool isOnlyDir = true);
};

#endif // DESKTOPMENU_H
