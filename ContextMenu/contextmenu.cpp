#include "contextmenu.h"
#include "SysFunctions.h"
#include "qdebug.h"
#include "shellitem.h"

#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QPoint>
#include <Shlobj.h>
#include <shlwapi.h>
#include <windows.h>

struct ContextMenuHelper
{
    static ContextMenuHelper* Instatnce()
    {
        static ContextMenuHelper helper;
        return &helper;
    }

    ~ContextMenuHelper()
    {
        if(pDesktop)
            pDesktop->Release();
    }

    bool isRootPath(QString const& path)
    {
        //for dirver D: D:/
        //for virtual driver, the path's len is larger than 3
        return path.size() > 1 && path.size() <= 3;
    }
    void showContextMenu(QStringList const& fileNames, void* handle, int x, int y)
    {
        QString parentPath = toWindowsPath(QFileInfo(fileNames[0]).path());
        qDebug()<<fileNames[0];
        qDebug()<<parentPath;
        LPSHELLFOLDER pParentFolder = getParentFolder(parentPath, isRootPath(fileNames[0]));

        if(!pParentFolder)
            return;

        LPITEMIDLIST* pidlFiles = (LPITEMIDLIST *)malloc(sizeof(LPITEMIDLIST) * fileNames.size());
        {
            LPENUMIDLIST pEnum;
            HRESULT hr = pParentFolder->EnumObjects(0, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &pEnum);
            if(SUCCEEDED(hr))
            {
                LPITEMIDLIST pidl;
                STRRET str;
                int index = 0;
                while(pEnum->Next(1, &pidl, 0) == S_OK)
                {
                    pParentFolder->GetDisplayNameOf(pidl, SHGDN_FORPARSING, &str);
                    QString fileName = toLinuxPath(strToString(pidl, &str));
                    qDebug()<<fileName;
                    if(!fileNames.contains(fileName)){
                            CoTaskMemFree(pidl);
                    }
                    else
                    {
                        if(index < fileNames.size())
                            pidlFiles[index++] = pidl;
                    }
                }
            }
        }

        IContextMenu   *pcm = nullptr;
        HRESULT hr = pParentFolder->GetUIObjectOf(0,
                                 fileNames.size(),
                                 (PCUITEMID_CHILD_ARRAY)pidlFiles,
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
                    IContextMenu2  *pcm2 = nullptr;
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
        for(int i = 0; i < fileNames.size(); i++)
            ILFree(pidlFiles[i]);
        free(pidlFiles);
        pParentFolder->Release();
    }

    LPSHELLFOLDER getParentFolder(QString const& filePath, bool isRoot = false)
    {
        LPSHELLFOLDER pDrives = getSpecialFolder(CSIDL_DRIVES);
        // LPSHELLFOLDER pDrives =  SHGetDesktopFolder(0);
        ShellItem::Ptr item;
        if(pDrives)
        {
            LPENUMIDLIST pEnum;
            HRESULT hr = pDrives->EnumObjects(0, SHCONTF_FOLDERS, &pEnum);
            if (SUCCEEDED(hr))
            {
                LPITEMIDLIST pidl;
                STRRET str;

                QString childPath;
                while(pEnum->Next(1, &pidl, 0) == S_OK)
                {
                    pDrives->GetDisplayNameOf(pidl, SHGDN_FORPARSING, &str);
                    childPath = strToString(pidl, &str);
                    qDebug()<<1<<childPath;

                    if(isRootPath(childPath) && filePath.startsWith(childPath))
                    {
                        item = ShellItem::Ptr(new ShellItem());
                        item->pidlRel = pidl;
                        pDrives->AddRef();
                        item->pParentFolder = pDrives;
                        break;
                    }
                }
                while(true)
                {
                    if(childPath == filePath)
                        break;

                   if(!getChildParent(item, childPath, filePath))
                       break;
                }
                pEnum->Release();
                if(item)
                {
                    if(isRoot)
                        return item->pParentFolder;

                    LPSHELLFOLDER pParentFolder = 0;
                    item->pParentFolder->BindToObject(item->pidlRel, 0, IID_IShellFolder,
                                                                                 (LPVOID*)&pParentFolder);
                    return pParentFolder;
                }
            }
        }
        return 0;
    }

    bool getChildParent(ShellItem::Ptr & item, QString &childPath, QString const& filePath)
    {
        if(!item)
            return  false;

        LPSHELLFOLDER  pParentFolder = NULL;
        HRESULT hr = item->pParentFolder->BindToObject(item->pidlRel, 0, IID_IShellFolder,
                                                             (LPVOID*)&pParentFolder);
        if(SUCCEEDED(hr))
        {
            LPENUMIDLIST   pEnum;
            hr = pParentFolder->EnumObjects(NULL, SHCONTF_FOLDERS|SHCONTF_INCLUDEHIDDEN, &pEnum);
            if(SUCCEEDED(hr))
            {
                LPITEMIDLIST   pidl;
                DWORD          dwFetched = 1;
                STRRET str;
                while(S_OK == (pEnum->Next(1, &pidl, &dwFetched)) && dwFetched)
                {
                     pParentFolder->GetDisplayNameOf(pidl, SHGDN_FORPARSING, &str);
                     childPath = strToString(pidl, &str);
                     qDebug()<<2<<childPath;

                     int index = filePath.indexOf(childPath);
                     if(index >= 0)
                     {
                        if(filePath == childPath || filePath.at(childPath.size()) == '\\')
                        {
                            ShellItem::Ptr newItem(new ShellItem());
                            newItem->pidlRel = pidl;
                            pParentFolder->AddRef();
                            newItem->pParentFolder = pParentFolder;
                            item = newItem;
                            pEnum->Release();
                            pParentFolder->Release();
                            qDebug()<<filePath<<childPath;
                            qDebug()<<"returned";
                            return true;
                        }
                     }
                }
                pEnum->Release();
            }
            pParentFolder->Release();
        }
        return false;
    }

    LPSHELLFOLDER getSpecialFolder(int idFolder)
    {
        LPITEMIDLIST pidl;
        LPSHELLFOLDER psf = NULL;

        HRESULT hr = SHGetSpecialFolderLocation(0, idFolder, &pidl);
        if (SUCCEEDED(hr))
        {
            pDesktop->BindToObject(pidl, NULL, IID_IShellFolder, (LPVOID *)&psf);
            CoTaskMemFree(pidl);
        }
        return psf;
    }

private:
    ContextMenuHelper()
    : pDesktop(0)
    {
        SHGetDesktopFolder(&pDesktop);
    }

    QString strToString(LPITEMIDLIST pidl, STRRET *str)
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

    IShellFolder* pDesktop;
};

void ContextMenu::show(QStringList const& fileNames,
                     void *handle, QPoint const& p)
{
    ContextMenuHelper::Instatnce()->showContextMenu(fileNames, handle, p.x(), p.y());
}
