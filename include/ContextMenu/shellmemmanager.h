#ifndef SHELLMEMMANAGER_H
#define SHELLMEMMANAGER_H
#include <shlobj.h>

class ShellMemManager
{
    ShellMemManager();
public:
    ~ShellMemManager();
    static ShellMemManager & Instatnce();

    void *alloc(UINT size);
    void free(void *pv);
private:
    LPMALLOC pMalloc;
};

#define theMemManager ShellMemManager::Instatnce()

#endif // SHELLMEMMANAGER_H
