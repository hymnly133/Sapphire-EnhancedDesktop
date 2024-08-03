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
#include <minwindef.h>
#include <winerror.h>
#include "dwmapi.h"
#include "stylesettotal.h"
#include"ui_styleSetting.h"


//把field作为List储存，name与fullname作为成员变量，有一点浪费，但问题不大
template <class T>
struct Val:public QObject{

public:
    //值指针
    T* pval;
    //值名（代码变量名）
    QString name;
    //全名（fields+name）
    QString fullName;
    //域s（从顶级开始）
    QStringList fields;
    //显示名，向用户显示
    QString displayName;
    T val(){return *pval;};

    //用类似于QProperty的val与set方法，可使得在程序其他位置也可以调用set并触发信号


    Val<T>(const QString& fullname,T* pval,const QString& displayName):pval(pval),fullName(fullname),displayName(displayName){
        QStringList list = fullname.split('/');
        name  = list.last();
        fields = list.mid(0,list.length()-1);
    };


    virtual void read(QSettings *styleIni){
        if(styleIni->contains(fullName)){
            readVal(styleIni);
            qDebug() <<"Read"<< fullName << ":" << val();
        }
        else{
            qDebug() <<"No exist"<< fullName << ",use default" << val();
        }
    }

    //外部调用封装方法
    void write(QSettings *styleIni){
        writeVal(styleIni);
        qDebug() <<"Wrote"<< fullName << ":" << val();
    }
    virtual bool set(T newVal){
        if(newVal == *pval) return false;
        *pval = newVal;
        return true;
    };

private:
    //从ini读值的主方法
    virtual void readVal(QSettings *styleIni) =0;
    //写入ini的主方法
    virtual void writeVal(QSettings *styleIni) =0;

};



template<class T>
struct LimitedVal:public Val<T>{
    T min;
    T max;
    LimitedVal<T>(const QString& fullname,T *pval,const QString& displayName,T min,T max):Val<T>(fullname,pval,displayName){
        this->min =min;
        this->max = max;
    };

    //重载以实行范围检查
    bool set(T newVal){
        return Val<T>::set(qBound(min,newVal,max));
    }
};

struct boolVal:public Val<bool>
{    Q_OBJECT
    using Val<bool>::Val;
    virtual void readVal(QSettings *styleIni){
        set(styleIni->value(fullName).toBool());
    };
    virtual void writeVal(QSettings *styleIni){
        styleIni->setValue(fullName, QString::number(val()));
    }

public:
    QCheckBox* checkbox;

    //set函数以触发相应类型的信号
public slots:
    virtual bool set(bool newVal){
        if(Val::set(newVal)){
            emit valueChanged(newVal);
            return true;
        }
        else{
            return false;
        }
    }

public: signals:
    void valueChanged(bool val);
};

struct intVal:public LimitedVal<int>{
    Q_OBJECT;
    using LimitedVal<int>::LimitedVal;
    virtual void readVal(QSettings *styleIni){
        set(qBound( min, styleIni->value(fullName).toInt(),max));
    };
    virtual void writeVal(QSettings *styleIni){
        styleIni->setValue(fullName, QString::number(val()));
    }


public:
    QSlider* slider;

public slots:
    virtual bool set(int newVal){
        if(LimitedVal::set(newVal)){
            emit valueChanged(newVal);
            return true;
        }
        else{
            return false;
        }
    }

public: signals:
    void valueChanged(int val);

};

struct doubleVal:public LimitedVal<double>{
    Q_OBJECT;
    using LimitedVal<double>::LimitedVal;
    virtual void readVal(QSettings *styleIni){
        set(qBound( min, styleIni->value(fullName).toDouble(),max));
    };
    virtual void writeVal(QSettings *styleIni){
        styleIni->setValue(fullName, QString::number(val()));
    }

public:

    QSlider* slider;
public slots:
    virtual bool set(double newVal){
        if(LimitedVal::set(newVal)){
            emit valueChanged(newVal);
            return true;
        }
        else{
            return false;
        }
    }

public: signals:
    void valueChanged(double val);
};

struct stringVal:public Val<QString>{
    Q_OBJECT;
    using Val<QString>::Val;
    virtual void readVal(QSettings *styleIni){
        set(styleIni->value(fullName).toString());
    };
    virtual void writeVal(QSettings *styleIni){
        styleIni->setValue(fullName, val());
    }
public:
public slots:
    virtual bool set(QString newVal){
        if(Val::set(newVal)){
            emit valueChanged(newVal);
            return true;
        }
        else{
            return false;
        }
    }

public: signals:
    void valueChanged(QString val);
};


QColor winThemeColor();

class StyleHelper{
public:

    //换为QMap<name,pointer>
    QMap<QString,intVal*> intStyles;
    QMap<QString,doubleVal*> doubleStyles;
    QMap<QString,boolVal*> boolStyles;
    QMap<QString,stringVal*> stringStyles;

    StyleHelper();

    //增加DisplayName
    void Add(QString,bool*,QString displayName,bool,bool);
    void Add(QString name, int * pval,QString displayName,int min,int max);
    void Add(QString, double*,QString displayName, double min, double max);
    void Add(QString, QString*,QString displayName, QString min, QString max);

    void readStyleIni();
    void writeStyleIni();


    intVal* intVal(QString name);
    doubleVal* doubleVal(QString name);
    boolVal* boolVal(QString name);
    stringVal* stringVal(QString name);
    QColor themeColor();


};



class StyleSettingWindow:public QDialog{
    Q_OBJECT
public:
    StyleSettingWindow();

    QMap<QString, QHBoxLayout*> layouts;
    QMap<QString, QVBoxLayout*> checklayouts;
    QMap<QString, QVBoxLayout*> sliderlayouts;

    //QHBoxLayout* buttons;
    Ui::Form* ui;
    //将Val类型设置到面板
    void setInLayout(QStringList fields,QString name,QWidget* content,bool checkBox);


    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void on_fontChangeBox_clicked();
    void on_rebootBox_clicked();
    void on_resizeBox_clicked();

    void onListClicked(QListWidgetItem *item);
private:
    // 初始化布局
    void initializeLayouts();

    // 添加布尔值控件
    void processBoolValues();
    // 添加整数值控件
    void processIntValues();
    // 添加浮点数值控件
    void processDoubleValues();

private:
    QMap<QString, QWidget*> m_widgets;
    QVBoxLayout* m_mainLayout;
    styleSetTotal* m_totalWidget;
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

extern bool resize_to_rect;
extern QString user_font;

extern bool enable_refresh_animation;
extern bool enable_background_transparent;
#endif // STYLE_H
