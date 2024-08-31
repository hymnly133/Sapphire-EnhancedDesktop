#ifndef SMYTASKSCHEDULE_H
#define SMYTASKSCHEDULE_H
// #include<stdio.h>
#define _WIN32_DCOM
#include <windows.h>
//  Include the task header file.
#include <taskschd.h>
#include <minwindef.h>
#include"QString"
// #include <atlbase.h>
class SMyTaskSchedule
{
public:
    SMyTaskSchedule();
    ~SMyTaskSchedule();

    //************************************
    // 函数名:  CMyTaskSchedule::NewTask
    // 返回类型:   BOOL
    // 功能: 创建计划任务
    // 参数1: char * lpszTaskName    计划任务名
    // 参数2: char * programPath    计划任务路径
    // 参数3: char * parameters        计划任务参数
    // 参数4: char * author            计划任务作者
    //************************************
    BOOL NewTask(QString taskName, QString programPath, QString parameters, QString author);
    //************************************
    // 函数名:  CMyTaskSchedule::Delete
    // 返回类型:   BOOL
    // 功能: 删除计划任务
    // 参数1: char * taskName    计划任务名
    //************************************
    BOOL Delete(QString taskName);
    BOOL Have(QString taskName);

private:
    ITaskService *m_lpITS;
    ITaskFolder *m_lpRootFolder;
};

#endif // SMYTASKSCHEDULE_H
