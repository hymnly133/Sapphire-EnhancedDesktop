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


QColor winThemeColor();

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

    //将Val类型设置到面板
    void setInLayout(QString field,QString name,QWidget* content,bool checkBox);


    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};

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

#endif // STYLE_H
