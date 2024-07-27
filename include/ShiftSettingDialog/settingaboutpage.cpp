#include "settingaboutpage.h"
#include "ui_settingaboutpage.h"

settingAboutPage::settingAboutPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settingAboutPage)
{
    ui->setupUi(this);
}

settingAboutPage::~settingAboutPage()
{
    delete ui;
}

void settingAboutPage::setVersion(const QString& version)
{
    ui->label_version->setText(version);

}
