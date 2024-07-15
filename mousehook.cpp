#include "mousehook.h"
#include<windows.h>
#include <QApplication>
#include <QDebug>
#include <QCursor>
#include <QMouseEvent>

//静态变量类外初始化
HHOOK MouseHook:: mouse_hook_ = NULL;
void(*MouseHook::m_OnFunc)(int,int)  = NULL;
void(*MouseHook::m_OffFunc)(int,int)  = NULL;
void(*MouseHook::m_MoveFunc)(int,int)  = NULL;
MouseHook::MouseHook()
{
    Q_ASSERT(!mouse_hook_);
    //构造函数里把MouseHookEvent函数的地址当钩子写入到windows消息循环
    mouse_hook_ = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseHookEvent, GetModuleHandle(NULL), 0);
    qDebug("hook created");
}

MouseHook::~MouseHook()
{
    //析构函数中释放这个钩子
    if (nullptr != mouse_hook_)
    {
        UnhookWindowsHookEx(mouse_hook_);
    }
}

void MouseHook::SetMouseMoveCallBack(void(*func)(int,int))
{
    m_MoveFunc=func;
        qDebug("func1 created");
}
void MouseHook::SetMouseOnCallBack(void(*func)(int,int))
{
    m_OnFunc=func;
        qDebug("func2 created");
}
void MouseHook::SetMouseOffCallBack(void(*func)(int,int))
{
    m_OffFunc=func;
        qDebug("func3 created");
}


LRESULT CALLBACK MouseHook::MouseHookEvent(int ncode, WPARAM wParam, LPARAM lParam)
{
    LPMSLLHOOKSTRUCT p = (LPMSLLHOOKSTRUCT)lParam;
    POINT   pt = p->pt;
    DWORD   mouseData = p->time;

    const char* info = NULL;
    char text[60], pData[50], mData[50];

    PAINTSTRUCT ps;
    HDC hdc;
    // QPoint point = QCursor::pos();  // 获取鼠标当前位置
    // switch (wParam)
    // {
    // case WM_LBUTTONDOWN:   // 鼠标左键按下
    //     emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
    //     break;
    // case WM_MOUSEMOVE:     // 鼠标移动
    //     emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseMove, point, Qt::NoButton, Qt::NoButton, Qt::NoModifier));
    //     break;
    // case WM_RBUTTONDOWN:   // 鼠标右键按下
    //     emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
    //     break;
    // case WM_RBUTTONUP:     // 鼠标右键抬起
    //     emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
    //     break;
    // case WM_LBUTTONUP:     // 鼠标左键抬起
    //     emit GlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
    //     break;
    // case WM_MOUSEWHEEL:    // 鼠标滚轮
    // {
    //     MSLLHOOKSTRUCT * msll = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
    //     //        qDebug() << QString("坐标：(%1, %2)").arg(msll->pt.x).arg(msll->pt.y);     // 获取鼠标坐标
    //     int delta = GET_WHEEL_DELTA_WPARAM(msll->mouseData);                     // 获取滚轮状态，向前：120，向后-120
    //     emit GlobalMouseEvent::getInstance()->wheelEvent(new QWheelEvent(point, delta, Qt::MiddleButton, Qt::NoModifier));
    //     break;
    // }
    // default:
    //     break;
    // }

  if (ncode >= 0)
    {
        if (wParam == WM_MOUSEMOVE)
        {
            info = "鼠标 [移动]";
            m_MoveFunc(pt.x,pt.y);
        }
        else if (wParam == WM_LBUTTONDOWN)
        {
            info = "鼠标 [左键] 按下";
            m_OnFunc(pt.x,pt.y);
        }
        else if (wParam == WM_LBUTTONUP)
        {
            info = "鼠标 [左键] 抬起";
            m_OffFunc(pt.x,pt.y);
        }
        else if (wParam == WM_LBUTTONDBLCLK)
        {
            info = "鼠标 [左键] 双击";
        }
        else if (wParam == WM_RBUTTONDOWN)
        {
            info = "鼠标 [右键] 按下";
            m_OnFunc(pt.x,pt.y);
        }
        else if (wParam == WM_RBUTTONUP)
        {
            info = "鼠标 [右键] 抬起";
            m_OffFunc(pt.x,pt.y);
        }
        else if (wParam == WM_RBUTTONDBLCLK)
        {
            info = "鼠标 [右键] 双击";
        }
        else if (wParam == WM_MBUTTONDOWN)
        {
            info = "鼠标 [滚轮] 按下";
        }
        else if (wParam == WM_MBUTTONUP)
        {
            info = "鼠标 [滚轮] 抬起";
        }
        else if (wParam == WM_MBUTTONDBLCLK)
        {
            info = "鼠标 [滚轮] 双击";
        }
        else if (wParam == WM_MOUSEWHEEL)
        {
            info = "鼠标 [滚轮] 滚动";
        }

        ZeroMemory(text, sizeof(text));
        ZeroMemory(pData, sizeof(pData));
        ZeroMemory(mData, sizeof(mData));

        qDebug() << "鼠标状态: " << info;
        qDebug() << "X: " << pt.x << " Y: " << pt.y ;
        qDebug() << "附加数据: " << mouseData ;
    }

    return CallNextHookEx(mouse_hook_, ncode, wParam, lParam);
}
