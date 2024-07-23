#ifndef SFILE_H
#define SFILE_H

#include "smultifunc.h"
#include <QWidget>
//新增
#include <QVBoxLayout>

class SFile : public SMultiFunc
{
    Q_OBJECT;
public:
    QString filePath;
    bool isDir = false;
    bool useFileIcon = true;


    explicit SFile(SLayout* dis = nullptr, int sizex =1, int sizey=1, QString filePath = "");

    QPropertyAnimation* defaultScaleAnimation;

    SFile(const SFile& other):SFile(other.layout,other.sizeX,other.sizeY){};

    void mouse_enter_action() override;

    virtual void loadFromPath(QString filepath, bool init);
    virtual void loadFromMyFI(MyFileInfo info, bool init);

    void double_click_action(QMouseEvent* event) override;

    QJsonObject to_json() override;
    void load_json(QJsonObject rootObject) override;

    void processorTip() override;
    void onShiftContextMenu(QContextMenuEvent *event) override;
    bool ProcessPath(QString path) override;

    void loadAimIcon(MyFileInfo info);

    // SMultiFunc interface
    void setPix(QString pixPath, bool save) override;
};

Q_DECLARE_METATYPE(SFile)

#endif // SFILE_H
