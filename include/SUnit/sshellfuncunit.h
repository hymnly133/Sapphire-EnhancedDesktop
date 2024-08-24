#ifndef SSHELLFUNCUNIT_H
#define SSHELLFUNCUNIT_H

#include "smultifunc.h"
#include <QWidget>

class SShellFuncUnit : public SMultiFunc
{
    Q_OBJECT
    enum Type {
        Multi = 0,
        Computer = 1,
        Trash = 2
    };

public:
    Type type = Multi;
    explicit SShellFuncUnit(SLayout* dis = nullptr, Type = Multi);

    QJsonObject to_json() override;
    void load_json(QJsonObject rootObject) override;

    void processorTip() override;

    void double_click_action(QMouseEvent* event) override;

    void setProcessor(bool val) override;

    void processFile(SFileInfo* sfileInfo) override;

    void onShiftContextMenu(QContextMenuEvent *event) override;

    void setupDesktopMenu() override;
    void setupEditMenu() override;

    bool containComputer()
    {
        if(type == Computer || (type == Multi) ) {
            return true;
        }
        return false;
    };
    bool containTrash()
    {
        if(type == Trash || (type == Multi) ) {
            return true;
        }
        return false;
    };

    void updateDisplay();

    void split();
};

Q_DECLARE_METATYPE(SShellFuncUnit)
#endif // SSHELLFUNCUNIT_H
