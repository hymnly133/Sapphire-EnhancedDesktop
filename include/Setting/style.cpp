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
#include <QScrollArea>

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

    // 常用设置设计, 恢复选项，加载设置，
    m_totalWidget = new styleSetTotal;
    ui->stackedWidget->addWidget(m_totalWidget);

    // 对各个字段添加到list和StackWidget中
    IniStacked();

    // 初始化布局
    initializeLayouts();

    // 添加控件
    processBoolValues();
    processIntValues();
    processDoubleValues();

    // 最后一页添加《关于》页面
    IniAboutPage();
    connect(ui->listWidget, &QListWidget::itemClicked, this, &StyleSettingWindow::onListClicked);
    connect(m_totalWidget, &styleSetTotal::on_fontChangeBox_clicked, this, &StyleSettingWindow::on_fontChangeBox_clicked);
    connect(m_totalWidget, &styleSetTotal::on_rebootBox_clicked, this, &StyleSettingWindow::on_rebootBox_clicked);
    connect(m_totalWidget, &styleSetTotal::on_resizeBox_clicked, this, &StyleSettingWindow::on_resizeBox_clicked);
}

//通过参数将content设置到应有的地方
void StyleSettingWindow::setInLayout(QStringList fields, QString name, QWidget* content, bool isCheck)
{
    QString fieldKey = fields[0];
    if (isCheck && !checklayouts.contains(fieldKey)) {
        auto k = new QVBoxLayout();
        k->setObjectName(fieldKey + name + "Check");
        checklayouts.insert(fieldKey, k);
        k->addWidget(content);
        layouts[fieldKey]->addLayout(k, 1);
    }

    if (isCheck) {
        checklayouts[fieldKey]->addWidget(content);
    } else {
        if (!sliderlayouts.contains(fieldKey)) {
            auto k = new QVBoxLayout();
            k->setAlignment(Qt::AlignCenter);
            k->setObjectName(fieldKey + "layout");
            sliderlayouts.insert(fieldKey, k);
            layouts[fieldKey]->addLayout(k, 1);
        }
        sliderlayouts[fieldKey]->addWidget(content);
        sliderlayouts[fieldKey]->addWidget(new QLabel(name));
    }
}


// 加载对应的Wedget
void StyleSettingWindow::IniStacked()
{
    for(auto& c : psh->boolStyles) {
        iniAllWidget(c->fields);
    }
    for(auto& c : psh->intStyles) {
        iniAllWidget(c->fields);
    }
    for(auto& c : psh->doubleStyles) {
        iniAllWidget(c->fields);
    }
    for(auto& c : psh->stringStyles) {
        iniAllWidget(c->fields);
    }

    // 让list中字段排序
    QStringList sortedFields;
    for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it) {
        sortedFields.append(it.key());
    }
    // 对字段名列表进行排序
    std::sort(sortedFields.begin(), sortedFields.end());

    // 遍历排序后的字段名列表，将对应的控件添加到 QStackedWidget 中
    for (const auto& field : sortedFields) {
        qDebug() << field;
        QWidget* widget = m_widgets[field];
        ui->listWidget->addItem(field);
        ui->stackedWidget->addWidget(widget);
    }
}

// 初始化各个主空间，Color/xxx
void StyleSettingWindow::iniAllWidget(const QStringList& fields)
{
    int listSize = fields.size();

    if(listSize == 0){ qDebug()<<"error"; return;}

    //主级空间
    if(!m_widgets.contains(fields[0]))
    {
        m_widgets[fields[0]] =  new QWidget;
    }

    //仅有一层，直接返回
    if(listSize == 1){ return; }


    //todo: 添加到对应的list下
    //QMap<QString, QWidget*> m_fistWidget
    //二级页面
    if(!m_widgets.contains(fields[0]+"/"+fields[1]))
    {
        m_widgets[fields[0]+"/"+fields[1]] = new QWidget;
    }
}


void StyleSettingWindow::initializeLayouts()
{
    for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it) {
        const QString& name = it.key();
        QWidget* widget = it.value();

        // 创建一个QWidget作为QScrollArea的内容部件
        QWidget *contentWidget = new QWidget;
        QHBoxLayout *contentLayout = new QHBoxLayout(contentWidget);
        layouts[name] = contentLayout;

        QScrollArea *scrollArea = new QScrollArea;
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(contentWidget);

        // 创建一个垂直布局管理器
        QVBoxLayout *mainLayout = new QVBoxLayout(widget);
        mainLayout->addWidget(scrollArea);  // 将QScrollArea添加到布局中

        // 设置QWidget的布局
        widget->setLayout(mainLayout);

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

void StyleSettingWindow::IniAboutPage()
{



    // 添加 "关于" 项到 QListWidget
    ui->listWidget->addItem("关于");

    auto aboutWidget = new QWidget();
    auto layout = new QVBoxLayout(aboutWidget);
    ui->stackedWidget->addWidget(aboutWidget);

    // 标题
    auto titleLabel = new QLabel("软件名称 - 关于");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    //todo: 修改图片
    QPixmap pixmap(":/images/background");
    auto logoLabel = new QLabel;
    logoLabel->setPixmap(pixmap.scaledToWidth(64, Qt::SmoothTransformation));
    layout->addWidget(logoLabel, 0, Qt::AlignHCenter);

    // 版本号
    auto versionLabel = new QLabel("版本: 1.0.0");
    layout->addWidget(versionLabel);

    // 作者
    auto authorLabel = new QLabel("作者: xxx\n其他参与者:xxx,xxx,xxx");
    layout->addWidget(authorLabel);

    // 添加许可证信息
    auto licenseLabel = new QLabel("本软件遵循 GNU General Public License v3.0 许可证.");
    licenseLabel->setWordWrap(true);
    layout->addWidget(licenseLabel);

    // 许可证链接
    auto licenseLinkLabel = new QLabel("<a href=\"https://www.gnu.org/licenses/gpl-3.0.html\">查看许可证</a>");
    licenseLinkLabel->setOpenExternalLinks(true);
    layout->addWidget(licenseLinkLabel);
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

