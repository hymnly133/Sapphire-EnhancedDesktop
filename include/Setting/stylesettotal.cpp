#include "stylesettotal.h"
#include "ui_stylesettotal.h"

styleSetTotal::styleSetTotal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::styleSetTotal)
{
    ui->setupUi(this);

    connect(ui->fontChangeBox, &QPushButton::clicked, this, &styleSetTotal::on_fontChangeBox_clicked);
    connect(ui->rebootBox, &QPushButton::clicked, this, &styleSetTotal::on_rebootBox_clicked);
    connect(ui->resizeBox, &QPushButton::clicked, this, &styleSetTotal::on_resizeBox_clicked);
}

styleSetTotal::~styleSetTotal()
{
    delete ui;
}

QString styleSetTotal::getFontName()
{
    return ui->fontComboBox->currentFont().family();
}




