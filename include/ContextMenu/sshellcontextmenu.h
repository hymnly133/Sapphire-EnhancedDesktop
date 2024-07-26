#ifndef SSHELLCONTEXTMENU_H
#define SSHELLCONTEXTMENU_H
#include "SysFunctions.h"
#include "mainwindow.h"
#include <windows.h>
#include <windowsx.h>
#include <ole2.h>
#include <commctrl.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <stdio.h>


class SShellContextMenu
{
public:
    SShellContextMenu();
    HINSTANCE g_hinst;                          /* This application's HINSTANCE */
    HWND g_hwndChild;                           /* Optional child window */
    IContextMenu2 *g_pcm2;						/* context-submenus */
    IContextMenu3 *g_pcm3;
    LPWSTR g_fileName;
    static void showContextMenu(QString path, QPoint pos);
};

#endif // SSHELLCONTEXTMENU_H
