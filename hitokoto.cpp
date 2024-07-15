#include "hitokoto.h"
#include "QWidget"
#include"QProcess"
#include "qboxlayout.h"
#include "qlabel.h"
#include"QDebug"
#include"QUrl"
#include <QJsonDocument>
#include <QJsonObject>
#include "QtNetwork/QHttpMultiPart"
#include "QHttpMultiPart"
#include <QMessageBox>
#include <QUrlQuery>
#include"QDesktopServices"
#include"QTextCodec"
#include"QGraphicsDropShadowEffect"

Hitokoto::Hitokoto(QWidget *parent,int sizex,int sizey)
    : ED_Unit(parent,sizex,sizey)
{
    wlayout=new QVBoxLayout(this);
    manager=new QNetworkAccessManager(this);
    hitokotolabel=new QLabel(this);
    hitokotolabel->setWordWrap(true);
    wlayout->addWidget(hitokotolabel, 0, Qt::AlignHCenter);
    connect(manager,&QNetworkAccessManager::finished,this,&Hitokoto::handlehitokotoresponse);
    gethitokotoinfo();
}

void Hitokoto::gethitokotoinfo()
{
    QUrl url("https://hitokoto.cn/api/restful/v1/hitokoto/62c12303-b3fa-4720-a7c5-2985bf049e60?token=xxx");
    manager->get(QNetworkRequest(url));
}

void Hitokoto::handlehitokotoresponse(QNetworkReply *reply)
{
    int statuscode=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<statuscode;
    QByteArray byteArray=reply->readAll();
    qDebug()<<byteArray.data();
    parsejson(byteArray);
    if(statuscode==0)
        hitokotolabel->setText("让子弹飞一会");
}

void Hitokoto::parsejson(QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument doc=QJsonDocument::fromJson(byteArray,&err);
    QJsonObject rootobj=doc.object();
    QJsonObject dataobj=rootobj.value("data").toObject();
    hitokotolabel->setText(rootobj.value("data").toObject().value("hitokoto").toString());

}
