#ifndef MOUSEHOOK_H
#define MOUSEHOOK_H
#include<windows.h>
#include <QObject>
class MouseHook:public QObject
{
public:
    MouseHook();
    ~MouseHook();
public:
    //钩子函数，处理键盘事件
    static LRESULT CALLBACK MouseHookEvent(int nCode, WPARAM wParam,
                                         LPARAM lParam);

    //设置回调函数，可以从外部设置，这样keyHookEvent处理的时候可以执行这个回调
    void SetMouseOnCallBack( void(*func)(int,int));
    void SetMouseOffCallBack( void(*func)(int,int));
    void SetMouseMoveCallBack( void(*func)(int,int));
private:
    static HHOOK mouse_hook_;//hook对象
    static void(*m_OnFunc)(int,int) ;//按下的回调,int代表键码
    static void(*m_OffFunc)(int,int) ;//抬起的回调,int代表键码
    static void(*m_MoveFunc)(int,int) ;//抬起的回调,int代表键码
};
#endif
