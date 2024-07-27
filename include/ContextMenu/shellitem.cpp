#include "shellitem.h"
#include "shellmemmanager.h"

#include <QDebug>

ShellItem::ShellItem()
    : pParentFolder(0)
    , pidlRel(0)
    , pidlFQ(0)
{
}

ShellItem::~ShellItem()
{
    if(pParentFolder)
    {
        pParentFolder->Release();
        pParentFolder = 0;
    }
    if(pidlFQ)
    {
        theMemManager.free(pidlFQ);
        pidlFQ = 0;
    }
    if(pidlRel)
    {
        theMemManager.free(pidlRel);
        pidlRel = 0;
    }
}

LPITEMIDLIST ShellItem::clone(LPCITEMIDLIST pidl)
{
    if(!pidl)
        return 0;
    UINT size = getSize(pidl);
    LPITEMIDLIST   pidlTarget = (LPITEMIDLIST)theMemManager.alloc(size);
    if(!pidlTarget)
        return 0;

    CopyMemory(pidlTarget, pidl, size);
    return pidlTarget;
}

LPITEMIDLIST ShellItem::concat(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
    LPITEMIDLIST   pidlNew;
    UINT  cb1 = 0;
    if(pidl1)
        cb1 = getSize(pidl1) - 2 * sizeof(BYTE);
    UINT cb2 = getSize(pidl2);
    pidlNew = createItem(cb1 + cb2);
    if(pidlNew)
    {
        if(pidl1)
            CopyMemory(pidlNew, pidl1, cb1);
        CopyMemory(((LPBYTE)pidlNew) + cb1, pidl2, cb2);
    }
    return pidlNew;
}

bool ShellItem::operator < (ShellItem const& r)
{
    if(pParentFolder)
    {
        HRESULT hr = pParentFolder->CompareIDs(0, pidlRel, r.pidlRel);
        if(FAILED(hr))
            return false;
        int v = (short)SCODE_CODE(GetScode(hr));
        /*
         v < 0  pidl1 < pidl2
         v = 0  pidl1 = pidl2
         v > 0  pidl1 > pidl2
        */
        if(v < 0)
            return true;
    }
    return false;
}

void ShellItem::exec() const
{
    if(!pParentFolder)
        return;

    IContextMenu   *pcm;
    pParentFolder->AddRef();
    HRESULT  hr = pParentFolder->GetUIObjectOf(0,
                                 1,
                                 (LPCITEMIDLIST*)&pidlRel,
                                 IID_IContextMenu,
                                 0,
                                 (LPVOID*)&pcm);
    if(SUCCEEDED(hr))
    {
        HMENU hPopup = CreatePopupMenu();
        if(hPopup)
        {
            hr = pcm->QueryContextMenu(hPopup, 0, 1, 0x7fff, CMF_DEFAULTONLY | CMF_EXPLORE);

            if(SUCCEEDED(hr))
            {
                UINT  idCmd;

                idCmd = GetMenuItemID(hPopup, 0);

                if(idCmd && (idCmd != (UINT)-1))
                {
                    CMINVOKECOMMANDINFO  cmi;
                    cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
                    cmi.fMask = 0;
                    cmi.hwnd = 0;
                    cmi.lpVerb = (LPCSTR)(INT_PTR)(idCmd - 1);
                    cmi.lpParameters = NULL;
                    cmi.lpDirectory = NULL;
                    cmi.nShow = SW_SHOWNORMAL;
                    cmi.dwHotKey = 0;
                    cmi.hIcon = NULL;
                    pcm->InvokeCommand(&cmi);
                }
            }
            DestroyMenu(hPopup);
        }
    }
    pParentFolder->Release();
}

void ShellItem::contextMenu(void* handle, int x, int y) const
{
    if(!pParentFolder)
        return;

    IContextMenu   *pcm;
    pParentFolder->AddRef();
    HRESULT  hr = pParentFolder->GetUIObjectOf(0,
                                 1,
                                 (LPCITEMIDLIST*)&pidlRel,
                                 IID_IContextMenu,
                                 0,
                                 (LPVOID*)&pcm);
    if(SUCCEEDED(hr))
    {
        HMENU hPopup = CreatePopupMenu();
        if(hPopup)
        {
            hr = pcm->QueryContextMenu(hPopup, 0, 1, 0x7fff,  CMF_NORMAL);

            if(SUCCEEDED(hr))
            {
                IContextMenu2  *pcm2;
                pcm->QueryInterface(IID_IContextMenu2, (LPVOID*)&pcm2);
                UINT  idCmd;

                idCmd = TrackPopupMenu( hPopup,
                                        TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON,
                                        x,
                                        y,
                                        0,
                                        (HWND)handle,
                                        0);

                if(pcm2)
                {
                    pcm2->Release();
                    pcm2 = NULL;
                }
                if(idCmd && (idCmd != (UINT)-1))
                {
                    CMINVOKECOMMANDINFO  cmi;
                    cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
                    cmi.fMask = 0;
                    cmi.hwnd = 0;
                    cmi.lpVerb = (LPCSTR)(INT_PTR)(idCmd - 1);
                    cmi.lpParameters = NULL;
                    cmi.lpDirectory = NULL;
                    cmi.nShow = SW_SHOWNORMAL;
                    cmi.dwHotKey = 0;
                    cmi.hIcon = NULL;
                    pcm->InvokeCommand(&cmi);
                }
            }
            DestroyMenu(hPopup);
        }
    }
    pParentFolder->Release();
}

LPITEMIDLIST ShellItem::createItem(UINT size)
{
    LPITEMIDLIST pidl = (LPITEMIDLIST)theMemManager.alloc(size);

    if(pidl)
        ZeroMemory(pidl, size);
    return pidl;
}

UINT ShellItem::getSize(LPCITEMIDLIST pidl)
{
    UINT           size = 0;
    LPITEMIDLIST   pidlTemp = (LPITEMIDLIST) pidl;
    if(pidlTemp)
    {
        while(pidlTemp->mkid.cb)
        {
            size += pidlTemp->mkid.cb;
            pidlTemp = nextItem(pidlTemp);
        }
        size += 2 * sizeof(BYTE);
    }
    return size;
}

LPITEMIDLIST  ShellItem::nextItem(LPCITEMIDLIST pidl)
{
    if(pidl)
        return (LPITEMIDLIST)(LPBYTE)(((LPBYTE)pidl) + pidl->mkid.cb);
    else
        return 0;
}
