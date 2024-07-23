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
#include"Windows.h"
#include <minwindef.h>
#include <winerror.h>
#include "dwmapi.h"

struct boolVal{
    bool* pval;
    QString fullname;
    bool& val(){return *pval;};
    boolVal(QString fullname,bool *pval,bool ,bool):pval(pval),fullname(fullname){};
    void read(QSettings *styleIni){
        if(styleIni->contains(fullname)){
            val() = styleIni->value(fullname).toBool();
            qDebug() <<"Read"<< fullname << ":" << val();
        }
        else{
            qDebug() <<"No exist"<< fullname << ",use default" << val();
        }

    }
    void write(QSettings *styleIni){
        styleIni->setValue(fullname, QString::number(val()));
        qDebug() <<"Wrote"<< fullname << ":" << val();
    }
    QString field(){
        return fullname.split("/")[0];
    }
    QString name(){
        return fullname.split("/")[1];
    }
    QCheckBox* checkBox;
};

struct intVal{
    int* pval;
    QString fullname;
    int min;
    int max;
    int& val(){return *pval;};
    intVal(QString fullname,int *pval,int min,int max):pval(pval),fullname(fullname){
        this->min = min;
        this->max = max;
          };
    void read(QSettings *styleIni){
        if(styleIni->contains(fullname)){
            val() = styleIni->value(fullname).toInt();
            qDebug() <<"Read"<< fullname << ":" << val();
        }
        else{
            qDebug() <<"No exist"<< fullname << ",use default" << val();
        }
    }
    void write(QSettings *styleIni){
        styleIni->setValue(fullname, QString::number(val()));
        qDebug() <<"Wrote"<< fullname << ":" << val();
    }
    QString field(){
        return fullname.split("/")[0];
    }
    QString name(){
        return fullname.split("/")[1];
    }
    QSlider* slider;

};

struct doubleVal{
    double* pval;
    QString fullname;
    double min;
    double max;
    double& val(){return *pval;};
    doubleVal(QString fullname,double *pval,double min,double max):pval(pval),fullname(fullname){
        this->min = min;
        this->max = max;
          };
    void read(QSettings *styleIni){
        if(styleIni->contains(fullname)){
            val() = styleIni->value(fullname).toDouble();
            qDebug() <<"Read"<< fullname << ":" << val();
        }
        else{
            qDebug() <<"No exist"<< fullname << ",use default" << val();
        }
    }
    void write(QSettings *styleIni){
        styleIni->setValue(fullname, QString::number(val()));
        qDebug() <<"Wrote"<< fullname << ":" << val();
    }
    QString field(){
        return fullname.split("/")[0];
    }
    QString name(){
        return fullname.split("/")[1];
    }
    QSlider* slider;
};


QColor GetWindowsThemeColor();

class StyleHelper{
public:

    QVector<intVal*> intStyles;
    QVector<doubleVal*> doubleStyles;
    QVector<boolVal*> boolStyles;

    StyleHelper();

    void Add(QString,bool*,bool,bool);
    void Add(QString name, int * pval,int min,int max);
    void Add(QString, double*, double min, double max);
    void readStyleIni();
    void writeStyleIni();
    void showSetting();
    QColor themeColor();


};extern StyleHelper* psh;



class StyleSettingWindow:public QDialog{
    Q_OBJECT
public:
    StyleSettingWindow();
    QMap<QString,QHBoxLayout*> layouts;
    QMap<QString,QVBoxLayout*> checklayouts;
    QMap<QString,QVBoxLayout*> sliderlayouts;
    QVBoxLayout* mainLayout;
    void InitWindow();
    void setInLayout(QString field,QString name,QWidget* content,bool checkBox);
};


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
extern int focus_animation_time;

extern bool enable_background_blur;
extern bool enable_light_track;

extern bool enable_intime_repaint;

extern bool enable_image_fill;



extern int default_steam_icon_type;

extern int long_focus_in_delta_time;
extern int long_focus_out_delta_time;
extern double long_focus_container_fix_ratio;

extern double scale_fix_ratio;

extern bool enable_highdef_icon;

extern bool enable_tooltip_right_animation;

extern bool enable_text_shadow;
extern bool use_pic_as_icon;;
#endif // STYLE_H
