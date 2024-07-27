#ifndef STYLESETTINGDIALOG_H
#define STYLESETTINGDIALOG_H


#include "settingaboutpage.h"
#include <QDialog>
#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QMap>
namespace Ui {
class StyleSettingDialog;
}

class StyleSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StyleSettingDialog(QWidget *parent = nullptr);
    ~StyleSettingDialog();

signals:
    // ColorPage
    void onUnfocusedAlphaValueChanged(int);
    void onFocusedAlphaValueChanged(int);
    void onUnfocusedColorRatioValueChanged(int);
    void onFocusedColorRatioValueChanged(int);

    // EffectPage
    void onLightAlphaStartValueChanged(int);
    void onLightAlphaEndValueChanged(int);
    void onIconShadowAlphaValueChanged(int);
    void onIconShadowBlurRadiusValueChanged(int);

    // RenderPage
    void onShowRectStateChanged(int);
    void onShowSideStateChanged(int);
    void onShowLightStateChanged(int);
    void onEnableBackgroundBlurStateChanged(int);
    void onEnableLightTrackStateChanged(int);
    void onEnableIntimeRepaintStateChanged(int);
    void onEnableImageFillStateChanged(int);
    void onEnableHighDefinitionIconStateChanged(int);
    void onEnableTextShadowStateChanged(int);
    void onUsePicAsIconStateChanged(int);

    // PreferencePage
    void onDefaultSteamIconTypeValueChanged(int);
    void onPositionAnimationTimeValueChanged(int);
    void onFocusAnimationTimeValueChanged(int);
    void onLongFocusInDeltaTimeValueChanged(int);
    void onLongFocusOutDeltaTimeValueChanged(int);
    void onLongFocusContainerFixRatioValueChanged(int);
    void onScaleFixRatioValueChanged(int);

    // UnitPage
    void onUnitShadowAlphaValueChanged(int);
    void onUnitShadowBlurRadiusValueChanged(int);
    void onUnitRadiusValueChanged(int);

    // createAboutPage
    // 待定

private:
    // ColorPage
    // EffectPage
    // RenderPage
    // PreferencePage
    // UnitPage
    // createAboutPage
    void setAboutPageVersion(const QString& version);

private slots:
    void on_listWidget_currentRowChanged(int currentRow);



private:
    void setupUi();
    void setupConnections();
    void createSettingPages();

    void createColorPage();
    void createEffectPage();
    void createRenderPage();
    void createPreferencePage();
    void createUnitPage();
    void createAboutPage();
private:
    Ui::StyleSettingDialog *ui;

    QMap<QString, QCheckBox*> boolSettings;
    QMap<QString, QSlider*> intSettings;
    QMap<QString, QSlider*> doubleSettings;

    settingAboutPage* aboutPage;

};

#endif // STYLESETTINGDIALOG_H
