#ifndef SFILE_H
#define SFILE_H

#include "filefunc.h"
#include "qfileinfo.h"
#include "smultifunc.h"
#include <QWidget>
//新增
#include <QVBoxLayout>
#include"sfileinfo.h"

class SFile : public SMultiFunc, public SFileInfo
{
    Q_OBJECT;
public:

    explicit SFile(SLayout* dis = nullptr, int sizex = 1, int sizey = 1, QString filePath = "");

    SFile(const SFile& other);;

    void mouse_enter_action() override;


    void loadFromPath(QString filepath, bool init);
    virtual void loadFromMyFI(MyFileInfo &info, bool init);

    void double_click_action(QMouseEvent* event) override;



    QJsonObject to_json() override;
    void load_json(QJsonObject rootObject) override;

    void processorTip() override;
    void onShiftContextMenu(QContextMenuEvent *event) override;
    void processFile(SFileInfo *sfileInfo) override;

    void loadAimIcon( MyFileInfo &info);


    //呼出一个对话框来进行重命名
    void renameWithDialog();

    //设置显示的name名
    void setName(QString name) override;

    //重置
    void recoverForDefault();

    //作为SUnit的rename，调用文件重命名并更新unit显示效果
    bool rename(QString newNameWithSuffix);



    // SUnit interface
public:
    void remove() override;

    // SUnit interface
public:
    void setupDesktopMenu() override;

    // SUnit interface
public:
    void single_click_action(QMouseEvent *event) override;
};

inline SFile::SFile(const SFile &other): SFile(other.layout, other.sizeX, other.sizeY) {}

Q_DECLARE_METATYPE(SFile)

#endif // SFILE_H
