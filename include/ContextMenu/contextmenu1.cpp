// #include <iostream>
// #define STRICT
// #include <windows.h>
// #include <windowsx.h>
// #include <ole2.h>
// #include <commctrl.h>
// #include <shlwapi.h>
// #include <shlobj.h>
// #include <stdio.h>

// #undef HANDLE_WM_CONTEXTMENU
// #define HANDLE_WM_CONTEXTMENU(hwnd, wParam, lParam, fn) \
// ((fn)((hwnd), (HWND)(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)), 0L)
// #define SCRATCH_QCM_FIRST 1
// #define SCRATCH_QCM_LAST  0x7FFF
// #define WND_CLASS_NAME L"ContextMenuMainHidden"


//     HINSTANCE g_hinst;                          /* This application's HINSTANCE */
// HWND g_hwndChild;                           /* Optional child window */
// IContextMenu2 *g_pcm2;						/* context-submenus */
// IContextMenu3 *g_pcm3;
// LPWSTR g_fileName;							/* file name to display the context menu for (1st argument)*/


// /*
//  * Returns the UI Object for the given file, allowing to display the context menu.
//  */
// HRESULT GetUIObjectOfFile(HWND hwnd, LPCWSTR pszPath, REFIID riid, void **ppv)
// {
//     *ppv = NULL;
//     HRESULT hr;
//     LPITEMIDLIST pidl;
//     SFGAOF sfgao;
//     if (SUCCEEDED(hr = SHParseDisplayName(pszPath, NULL, &pidl, 0, &sfgao))) {
//         IShellFolder *psf;
//         LPCITEMIDLIST pidlChild;
//         if (SUCCEEDED(hr = SHBindToParent(pidl, IID_IShellFolder,
//                                           (void**)&psf, &pidlChild))) {
//             hr = psf->GetUIObjectOf(hwnd, 1, &pidlChild, riid, NULL, ppv);
//             psf->Release();
//         }
//         CoTaskMemFree(pidl);
//     }
//     return hr;
// }


// /*
//  * Browses top-level windows, stores "1" into the address pointed to by lParam
//  * if a dialog window belonging to this process is found (i.e. the "file properties"
//  * window).
//  */
// BOOL CALLBACK OnEnum(HWND hwnd, LPARAM lParam)
// {
//     TCHAR buf [255];
//     /*, buf2[255];	//DEBUG
//       {
//     FILE * f = fopen("log.txt", "a");*/
//     DWORD pid;

//     GetClassName(hwnd, buf, 255);
//     GetWindowThreadProcessId(hwnd, &pid);
//     if (wcscmp(buf, L"#32770") == 0 && pid == GetCurrentProcessId()){
//         *((DWORD *) lParam) = 1;
//     }
//     /*	GetWindowText(hwnd, buf2, 255); //DEBUG
//         fwprintf(f, L"0x%x %s %s %d\n", hwnd, buf, buf2, pid);
//         fclose(f);
//         } */
//     return true;
// }


// /*
//  * OnTimer - each 3 secs. tests if the app should finish (i.e. no dialogs
//  * from this process are displayed).
//  */
// VOID CALLBACK OnTimer(HWND hwnd, UINT msg, unsigned int idEvent, DWORD dwTime)
// {
//     /* FILE * f = fopen("log.txt", "a"); // DEBUG
//        fprintf(f, "NEXT\n", hwnd);
//        fclose(f); */
//     DWORD found = 0;
//     EnumWindows(&OnEnum, (LPARAM) &found);

//     if (found){
//         SetTimer(hwnd, 1, 3000, &OnTimer);
//     }
//     else {
//         DestroyWindow(hwnd);
//     }
// }


// /*
//  * Displays the context menu, sets finish test timer at the end.
//  */
// void OnContextMenu(HWND hwnd, HWND hwndContext, int xPos, int yPos)
// {
//     POINT pt = { xPos, yPos };
//     if (pt.x == -1 && pt.y == -1) {
//         pt.x = pt.y = 0;
//         ClientToScreen(hwnd, &pt);
//     }

//     IContextMenu *pcm;
//     if (SUCCEEDED(GetUIObjectOfFile(hwnd, g_fileName,
//                                     IID_IContextMenu, (void**)&pcm))) {
//         HMENU hmenu = CreatePopupMenu();
//         if (hmenu) {
//             if (SUCCEEDED(pcm->QueryContextMenu(hmenu, 0,
//                                                 SCRATCH_QCM_FIRST, SCRATCH_QCM_LAST,
//                                                 CMF_NORMAL))) {

//                 pcm->QueryInterface(IID_IContextMenu2, (void**)&g_pcm2); // additional context menu stuff
//                 pcm->QueryInterface(IID_IContextMenu3, (void**)&g_pcm3);

//                 int iCmd = TrackPopupMenuEx(hmenu, TPM_RETURNCMD,
//                                             pt.x, pt.y, hwnd, NULL);
//                 if (g_pcm2) { // additional context menu stuff
//                     g_pcm2->Release();
//                     g_pcm2 = NULL;
//                 }
//                 if (g_pcm3) {
//                     g_pcm3->Release();
//                     g_pcm3 = NULL;
//                 }
//                 if (iCmd > 0) {
//                     CMINVOKECOMMANDINFOEX info = { 0 };
//                     info.cbSize = sizeof(info);
//                     info.fMask = CMIC_MASK_UNICODE | CMIC_MASK_PTINVOKE; // remember the point for "properties"
//                     if (GetKeyState(VK_CONTROL) < 0) { // send key states (for delete command)
//                         info.fMask |= CMIC_MASK_CONTROL_DOWN;
//                     }
//                     if (GetKeyState(VK_SHIFT) < 0) {
//                         info.fMask |= CMIC_MASK_SHIFT_DOWN;
//                     }
//                     info.hwnd = hwnd;
//                     info.lpVerb  = MAKEINTRESOURCEA(iCmd - SCRATCH_QCM_FIRST);
//                     info.lpVerbW = MAKEINTRESOURCEW(iCmd - SCRATCH_QCM_FIRST);
//                     info.nShow = SW_SHOWNORMAL;
//                     info.ptInvoke = pt; // pass the point to "properties"
//                     pcm->InvokeCommand((LPCMINVOKECOMMANDINFO)&info);
//                 }
//             }
//             DestroyMenu(hmenu);
//         }
//         pcm->Release();
//         // set timer to test if we need to finish
//         SetTimer(hwnd, 1, 3000, &OnTimer);
//     }
//     else { // finish instantly if we don't have the UI Object
//         DestroyWindow(hwnd);
//     }
// }



// /*
//  *  OnSize
//  *      If we have an inner child, resize it to fit.
//  */
// void
// OnSize(HWND hwnd, UINT state, int cx, int cy)
// {
//     if (g_hwndChild) {
//         MoveWindow(g_hwndChild, 0, 0, cx, cy, TRUE);
//     }
// }

// /*
//  *  OnCreate
//  *      Applications will typically override this and maybe even
//  *      create a child window.
//  */
// BOOL
// OnCreate(HWND hwnd, LPCREATESTRUCT lpcs)
// {
//     return TRUE;
// }

// /*
//  *  OnDestroy
//  *      Post a quit message because our application is over when the
//  *      user closes this window.
//  */
// void
// OnDestroy(HWND hwnd)
// {
//     PostQuitMessage(0);
// }

// /*
//  *  PaintContent
//  *      Interesting things will be painted here eventually.
//  */
// void
// PaintContent(HWND hwnd, PAINTSTRUCT *pps)
// {
// }

// /*
//  *  OnPaint
//  *      Paint the content as part of the paint cycle.
//  */
// void
// OnPaint(HWND hwnd)
// {
//     PAINTSTRUCT ps;
//     BeginPaint(hwnd, &ps);
//     PaintContent(hwnd, &ps);
//     EndPaint(hwnd, &ps);
// }

// /*
//  *  OnPrintClient
//  *      Paint the content as requested by USER.
//  */
// void
// OnPrintClient(HWND hwnd, HDC hdc)
// {
//     PAINTSTRUCT ps;
//     ps.hdc = hdc;
//     GetClientRect(hwnd, &ps.rcPaint);
//     PaintContent(hwnd, &ps);

// }

// /*
//  *  Window procedure for the hidden main window (gets notifications from
//  *  the context menu, displays the context menu)
//  */
// LRESULT CALLBACK
// WndProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
// {
//     if (g_pcm3) { // additional context menu stuff
//         LRESULT lres;
//         if (SUCCEEDED(g_pcm3->HandleMenuMsg2(uiMsg, wParam, lParam, &lres))) {
//             return lres;
//         }
//     }
//     else if (g_pcm2) {
//         if (SUCCEEDED(g_pcm2->HandleMenuMsg(uiMsg, wParam, lParam))) {
//             return 0;
//         }
//     }


//     switch (uiMsg) {

//         HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
//         HANDLE_MSG(hwnd, WM_SIZE, OnSize);
//         HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
//         HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
//         HANDLE_MSG(hwnd, WM_CONTEXTMENU, OnContextMenu);
//     case WM_PRINTCLIENT: OnPrintClient(hwnd, (HDC)wParam); return 0;
//     }

//     return DefWindowProc(hwnd, uiMsg, wParam, lParam);
// }

// /*
//  * Initialize the (hidden) main window
//  */
// BOOL
// InitApp(void)
// {
//     WNDCLASS wc;

//     wc.style = 0;
//     wc.lpfnWndProc = WndProc;
//     wc.cbClsExtra = 0;
//     wc.cbWndExtra = 0;
//     wc.hInstance = g_hinst;
//     wc.hIcon = NULL;
//     wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//     wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//     wc.lpszMenuName = NULL;
//     wc.lpszClassName = WND_CLASS_NAME;

//     if (!RegisterClass(&wc)) return FALSE;

//     InitCommonControls();               /* In case we use a common control */

//     return TRUE;
// }


// /*
//  *
//  */
// int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hinstPrev,
//                    LPSTR lpCmdLine, int nShowCmd)
// {
//     MSG msg;
//     HWND hwnd;
//     LPWSTR * argv;
//     int argc;

//     g_hinst = hinst;

//     if (!InitApp()) return 1;

//     /* get the file name as the first command line argument */
//     argv = CommandLineToArgvW(GetCommandLineW(), &argc);
//     if (argc < 2){
//         return 1;
//     }

//     g_fileName = argv[1];

//     /* correct for stupid directory name parsing in quotes
//        (last char & file doesn't exist " -> \) */
//     if (g_fileName[wcslen(g_fileName)-1] == '"'
//         && GetFileAttributes(g_fileName) == INVALID_FILE_ATTRIBUTES){

//         DWORD err = GetLastError();
//         if (err == ERROR_INVALID_NAME){
//             g_fileName[wcslen(g_fileName)-1] = '\\';
//         }
//     }

//     if (SUCCEEDED(CoInitialize(NULL))) {/* In case we use COM */

//         hwnd = CreateWindow( // dummy window
//             WND_CLASS_NAME,                      /* Class Name */
//             L"",                      /* Title */
//             WS_OVERLAPPEDWINDOW,            /* Style */
//             CW_USEDEFAULT, CW_USEDEFAULT,   /* Position */
//             0, 0,   /* Size */
//             NULL,                           /* Parent */
//             NULL,                           /* No menu */
//             hinst,                          /* Instance */
//             0);                             /* No special parameters */

//         ShowWindow(hwnd, SW_HIDE);
//         PostMessage(hwnd, WM_CONTEXTMENU, (WPARAM) hwnd, 0); // force show context menu

//         while (GetMessage(&msg, NULL, 0, 0)) {
//             TranslateMessage(&msg);
//             DispatchMessage(&msg);
//         }

//         CoUninitialize();
//     }
//     else {
//         return 1;
//     }

//     return 0;
// }
