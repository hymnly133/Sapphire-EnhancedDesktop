#include "stylesettotal.h"
#include "global.h"
#include "iconfunc.h"
#include "ui_stylesettotal.h"

styleSetTotal::styleSetTotal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::styleSetTotal)
{
    ui->setupUi(this);

    connect(ui->fontChangeBox, &QPushButton::clicked, this, &styleSetTotal::on_fontChangeBox_clicked);
    connect(ui->resizeBox, &QPushButton::clicked, this, [=](bool val){
        if(val){
            emit on_resizeBox_clicked();
        }
    });
}

styleSetTotal::~styleSetTotal()
{
    delete ui;
}

QString styleSetTotal::getFontName()
{
    return ui->fontComboBox->currentFont().family();
}





void styleSetTotal::on_ButtonLoadCustomIcons_pressed()
{
    pir->applyForAllSFile();
}


void styleSetTotal::on_ButtonRecoverDefaultIcons_pressed()
{
    pir->recoverForDefault();
}



