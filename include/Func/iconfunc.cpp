#include "iconfunc.h"
#include"QMessageBox"
#include "global.h"
#include "sfile.h"
#include "xlsxdocument.h"
#include "xlsxworkbook.h"
QXLSX_USE_NAMESPACE
IconReader::IconReader()
{
}

void IconReader::readPackage2IconXml(QString xml)
{
    QFile file(xml);
    if (!file.open(QFileDevice::ReadOnly)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }
    qDebug() << "read Xml" << xml;
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
        file.close();
        return;
    }
    // 获得根节点
    QDomElement root = doc.documentElement();
    qDebug() << "根节点：" << root.nodeName();
    // 获取所有Book1节点
    QDomNodeList list = root.elementsByTagName("item");
    /* 获取属性中的值 */
    for (int i = 0; i < list.count(); i++) {
        // 获取链表中的值
        QDomElement element = list.at(i).toElement();
        // 找到需要读取的节点
        if (element.hasAttribute("component")) {
            // 通过attribute方法返回属性中的值
            QString component = element.attribute("component");
            QString drawable = element.attribute("drawable");
            Package2Icon.insert(component.mid(component.indexOf('{') + 1, component.indexOf('/') - component.indexOf('{') - 1).toLower(), (drawable + ".png").toLower());
        }
    }
}

void IconReader::addIconsInDir(QString dirPath)
{
    QDir dir(dirPath);
    foreach(QFileInfo info, dir.entryInfoList(QDir::Files)) {
        icons.insert(info.fileName().toLower(), info.filePath());
    };
    foreach(QFileInfo childDir, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        addIconsInDir(childDir.filePath());
    };
}

void IconReader::readExe2PackageXlsx(QString xlsxpath)
{
    // QString path ="D:/Github/Sapphire-EnhancedDesktop/xlsx/list.xlsx";
    QFile file(xlsxpath);
    if(!file.exists()) {
        return;
    }
    qDebug() << "read Xlsx" << xlsxpath;
    QXlsx::Document xlsx(xlsxpath);
    QXlsx::Workbook *workBook = xlsx.workbook();
    QXlsx::Worksheet *workSheet = static_cast<QXlsx::Worksheet*>(workBook->sheet(0));
    unsigned int xlsxrows = workSheet->dimension().rowCount(); //读取文件行数
    for (unsigned int i = 2; i <= xlsxrows; i++) {
        if(workSheet->cellAt(i, 2) && workSheet->cellAt(i, 3))
            foreach (QString thisPackageName, workSheet->cellAt(i, 2)->value().toString().split(',')) {
                thisPackageName = thisPackageName.simplified().toLower();
                foreach (QString thisExeName, workSheet->cellAt(i, 3)->value().toString().split(',')) {
                    thisExeName = thisExeName.simplified().toLower();
                    Exe2Package.insert(thisExeName, thisPackageName);
                }
            }
    }
    xlsx.deleteLater();
}

void IconReader::scanForDefault()
{
    QDir xmlDir(xmlDirPath);
    if(!xmlDir.exists()) {
        return;
    }
    foreach(QFileInfo info, xmlDir.entryInfoList(QDir::Files)) {
        if(info.suffix() == "xml") {
            readPackage2IconXml(info.filePath());
        }
    };
    QDir xlsxDir(xlsxDirPath);
    if(!xlsxDir.exists()) {
        return;
    }
    foreach(QFileInfo info, xlsxDir.entryInfoList(QDir::Files)) {
        if(info.suffix() == "xlsx") {
            readExe2PackageXlsx(info.filePath());
        }
    };
    if(!QFile::exists(iconDirPath)) {
        return;
    }
    addIconsInDir(iconDirPath);
}

void IconReader::applyForAllSFile()
{
    foreach (QString filePath, nowExitFiles.keys()) {
        QString target = filePath;
        if(QFileInfo(filePath).isSymLink()) {
            target = QFileInfo(filePath).symLinkTarget();
        }
        QString res = getCustomIconPath(QFileInfo(target).fileName().toLower());
        if(res == "") {
            continue;
        }
        static_cast<SFile * >(nowExitFiles[filePath])->setPix(res, true);
    }
}

void IconReader::recoverForDefault()
{
    foreach (SUnit* unit, nowExitFiles.values()) {
        if(unit->inherits("SFile")) {
            SFile* sfile = static_cast<SFile * >(unit);
            if(sfile->pixPath != "") {
                sfile->recoverForDefault();
            }
        }
    }
}

QString IconReader::getCustomIconPath(QString exeName)
{
    QString null;
    if(!Exe2Package.contains(exeName)) {
        return null;
    }
    QString package = Exe2Package.values(exeName).first();
    if(!Package2Icon.contains(package)) {
        return null;
    }
    QString icon = Package2Icon.values(package).first();
    if(!icons.contains(icon)) {
        return null;
    }
    return icons[icon];
}
