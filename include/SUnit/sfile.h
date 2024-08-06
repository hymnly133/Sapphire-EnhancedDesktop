#ifndef SFILE_H
#define SFILE_H

#include "filefunc.h"
#include "qfileinfo.h"
#include "smultifunc.h"
#include <QWidget>
//新增
#include <QVBoxLayout>

class SFile : public SMultiFunc
{
    Q_OBJECT;
public:
    QString filePath;
    QString fullName();
    QString suffix();
    QString baseName();
    bool isDir = false;


    explicit SFile(SLayout* dis = nullptr, int sizex =1, int sizey=1, QString filePath = "");

    QPropertyAnimation* defaultScaleAnimation;

    SFile(const SFile& other);;

    void mouse_enter_action() override;

    virtual void loadFromPath(QString filepath, bool init);
    virtual void loadFromMyFI(MyFileInfo info, bool init);

    void double_click_action(QMouseEvent* event) override;

    void open(bool Admin = false);

    QJsonObject to_json() override;
    void load_json(QJsonObject rootObject) override;

    void processorTip() override;
    void onShiftContextMenu(QContextMenuEvent *event) override;
    bool ProcessPath(QString path) override;

    void loadAimIcon(MyFileInfo info);

    // SMultiFunc interface
    void setPix(QString pixPath, bool save) override;
    void renameFile(QString newNameWithSuffix);
    void renameWithDialog();

    void setName(QString name) override;

    void recoverForDefault();




    // SUnit interface
public:
    void remove() override;

    // SUnit interface
public:
    void setupDesktopMenu() override;
};

inline QString SFile::fullName(){
    return QFileInfo(filePath).fileName();
}

inline QString SFile::suffix(){
    return QFileInfo(filePath).suffix();
}

inline QString SFile::baseName(){
    return QFileInfo(filePath).baseName();
}

inline SFile::SFile(const SFile &other):SFile(other.layout,other.sizeX,other.sizeY){}

Q_DECLARE_METATYPE(SFile)

#endif // SFILE_H
