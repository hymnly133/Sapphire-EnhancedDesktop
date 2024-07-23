#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H


class QStringList;
class QPoint;
class ContextMenu
{
public:
    static void show(QStringList const& fileNames,
                     void *handle, QPoint const& p);
};

#endif // CONTEXTMENU_H
