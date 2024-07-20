#ifndef SHELLMENUITEM_H
#define SHELLMENUITEM_H
#include <Shlobj.h>
#include <shlwapi.h>
#include <windows.h>
#include "ContextMenu/shellitem.h"
#include "qicon.h"
class ShellMenuItem
{
public:
    QString caption;
    QString filePath;
    QIcon   icon;
    ShellItem::Ptr pItem;

    bool operator < (ShellMenuItem const& r)
    {
        return *pItem < *(r.pItem);
    }
    void exec() const { pItem->exec(); }
};
typedef QList<ShellMenuItem> ShellMenuItems;

#endif // SHELLMENUITEM_H
