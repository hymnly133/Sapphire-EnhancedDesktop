#ifndef UNITFUNC_H
#define UNITFUNC_H

#include "slayout.h"

//拖出
void dragOut();
//移动选择的组件
void moveCelect();
//清楚选择的组件
void cleanCelect();
//释放选择的组件
void releaseCelect();

QPair<SLayout *, QPoint> deepFind(SUnit* aim);

//从Json数据生成Unit并加载数据
SUnit* from_json(QJsonObject data, SLayout *parent);

//获取所有Unit
QList<SUnit*> units();

#endif // UNITFUNC_H
