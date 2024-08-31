#include "stylesettingwindow.h"
#include "stylehelper.h"
#include "global.h"
#include "qscrollarea.h"
#include "qspinbox.h"
#include "scolorview.h"
#include "userfunc.h"
StyleSettingWindow::StyleSettingWindow(): QDialog(nullptr), ui(new Ui::Form)
{
    ui->setupUi(this);
    resize(1024, 720);
    // 常用设置设计, 恢复选项，加载设置，
    ui->treeWidget->setHeaderLabel("功能导航");
    m_totalWidget = new styleSetTotal(this);
    // ui->stackedWidget->addWidget(m_totalWidget);
    addContent(QStringList() << tr("通用"), m_totalWidget);
    processVals();
    // 最后一页添加《关于》页面
    IniAboutPage();
    connect(ui->rebootButton, &QPushButton::clicked, this, &StyleSettingWindow::on_rebootBox_clicked);
    // connect(ui->listWidget, &QListWidget::itemClicked, this, &StyleSettingWindow::onListClicked);
    connect(m_totalWidget, &styleSetTotal::on_fontChangeBox_clicked, this, &StyleSettingWindow::on_fontChangeBox_clicked);
    // connect(m_totalWidget, &styleSetTotal::on_rebootBox_clicked, this, &StyleSettingWindow::on_rebootBox_clicked);
    connect(m_totalWidget, &styleSetTotal::on_resizeBox_clicked, this, &StyleSettingWindow::on_resizeBox_clicked);
}

//通过参数将content设置到应有的地方
void StyleSettingWindow::setInLayout(QStringList fields, QWidget* content, bool issmall)
{
    QString leaveField = fields.last();
    QStringList parentField = fields.mid(0, fields.length() - 1);
    if(!field2widget.contains(leaveField)) {
        field2widget.insert(leaveField, new SFieldsWidget(this));
    }
    field2widget[leaveField]->add(parentField, content, issmall);
}



// 初始化各个主空间，xxx
void StyleSettingWindow::iniFieldWidget(const QStringList& fields)
{
    int listSize = fields.size();
    if(listSize == 0) {
        qDebug() << "error";
        return;
    }
    QString leaveField = fields.last();
    if(!field2widget.contains(leaveField)) {
        field2widget.insert(leaveField, new SFieldsWidget(this));
    }
    if(!fields2widget.contains(fields)) {
        fields2widget[fields] = field2widget[leaveField];
    }
}


// 添加布尔值控件
void StyleSettingWindow::processBoolValues()
{
    foreach (auto val, psh->boolStyles.values()) {
        QCheckBox* check = new QCheckBox(this);
        check->setText(val->displayName);
        check->setChecked(val->val());
        val->valWidget = check;
        //连接控件与变量函数
        connect(check, &QCheckBox::stateChanged, val, &boolVal::set);
        connect(val, &boolVal::valueChanged, check, &QCheckBox::setChecked);
        // 待完善
        // setInLayout(val->fields,val->valWidget,1);
        // addContent(val->fields,val->valWidget,1);
    }
}

// 添加整数值控件
void StyleSettingWindow::processIntValues()
{
    foreach (auto val, psh->intStyles.values()) {
        QWidget* valWidget = new QWidget(this);
        QSlider* slider = new QSlider(Qt::Horizontal, this);
        slider->setRange(val->min, val->max);
        slider->setValue(val->val());
        QVBoxLayout* insideLayout = new QVBoxLayout();
        valWidget->setLayout(insideLayout);
        //信息显示
        QHBoxLayout* infolayout = new QHBoxLayout();
        QLabel* displayNameLable = new QLabel(this);
        displayNameLable->setText(val->displayName);
        QSpinBox* spin = new QSpinBox(this);
        spin->setMinimum(val->min);
        spin->setMaximum(val->max);
        spin->setValue(val->val());
        infolayout->addWidget(displayNameLable);
        infolayout->addStretch();
        infolayout->addWidget(spin);
        insideLayout->addStretch();
        insideLayout->addLayout(infolayout);
        // insideLayout->addStretch();
        insideLayout->addWidget(slider);
        insideLayout->addStretch();
        val->valWidget = valWidget;
        //将控件连接到Val上
        connect(slider, &QSlider::valueChanged, val, &intVal::set);
        //显示指定int类型的valueChanged
        void (QSpinBox:: *spinBoxSignal)(int) = &QSpinBox::valueChanged;
        connect(spin, spinBoxSignal, val, &intVal::set);
        //将Val连接到控件上
        connect(val, &intVal::valueChanged, slider, &QSlider::setValue);
        connect(val, &intVal::valueChanged, spin, &QSpinBox::setValue);
        //待完善
        // setInLayout(val->fields,val->valWidget,0);
        // addContent(val->fields,val->valWidget,1);
    }
}

// 添加浮点数值控件
void StyleSettingWindow::processDoubleValues()
{
    foreach (auto val, psh->doubleStyles.values()) {
        QWidget* valWidget = new QWidget(this);
        QSlider* slider = new QSlider(Qt::Horizontal, this);
        //将置于投射到0-1000的范围内
        slider->setRange(0, 1000);
        int var = (double)(val->val() - val->min) / (val->max - val->min) * 1000;
        slider->setValue(var);
        QVBoxLayout* insideLayout = new QVBoxLayout();
        valWidget->setLayout(insideLayout);
        //信息显示
        QHBoxLayout* infolayout = new QHBoxLayout();
        QLabel* displayNameLable = new QLabel(this);
        displayNameLable->setText(val->displayName);
        QDoubleSpinBox* spin = new QDoubleSpinBox(this);
        spin->setMinimum(val->min);
        spin->setMaximum(val->max);
        spin->setValue(val->val());
        infolayout->addWidget(displayNameLable);
        infolayout->addStretch();
        infolayout->addWidget(spin);
        insideLayout->addStretch();
        insideLayout->addLayout(infolayout);
        // insideLayout->addStretch();
        insideLayout->addWidget(slider);
        insideLayout->addStretch();
        val->valWidget = valWidget;
        //将控件连接到Val上
        connect(slider, &QSlider::valueChanged, val, [ = ](int sliderValue) {
            val->set((1.0 * sliderValue / 1000) * (val->max - val->min) + val->min);
        });
        //显示指定double类型的valueChanged
        void (QDoubleSpinBox:: *spinBoxSignal)(double) = &QDoubleSpinBox::valueChanged;
        connect(spin, spinBoxSignal, val, &doubleVal::set);
        //将Val连接到控件上
        connect(val, &doubleVal::valueChanged, slider, [ = ](double trueVal) {
            slider->setValue((trueVal - val->min) / (val->max - val->min) * 1000);
        });
        connect(val, &doubleVal::valueChanged, spin, &QDoubleSpinBox::setValue);
    }
}

void StyleSettingWindow::processColorValues()
{
    foreach (auto val, psh->colorStyles.values()) {
        QWidget* valWidget = new QWidget(this);
        QVBoxLayout* insideLayout = new QVBoxLayout(valWidget);
        insideLayout->addStretch();
        valWidget->setLayout(insideLayout);
        SColorView* colorView = new SColorView(valWidget, *val->pval);
        insideLayout->addWidget(colorView);
        insideLayout->setAlignment(Qt::AlignCenter);
        //信息显示
        QLabel* displayNameLable = new QLabel(this);
        displayNameLable->setText(val->displayName);
        displayNameLable->setAlignment(Qt::AlignCenter);
        insideLayout->addWidget(displayNameLable);
        insideLayout->addStretch();
        val->valWidget = valWidget;
        //将控件连接到Val上
        connect(colorView, &SColorView::colorChanged, val, &colorVal::set);
        //将Val连接到控件上
        connect(val, &colorVal::valueChanged, colorView, [ = ](QColor newColor) {
            colorView->now = newColor;
        });
    }
}

void StyleSettingWindow::setTreeView(const QStringList &fields)
{
    qDebug() << fields;
    if(!field2ind.contains(fields.last())) {
        //若叶field没被指定，则指定
        QWidget* widget = fields2widget[fields];
        QScrollArea *scrollArea = new QScrollArea;
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(widget);
        // 创建一个垂直布局管理器
        QVBoxLayout *mainLayout = new QVBoxLayout(scrollArea);
        mainLayout->addWidget(widget);
        int ind = ui->stackedWidget->addWidget(scrollArea);
        field2ind.insert(fields.last(), ind);
        ind2widget.insert(ind, fields2widget[fields]);
    }
    //储存item对应的widget
    item2ind.insert(fields2treeItem[fields], field2ind[fields.last()]);
}

void StyleSettingWindow::addAItem(const QStringList &fields)
{
    if(fields2treeItem.contains(fields)) {
        return;
    }
    if(fields.size() == 1) {
        QTreeWidgetItem *item1 = new QTreeWidgetItem(ui->treeWidget, fields);
        fields2treeItem.insert(fields, item1);
        item2fields.insert(item1, fields);
        return;
    }
    QStringList pFields(fields.mid(0, fields.length() - 1));
    addAItem(pFields);
    QTreeWidgetItem *item1 = new QTreeWidgetItem(fields2treeItem[pFields], QStringList(fields.last()));
    fields2treeItem.insert(fields, item1);
    item2fields.insert(item1, fields);
}

void StyleSettingWindow::addContent(const QStringList &fields, QWidget *widget, bool issmall)
{
    addAItem(fields);
    iniFieldWidget(fields);
    fields2widget[fields]->add(fields.mid(0, fields.length() - 1), widget, issmall);
    setTreeView(fields);
}

void StyleSettingWindow::IniAboutPage()
{
    // // 添加 "关于" 项到 QListWidget
    // QTreeWidgetItem *item1 = new QTreeWidgetItem(ui->treeWidget,QStringList("关于"));
    auto aboutWidget = new QWidget();
    auto layout = new QVBoxLayout(aboutWidget);
    // ui->stackedWidget->addWidget(aboutWidget);
    // 标题
    auto titleLabel = new QLabel("Sapphire - 关于");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    //todo: 修改图片
    QPixmap pixmap(":/appIcon/Sapphire.png");
    auto logoLabel = new QLabel;
    logoLabel->setPixmap(pixmap.scaledToWidth(64, Qt::SmoothTransformation));
    layout->addWidget(logoLabel, 0, Qt::AlignHCenter);
    // 版本号
    auto versionLabel = new QLabel("版本:" + QApplication::applicationVersion());
    layout->addWidget(versionLabel);
    // 作者
    auto authorLabel = new QLabel(QString(R"(
作者: 诗音种的土豆/Hymnly 1336325450@qq.com
其他制作者：微风中的快乐 2329484200
Icon贡献：L St4r 1207638671


使用：QXlsx,qBreakpad,OpenSSl
    )"));
    layout->addWidget(authorLabel);
    // 添加许可证信息
    auto licenseLabel = new QLabel("本软件遵循 GNU General Public License v3.0 许可证.");
    licenseLabel->setWordWrap(true);
    layout->addWidget(licenseLabel);

    // 许可证链接
    auto licenseLinkLabel = new QLabel("<a href=\"https://www.gnu.org/licenses/gpl-3.0.html\">查看许可证</a>");
    licenseLinkLabel->setOpenExternalLinks(true);
    layout->addWidget(licenseLinkLabel);

    auto homeLabel = new QLabel("<a href=\"https://space.bilibili.com/176830290\">作者主页</a>");
    homeLabel->setOpenExternalLinks(true);
    layout->addWidget(homeLabel);

    auto donateLinkLabel = new QLabel("<a href=\"https://ifdian.net/a/Hymnly\">捐赠渠道</a>");
    donateLinkLabel->setOpenExternalLinks(true);
    layout->addWidget(donateLinkLabel);

    auto userHelpLabel = new QLabel("<a href=\"https://www.yuque.com/hymnly/ux6umv/ce3cqy419av769fk?singleDoc#\">使用手册</a>");
    userHelpLabel->setOpenExternalLinks(true);
    layout->addWidget(userHelpLabel);



    addContent(QStringList(tr("关于")), aboutWidget);
}

void StyleSettingWindow::processVals()
{
    // 处理Val的内部控件
    processBoolValues();
    processIntValues();
    processDoubleValues();
    processColorValues();
    QList<QStringList> sortedFields;
    // 让list中字段排序
    // QMap<QStringList,boolVal*> boolVals;
    // QMap<QStringList,intVal*> intVals;
    // QMap<QStringList,doubleVal*> doubleVals;
    foreach (auto val, psh->intStyles.values()) {
        if(!sortedFields.contains(val->fields)) {
            sortedFields.append(val->fields);
        }
    }
    foreach (auto val, psh->doubleStyles.values()) {
        if(!sortedFields.contains(val->fields)) {
            sortedFields.append(val->fields);
        }
    }
    foreach (auto val, psh->boolStyles.values()) {
        if(!sortedFields.contains(val->fields)) {
            sortedFields.append(val->fields);
        }
    }
    foreach (auto val, psh->colorStyles.values()) {
        if(!sortedFields.contains(val->fields)) {
            sortedFields.append(val->fields);
        }
    }
    // 对字段名列表进行排序
    std::sort(sortedFields.begin(), sortedFields.end());
    // 遍历排序后的字段名列表，将对应的控件添加到 窗口 中
    for (const auto& fields : sortedFields) {
        foreach (auto val, psh->intStyles.values()) {
            if(val->fields == fields) {
                addContent(fields, val->valWidget, val->issmall);
            }
        }
        foreach (auto val, psh->doubleStyles.values()) {
            if(val->fields == fields) {
                addContent(fields, val->valWidget, val->issmall);
            }
        }
        foreach (auto val, psh->boolStyles.values()) {
            if(val->fields == fields) {
                addContent(fields, val->valWidget, val->issmall);
            }
        }
        foreach (auto val, psh->colorStyles.values()) {
            if(val->fields == fields) {
                addContent(fields, val->valWidget, val->issmall);
            }
        }
    }
}

void StyleSettingWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    foreach(auto pmw, pmws) {
        pmw->endUpdate();
    }
    psh->writeStyleIni();
}


void StyleSettingWindow::on_fontChangeBox_clicked()
{
    QString fontname = this->m_totalWidget->getFontName();
    psh->stringVal("user_font")->set(fontname);
    // user_font = fontname;
}


void StyleSettingWindow::on_rebootBox_clicked()
{
    SReboot();
}


void StyleSettingWindow::on_resizeBox_clicked()
{
    resizeForWithDialog((SBlockLayout*)(activepmw->inside));
}


void StyleSettingWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(item2ind.contains(current)) {
        ui->stackedWidget->setCurrentIndex(item2ind[current]);
        ind2widget[item2ind[current]]->raise(item2fields[current].mid(0, item2fields[current].length() - 1));
    }
}
