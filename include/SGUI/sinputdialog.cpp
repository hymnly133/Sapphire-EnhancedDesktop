#include "sinputdialog.h"
#include"QWidget"
#include "SysFunctions.h"
#include "mainwindow.h"
#include "qevent.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qmenu.h"
#include "qpainter.h"




 SInputDialog::SInputDialog(QWidget *parent):QDialog(parent){
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags()  | Qt::FramelessWindowHint| Qt::NoDropShadowWindowHint );
    pe = new QPalette();
    layout = new QVBoxLayout();
    infoLable = new QLabel(this);
    rs = new roundShower(this);
    arect = new SAnimationRect(this);
    lineEdit = new QLineEdit(this);

    layout->addWidget(infoLable);
    layout->addWidget(lineEdit);
    setLayout(layout);


    // rs->distriRadius(&arect->nowRadius);
    rs->aliment = roundShower::Default;
    rs->distri(&arect->nowSize,&arect->nowRadius);
    rs->raise();
    connect(arect,&SAnimationRect::whenEndAnimationEnd,this,[=]{
        deleteLater();
    });
    connect(arect,&SAnimationRect::animationUpdating,this,[=](QPoint pos,QSize size,int,int){
        move(previousPos+pos);
        QColor tem = winThemeColor();
        tem.setAlpha(arect->nowAlpha);
        rs->raise();

        pe->setColor(QPalette::Window,tem);
        setMinimumSize(size);
        update();
    });

    connect(lineEdit,&QLineEdit::returnPressed,this,[=](){
        end();
    });
 }

 void SInputDialog::setInfo(QString info)
 {
     infoLable->setText("->");
     this->info = info;
     QFontMetrics fm(infoLable->font());
    textSize = fm.boundingRect(info).size();
     pe->setColor(QPalette::Window,Qt::transparent);
     pe->setColor(QPalette::WindowText,Qt::black);
     infoLable->setPalette(*pe);
     infoLable->setVisible(true);
 }

 void SInputDialog::end()
 {
     emit finalText(lineEdit->text());
     arect->stop();
     arect->setEndValue(QPoint(0,0),
                        QSize(1,size().height()),
                        0,0);
     arect->setFinal();
     arect->start();
     lineEdit->setText("");
 }

 SInputDialog* SInputDialog::showInput(QString info,QString defaultString, QPoint pos ,QWidget* parent)
 {
     SInputDialog* sip;
     if(parent ==nullptr)
         sip = new SInputDialog(activepmw->pls);
     else
         sip = new SInputDialog(parent);
     if(pos == QPoint(-1,-1)){
        sip->move(QCursor::pos());
    }
     else{
         sip->move(pos);
     }
    sip->setInfo(info);
    sip->lineEdit->setText(defaultString);
    int dotInd = defaultString.lastIndexOf(".");
    if(defaultString.lastIndexOf(".")!=-1){
        //如果是文件名形式的
        qDebug()<<dotInd;
        sip->lineEdit->setSelection(0,dotInd);
    }
    // sip->lineEdit->setSelection(1,2);
    sip->show();
    return sip;
}

 void SInputDialog::resizeEvent(QResizeEvent *event)
 {
     rs->updateDisplay();
 }

 void SInputDialog::showEvent(QShowEvent *event)
 {
     qDebug()<<"ShowSize"<<size();
     qDebug()<<"ShowPos"<<pos();
     previousPos = pos();
     arect->setStartValue(QPoint(0,0),
                          QSize(1,size().height()),
                          0,0);

     arect->setEndValue(QPoint(0,0),
                        QSize(size().width(),size().height()),
                        240,10);


     arect->start();

     setFixedSize(1,1);
     setVisible(true);
     show();
     QDialog::showEvent(event);
 }

 void SInputDialog::paintEvent(QPaintEvent *event)
 {
     QColor tem = winThemeColor();
     // qDebug()<<mapToGlobal(QPoint(0,0));
     QPainter painter(this);
     tem.setAlpha(arect->nowAlpha);
     painter.setBrush(tem);
     painter.setPen(Qt::NoPen);
     painter.drawRect(rect());

     tem = QColor("black");
     tem.setAlpha(arect->nowAlpha);
     painter.setPen(tem);
     QRect textRect = QRect(infoLable->geometry().topLeft()+QPoint(20,0),QSize(textSize.width(),infoLable->height()));
     painter.drawText(textRect,Qt::AlignCenter,info);

 }

 void SInputDialog::keyPressEvent(QKeyEvent *event)
 {
     if(event->key()==Qt::Key_Enter){
         close();
     }
 }

 bool SInputDialog::nativeEvent(const QByteArray &eventType, void *message, long *result)
 {
     MSG *msg = static_cast< MSG * >( message );
     // TODO: filter out or modify msg struct here
     // ...
     if(msg->message == WM_NCACTIVATE)
     {
         bool active = (bool)(msg->wParam);
         if (msg->wParam)
         {
             qDebug() << "active:" << true;
         }
         else
         {
             qDebug() << "active:" << false;
         }
     }

     return false;
 }

