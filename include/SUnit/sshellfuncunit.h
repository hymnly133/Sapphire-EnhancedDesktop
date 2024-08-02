#ifndef SSHELLFUNCUNIT_H
#define SSHELLFUNCUNIT_H

#include "smultifunc.h"
#include <QWidget>

class SShellFuncUnit : public SMultiFunc
{
    Q_OBJECT

public:
    explicit SShellFuncUnit(SLayout* dis = nullptr);
signals:

    // SUnit interface
public:
    QJsonObject to_json() override;
    void load_json(QJsonObject rootObject) override;



    // SMultiFunc interface
public:
    void processorTip() override;



    // SUnit interface

public:
    void double_click_action(QMouseEvent* event) override;

    // SUnit interface
public:
    // void onProcessAnother(SUnit *another) override;


    // SMultiFunc interface
public:
    void onCelectedProcessor(bool val) override;

    // SMultiFunc interface
public:
    bool ProcessPath(QString path) override;

    // SUnit interface
public:
    void onShiftContextMenu(QContextMenuEvent *event) override;

    // SUnit interface
public:
    void setupDesktopMenu() override;
};

Q_DECLARE_METATYPE(SShellFuncUnit)
#endif // SSHELLFUNCUNIT_H
