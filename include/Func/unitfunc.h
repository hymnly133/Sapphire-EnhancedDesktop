#ifndef UNITFUNC_H
#define UNITFUNC_H

#include "qevent.h"
#include "slayout.h"
#define eachDoAsUnit(FUNC)\
if(numCelected>=1){\
    foreach (SUnit* unit, pCelectedUnits)\
    FUNC \
}\
else if(sender){\
    SUnit* unit = sender;\
    FUNC \
}\
else if(pFocusedUnit){\
SUnit* unit = pFocusedUnit;\
    FUNC \
}\

class SFile;
//拖出
void dragOutG(SUnit* sender = nullptr, QMouseEvent* event = nullptr);
//移动选择的组件
void moveCelect(SUnit* sender = nullptr);
//清楚选择的组件
void cleanCelect(SUnit* sender = nullptr);
//释放选择的组件
void releaseCelect(SUnit* sender = nullptr);
//更新选择的组件
void updateCelect(SUnit* sender = nullptr);

//请求右键菜单
void requireContexMenu(QContextMenuEvent* event, SUnit* sender);

//折叠所有文件夹
void foldG();

//尝试找到处理器
void findProcessor();

//提升移动组件
void raiseUnderMoving(SUnit *sender = nullptr);

//所有组件内部函数的全局版
void switchSimpleModeG(SUnit* sender = nullptr);
void swtichAlwayShowG(SUnit* sender = nullptr);
void switchFullShowG(SUnit* sender = nullptr);

void removeG(SUnit* sender = nullptr);

QPair<SLayout *, QPoint> deepFind(SUnit* aim);

//从Json数据生成Unit并加载数据

//各种生成unit的函数
SUnit* from_json(QJsonObject data, SLayout *layout);
SUnit* from_class(QString Class);
SFile* from_path(QString path, SLayout* layout = nullptr);
SFile *from_info(MyFileInfo& info, SLayout *layout);

//获取所有Unit
QList<SUnit*> units();

#endif // UNITFUNC_H
