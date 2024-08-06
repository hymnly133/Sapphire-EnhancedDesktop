#ifndef ICONFUNC_H
#define ICONFUNC_H

#include <QObject>
#include <QtXml>
#include <QDomDocument>
class IconReader
{
public:
    IconReader();
    void readPackage2IconXml(QString xml);
    void addIconsInDir(QString dirPath);
    void readExe2PackageXlsx(QString xlsxpath);

    void scanForDefault();
    void applyForAllSFile();
    void recoverForDefault();
    QMultiMap<QString,QString> Package2Icon;
    QMultiMap<QString,QString> Exe2Package;
    //iconName2iconPath
    QMap<QString,QString> icons;

    QString getCustomIconPath(QString exeName);
};

#endif // ICONFUNC_H
