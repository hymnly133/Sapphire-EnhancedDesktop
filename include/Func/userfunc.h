#ifndef USERFUNC_H
#define USERFUNC_H
#include "qevent.h"



//启动
void SetUp();

//设置自启
void setMyAppAutoRun(bool isStart);

//检查快捷键
void checkForKey(QKeyEvent* event);

//重布局主屏幕
void resizeForActiveMW();

#endif // USERFUNC_H
