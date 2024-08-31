#ifndef STYLEHELPER_H
#define STYLEHELPER_H

//把field作为List储存，name与fullname作为成员变量，有一点浪费，但问题不大
#include "qobject.h"
#include"sval.h"

#define LAST_REG_AUTORUN_VERSION QVersionNumber::fromString("1.3.2.3")

class StyleHelper: public QObject
{
    Q_OBJECT
public:

    //换为QMap<name,pointer>
    QMap<QString, intVal*> intStyles;
    QMap<QString, doubleVal*> doubleStyles;
    QMap<QString, boolVal*> boolStyles;
    QMap<QString, stringVal*> stringStyles;
    QMap<QString, colorVal*> colorStyles;

    StyleHelper();
    void connectSysChanges();
    QVersionNumber styleVersion;

    //增加DisplayName
    void Add(QStringList, QString, bool*, QString displayName, bool, bool);
    void Add(QStringList, QString, int * pval, QString displayName, int min, int max);
    void Add(QStringList, QString, double*, QString displayName, double min, double max);
    void Add(QStringList, QString, QString*, QString displayName, QString min, QString max);
    void Add(QStringList, QString, QColor*, QString displayName, QString min, QString max);

    void readStyleIni();
    void writeStyleIni();


    intVal* intVal(QString name);
    doubleVal* doubleVal(QString name);
    boolVal* boolVal(QString name);
    stringVal* stringVal(QString name);
    colorVal* colorVal(QString name);
    QColor themeColor();

public:
signals:
    void colorChanged();
    void fontChanged();
};



#endif // STYLEHELPER_H
