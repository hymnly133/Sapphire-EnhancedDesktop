#ifndef SBLOCKLAYOUTSETTINGWINDOW_H
#define SBLOCKLAYOUTSETTINGWINDOW_H

#include "sblocklayout.h"
#include <QDialog>
#include <QObject>
#include <QWidget>
#include<ui_settingDialogForBlockLayout.h>

class SBlockLayoutSettingWindow : public QDialog
{
    Q_OBJECT
public:
    SBlockLayoutSettingWindow(SBlockLayout* aimlayout);
    SBlockLayout* aimlayout;
    Ui::BlockLayoutSettingDialog* ui;
private slots:
    void on_pushButton_apply_clicked();
    void on_pushButton_cancel_clicked();
    void on_rowSlider_valueChanged(int value);
    void on_boradXRSlider_valueChanged(int value);
    void on_spaceXRSlider_valueChanged(int value);
    void on_colSlider_valueChanged(int value);
    void on_boradYRSlider_valueChanged(int value);
    void on_spaceYRSlider_valueChanged(int value);
};

#endif // SBLOCKLAYOUTSETTINGWINDOW_H
