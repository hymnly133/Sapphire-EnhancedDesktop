#ifndef STYLE_H
#define STYLE_H

#include "qboxlayout.h"
#include "qcheckbox.h"
#include "qdebug.h"
#include "qdialog.h"
#include "qglobal.h"
#include "qsettings.h"
#include "qslider.h"
#include"qstring.h"
#include"QObject"
#include"qmap"
#include "qwindowdefs.h"
#include <windows.h>
#include <winerror.h>
#include "dwmapi.h"
#include "sfieldswidget.h"
#include "stylesettotal.h"
#include"ui_styleSetting.h"




// 未/已聚焦时的色值alpha
extern int unfocused_alpha;
extern int focused_alpha;

// 未/已聚焦时深色图标的色值alpha（未启用
extern int unfocused_alpha_deep;
extern int focused_alpha_deep;

//色值混合比率
extern double unfocused_color_ratio;
extern double focused_color_ratio;

//光效特效的起点/终点alpha
extern int light_alpha_start;
extern int light_alpha_end;

//绘制开关
extern bool ShowRect;
extern bool ShowSide;
extern bool ShowLight;

//图标（SMultiFunc中的图片）阴影参数
extern int icon_shadow_alpha;
extern int icon_shadow_blur_radius;

//组件的阴影参数
extern int unit_shadow_alpha;
extern int unit_shadow_blur_radius;

//圆角
extern int unit_radius;
//放置动画时间
extern int position_animation_time;

//聚焦动画时间
extern int focus_animation_time;

//背景模糊（未启用
extern bool enable_background_blur;

//光效特效追踪
extern bool enable_light_track;

//即时重绘（debug用
extern bool enable_intime_repaint;

//铺满填充
extern bool enable_image_fill;


//默认steam读取图标0小1竖2横
extern int default_steam_icon_type;

//长聚焦进入需时
extern int long_focus_in_delta_time;
//长聚焦失焦需时
extern int long_focus_out_delta_time;

//长聚焦时Container缩放比率（计划作为类成员
extern double long_focus_container_fix_ratio;


//聚焦时scale的修正
extern double scale_fix_ratio;

//启用超清图标
extern bool enable_highdef_icon;

//启用文字阴影
extern bool enable_text_shadow;

//启用读取图片作为Icon
extern bool use_pic_as_icon;;

//启用开机自启
extern bool enable_auto_run;

extern bool resize_to_rect;
extern QString user_font;

extern bool enable_refresh_animation;
extern bool enable_background_transparent;
#endif // STYLE_H
