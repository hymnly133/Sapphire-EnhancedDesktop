#ifndef STYLEHELPER_H
#define STYLEHELPER_H

//把field作为List储存，name与fullname作为成员变量，有一点浪费，但问题不大
#include "qdebug.h"
#include "qdialog.h"
#include "qobject.h"
#include "qsettings.h"
#include "qtreewidget.h"
#include "sfieldswidget.h"
#include "stylesettotal.h"
#include "ui_styleSetting.h"

template <class T>
struct Val:public QObject{

public:
    //值指针
    T* pval;
    //值名（代码变量名）
    QString name;
    //全名（fields+name)
    QString fullName;

    //域s（从顶级开始）
    QStringList fields;
    //显示名，向用户显示
    QString displayName;
    //用类似于QProperty的val与set方法，可使得在程序其他位置也可以调用set并触发信号
    T val(){return *pval;};

    //是否占用小空间(布局使用)
    bool small = false;

    virtual bool set(T newVal){
        if(newVal == *pval) return false;
        *pval = newVal;
        return true;
    };

    Val<T>(const QStringList& fields,QString name,T* pval,const QString& displayName):pval(pval),fields(fields),name(name),displayName(displayName){
        fullName = fields.join("/")+name;
    };


    //外部调用封装方法
    virtual void read(QSettings *styleIni){
        if(styleIni->contains(name)){
            readVal(styleIni);
            qDebug() <<"Read"<< name << ":" << val();
        }
        else{
            qDebug() <<"No exist"<< name << ",use default" << val();
        }
    }

    void write(QSettings *styleIni){
        writeVal(styleIni);
        qDebug() <<"Wrote"<< name << ":" << val();
    }

    QWidget* valWidget = nullptr;
    bool operator<(const  Val<T>& another)const{
        if(fields==another.fields){
            return name<another.name;
        }
        else{
            return fields<another.fields;
        }
    }
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
    LimitedVal<T>(const QStringList& fields,QString name,T* pval,const QString& displayName,T min,T max):Val<T>(fields,name,pval,displayName){
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
        set(styleIni->value(name).toBool());
    };
    virtual void writeVal(QSettings *styleIni){
        styleIni->setValue(name, QString::number(val()));
    }


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
        set(qBound( min, styleIni->value(name).toInt(),max));
    };
    virtual void writeVal(QSettings *styleIni){
        styleIni->setValue(name, QString::number(val()));
    }


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
        set(qBound( min, styleIni->value(name).toDouble(),max));
    };
    virtual void writeVal(QSettings *styleIni){
        styleIni->setValue(name, QString::number(val()));
    }

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
        set(styleIni->value(name).toString());
    };
    virtual void writeVal(QSettings *styleIni){
        styleIni->setValue(name, val());
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


class StyleHelper:QObject{
    Q_OBJECT
public:

    //换为QMap<name,pointer>
    QMap<QString,intVal*> intStyles;
    QMap<QString,doubleVal*> doubleStyles;
    QMap<QString,boolVal*> boolStyles;
    QMap<QString,stringVal*> stringStyles;

    StyleHelper();

    //增加DisplayName
    void Add(QStringList,QString,bool*,QString displayName,bool,bool);
    void Add(QStringList,QString, int * pval,QString displayName,int min,int max);
    void Add(QStringList,QString, double*,QString displayName, double min, double max);
    void Add(QStringList,QString, QString*,QString displayName, QString min, QString max);

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


    Ui::Form* ui;
    //将整个content按照fields类型设置到面板
    void setInLayout(QStringList fields, QWidget* content, bool checkBox);
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void on_fontChangeBox_clicked();
    void on_rebootBox_clicked();
    void on_resizeBox_clicked();

    // void onListClicked(QListWidgetItem *item);
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:

    // 初始化各个主空间，Color等
    void iniFieldWidget(const QStringList& fields);

    //《关于》页面的制作
    void IniAboutPage();

    // 添加值参数参数的控件
    void processVals();

    // 添加布尔值控件
    void processBoolValues();
    // 添加整数值控件
    void processIntValues();
    // 添加浮点数值控件
    void processDoubleValues();

    //连接treeView与stackedWidget
    void setTreeView(const QStringList &fields);

    //根据fields添加TreeItem;
    void addAItem(const QStringList &fields);

    //添加内容的主方法
    void addContent(const QStringList &fields,QWidget* widget,bool small = false);

private:
    //field与fieldWidget
    QMap<QString,SFieldsWidget*> field2widget;
    QMap<QString ,int> field2ind;
    QMap<QStringList,SFieldsWidget*> fields2widget;
    QMap<QStringList,QTreeWidgetItem*> fields2treeItem;
    QMap<QTreeWidgetItem*,int> item2ind;
    QMap<QTreeWidgetItem*,QStringList> item2fields;
    QMap<int,SFieldsWidget*> ind2widget;
    //fields与widgetz
    // QMap<QString, QWidget*> m_widgets;
    styleSetTotal* m_totalWidget;
};

#endif // STYLEHELPER_H
