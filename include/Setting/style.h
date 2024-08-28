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
//长聚焦动画进行时长
extern int long_focus_animation_time;

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

//启用变为1X1
extern bool resize_to_rect;
//用户字体
extern QString user_font;

//刷新动画
extern bool enable_refresh_animation;

// 启用背景透视
extern bool enable_background_transparent;

//颜色系统
extern QColor theme_color;
extern QColor background_color;
extern QColor highlight_color;
extern QColor text_color;
extern bool use_syscolor_as_themecolor;

//默认精简
extern bool always_simple_mode;

//字体大小
extern int font_size;
extern bool enable_notice;
extern bool enable_tooltip;

extern bool always_fill_screen;
extern bool try_run_as_user;
extern bool enable_dir_func;
extern bool enable_dir_preview;
extern bool enable_low_memory_mode;
extern bool show_important_notice;

// 供外部调用的方法
QColor themeColor();
#endif // STYLE_H
