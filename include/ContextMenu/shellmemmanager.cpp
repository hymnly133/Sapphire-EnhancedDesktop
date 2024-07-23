#include "shellmemmanager.h"

ShellMemManager::ShellMemManager()
{
    SHGetMalloc(&pMalloc);
}

ShellMemManager::~ShellMemManager()
{
    pMalloc->Release();
}

void *ShellMemManager::alloc(UINT size)
{
    return pMalloc->Alloc(size);
}

void ShellMemManager::free(void *pv)
{
    if(pv)
        pMalloc->Free(pv);
}

ShellMemManager & ShellMemManager::Instatnce()
{
    static ShellMemManager memManger;

    return memManger;
}
