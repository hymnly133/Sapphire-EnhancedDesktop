#ifndef SINPUTDIALOG_H
#define SINPUTDIALOG_H

#include "SAnimationRect.h"
#include "qlabel.h"
#include "roundshower.h"
#include"SLinerAnimation.h"
#include <QDialog>
#include <QWidget>

class SInputDialog : public QDialog
{
    Q_OBJECT
public:
    SInputDialog(QWidget* parent);
    QVBoxLayout* layout;
    roundShower* rs;
    SAnimationRect* arect;
    QPoint previousPos;
    QString info = "TestInfo";
    QLineEdit* lineEdit;
    QLabel* infoLable;
    int radius=0;
    void setInfo(QString info);
    QPalette* pe;
    QSize textSize;

    void end();
    static  SInputDialog* showInput(QString info,QString defaultString = "", QPoint pos = QPoint(-1,-1) ,QWidget* parent = nullptr);


public: signals:
    void finalText(QString);
    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;



    // QWidget interface
protected:


    // QWidget interface
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
};



#endif // SINPUTDIALOG_H
