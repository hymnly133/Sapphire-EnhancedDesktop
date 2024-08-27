#include "aerowidget.h"

AeroWidget::AeroWidget(QWidget *parent) : QWidget(parent)
{
    _parent = parent;
    HWND hWnd = HWND(parent->winId());
    HMODULE hUser = GetModuleHandle(L"user32.dll");
    if (hUser) {
        pfun setWindowCompositionAttribute = (pfun)GetProcAddress(hUser, "SetWindowCompositionAttribute");
        if (setWindowCompositionAttribute) {
            AccentPolicy accent = { ACCENT_ENABLE_BLURBEHIND, 0, 0, 0 };
            WindowCompositionAttributeData data;
            data.Attribute = WCA_ACCENT_POLICY;
            data.Data = reinterpret_cast<int *>(&accent) ;
            data.SizeOfData = sizeof(accent);
            setWindowCompositionAttribute(hWnd, &data);
            /*
                setWindowCompositionAttribute
                一个官方文档里面没有记录上去的函数,具体上网百度去
                该函数在此处用于设置毛玻璃背景
            */
        }
    }
    parent->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    bgColor = QColor(255, 255, 255, 100);
}
//当毛玻璃的透明程度变化时就可以用下面的一个slot和一个函数来更新
void AeroWidget::valueChanged_Slot(int v)
{
    bgColor.setAlpha(v);//设置透明度
    this->update();//更新
}
void AeroWidget::setAlpha(int v)
{
    bgColor.setAlpha(v);//设置透明度
    this->update();//更新
}
void AeroWidget::paintEvent(QPaintEvent *ev)
{
    AERO(this->_parent, this->bgColor); //更新透明毛玻璃背景
}
