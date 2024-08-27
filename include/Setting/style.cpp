#include "style.h"
#include "SysFunctions.h"
#include "global.h"
#include "mainwindow.h"
#include "qfileinfo.h"
#include "qsettings.h"
#include"QInputDialog"
#include "stylehelper.h"
#include"ui_styleSetting.h"
#include "userfunc.h"
#include "QProcess"
#include <QScrollArea>
#include"QSpinBox"
#include"QDoubleSpinBox"

int unfocused_alpha = 130;
int focused_alpha = 220;

int unfocused_alpha_deep = 140;
int focused_alpha_deep = 200;

double unfocused_color_ratio = 0.35;
double focused_color_ratio = 0.95;

int light_alpha_start = 120;
int light_alpha_end = 0;

bool ShowRect = true;
bool ShowSide = false;
bool ShowLight = true;

int icon_shadow_alpha = 180;
int icon_shadow_blur_radius = 40;

int unit_shadow_alpha = 100;
int unit_shadow_blur_radius = 8;

int unit_radius = 15;
int position_animation_time = 200;
int focus_animation_time = 100;

bool enable_background_blur = false;
bool enable_light_track = false;

bool enable_intime_repaint = false;

bool enable_image_fill = false;
bool enable_highdef_icon = 1;
bool dark_mode = 0;

int default_steam_icon_type = 1;

int long_focus_in_delta_time = 500;
int long_focus_out_delta_time = 1000;


int long_focus_animation_time = 500;
double long_focus_container_fix_ratio = 1.71;

double scale_fix_ratio = 1.3;

bool enable_tooltip_right_animation = 0;

bool enable_text_shadow = 0;
bool use_pic_as_icon = 1;

bool enable_auto_run = 0;
bool enable_resize_to_rect = 0;

bool enable_refresh_animation = 0;
bool enable_background_transparent = 1;

bool always_fill_screen = 0;
bool enable_dir_func = 0;
QString user_font = "Microsoft YaHei";


QColor background_color = QColor(QString("ffe0dcf0").toUInt(NULL, 16));
QColor theme_color = QColor(QString("ffaec3f5").toUInt(NULL, 16));
QColor highlight_color = QColor(QString("ff7e84ff").toUInt(NULL, 16));
QColor text_color = QColor(QString("ff26242f").toUInt(NULL, 16));
bool use_syscolor_as_themecolor = false;
bool always_simple_mode = false;
int font_size = 9;
bool enable_notice = true;
bool enable_tooltip = true;
bool try_run_as_user = true;
bool enable_dir_preview = true;
bool enable_low_memory_mode = false;
QColor themeColor()
{
    if(use_syscolor_as_themecolor) {
        return winThemeColor();
    } else {
        return theme_color;
    }
}


