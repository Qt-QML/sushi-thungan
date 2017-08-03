#include "foodcontroller.h"
#include "utils.h"
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QTimerEvent>
#include <QDebug>

FoodController::FoodController(QObject *parent): QObject(parent)
{
    _urlFood = "https://gbcstaging.zing.vn/vpos/api/item?cm=getlist&dt=";
    merchant_code = "sushi";
}

void FoodController::getFoods() {
    QJsonObject data;
    QString url;
    data["merchant_code"] = merchant_code;
    url = _urlFood + QJsonDocument(data).toJson();
    qDebug() << "get food url: " << url;
    QNetworkReply *reply = Utils::getInstance()->syncGet(url);

    if (Q_NULLPTR == reply) {
        qWarning() << "get food information null";
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "get food request success";
        QString response = QString(reply->readAll());
        qDebug() << "get food info data: " << response;
        emit doneGetFoods(response);
    } else {
        qDebug() << "Have error while get food info " << reply->errorString();
    }

    delete reply;
}
