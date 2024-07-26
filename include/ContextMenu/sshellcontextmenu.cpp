#include "sshellcontextmenu.h"
#include "SysFunctions.h"
#include "mainwindow.h"
#include <windows.h>
#include <windowsx.h>
#include <ole2.h>
#include <commctrl.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <stdio.h>

#define HANDLE_WM_CONTEXTMENU(hwnd, wParam, lParam, fn) \
((fn)((hwnd), (HWND)(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)), 0L)
#define SCRATCH_QCM_FIRST 1
#define SCRATCH_QCM_LAST  0x7FFF



SShellContextMenu::SShellContextMenu() {}

HRESULT GetUIObjectOfFile(HWND hwnd, LPCWSTR pszPath, REFIID riid, void **ppv)
{
    *ppv = NULL;
    HRESULT hr;
    LPITEMIDLIST pidl;
    SFGAOF sfgao;
    if (SUCCEEDED(hr = SHParseDisplayName(pszPath, NULL, &pidl, 0, &sfgao))) {
        IShellFolder *psf;
        LPCITEMIDLIST pidlChild;
        if (SUCCEEDED(hr = SHBindToParent(pidl, IID_IShellFolder,
                                          (void**)&psf, &pidlChild))) {
            hr = psf->GetUIObjectOf(hwnd, 1, &pidlChild, riid, NULL, ppv);
            psf->Release();
        }
        CoTaskMemFree(pidl);
    }
    return hr;
}

void SShellContextMenu::showContextMenu(QString path,QPoint pos)
{
    HWND hwnd = (HWND)activepmw->winId();
    SShellContextMenu* scm = new SShellContextMenu();
    std::wstring wlpstrstd = path.toStdWString();
    LPCWSTR lpcwStr = wlpstrstd.c_str();

    POINT pt = {pos.x(),pos.y()};

    IContextMenu *pcm;
    if (SUCCEEDED(GetUIObjectOfFile(hwnd, lpcwStr,
                                    IID_IContextMenu, (void**)&pcm))) {
        qDebug()<<"Succeed";
        HMENU hmenu = CreatePopupMenu();
        if (hmenu) {
        if (SUCCEEDED(pcm->QueryContextMenu(hmenu, 0,
                                            SCRATCH_QCM_FIRST, SCRATCH_QCM_LAST,
                                            CMF_SYNCCASCADEMENU))) {

            pcm->QueryInterface(IID_IContextMenu2, (void**)&scm->g_pcm2); // additional context menu stuff
            pcm->QueryInterface(IID_IContextMenu3, (void**)&scm->g_pcm3);

            int iCmd = TrackPopupMenuEx(hmenu, TPM_RETURNCMD,
                                        pos.x(), pos.y(), hwnd, NULL);
            if (scm->g_pcm2) { // additional context menu stuff
                scm->g_pcm2->Release();
                scm->g_pcm2 = NULL;
            }
            if (scm->g_pcm3) {
                scm->g_pcm3->Release();
                scm->g_pcm3 = NULL;
            }
            if (iCmd > 0) {
                qDebug()<<"kkk";
                CMINVOKECOMMANDINFOEX info = { 0 };
                info.cbSize = sizeof(info);
                info.fMask = CMIC_MASK_UNICODE | CMIC_MASK_PTINVOKE; // remember the point for "properties"
                if (GetKeyState(VK_CONTROL) < 0) { // send key states (for delete command)
                    info.fMask |= CMIC_MASK_CONTROL_DOWN;
                }
                if (GetKeyState(VK_SHIFT) < 0) {
                    info.fMask |= CMIC_MASK_SHIFT_DOWN;
                }
                info.hwnd = hwnd;
                info.lpVerb  = MAKEINTRESOURCEA(iCmd - SCRATCH_QCM_FIRST);
                info.lpDirectory = path.toLocal8Bit();
                info.lpVerbW = MAKEINTRESOURCEW(iCmd - SCRATCH_QCM_FIRST);
                info.nShow = SW_SHOWNORMAL;
                info.ptInvoke = (POINT)pt; // pass the point to "properties"
                pcm->InvokeCommand((LPCMINVOKECOMMANDINFO)&info);
            }
        }

        }
        pcm->Release();
    }
    else { // finish instantly if we don't have the UI Object
        qDebug()<<"No UI Object";
        return;
    }
}
