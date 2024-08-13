#ifndef USERFUNC_H
#define USERFUNC_H
#include "mainwindow.h"
#include "qevent.h"



//启动
void SetUp();

//加载主窗口数据
bool loadMainWindows();

//创建主窗口
void setupMainWindows();

//设置自启
void setMyAppAutoRun(bool isStart);



//设置创建注册表数据
void setSapphireRegDate(bool isSet);

//检查快捷键
void checkForKey(QKeyEvent* event);

//重新调整对应的BlockLayout
void resizeForWithDialog(SBlockLayout* aimlayout);

//模式切换
void toDesktopMode();
void toEditMode();
void switchMode();

//更新字体
void updateFont();

//扫描文件改动
void scanForChange();

void SExit();
#endif // USERFUNC_H
