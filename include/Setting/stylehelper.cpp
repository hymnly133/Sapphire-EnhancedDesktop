#include "stylehelper.h"
#include "global.h"
#include "qfileinfo.h"
#include "style.h"
#include "userfunc.h"


#define ADD(FIELDS,NAME,DISPALY,MIN,MAX)\
Add(QStringList()<<FIELDS,#NAME,&NAME,DISPALY,MIN,MAX);

StyleHelper::StyleHelper()
{
    ADD(tr("颜色"), theme_color, tr("主题色"), 0, 0);
    ADD(tr("颜色"), use_syscolor_as_themecolor, tr("使用系统主题色"), 0, 0);
    ADD(tr("颜色"), background_color, tr("背景色"), 0, 0);
    ADD(tr("颜色"), text_color, tr("文字色"), 0, 0);
    ADD(tr("颜色"), highlight_color, tr("高亮色"), 0, 0);
    ADD(tr("颜色") << tr("聚焦"), unfocused_alpha, tr("未聚焦的外框颜色 Alpha"), 0, 255);
    ADD(tr("颜色") << tr("聚焦"), focused_alpha, tr("聚焦时的外框颜色 Alpha"), 0, 255);
    ADD(tr("颜色") << tr("聚焦"), unfocused_alpha_deep, tr("深色图标未聚焦的颜色 Alpha"), 0, 255);
    ADD(tr("颜色") << tr("聚焦"), focused_alpha_deep, tr("深色图标聚焦时的颜色 Alpha"), 0, 255);
    ADD(tr("颜色") << tr("聚焦"), unfocused_color_ratio, tr("未聚焦的色值混合比率"), 0, 1);
    ADD(tr("颜色") << tr("聚焦"), focused_color_ratio, tr("聚焦时的色值混合比率"), 0, 1);
    ADD(tr("颜色") << tr("光效"), light_alpha_start, tr("光效的起点 Alpha"), 0, 255);
    ADD(tr("颜色") << tr("光效"), light_alpha_end, tr("光效的终点 Alpha"), 0, 255);
    ADD(tr("颜色") << tr("光效"), enable_light_track, tr("特效追踪"), 0, 0);
    ADD(tr("颜色") << tr("阴影"), icon_shadow_alpha, tr("图标阴影特效 Alpha"), 0, 255);
    ADD(tr("颜色") << tr("阴影"), icon_shadow_blur_radius, tr("图标阴影特效 Radius"), 1, 100);
    ADD(tr("颜色") << tr("阴影"), unit_shadow_alpha, tr("所有组件阴影特效 Alpha"), 0, 255);
    ADD(tr("颜色") << tr("阴影"), unit_shadow_blur_radius, tr("所有组件阴影特效 Radius"), 1, 100);
    ADD(tr("颜色") << tr("阴影"), enable_text_shadow, tr("文字阴影"), 0, 0);
    ADD(tr("动画") << tr("放置"), position_animation_time, tr("放置动画时长"), 0, 400);
    ADD(tr("动画") << tr("聚焦"), focus_animation_time, tr("聚焦动画时长"), 0, 400);
    ADD(tr("动画") << tr("长聚焦"), long_focus_animation_time, tr("长聚焦动画时长"), 0, 1000);
    ADD(tr("动画"), enable_refresh_animation, tr("刷新闪烁动画"), 0, 0);
    ADD(tr("交互") << tr("长聚焦"), long_focus_in_delta_time, tr("长聚焦进入需时"), 30, 1000);
    ADD(tr("交互") << tr("长聚焦"), long_focus_out_delta_time, tr("长聚焦失焦需时"), 200, 2000);
    ADD(tr("交互") << tr("长聚焦"), long_focus_container_fix_ratio, tr("长聚焦时格子缩放比率"), 1.1, 3.0);
    ADD(tr("交互") << tr("聚焦"), scale_fix_ratio, tr("聚焦时缩放比率"), 1, 2);
    ADD(tr("交互") << tr("文件夹"), enable_dir_func, tr("文件夹高级交互"), 0, 0);
    ADD(tr("交互") << tr("文件夹"), enable_dir_preview, tr("聚焦预览（高级交互启用时）"), 0, 0);
    ADD(tr("外观"), unit_radius, tr("组件圆角"), 0, 100);
    ADD(tr("外观"), ShowRect, tr("绘制组件的矩形"), 0, 0);
    ADD(tr("外观"), ShowSide, tr("绘制组件边框"), 0, 0);
    ADD(tr("外观"), ShowLight, tr("绘制组件光效"), 0, 0);
    ADD(tr("外观") << tr("界面"), enable_notice, tr("启用通知"), 0, 0);
    ADD(tr("外观") << tr("界面"), show_important_notice, tr("关闭通知时仍然显示重要通知（如更新检测）"), 0, 0);
    ADD(tr("外观") << tr("界面"), enable_tooltip, tr("启用标签"), 0, 0);
    ADD(tr("外观"), enable_background_transparent, tr("背景透视"), 0, 0);
    ADD(tr("外观"), enable_background_blur, tr("背景模糊-未完工"), 0, 0);
    ADD(tr("外观"), enable_image_fill, tr("大图标填充"), 0, 0);
    ADD(tr("外观") << tr("字体"), font_size, tr("字体大小"), 3, 20);
    ADD(tr("系统"), enable_highdef_icon, tr("超清图标"), 0, 0);
    ADD(tr("系统"), always_fill_screen, tr("布局填满屏幕（即包括任务栏区域）"), 0, 0);
    ADD(tr("偏好"), default_steam_icon_type, tr("封面获取方式"), 0, 2);
    ADD(tr("偏好"), use_pic_as_icon, tr("使用图片作为Icon"), 0, 0);
    ADD(tr("系统"), enable_intime_repaint, tr("即时重绘"), 0, 0);
    ADD(tr("系统"), try_run_as_user, tr("尝试降权运行"), 0, 0);
    ADD(tr("系统"), enable_low_memory_mode, tr("低内存模式（更消耗性能）"), 0, 0);

    ADD(tr("偏好"), user_font, tr("用户字体"), 0, 0);
    ADD(tr("偏好"), always_simple_mode, tr("新图标默认精简"), 0, 0);
    psh = this;
    connectSysChanges();
}

void StyleHelper::connectSysChanges()
{
    connect(boolVal("use_syscolor_as_themecolor"), &boolVal::valueChanged, this, [ = ]() {
        emit colorChanged();
    });
    connectTo(font_size, int, int, {
        emit fontChanged();
    })
    connectTo(user_font, string, QString, {
        emit fontChanged();
    })
    connect(this, &StyleHelper::fontChanged, this, updateFont);
}


void StyleHelper::Add(QStringList fields, QString name, bool * pval, QString displayName, bool min, bool max)
{
    struct boolVal* res = new struct boolVal(fields, name, pval, displayName);
    boolStyles.insert(res->name, res);
}

void StyleHelper::Add(QStringList fields, QString name, QString * pval, QString displayName, QString min, QString max)
{
    struct stringVal* res = new struct stringVal(fields, name, pval, displayName);
    stringStyles.insert(res->name, res);
}

void StyleHelper::Add(QStringList fields, QString name, QColor *pval, QString displayName, QString min, QString max)
{
    struct colorVal* res = new struct colorVal(fields, name, pval, displayName);
    colorStyles.insert(res->name, res);
    connect(res, &colorVal::valueChanged, this, [ = ]() {
        emit colorChanged();
    });
}


void StyleHelper::Add(QStringList fields, QString name, int * pval, QString displayName, int min, int max)
{
    struct intVal* res = new struct intVal(fields, name, pval, displayName, min, max);
    intStyles.insert(res->name, res);
}

void StyleHelper::Add(QStringList fields, QString name, double * pval, QString displayName, double min, double max)
{
    struct doubleVal* res = new struct doubleVal(fields, name, pval, displayName, min, max);
    doubleStyles.insert(res->name, res);
}



void StyleHelper::readStyleIni()
{
    QFileInfo fi(QApplication::applicationDirPath() + "/style.ini");
    if(fi.exists()) {
        QSettings *styleIni = new QSettings(QApplication::applicationDirPath() + "/style.ini", QSettings::IniFormat);
        //拥有version变量前的最后一版
        styleVersion = LAST_REG_AUTORUN_VERSION;
        if(styleIni->contains("Version")) {
            styleVersion = QVersionNumber::fromString(styleIni->value("Version").toString());
        }
        qInfo() << "StyleVersion :" << styleVersion;
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
        foreach (auto val, colorStyles.values()) {
            val->read(styleIni);
        }
        delete styleIni;
    } else {
        writeStyleIni();
    }
}

void StyleHelper::writeStyleIni()
{
    QSettings *styleIni = new QSettings(QApplication::applicationDirPath() + "/style.ini", QSettings::IniFormat);
    styleIni->setValue("Version", version.toString());
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
    foreach (auto val, colorStyles.values()) {
        val->write(styleIni);
    }
    delete styleIni;
}

intVal* StyleHelper::intVal(QString name)
{
    if(intStyles.contains(name)) {
        return intStyles[name];
    }
    return nullptr;
}
doubleVal* StyleHelper::doubleVal(QString name)
{
    if(doubleStyles.contains(name)) {
        return doubleStyles[name];
    }
    return nullptr;
}
boolVal* StyleHelper::boolVal(QString name)
{
    if(boolStyles.contains(name)) {
        return boolStyles[name];
    }
    return nullptr;
}
stringVal* StyleHelper::stringVal(QString name)
{
    if(stringStyles.contains(name)) {
        return stringStyles[name];
    }
    return nullptr;
}

colorVal *StyleHelper::colorVal(QString name)
{
    if(colorStyles.contains(name)) {
        return colorStyles[name];
    }
    return nullptr;
}





