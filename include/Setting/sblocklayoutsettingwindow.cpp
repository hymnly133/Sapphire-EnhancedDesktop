#include "sblocklayoutsettingwindow.h"



SBlockLayoutSettingWindow::SBlockLayoutSettingWindow(SBlockLayout *aimlayout):QDialog(nullptr),ui(new Ui::BlockLayoutSettingDialog)
{
    this->aimlayout = aimlayout;
    ui->setupUi(this);
    ui->rowSlider->setValue(aimlayout->row);
    ui->colSlider->setValue(aimlayout->col);
    ui->boradXRSlider->setValue(aimlayout->boradXR*100);
    ui->boradYRSlider->setValue(aimlayout->boradYR*100);
    ui->spaceXRSlider->setValue(aimlayout->spaceXR*100);
    ui->spaceYRSlider->setValue(aimlayout->spaceYR*100);
    resize(1000,500);
}

void SBlockLayoutSettingWindow::on_pushButton_apply_clicked()
{
    aimlayout->resize(ui->rowSlider->value()
                    ,ui->colSlider->value()
                    ,ui->boradXRSlider->value()/100.0
                    ,ui->boradYRSlider->value()/100.0
                    ,ui->spaceXRSlider->value()/100.0
                    ,ui->spaceYRSlider->value()/100.0
                    );
}


void SBlockLayoutSettingWindow::on_pushButton_cancel_clicked()
{
    close();
    deleteLater();
}


void SBlockLayoutSettingWindow::on_rowSlider_valueChanged(int value)
{
    ui->rowDisplay->setText(QString::number(value));
}


void SBlockLayoutSettingWindow::on_boradXRSlider_valueChanged(int value)
{
    double fixed = value/100.0;
    ui->boradXRDispaly->setText(QString::number(fixed));
}


void SBlockLayoutSettingWindow::on_spaceXRSlider_valueChanged(int value)
{

    double fixed = value/100.0;
    ui->spaceXRDisplay->setText(QString::number(fixed));
}


void SBlockLayoutSettingWindow::on_colSlider_valueChanged(int value)
{
    ui->colDisplay->setText(QString::number(value));

}


void SBlockLayoutSettingWindow::on_boradYRSlider_valueChanged(int value)
{
    double fixed = value/100.0;
    ui->boradYRDisplay->setText(QString::number(fixed));
}


void SBlockLayoutSettingWindow::on_spaceYRSlider_valueChanged(int value)
{
    double fixed = value/100.0;
    ui->spaceYRDisplay->setText(QString::number(fixed));
}

