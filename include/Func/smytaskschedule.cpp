//CMyTaskSchedule.cpp

// #include "pch.h"
#include "sMyTaskSchedule.h"

SMyTaskSchedule::SMyTaskSchedule()
{
    m_lpITS = NULL;
    m_lpRootFolder = NULL;
    //初始化COM
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        MessageBox(NULL, L"初始化COM接口环境失败!", L"Error", MB_OK);
        return;
    }

    //创建任务服务对象
    hr = CoCreateInstance(CLSID_TaskScheduler, NULL,
                          CLSCTX_INPROC_SERVER, IID_ITaskService,
                          (LPVOID*)(&m_lpITS));
    if (FAILED(hr)) {
        MessageBox(NULL, L"创建任务服务对象失败!", L"Error", MB_OK);
        return;
    }

    //连接到任务服务
    hr = m_lpITS->Connect(VARIANT(), VARIANT(), VARIANT(), VARIANT());
    if (FAILED(hr)) {
        MessageBox(NULL, L"连接到任务服务失败!", L"Error", MB_OK);
        return;
    }

    //获取根任务的指针
    //获取Root Task Folder 的指针 ，这个指针指向的是新注册的任务
    // wchar_t* wnewrstr = nullptr;

    std::wstring str_ = QString("\\").toStdWString();
    BSTR wnewrstr = SysAllocString(str_.c_str());



    hr = m_lpITS->GetFolder(wnewrstr, &m_lpRootFolder);
    if (FAILED(hr)) {
        MessageBox(NULL, L"获取根任务的指针失败!", L"Error", MB_OK);
        return;
    }
    SysFreeString(wnewrstr);
}

SMyTaskSchedule::~SMyTaskSchedule()
{

    if (m_lpITS) {
        m_lpITS->Release();
    }
    if (m_lpRootFolder) {
        m_lpRootFolder->Release();
    }
    // 卸载COM
    CoUninitialize();
}

//创建任务计划
BOOL SMyTaskSchedule::NewTask(QString taskName, QString programPath, QString parameters, QString author)
{
    if (NULL == m_lpRootFolder) {
        return FALSE;
    }

    //如果存在相同的计划任务，则删除
    Delete(taskName);

    //创建任务定义对象来创建任务
    ITaskDefinition *pTaskDefinition = NULL;
    HRESULT hr = m_lpITS->NewTask(0, &pTaskDefinition);
    if (FAILED(hr)) {
        MessageBox(NULL, L"创建任务失败!", L"Error", MB_OK);
        return FALSE;
    }
    //设置注册信息
    IRegistrationInfo *pRegInfo = NULL;
    hr = pTaskDefinition->get_RegistrationInfo(&pRegInfo);
    if (FAILED(hr)) {
        MessageBox(NULL, L"设置注册信息失败!", L"Error", MB_OK);
        return FALSE;
    }
    // 设置作者信息
    // wchar_t* wauthorstr = nullptr;
    // author.toWCharArray(wauthorstr);
    std::wstring str_wauthorstr = author.toStdWString();
    BSTR wauthorstr = SysAllocString(str_wauthorstr.c_str());
    hr = pRegInfo->put_Author(wauthorstr);


    pRegInfo->Release();
    // 设置登录类型和运行权限
    IPrincipal *pPrincipal = NULL;
    hr = pTaskDefinition->get_Principal(&pPrincipal);
    if (FAILED(hr)) {
        MessageBox(NULL, L"设置登录类型和运行权限失败!", L"Error", MB_OK);
        return FALSE;
    }
    // 设置登录类型
    hr = pPrincipal->put_LogonType(TASK_LOGON_INTERACTIVE_TOKEN);
    // 设置运行权限
    // 最高权限
    hr = pPrincipal->put_RunLevel(TASK_RUNLEVEL_HIGHEST);
    pPrincipal->Release();
    // 设置其他信息
    ITaskSettings *pSettting = NULL;
    hr = pTaskDefinition->get_Settings(&pSettting);
    if (FAILED(hr)) {
        MessageBox(NULL, L"设置其他信息失败!", L"Error", MB_OK);
        return FALSE;
    }
    // 设置其他信息
    hr = pSettting->put_StopIfGoingOnBatteries(VARIANT_FALSE);
    hr = pSettting->put_DisallowStartIfOnBatteries(VARIANT_FALSE);
    hr = pSettting->put_AllowDemandStart(VARIANT_TRUE);
    hr = pSettting->put_StartWhenAvailable(VARIANT_FALSE);
    hr = pSettting->put_MultipleInstances(TASK_INSTANCES_PARALLEL);
    pSettting->Release();
    // 创建执行动作
    IActionCollection *pActionCollect = NULL;
    hr = pTaskDefinition->get_Actions(&pActionCollect);
    if (FAILED(hr)) {
        MessageBox(NULL, L"创建执行动作失败!", L"Error", MB_OK);
        return FALSE;
    }
    IAction *pAction = NULL;
    // 创建执行操作
    hr = pActionCollect->Create(TASK_ACTION_EXEC, &pAction);
    pActionCollect->Release();
    // 设置执行程序路径和参数
    // CComVariant variantProgramPath(NULL);
    // CComVariant variantParameters(NULL);
    IExecAction *pExecAction = NULL;
    hr = pAction->QueryInterface(IID_IExecAction, (PVOID *)(&pExecAction));
    if (FAILED(hr)) {
        pAction->Release();
        MessageBox(NULL, L"创建执行动作失败!", L"Error", MB_OK);
        return FALSE;
    }
    pAction->Release();
    // 设置程序路径和参数
    std::wstring str_wprostr = programPath.toStdWString();
    BSTR wprostr = SysAllocString(str_wprostr.c_str());

    std::wstring str_wparastr = parameters.toStdWString();
    BSTR wparastr = SysAllocString(str_wparastr.c_str());


    pExecAction->put_Path(wprostr);
    pExecAction->put_Arguments(wparastr);
    pExecAction->Release();
    // 设置触发器信息，包括用户登录时触发
    ITriggerCollection *pTriggers = NULL;
    hr = pTaskDefinition->get_Triggers(&pTriggers);
    if (FAILED(hr)) {
        MessageBox(NULL, L"设置触发器信息失败!", L"Error", MB_OK);
        return FALSE;
    }
    // 创建触发器
    ITrigger *pTrigger = NULL;
    hr = pTriggers->Create(TASK_TRIGGER_LOGON, &pTrigger);
    if (FAILED(hr)) {
        MessageBox(NULL, L"创建触发器失败!", L"Error", MB_OK);
        return FALSE;
    }
    // 注册任务计划
    IRegisteredTask *pRegisteredTask = NULL;

    // wchar_t* wtaskstr = nullptr;
    // taskName.toWCharArray(wtaskstr);

    std::wstring str_wtaskstr = taskName.toStdWString();
    BSTR wtaskstr = SysAllocString(str_wtaskstr.c_str());


    hr = m_lpRootFolder->RegisterTaskDefinition((wtaskstr),
         pTaskDefinition,
         TASK_CREATE_OR_UPDATE,
         VARIANT(),
         VARIANT(),
         TASK_LOGON_INTERACTIVE_TOKEN,
         VARIANT(),
         &pRegisteredTask);
    if (FAILED(hr)) {
        pTaskDefinition->Release();
        MessageBox(NULL, L"注册任务计划失败!", L"Error", MB_OK);
        return FALSE;
    }
    pTaskDefinition->Release();
    pRegisteredTask->Release();
    SysFreeString(wtaskstr);
    SysFreeString(wparastr);
    SysFreeString(wprostr);
    SysFreeString(wauthorstr);
    return TRUE;
}

//删除计划任务
BOOL SMyTaskSchedule::Delete(QString taskName)
{
    if(NULL == m_lpRootFolder) {
        return FALSE;
    }
    // CComVariant variantTaskName(NULL);
    // variantTaskName = lpszTaskName;
    std::wstring str_wtaskstr = taskName.toStdWString();
    BSTR wtaskstr = SysAllocString(str_wtaskstr.c_str());

    HRESULT hr = m_lpRootFolder->DeleteTask(wtaskstr, 0);

    SysFreeString(wtaskstr);
    if (FAILED(hr)) {
        return FALSE;
    }
    return TRUE;
};

BOOL SMyTaskSchedule::Have(QString taskName)
{
    IRegisteredTask* task = NULL;
    taskName = "\\" + taskName;
    if(NULL == m_lpRootFolder) {
        return FALSE;
    }
    std::wstring str_wtaskstr = taskName.toStdWString();
    BSTR wtaskstr = SysAllocString(str_wtaskstr.c_str());

    HRESULT hr = m_lpRootFolder->GetTask(wtaskstr, &task);

    SysFreeString(wtaskstr);

    if (FAILED(hr)) {
        return FALSE;
    }
    return TRUE;
}
