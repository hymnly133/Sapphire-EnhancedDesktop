#include "stylesettingdialog.h"
#include "ui_stylesettingdialog.h"
#include "style.h"

StyleSettingDialog::StyleSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StyleSettingDialog)
{
    ui->setupUi(this);
    setupUi();
    setupConnections();
}

void StyleSettingDialog::setupConnections()
{
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &StyleSettingDialog::on_listWidget_currentRowChanged);

    // ColorPage
    // EffectPage
    // RenderPage
    // PreferencePage
    // UnitPage
    // createAboutPage
    connect(this, &StyleSettingDialog::setAboutPageVersion,aboutPage, &settingAboutPage::setVersion);

}

void StyleSettingDialog::on_listWidget_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}

StyleSettingDialog::~StyleSettingDialog()
{
    delete ui;
}

void StyleSettingDialog::setupUi()
{
    createSettingPages();
    ui->listWidget->setCurrentRow(0);

}

void StyleSettingDialog::createSettingPages()
{
    ui->stackedWidget->setAttribute(Qt::WA_TranslucentBackground); // 设置背景为透明
    ui->stackedWidget->setWindowFlags(ui->stackedWidget->windowFlags() | Qt::WindowDoesNotAcceptFocus); // 允许透明度

    createColorPage();
    createEffectPage();
    createRenderPage();
    createPreferencePage();
    createUnitPage();
    createAboutPage();
}

void StyleSettingDialog::createColorPage()
{
    QWidget *colorPage = new QWidget();
    colorPage->setWindowOpacity(0.62);

    QVBoxLayout *colorLayout = new QVBoxLayout(colorPage);

    intSettings["unfocused_alpha"] = new QSlider(Qt::Horizontal);
    intSettings["unfocused_alpha"]->setRange(0, 255);
    intSettings["unfocused_alpha"]->setValue(unfocused_alpha);
    colorLayout->addWidget(new QLabel("Unfocused Alpha"));
    colorLayout->addWidget(intSettings["unfocused_alpha"]);

    intSettings["focused_alpha"] = new QSlider(Qt::Horizontal);
    intSettings["focused_alpha"]->setRange(0, 255);
    intSettings["focused_alpha"]->setValue(focused_alpha);
    colorLayout->addWidget(new QLabel("Focused Alpha"));
    colorLayout->addWidget(intSettings["focused_alpha"]);

    doubleSettings["unfocused_color_ratio"] = new QSlider(Qt::Horizontal);
    doubleSettings["unfocused_color_ratio"]->setRange(0, 100);
    doubleSettings["unfocused_color_ratio"]->setValue(unfocused_color_ratio * 100);
    colorLayout->addWidget(new QLabel("Unfocused Color Ratio"));
    colorLayout->addWidget(doubleSettings["unfocused_color_ratio"]);

    doubleSettings["focused_color_ratio"] = new QSlider(Qt::Horizontal);
    doubleSettings["focused_color_ratio"]->setRange(0, 100);
    doubleSettings["focused_color_ratio"]->setValue(focused_color_ratio * 100);
    colorLayout->addWidget(new QLabel("Focused Color Ratio"));
    colorLayout->addWidget(doubleSettings["focused_color_ratio"]);

    connect(intSettings["unfocused_alpha"], &QSlider::valueChanged, this, &StyleSettingDialog::onUnfocusedAlphaValueChanged);
    connect(intSettings["focused_alpha"], &QSlider::valueChanged, this, &StyleSettingDialog::onFocusedAlphaValueChanged);
    connect(doubleSettings["unfocused_color_ratio"], &QSlider::valueChanged, this, &StyleSettingDialog::onUnfocusedColorRatioValueChanged);
    connect(doubleSettings["focused_color_ratio"], &QSlider::valueChanged, this, &StyleSettingDialog::onFocusedColorRatioValueChanged);


    ui->stackedWidget->addWidget(colorPage);
}

void StyleSettingDialog::createEffectPage()
{
    QWidget *effectPage = new QWidget();
    effectPage->setWindowOpacity(0.96);
    QVBoxLayout *effectLayout = new QVBoxLayout(effectPage);

    intSettings["light_alpha_start"] = new QSlider(Qt::Horizontal);
    intSettings["light_alpha_start"]->setRange(0, 255);
    intSettings["light_alpha_start"]->setValue(light_alpha_start);
    effectLayout->addWidget(new QLabel("Light Alpha Start"));
    effectLayout->addWidget(intSettings["light_alpha_start"]);

    intSettings["light_alpha_end"] = new QSlider(Qt::Horizontal);
    intSettings["light_alpha_end"]->setRange(0, 255);
    intSettings["light_alpha_end"]->setValue(light_alpha_end);
    effectLayout->addWidget(new QLabel("Light Alpha End"));
    effectLayout->addWidget(intSettings["light_alpha_end"]);

    intSettings["icon_shadow_alpha"] = new QSlider(Qt::Horizontal);
    intSettings["icon_shadow_alpha"]->setRange(0, 255);
    intSettings["icon_shadow_alpha"]->setValue(icon_shadow_alpha);
    effectLayout->addWidget(new QLabel("Icon Shadow Alpha"));
    effectLayout->addWidget(intSettings["icon_shadow_alpha"]);

    intSettings["icon_shadow_blur_radius"] = new QSlider(Qt::Horizontal);
    intSettings["icon_shadow_blur_radius"]->setRange(0, 100);
    intSettings["icon_shadow_blur_radius"]->setValue(icon_shadow_blur_radius);
    effectLayout->addWidget(new QLabel("Icon Shadow Blur Radius"));
    effectLayout->addWidget(intSettings["icon_shadow_blur_radius"]);

    ui->stackedWidget->addWidget(effectPage);

    connect(intSettings["light_alpha_start"], &QSlider::valueChanged, this, &StyleSettingDialog::onLightAlphaStartValueChanged);
    connect(intSettings["light_alpha_end"], &QSlider::valueChanged, this, &StyleSettingDialog::onLightAlphaEndValueChanged);
    connect(intSettings["icon_shadow_alpha"], &QSlider::valueChanged, this, &StyleSettingDialog::onIconShadowAlphaValueChanged);
    connect(intSettings["icon_shadow_blur_radius"], &QSlider::valueChanged, this, &StyleSettingDialog::onIconShadowBlurRadiusValueChanged);

}

void StyleSettingDialog::createRenderPage()
{
    QWidget *renderPage = new QWidget();
    renderPage->setWindowOpacity(0.76);
    QVBoxLayout *renderLayout = new QVBoxLayout(renderPage);

    boolSettings["ShowRect"] = new QCheckBox("Show Rect");
    boolSettings["ShowRect"]->setChecked(ShowRect);
    renderLayout->addWidget(boolSettings["ShowRect"]);

    boolSettings["ShowSide"] = new QCheckBox("Show Side");
    boolSettings["ShowSide"]->setChecked(ShowSide);
    renderLayout->addWidget(boolSettings["ShowSide"]);

    boolSettings["ShowLight"] = new QCheckBox("Show Light");
    boolSettings["ShowLight"]->setChecked(ShowLight);
    renderLayout->addWidget(boolSettings["ShowLight"]);

    boolSettings["enable_background_blur"] = new QCheckBox("Enable Background Blur");
    boolSettings["enable_background_blur"]->setChecked(enable_background_blur);
    renderLayout->addWidget(boolSettings["enable_background_blur"]);

    boolSettings["enable_light_track"] = new QCheckBox("Enable Light Track");
    boolSettings["enable_light_track"]->setChecked(enable_light_track);
    renderLayout->addWidget(boolSettings["enable_light_track"]);

    boolSettings["enable_intime_repaint"] = new QCheckBox("Enable Intime Repaint");
    boolSettings["enable_intime_repaint"]->setChecked(enable_intime_repaint);
    renderLayout->addWidget(boolSettings["enable_intime_repaint"]);

    boolSettings["enable_image_fill"] = new QCheckBox("Enable Image Fill");
    boolSettings["enable_image_fill"]->setChecked(enable_image_fill);
    renderLayout->addWidget(boolSettings["enable_image_fill"]);

    boolSettings["enable_highdef_icon"] = new QCheckBox("Enable High Definition Icon");
    boolSettings["enable_highdef_icon"]->setChecked(enable_highdef_icon);
    renderLayout->addWidget(boolSettings["enable_highdef_icon"]);

    boolSettings["enable_text_shadow"] = new QCheckBox("Enable Text Shadow");
    boolSettings["enable_text_shadow"]->setChecked(enable_text_shadow);
    renderLayout->addWidget(boolSettings["enable_text_shadow"]);

    boolSettings["use_pic_as_icon"] = new QCheckBox("Use Picture As Icon");
    boolSettings["use_pic_as_icon"]->setChecked(use_pic_as_icon);
    renderLayout->addWidget(boolSettings["use_pic_as_icon"]);

    ui->stackedWidget->addWidget(renderPage);

    connect(boolSettings["ShowRect"], &QCheckBox::stateChanged, this, &StyleSettingDialog::onShowRectStateChanged);
    connect(boolSettings["ShowSide"], &QCheckBox::stateChanged, this, &StyleSettingDialog::onShowSideStateChanged);
    connect(boolSettings["ShowLight"], &QCheckBox::stateChanged, this, &StyleSettingDialog::onShowLightStateChanged);
    connect(boolSettings["enable_background_blur"], &QCheckBox::stateChanged, this, &StyleSettingDialog::onEnableBackgroundBlurStateChanged);
    connect(boolSettings["enable_light_track"], &QCheckBox::stateChanged, this, &StyleSettingDialog::onEnableLightTrackStateChanged);
    connect(boolSettings["enable_intime_repaint"], &QCheckBox::stateChanged, this, &StyleSettingDialog::onEnableIntimeRepaintStateChanged);
    connect(boolSettings["enable_image_fill"], &QCheckBox::stateChanged, this, &StyleSettingDialog::onEnableImageFillStateChanged);
    connect(boolSettings["enable_highdef_icon"], &QCheckBox::stateChanged, this, &StyleSettingDialog::onEnableHighDefinitionIconStateChanged);
    connect(boolSettings["enable_text_shadow"], &QCheckBox::stateChanged, this, &StyleSettingDialog::onEnableTextShadowStateChanged);
    connect(boolSettings["use_pic_as_icon"], &QCheckBox::stateChanged, this, &StyleSettingDialog::onUsePicAsIconStateChanged);
}

void StyleSettingDialog::createPreferencePage()
{
    QWidget *preferencePage = new QWidget();
    preferencePage->setWindowOpacity(0.66);
    QVBoxLayout *preferenceLayout = new QVBoxLayout(preferencePage);

    intSettings["default_steam_icon_type"] = new QSlider(Qt::Horizontal);
    intSettings["default_steam_icon_type"]->setRange(0, 2);
    intSettings["default_steam_icon_type"]->setValue(default_steam_icon_type);
    preferenceLayout->addWidget(new QLabel("Default Steam Icon Type"));
    preferenceLayout->addWidget(intSettings["default_steam_icon_type"]);

    intSettings["position_animation_time"] = new QSlider(Qt::Horizontal);
    intSettings["position_animation_time"]->setRange(0, 10000);
    intSettings["position_animation_time"]->setValue(position_animation_time);
    preferenceLayout->addWidget(new QLabel("Position Animation Time"));
    preferenceLayout->addWidget(intSettings["position_animation_time"]);

    intSettings["focus_animation_time"] = new QSlider(Qt::Horizontal);
    intSettings["focus_animation_time"]->setRange(0, 10000);
    intSettings["focus_animation_time"]->setValue(focus_animation_time);
    preferenceLayout->addWidget(new QLabel("Focus Animation Time"));
    preferenceLayout->addWidget(intSettings["focus_animation_time"]);

    intSettings["long_focus_in_delta_time"] = new QSlider(Qt::Horizontal);
    intSettings["long_focus_in_delta_time"]->setRange(0, 10000);
    intSettings["long_focus_in_delta_time"]->setValue(long_focus_in_delta_time);
    preferenceLayout->addWidget(new QLabel("Long Focus In Delta Time"));
    preferenceLayout->addWidget(intSettings["long_focus_in_delta_time"]);

    intSettings["long_focus_out_delta_time"] = new QSlider(Qt::Horizontal);
    intSettings["long_focus_out_delta_time"]->setRange(0, 10000);
    intSettings["long_focus_out_delta_time"]->setValue(long_focus_out_delta_time);
    preferenceLayout->addWidget(new QLabel("Long Focus Out Delta Time"));
    preferenceLayout->addWidget(intSettings["long_focus_out_delta_time"]);

    doubleSettings["long_focus_container_fix_ratio"] = new QSlider(Qt::Horizontal);
    doubleSettings["long_focus_container_fix_ratio"]->setRange(0, 100);
    doubleSettings["long_focus_container_fix_ratio"]->setValue(long_focus_container_fix_ratio * 100);
    preferenceLayout->addWidget(new QLabel("Long Focus Container Fix Ratio"));
    preferenceLayout->addWidget(doubleSettings["long_focus_container_fix_ratio"]);

    doubleSettings["scale_fix_ratio"] = new QSlider(Qt::Horizontal);
    doubleSettings["scale_fix_ratio"]->setRange(0, 100);
    doubleSettings["scale_fix_ratio"]->setValue(scale_fix_ratio * 100);
    preferenceLayout->addWidget(new QLabel("Scale Fix Ratio"));
    preferenceLayout->addWidget(doubleSettings["scale_fix_ratio"]);

    ui->stackedWidget->addWidget(preferencePage);

    connect(intSettings["default_steam_icon_type"], &QSlider::valueChanged, this, &StyleSettingDialog::onDefaultSteamIconTypeValueChanged);
    connect(intSettings["position_animation_time"], &QSlider::valueChanged, this, &StyleSettingDialog::onPositionAnimationTimeValueChanged);
    connect(intSettings["focus_animation_time"], &QSlider::valueChanged, this, &StyleSettingDialog::onFocusAnimationTimeValueChanged);
    connect(intSettings["long_focus_in_delta_time"], &QSlider::valueChanged, this, &StyleSettingDialog::onLongFocusInDeltaTimeValueChanged);
    connect(intSettings["long_focus_out_delta_time"], &QSlider::valueChanged, this, &StyleSettingDialog::onLongFocusOutDeltaTimeValueChanged);
    connect(doubleSettings["long_focus_container_fix_ratio"], &QSlider::valueChanged, this, &StyleSettingDialog::onLongFocusContainerFixRatioValueChanged);
    connect(doubleSettings["scale_fix_ratio"], &QSlider::valueChanged, this, &StyleSettingDialog::onScaleFixRatioValueChanged);

}

void StyleSettingDialog::createUnitPage()
{
    QWidget *unitPage = new QWidget();
    QVBoxLayout *unitLayout = new QVBoxLayout(unitPage);

    intSettings["unit_shadow_alpha"] = new QSlider(Qt::Horizontal);
    intSettings["unit_shadow_alpha"]->setRange(0, 255);
    intSettings["unit_shadow_alpha"]->setValue(unit_shadow_alpha);
    unitLayout->addWidget(new QLabel("Unit Shadow Alpha"));
    unitLayout->addWidget(intSettings["unit_shadow_alpha"]);

    intSettings["unit_shadow_blur_radius"] = new QSlider(Qt::Horizontal);
    intSettings["unit_shadow_blur_radius"]->setRange(0, 100);
    intSettings["unit_shadow_blur_radius"]->setValue(unit_shadow_blur_radius);
    unitLayout->addWidget(new QLabel("Unit Shadow Blur Radius"));
    unitLayout->addWidget(intSettings["unit_shadow_blur_radius"]);

    intSettings["unit_radius"] = new QSlider(Qt::Horizontal);
    intSettings["unit_radius"]->setRange(0, 100);
    intSettings["unit_radius"]->setValue(unit_radius);
    unitLayout->addWidget(new QLabel("Unit Radius"));
    unitLayout->addWidget(intSettings["unit_radius"]);

    ui->stackedWidget->addWidget(unitPage);

    connect(intSettings["unit_shadow_alpha"], &QSlider::valueChanged, this, &StyleSettingDialog::onUnitShadowAlphaValueChanged);
    connect(intSettings["unit_shadow_blur_radius"], &QSlider::valueChanged, this, &StyleSettingDialog::onUnitShadowBlurRadiusValueChanged);
    connect(intSettings["unit_radius"], &QSlider::valueChanged, this, &StyleSettingDialog::onUnitRadiusValueChanged);
}

void StyleSettingDialog::createAboutPage()
{
    aboutPage = new settingAboutPage(this);
    aboutPage->setVersion("1.0.0");
    ui->stackedWidget->addWidget(aboutPage);
}
