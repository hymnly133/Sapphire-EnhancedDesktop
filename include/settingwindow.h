#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include "qdialog.h"
#include <QWidget>
class T_Home;
class T_Icon;
class T_ElaScreen;
class T_BaseComponents;
class T_TabWidget;
class T_Popup;
class SettingWindow : public QDialog
{
    Q_OBJECT

    Q_SLOT void onCloseButtonClicked();
public:
    explicit SettingWindow(QDialog *parent = nullptr);
    void initWindow();
    void initEdgeLayout();
    void initContent();

private:
    T_Home* _homePage{nullptr};
    T_ElaScreen* _elaScreenPage{nullptr};
    T_Icon* _iconPage{nullptr};
    T_BaseComponents* _baseComponentsPage{nullptr};
    T_TabWidget* _tabWidgetPage{nullptr};
    T_Popup* _popupPage{nullptr};
    QString _elaDxgiKey{""};
    QString _aboutKey{""};
    QString _settingKey{""};
};

#endif // SETTINGWINDOW_H
