#include "style.h"
#include "qdebug.h"
#include "qfileinfo.h"
#include "qsettings.h"


int sleep_alpha = 80;
int active_alpha = 160;

int sleep_alpha_deep = 140;
int active_alpha_deep = 200;

double sleep_color_ratio = 0.9;
double active_color_ratio = 0.9;

int light_alpha_start = 120;
int light_alpha_end = 0;

bool ShowRect = true;
bool ShowSide = false;
bool ShowLight = true;

int icon_shadow_alpha = 180;
int icon_shadow_blur_radius = 50;

int unit_shadow_alpha = 120;
int unit_shadow_blur_radius = 80;

int unit_radius = 30;
int position_animation_time = 100;

bool enable_background_transparent = true;
bool enable_background_blur = false;
bool enable_light_track = false;

bool enable_intime_repaint = false;

bool enable_image_fill=false;

int muilt_icon_default_type =1;
double scale_fix_ratio = 1.3;
bool enable_lnk_redirect = 1;

#define ADD(TYPE,NAME)\
Add(#TYPE"/"#NAME,&NAME);

StyleHelper::StyleHelper()
{
    intStyles = QMap<QString,int*>();
    boolStyles = QMap<QString,bool*>();
    doubleStyles = QMap<QString,double*>();

    ADD(Color,sleep_alpha);
    ADD(Color,active_alpha);

    ADD(Color,sleep_alpha_deep);
    ADD(Color,active_alpha_deep);

    ADD(Color,sleep_color_ratio);
    ADD(Color,active_color_ratio);

    ADD(Effect,light_alpha_start);
    ADD(Effect,light_alpha_end);

    ADD(Effect,icon_shadow_alpha);
    ADD(Effect,icon_shadow_blur_radius);

    ADD(Effect,unit_shadow_alpha);
    ADD(Effect,unit_shadow_blur_radius);

    ADD(Render,unit_radius);

    ADD(Render,ShowRect);
    ADD(Render,ShowSide);
    ADD(Render,ShowLight);

    ADD(Render,enable_background_transparent);
    ADD(Render,enable_background_blur);
    ADD(Render,enable_light_track);
    ADD(Render,enable_intime_repaint);

    ADD(Preference,enable_image_fill);
    ADD(Preference,muilt_icon_default_type);

    ADD(Preference,scale_fix_ratio);
    ADD(Preference,enable_lnk_redirect);
    psh = this;
}

void StyleHelper::Add(QString name, bool * pval)
{

    boolStyles.insert(name,pval);
}
void StyleHelper::Add(QString name, int * pval)
{
    intStyles.insert(name,pval);
}
void StyleHelper::Add(QString name, double * pval)
{
    doubleStyles.insert(name,pval);
}

void StyleHelper::readStyleIni()
{
    QFileInfo fi("style.ini");
    if(fi.exists()){
        QSettings *styleIni = new QSettings("style.ini", QSettings::IniFormat);
        QMapIterator<QString, bool*> iterator0(boolStyles);
        while (iterator0.hasNext()) {
            iterator0.next();
            *iterator0.value() = styleIni->value( iterator0.key()).toBool();
            qDebug() <<"Read"<< iterator0.key() << ":" << *iterator0.value();
        }

        QMapIterator<QString, int*> iterator1(intStyles);
        while (iterator1.hasNext()) {
            iterator1.next();
            *iterator1.value() = styleIni->value( iterator1.key()).toInt();
            qDebug() <<"Read"<< iterator1.key() << ":" << *iterator1.value();
        }
        QMapIterator<QString, double*> iterator2(doubleStyles);
        while (iterator2.hasNext()) {
            iterator2.next();
            *iterator2.value() = styleIni->value( iterator2.key()).toDouble();
            qDebug() <<"Read"<< iterator2.key() << ":" << *iterator2.value();
        }
        delete styleIni;
    }
    else{
        writeStyleIni();
    }
}

void StyleHelper::writeStyleIni()
{
    QSettings *styleIni = new QSettings("style.ini", QSettings::IniFormat);
    QMapIterator<QString, bool*> iterator0(boolStyles);
    while (iterator0.hasNext()) {
        iterator0.next();
        styleIni->setValue(iterator0.key(), QString::number(*iterator0.value()));
        qDebug() <<"Wrote"<< iterator0.key() << ":" << *iterator0.value();
    }

    QMapIterator<QString, int*> iterator1(intStyles);
    while (iterator1.hasNext()) {
        iterator1.next();
        styleIni->setValue(iterator1.key(), QString::number(*iterator1.value()));
        qDebug() <<"Wrote"<< iterator1.key() << ":" << *iterator1.value();
    }

    QMapIterator<QString, double*> iterator2(doubleStyles);
    while (iterator2.hasNext()) {
        iterator2.next();
        styleIni->setValue(iterator2.key(), QString::number(*iterator2.value()));
        qDebug() <<"Wrote"<< iterator2.key() << ":" << *iterator2.value();
    }
    delete styleIni;
}
