#include "style.h"
#include "SysFunctions.h"
#include "global.h"
#include "mainwindow.h"
#include "qfileinfo.h"
#include "qsettings.h"
#include"QInputDialog"
#include"SNotice.h"
#include"ui_styleSetting.h""
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

int unit_radius = 30;
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

QString user_font = "";


#define ADD(TYPE,NAME,MIN,MAX)\
Add(#TYPE"/"#NAME,&NAME,MIN,MAX);

StyleHelper::StyleHelper()
{

    ADD(Color,unfocused_alpha,0,255);
    ADD(Color,focused_alpha,0,255);

    ADD(Color,unfocused_alpha_deep,0,255);
    ADD(Color,focused_alpha_deep,0,255);

    ADD(Color,unfocused_color_ratio,0,1);
    ADD(Color,focused_color_ratio,0,1);

    ADD(Effect,light_alpha_start,0,255);
    ADD(Effect,light_alpha_end,0,255);

    ADD(Effect,icon_shadow_alpha,0,255);
    ADD(Effect,icon_shadow_blur_radius,0,100);

    ADD(Effect,unit_shadow_alpha,0,255);
    ADD(Effect,unit_shadow_blur_radius,0,100);

    ADD(Effect,position_animation_time,0,200);
    ADD(Effect,focus_animation_time,0,200);
    ADD(Effect,enable_text_shadow,0,0);


    ADD(Render,unit_radius,0,100);

    ADD(Render,ShowRect,0,0);
    ADD(Render,ShowSide,0,0);
    ADD(Render,ShowLight,0,0);

    ADD(Render,enable_background_blur,0,0);
    ADD(Render,enable_light_track,0,0);
    ADD(Render,enable_intime_repaint,0,0);

    ADD(Preference,enable_image_fill,0,0);
    ADD(Preference,enable_highdef_icon,0,0);

    ADD(Preference,default_steam_icon_type,0,2);

    ADD(Preference,scale_fix_ratio,1,2);

    ADD(Preference,long_focus_in_delta_time,30,1000);
    ADD(Preference,long_focus_out_delta_time,200,2000);
    ADD(Preference,long_focus_container_fix_ratio,1.1,3.0);
    ADD(Preference,enable_tooltip_right_animation,0,0);

    ADD(Preference,use_pic_as_icon,0,0);
    ADD(Preference,user_font,0,0);

    psh = this;
}

void StyleHelper::Add(QString name, bool * pval,bool min,bool max)
{
    boolStyles.push_back(new boolVal(name,pval));
}

void StyleHelper::Add(QString name, QString * pval, QString min, QString max)
{
    stringStyles.push_back(new stringVal(name,pval));
}


void StyleHelper::Add(QString name, int * pval,int min,int max)
{
    intStyles.push_back(new intVal(name,pval,min,max));
}
void StyleHelper::Add(QString name, double * pval,double min,double max)
{
    doubleStyles.push_back(new doubleVal(name,pval,min,max));
}



void StyleHelper::readStyleIni()
{
    QFileInfo fi(QApplication::applicationDirPath()+"/style.ini");
    if(fi.exists()){
        QSettings *styleIni = new QSettings(QApplication::applicationDirPath()+"/style.ini", QSettings::IniFormat);
        QMutableListIterator<boolVal*> iterator0(boolStyles);
        while (iterator0.hasNext()) {
            iterator0.next();
            iterator0.value()->read(styleIni);
        }

        QMutableListIterator<intVal*> iterator1(intStyles);
        while (iterator1.hasNext()) {
            iterator1.next();
            iterator1.value()->read(styleIni);
        }

        QMutableListIterator<doubleVal*> iterator2(doubleStyles);
        while (iterator2.hasNext()) {
            iterator2.next();
            iterator2.value()->read(styleIni);
        }


        QMutableListIterator<stringVal*> iterator3(stringStyles);
        while (iterator3.hasNext()) {
            iterator3.next();
            iterator3.value()->read(styleIni);
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
    QMutableListIterator<boolVal*> iterator0(boolStyles);
    while (iterator0.hasNext()) {
        iterator0.next();
        iterator0.value()->write(styleIni);
    }

    QMutableListIterator<intVal*> iterator1(intStyles);
    while (iterator1.hasNext()) {
        iterator1.next();
        iterator1.value()->write(styleIni);
    }

    QMutableListIterator<doubleVal*> iterator2(doubleStyles);
    while (iterator2.hasNext()) {
        iterator2.next();
        iterator2.value()->write(styleIni);
    }

    QMutableListIterator<stringVal*> iterator3(stringStyles);
    while (iterator3.hasNext()) {
        iterator3.next();
        iterator3.value()->write(styleIni);
    }
    delete styleIni;
}

void StyleHelper::showSetting()
{

}

StyleSettingWindow::StyleSettingWindow():QDialog(nullptr),ui(new Ui::Form)
{

    ui->setupUi(this);
    mainLayout = ui->layouts;
    layouts["Color"] = ui->colorLayout;
    layouts["Effect"] = ui->effectLayout;
    layouts["Render"] = ui->renderLayout;
    layouts["Prefernce"] = ui->preferenceLayout;
    setLayout(ui->layouts);
    QMutableListIterator<boolVal*> iterator0(psh->boolStyles);
    while (iterator0.hasNext()) {
        iterator0.next();
        iterator0.value()->checkbox = new QCheckBox(this);
        iterator0.value()->checkbox->setText(iterator0.value()->name());
        iterator0.value()->checkbox->setChecked(iterator0.value()->val());
        connect(iterator0.value()->checkbox,&QCheckBox::stateChanged,this,[=](bool value){
            iterator0.value()->val() = value;
            foreach (auto pmw, pmws) {
                pmw->update();
            }
        });
        setInLayout(iterator0.value()->field(),iterator0.value()->name(),iterator0.value()->checkbox,1);
    }

    QMutableListIterator<intVal*> iterator1(psh->intStyles);
    while (iterator1.hasNext()) {
        iterator1.next();
        iterator1.value()->slider = new QSlider(this);
        iterator1.value()->slider->setRange(0,1000);
        int var =(double)(iterator1.value()->val()-iterator1.value()->min)/(iterator1.value()->max-iterator1.value()->min)*1000;
        iterator1.value()->slider->setValue(var);
        // iterator1.value()->slider->setText(iterator1.value()->name());
        connect(iterator1.value()->slider,&QSlider::valueChanged,this,[=](int value){
            iterator1.value()->val() = (double)value/1000*(iterator1.value()->max-iterator1.value()->min)+iterator1.value()->min;
            foreach (auto pmw, pmws) {
                pmw->update();
            }
        });
        setInLayout(iterator1.value()->field(),iterator1.value()->name(),iterator1.value()->slider,0);

    }

    QMutableListIterator<doubleVal*> iterator2(psh->doubleStyles);
    while (iterator2.hasNext()) {
        iterator2.next();
        iterator2.value()->slider = new QSlider(this);
        iterator2.value()->slider->setRange(0,1000);
        int var = (double)(iterator2.value()->val()-iterator2.value()->min)/(iterator2.value()->max-iterator2.value()->min)*1000;
        iterator2.value()->slider->setValue(var);
        connect(iterator2.value()->slider,&QSlider::valueChanged,this,[=](int value){
            iterator2.value()->val() = (double)1.0*value/1000*(iterator2.value()->max-iterator2.value()->min)+iterator2.value()->min;
            foreach (auto pmw, pmws) {
                pmw->update();
            }
        });
        setInLayout(iterator2.value()->field(),iterator2.value()->name(),iterator2.value()->slider,0);
    }
}

void StyleSettingWindow::setInLayout(QString field, QString name, QWidget *content, bool checkBox)
{

    if(!layouts.contains(field)){
        auto k = new QHBoxLayout();
        k->setObjectName(field);
        layouts.insert(field,k);
        mainLayout->addLayout(k,1);
    }

    if(checkBox&& !checklayouts.contains(field)){
        auto k = new QVBoxLayout();
        k->setObjectName(field+"Check");
        checklayouts.insert(field,k);
        layouts[field]->addLayout(k,1);
    }

    if(checkBox)
        checklayouts[field]->addWidget(content);
    else{
        if(!sliderlayouts.contains(name)){
            auto k = new QVBoxLayout();
            k->setAlignment(Qt::AlignCenter);
            k->setObjectName(name+"layout");
            sliderlayouts.insert(field,k);
            layouts[field]->addLayout(k,1);
        }
        sliderlayouts[field]->addWidget(content);
        sliderlayouts[field]->addWidget(new QLabel(name));
    }
}

void StyleSettingWindow::closeEvent(QCloseEvent *event)
{
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
    QString fontname = ui->fontComboBox->currentFont().family();
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
    resizeForActiveMW();
}

