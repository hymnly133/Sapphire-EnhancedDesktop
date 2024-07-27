#include "seditbox.h"
#include "SysFunctions.h"
#include "mainwindow.h"
#include "qmessagebox.h"
#include"qcheckbox.h"
#include"QSlider"
#include "screenfunc.h"




SEditBox::SEditBox(SLayout *dis, int sizex, int sizey)
    : SUnit(dis,sizex,sizey)
{
    type = Unit;
    settings= new QHBoxLayout(this);

    overall = new QVBoxLayout();
    settings->addLayout(overall);

    QCheckBox *checkBox1 = new QCheckBox("背景透视",this);             //创建QCheckBox对象
    checkBox1->setChecked(true);
    overall->addWidget(checkBox1);
    connect(checkBox1, &QCheckBox::clicked, this, [this](bool checked) {
        pmw->setTransparent(checked);
        pmw->update();
    });

#ifdef QT_DEBUG
        QCheckBox *checkBox2 = new QCheckBox("背景模糊",this);
        checkBox2->setChecked(enable_background_blur);
        overall->addWidget(checkBox2);
        connect(checkBox2, &QCheckBox::clicked, this, [this](bool checked) {
            pmw->setBlur(checked);
            pmw->update();
        });
#endif

    QCheckBox *checkBox3 = new QCheckBox("特效追踪",this);
    checkBox3->setChecked(enable_light_track);
    overall->addWidget(checkBox3);
    connect(checkBox3, &QCheckBox::clicked, this, [this](bool checked) {
        enable_light_track = checked;
        pmw->update();
    });

    QCheckBox *checkBoxTextShadow = new QCheckBox("文字阴影",this);
    checkBoxTextShadow->setChecked(enable_text_shadow);
    overall->addWidget(checkBoxTextShadow);
    connect(checkBoxTextShadow, &QCheckBox::clicked, this, [](bool checked) {
        enable_text_shadow = checked;
        foreach (auto pmw, pmws) {
            QList<SMultiFunc*> list = pmw->findChildren<SMultiFunc*>();
            foreach (auto sf, list) {
                sf->text_shadow->setEnabled(checked);
                sf->text_shadow->update();
            }
            pmw->update();
        }

    });

    QCheckBox *checkBoxIcon = new QCheckBox("超清图标(需重启）",this);
    checkBoxIcon->setChecked(enable_highdef_icon);
    overall->addWidget(checkBoxIcon);
    connect(checkBoxIcon, &QCheckBox::clicked, this, [](bool checked) {
        enable_highdef_icon = checked;
    });



    paint =  new QVBoxLayout();
    settings->addLayout(paint);



#ifdef QT_DEBUG

    QCheckBox *checkBox4 = new QCheckBox("绘制边框",this);
    checkBox4->setChecked(ShowSide);
    paint->addWidget(checkBox4);
    connect(checkBox4, &QCheckBox::clicked, this, [this](bool checked) {
        ShowSide = checked;
        pmw->update();
    });

#endif

    QCheckBox *checkBox5 = new QCheckBox("绘制矩形",this);
    checkBox5->setChecked(ShowRect);
    paint->addWidget(checkBox5);
    connect(checkBox5, &QCheckBox::clicked, this, [this](bool checked) {
        ShowRect = checked;
        pmw->update();
    });

    QCheckBox *checkBox6 = new QCheckBox("绘制光效",this);
    checkBox6->setChecked(ShowLight);
    paint->addWidget(checkBox6);
    connect(checkBox6, &QCheckBox::clicked, this, [this](bool checked) {
        ShowLight = checked;
        pmw->repaint();
    });


    QCheckBox *checkBox7 = new QCheckBox("大图标填充",this);
    checkBox7->setChecked(enable_image_fill);
    connect(checkBox7, &QCheckBox::clicked, this, [this](bool checked) {
        enable_image_fill = checked;
        pmw->endUpdate();
        pls->raise();
    });
    overall->addWidget(checkBox7);


#ifdef QT_DEBUG
    QCheckBox *checkBox8 = new QCheckBox("即时重绘",this);
    checkBox8->setChecked(enable_intime_repaint);
    connect(checkBox8, &QCheckBox::clicked, this, [this](bool checked) {
        enable_intime_repaint = checked;
        pmw->repaint();
    });
    paint->addWidget(checkBox8);
#endif

    scale_Slider = new QSlider(this);
    scale_Slider->setRange(0, 100);
    scale_Slider->setValue(100);
    connect(scale_Slider, &QSlider::valueChanged, this, [this](int val) {
        pmw->setScale(1.0*val/100);
    });
    settings->addWidget(scale_Slider);

    QSlider* radius_Slider = new QSlider(this);
    radius_Slider->setRange(0, 80);
    radius_Slider->setValue(unit_radius);
    connect(radius_Slider, &QSlider::valueChanged, this, [this](int val) {
        unit_radius = val;
        pmw->endUpdate();
    });
    settings->addWidget(radius_Slider);

    QPushButton *button = new QPushButton("选择背景", this);
    connect(button, &QPushButton::clicked, this, [this]() {
        // 点击按钮后的处理逻辑
        pmw->onSelectBackground();
    });
        paint->addWidget(button);
}
