#ifndef STYLE_H
#define STYLE_H

#include "qglobal.h"
#include"qstring.h"
#include"qmap"
class StyleHelper{
    QMap<QString,int*> intStyles;
    QMap<QString,double*> doubleStyles;
    QMap<QString,bool*> boolStyles;
public:
    StyleHelper();
    void Add(QString,bool*);
    void Add(QString,int*);
    void Add(QString, double*);
    void readStyleIni();
    void writeStyleIni();
};extern StyleHelper* psh;


extern int sleep_alpha;
extern int active_alpha;
extern int sleep_alpha_deep;
extern int active_alpha_deep;

extern double sleep_color_ratio;
extern double active_color_ratio;

extern int light_alpha_start;
extern int light_alpha_end;

extern bool ShowRect;
extern bool ShowSide;
extern bool ShowLight;

extern int icon_shadow_alpha;
extern int icon_shadow_blur_radius;

extern int unit_shadow_alpha;
extern int unit_shadow_blur_radius;

extern int unit_radius;
extern int position_animation_time;

extern bool enable_background_transparent;
extern bool enable_background_blur;
extern bool enable_light_track;

extern bool enable_intime_repaint;

extern bool enable_image_fill;

extern int muilt_icon_default_type;

extern double scale_fix_ratio;

extern bool enable_lnk_redirect;

#endif // STYLE_H
