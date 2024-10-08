﻿#ifndef USERFUNC_H
#define USERFUNC_H
#include "mainwindow.h"
#include "qevent.h"
class SDir;

//最初步的初始化，并创建主窗口并显示启动动画
void preSetupG();

//主要的加载函数
void setupG();

//加载主窗口数据
bool loadMainWindows();

//设置自启动
void setMyAppAutoRun(bool isStart);

//设置创建注册表数据
void setSapphireRegDate(bool isSet);

//检查快捷键
void checkForKey(QKeyEvent* event);

//重新调整对应的BlockLayout
void resizeForWithDialog(SBlockLayout* aimlayout);

//切换到桌面模式
void toDesktopMode();
//切换到编辑模式
void toEditMode();


//切换模式
void switchMode();

//更新字体
void updateFont();

//扫描文件改动
void scanForChange();

//扫描一个已经存在的SFileInfo（Dir）下的改动
void scanForChangeInDir(QString path, SLayoutContainer* layoutContainer, QStringList *newfiles, bool add = false);

//打开设置面板
void openSettingWindow();

// 退出
void SExit();
// 重启
void SReboot();

//处理参数
void processArguments();

//检测自启动并更新启动方法,反映在全局变量
void checkForAutoRun();
#endif // USERFUNC_H
