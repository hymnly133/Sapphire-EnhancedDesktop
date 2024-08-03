#include "style.h"
#include "SysFunctions.h"
#include "global.h"
#include "mainwindow.h"
#include "qfileinfo.h"
#include "qsettings.h"
#include"QInputDialog"
#include"ui_styleSetting.h"
#include "userfunc.h"
#include "QProcess"

int unfocused_alpha = 130;
int focused_alpha = 220;

int unfocused_alpha_deep = 140;
int focused_alpha_deep = 200;

double unfocused_color_ratio = 0.8;
double focused_color_ratio = 0.95;

int light_alpha_start = 120;
int light_alpha_end = 0;

bool ShowRect = true;
bool ShowSide = false;
bool ShowLight = true;

int icon_shadow_alpha = 180;
int icon_shadow_blur_radius = 40;

int unit_shadow_alpha = 120;
int unit_shadow_blur_radius = 8;

int unit_radius = 15;
int position_animation_time = 100;
int focus_animation_time = 100;

bool enable_background_blur = false;
bool enable_light_track = false;

bool enable_intime_repaint = false;

bool enable_image_fill=false;
bool enable_highdef_icon =1;
bool dark_mode = 0;

int default_steam_icon_type =1;

int long_focus_in_delta_time = 500;
int long_focus_out_delta_time = 1000;
double long_focus_container_fix_ratio = 1.71;

double scale_fix_ratio = 1.3;

bool enable_tooltip_right_animation = 0;

bool enable_text_shadow = 0;
bool use_pic_as_icon = 1;

bool enable_auto_run = 0;
bool enable_resize_to_rect = 0;

bool enable_refresh_animation=0;
bool enable_background_transparent = 1;
QString user_font = "";


#define ADD(TYPE,NAME,DISPALY,MIN,MAX)\
Add(#TYPE"/"#NAME,&NAME,#DISPALY,MIN,MAX);

StyleHelper::StyleHelper()
{


    ADD(Color/Focus,unfocused_alpha,未聚焦的外框颜色 Alpha,0,255);
    ADD(Color/Focus,focused_alpha,聚焦时的外框颜色 Alpha,0,255);

    ADD(Color/Focus,unfocused_alpha_deep,深色图标未聚焦的颜色 Alpha,0,255);
    ADD(Color/Focus,focused_alpha_deep,深色图标聚焦时的颜色 Alpha,0,255);

    ADD(Color/Focus,unfocused_color_ratio,未聚焦的色值混合比率,0,1);
    ADD(Color/Focus,focused_color_ratio,聚焦时的色值混合比率,0,1);

    ADD(Effect/Light,light_alpha_start,光效的起点 Alpha,0,255);
    ADD(Effect/Light,light_alpha_end,光效的终点 Alpha ,0,255);
    ADD(Effect/Light,enable_light_track,特效追踪,0,0);

    ADD(Effect/Shadow,icon_shadow_alpha,图标阴影特效 Alpha,0,255);
    ADD(Effect/Shadow,icon_shadow_blur_radius,图标阴影特效 Radius,0,100);

    ADD(Effect/Shadow,unit_shadow_alpha,所有组件阴影特效 Alpha,0,255);
    ADD(Effect/Shadow,unit_shadow_blur_radius,所有组件阴影特效 Radius,0,100);
    ADD(Effect/Shadow,enable_text_shadow,文字阴影,0,0);


    ADD(Animation/Position,position_animation_time,放置动画时长,0,200);
    ADD(Animation/Focus,focus_animation_time,聚焦动画时长,0,200);
    ADD(Animation,enable_refresh_animation,刷新闪烁动画,0,0);


    ADD(Interact/LongFocus,long_focus_in_delta_time,长聚焦进入需时,30,1000);
    ADD(Interact/LongFocus,long_focus_out_delta_time,长聚焦失焦需时,200,2000);
    ADD(Interact/LongFocus,long_focus_container_fix_ratio,长聚焦时格子缩放比率,1.1,3.0);

    ADD(Interact/Focus,scale_fix_ratio,聚焦时缩放比率,1,2);

    ADD(Appearance,unit_radius,组件圆角,0,100);

    ADD(Appearance,ShowRect,绘制组件的矩形,0,0);
    ADD(Appearance,ShowSide,绘制组件边框,0,0);
    ADD(Appearance,ShowLight,绘制组件光效,0,0);

    ADD(Appearance,enable_background_transparent,背景透视,0,0);
    ADD(Appearance,enable_background_blur,背景模糊-未完工,0,0);

    ADD(Preference,enable_image_fill,大图标填充,0,0);
    ADD(Preference,enable_highdef_icon,超清图标,0,0);
    ADD(Preference,default_steam_icon_type,封面获取方式,0,2);
    ADD(Preference,use_pic_as_icon,使用图片作为Icon,0,0);


    ADD(System,enable_intime_repaint,即时重绘,0,0);
    ADD(System,user_font,开机自启,0,0);

    psh = this;
}


void StyleHelper::Add(QString fullName, bool * pval, QString displayName,bool min,bool max)
{
    struct boolVal* res = new struct boolVal(fullName,pval,displayName);
    boolStyles.insert(res->name,res);
}

void StyleHelper::Add(QString fullName, QString * pval, QString displayName,QString min, QString max)
{
    struct stringVal* res = new struct stringVal(fullName,pval,displayName);
    stringStyles.insert(res->name,res);
}


void StyleHelper::Add(QString fullName, int * pval,QString displayName,int min,int max)
{
    struct intVal* res = new struct intVal(fullName,pval,displayName,min,max);
    intStyles.insert(res->name,res);
}

void StyleHelper::Add(QString fullName, double * pval,QString displayName,double min,double max)
{
    struct doubleVal* res = new struct doubleVal(fullName,pval,displayName,min,max);
    doubleStyles.insert(res->name,res);
}



void StyleHelper::readStyleIni()
{
    QFileInfo fi(QApplication::applicationDirPath()+"/style.ini");
    if(fi.exists()){
        QSettings *styleIni = new QSettings(QApplication::applicationDirPath()+"/style.ini", QSettings::IniFormat);
        foreach (auto val, boolStyles.values()) {
            val->read(styleIni);
        }
        foreach (auto val, intStyles.values()) {
            val->read(styleIni);
        }
        foreach (auto val, doubleStyles.values()) {
            val->read(styleIni);
        }
        foreach (auto val, stringStyles.values()) {
            val->read(styleIni);
        }

        delete styleIni;
    }
    else{
        writeStyleIni();
    }
}

void StyleHelper::writeStyleIni()
{
    QSettings *styleIni = new QSettings(QApplication::applicationDirPath()+"/style.ini", QSettings::IniFormat);

    foreach (auto val, boolStyles.values()) {
        val->write(styleIni);
    }
    foreach (auto val, intStyles.values()) {
        val->write(styleIni);
    }
    foreach (auto val, doubleStyles.values()) {
        val->write(styleIni);
    }
    foreach (auto val, stringStyles.values()) {
        val->write(styleIni);
    }

    delete styleIni;
}

intVal* StyleHelper::intVal(QString name)
{
    if(intStyles.contains(name)) return intStyles[name];
    return nullptr;
}
doubleVal* StyleHelper::doubleVal(QString name)
{
    if(doubleStyles.contains(name)) return doubleStyles[name];
    return nullptr;
}
boolVal* StyleHelper::boolVal(QString name)
{
    if(boolStyles.contains(name)) return boolStyles[name];
    return nullptr;
}
stringVal* StyleHelper::stringVal(QString name)
{
    if(stringStyles.contains(name)) return stringStyles[name];
    return nullptr;
}

StyleSettingWindow::StyleSettingWindow():QDialog(nullptr),ui(new Ui::Form)
{

    ui->setupUi(this);

    // 常用设置设计
    m_totalWidget = new styleSetTotal;
    ui->stackedWidget->addWidget(m_totalWidget);

    m_widgets["Color"] = new QWidget;
    m_widgets["Effect"] = new QWidget;
    m_widgets["Render"] = new QWidget;
    m_widgets["Preference"] = new QWidget;
    for(auto& widget : m_widgets)
    {
        ui->stackedWidget->addWidget(widget);
    }

    // 初始化布局
    initializeLayouts();

    // 添加控件
    // processBoolValues();
    processIntValues();
    // processDoubleValues();

    connect(ui->listWidget, &QListWidget::itemClicked, this, &StyleSettingWindow::onListClicked);
    connect(m_totalWidget, &styleSetTotal::on_fontChangeBox_clicked, this, &StyleSettingWindow::on_fontChangeBox_clicked);
    connect(m_totalWidget, &styleSetTotal::on_rebootBox_clicked, this, &StyleSettingWindow::on_rebootBox_clicked);
    connect(m_totalWidget, &styleSetTotal::on_resizeBox_clicked, this, &StyleSettingWindow::on_resizeBox_clicked);
}

//通过参数将content设置到应有的地方
void StyleSettingWindow::setInLayout(QStringList fields, QString name, QWidget *content, bool checkBox)
{
    // if(checkBox&& !checklayouts.contains(field)){
    //     auto k = new QVBoxLayout();
    //     k->setObjectName(field+"Check");
    //     checklayouts.insert(field,k);
    //     layouts[fields]->addLayout(k,1);
    // }

    // if(checkBox)
    // {
    //     checklayouts[field]->addWidget(content);
    // }
    // else
    // {
    //     if(!sliderlayouts.contains(name)){
    //         auto k = new QVBoxLayout();
    //         k->setAlignment(Qt::AlignCenter);
    //         k->setObjectName(name+"layout");
    //         sliderlayouts.insert(field,k);
    //         layouts[field]->addLayout(k,1);
    //     }
    //     sliderlayouts[field]->addWidget(content);
    //     sliderlayouts[field]->addWidget(new QLabel(name));
    // }
}

void StyleSettingWindow::initializeLayouts()
{
    for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it) {
        const QString& name = it.key();
        QWidget* widget = it.value();

        auto layout = new QHBoxLayout(widget);
        layouts[name] = layout;
        widget->setLayout(layout);
    }
}


// 关于设置页面list被点击时的变动信号槽
void StyleSettingWindow::onListClicked(QListWidgetItem *item)
{
    // 当前点击的项的行号
    int index = ui->listWidget->row(item);
    ui->stackedWidget->setCurrentIndex(index);
}

// 添加布尔值控件
void StyleSettingWindow::processBoolValues()
{
    foreach (auto val, psh->boolStyles.values()) {

        val->checkbox = new QCheckBox(this);
        val->checkbox->setText(val->displayName);
        val->checkbox->setChecked(val->val());

        //连接控件的信号与变量的set函数
        qDebug()<<connect(val->checkbox,&QCheckBox::stateChanged,val,&boolVal::set);

        //待完善
        setInLayout(val->fields,val->name,val->checkbox,1);
    }
}

// 添加整数值控件
void StyleSettingWindow::processIntValues()
{
    foreach (auto val, psh->intStyles.values()) {


        val->slider = new QSlider(this);
        val->slider->setRange(0,1000);
        int var =(double)(val->val()-val->min)/(val->max-val->min)*1000;
        val->slider->setValue(var);
        // val->slider->setText(val->name());

        //连接控件的信号与变量的set函数
        connect(val->slider,&QSlider::valueChanged,val,[=](int changedVal){
            val->set((double)1.0*changedVal/1000*(val->max-val->min)+val->min);
            qDebug()<<"changedVal"<<changedVal;
        });
        //模拟外部连接
        connect(psh->intVal(val->name),&intVal::valueChanged,this,[=](int newVal){
            qDebug()<<val->name<<"SignalReceived"<<newVal;
        });
        //待完善
        setInLayout(val->fields,val->name,val->slider,0);

    }
}

// 添加浮点数值控件
void StyleSettingWindow::processDoubleValues()
{
    foreach (auto val, psh->intStyles.values()) {
        val->slider = new QSlider(this);

        //将值域映射到0：1000
        val->slider->setRange(0,1000);
        int var = (double)(val->val()-val->min)/(val->max-val->min)*1000;
        val->slider->setValue(var);

        //连接控件的信号与变量的set函数
        connect(val->slider,&QSlider::valueChanged,val,[=](int value){
            val->set((double)1.0*value/1000*(val->max-val->min)+val->min);
        });

        //待完善
        setInLayout(val->fields,val->name,val->slider,0);
    }
}


void StyleSettingWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    foreach(auto pmw,pmws){
        pmw->endUpdate();
    }
}



QColor winThemeColor()
{
    DWORD crColorization;
    BOOL fOpaqueBlend;
    QColor res;
    HRESULT result = DwmGetColorizationColor(&crColorization, &fOpaqueBlend);
    if (result == S_OK)
    {
        BYTE r, g, b;
        r = (crColorization >> 16) % 256;
        g = (crColorization >> 8) % 256;
        b = crColorization % 256;
        res = QColor(r, g, b);
    }
    return res;
}

void StyleSettingWindow::on_fontChangeBox_clicked()
{
    QString fontname = this->m_totalWidget->getFontName();
    QFont currentFont = qApp->font();
    currentFont.setFamily(fontname);
    qApp->setFont(currentFont);
    user_font = fontname;
}


void StyleSettingWindow::on_rebootBox_clicked()
{

    qApp->exit(733);
}


void StyleSettingWindow::on_resizeBox_clicked()
{
    resizeForWithDialog(activepmw->inside);
}

