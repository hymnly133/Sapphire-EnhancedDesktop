#include "desktopmenu.h"
#include "ContextMenu/shellmenuitem.h"
#include "qwinfunctions.h"
#include <Shlobj.h>
#include <shlwapi.h>
#include <windows.h>

static QString strToString(LPITEMIDLIST pidl, STRRET *str)
{
    LPTSTR pszText;
    QString text;
    HRESULT hr = StrRetToStr(str, pidl, &pszText);
    if (SUCCEEDED(hr))
    {
        text = QString::fromStdWString(pszText);
        CoTaskMemFree(pszText);
    }
    return text;
}

ShellMenuItems DesktopMenu::computerShellItems()
{
    LPITEMIDLIST pidl;
    HRESULT hr = SHGetSpecialFolderLocation(0, CSIDL_DRIVES, &pidl);
    ShellMenuItems items;
    if(SUCCEEDED(hr))
    {
        LPSHELLFOLDER  pDesktop = NULL;
        if(FAILED(SHGetDesktopFolder(&pDesktop)))
            return items;

        ShellMenuItem menutItem;
        pDesktop->AddRef();
        menutItem.pItem = ShellItem::Ptr(new ShellItem());
        menutItem.pItem->pidlRel = pidl;
        menutItem.pItem->pidlFQ = ShellItem::clone(pidl);
        menutItem.pItem->pParentFolder = pDesktop;

        shellSubMenuItems(menutItem, items, true);
        pDesktop->Release();
    }
    return items;
}

ShellMenuItems DesktopMenu::desktopMenuItems()
{
    LPITEMIDLIST pidl;
    HRESULT hr = SHGetSpecialFolderLocation(0, CSIDL_DESKTOP, &pidl);
    ShellMenuItems items;
    if(SUCCEEDED(hr))
    {
        ShellMenuItem menutItem;
        menutItem.pItem = ShellItem::Ptr(new ShellItem());
        menutItem.pItem->pidlRel = pidl;
        menutItem.pItem->pidlFQ = ShellItem::clone(pidl);

        shellSubMenuItems(menutItem, items, true);
    }
    return items;
}

void DesktopMenu::shellSubMenuItems(ShellMenuItem const& item,
                                    ShellMenuItems & menuItems,
                                    bool isOnlyDir)
{
    LPSHELLFOLDER  pParentFolder = NULL;
    HRESULT        hr;

    if(!item.pItem->pParentFolder)
        hr = SHGetDesktopFolder(&pParentFolder);
    else
        hr = item.pItem->pParentFolder->BindToObject(item.pItem->pidlRel, 0, IID_IShellFolder, (LPVOID*)&pParentFolder);

    if(FAILED(hr))
        return;

    LPENUMIDLIST   pEnum;
    SHCONTF grfFlags = SHCONTF_FOLDERS;
    if(!isOnlyDir)
        grfFlags = SHCONTF_FOLDERS | SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN;
    hr = pParentFolder->EnumObjects(NULL, grfFlags, &pEnum);
    if(SUCCEEDED(hr))
    {
        LPITEMIDLIST   pidlTemp;
        DWORD          dwFetched = 1;
        STRRET str;
        while(S_OK == (pEnum->Next(1, &pidlTemp, &dwFetched)) && dwFetched)
        {
            ShellMenuItem menutItem;
            pParentFolder->GetDisplayNameOf(pidlTemp, SHGDN_FORPARSING, &str);
            menutItem.filePath = strToString(pidlTemp, &str);
            menutItem.pItem = ShellItem::Ptr(new ShellItem());
            menutItem.pItem->pParentFolder = pParentFolder;
            menutItem.pItem->pidlRel = pidlTemp;
            menutItem.pItem->pidlFQ = ShellItem::concat(item.pItem->pidlFQ, pidlTemp);
            pParentFolder->AddRef();

            SHFILEINFO     sfi;
            if(SHGetFileInfo((LPCTSTR)menutItem.pItem->pidlFQ, 0, &sfi, sizeof(sfi),
                              SHGFI_PIDL | SHGFI_DISPLAYNAME | SHGFI_TYPENAME | SHGFI_ICON | SHGFI_SMALLICON))
            {
                menutItem.caption = QString::fromStdWString(sfi.szDisplayName);
                menutItem.icon = QIcon(QtWin::fromHICON(sfi.hIcon));
            }
            menuItems << menutItem;
            dwFetched = 0;
        }
        pEnum->Release();
    }
    pParentFolder->Release();

    std::sort(menuItems.begin(), menuItems.end());
}
