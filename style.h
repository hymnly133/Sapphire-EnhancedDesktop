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

struct boolVal{
    bool* pval;
    QString fullname;
    bool& val(){return *pval;};
    boolVal(QString fullname,bool *pval,bool ,bool):pval(pval),fullname(fullname){};
    void read(QSettings *styleIni){
        val() = styleIni->value(fullname).toBool();
        qDebug() <<"Read"<< fullname << ":" << val();
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
    intVal(QString fullname,int *pval,int min,int max):pval(pval),fullname(fullname),min(min),max(max){};
    void read(QSettings *styleIni){
        val() = styleIni->value( fullname).toInt();
        qDebug() <<"Read"<< fullname << ":" << val();
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
    doubleVal(QString fullname,double *pval,double min,double max):pval(pval),fullname(fullname),min(min),max(max){};
    void read(QSettings *styleIni){
        val() = styleIni->value( fullname).toDouble();
        qDebug() <<"Read"<< fullname << ":" << val();
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
};extern StyleHelper* psh;



class StyleSettingWindow:public QDialog{
    Q_OBJECT
public:
    StyleSettingWindow();
    QMap<QString,QHBoxLayout*> layouts;
    QMap<QString,QVBoxLayout*> checklayouts;
    QMap<QString,QVBoxLayout*> sliderlayouts;
    QVBoxLayout* mainLayout;
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

extern bool enable_background_transparent;
extern bool enable_background_blur;
extern bool enable_light_track;

extern bool enable_intime_repaint;

extern bool enable_image_fill;

extern int default_steam_icon_type;

extern double scale_fix_ratio;

extern bool enable_lnk_redirect;

#endif // STYLE_H
