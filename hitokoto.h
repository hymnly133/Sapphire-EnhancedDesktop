#ifndef HITOKOTO_H
#define HITOKOTO_H


#include<QWidget>
#include"ed_unit.h"
#include<QLabel>
#include<QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>


class Hitokoto : public ED_Unit
{
    Q_OBJECT
public:
    explicit Hitokoto(QWidget *parent = nullptr,int sizex=1,int sizey=1);
    QLabel *hitokotolabel;
    QNetworkAccessManager *manager;
    QVBoxLayout *wlayout;
    void gethitokotoinfo();
    void parsejson(QByteArray& byteArray);
    void handlehitokotoresponse(QNetworkReply *reply);

};
Q_DECLARE_METATYPE(Hitokoto)
#endif // HITOKOTO_H
