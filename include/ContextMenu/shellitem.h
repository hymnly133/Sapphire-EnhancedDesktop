#ifndef SHELLITEM_H
#define SHELLITEM_H
#include <Shlobj.h>
#include <memory>

class ShellItem
{
public:
    ShellItem();
    ShellItem(ShellItem const&) = delete;
    void operator = (ShellItem const) = delete;

    ~ShellItem();

    typedef std::shared_ptr<ShellItem> Ptr;

    LPSHELLFOLDER pParentFolder;
    LPITEMIDLIST pidlRel;
    LPITEMIDLIST pidlFQ;

    bool operator < (ShellItem const& r);
    void exec() const;
    void contextMenu(void* handle, int x, int y) const;

    static LPITEMIDLIST clone(LPCITEMIDLIST pidl);
    static LPITEMIDLIST concat(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);
private:
    static UINT getSize(LPCITEMIDLIST pidl);
    static LPITEMIDLIST  createItem(UINT size);
    static LPITEMIDLIST  nextItem(LPCITEMIDLIST pidl);
};

#endif // SHELLITEM_H
