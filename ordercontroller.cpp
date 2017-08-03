#include "ordercontroller.h"
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

OrderController::OrderController(QObject *parent): QObject(parent)
{
    _urlOrder = "https://gbcstaging.zing.vn/vpos/api/invoice/?cm=get_order_list&dt=";
    merchant_code = "sushi";
}

void OrderController::getlistOrderInTable(int table_it) {
    QJsonObject data;
    data["table_id"] = table_it;
    data["merchant_code"] = merchant_code;
    qDebug() << "order url: " << _urlOrder + QJsonDocument(data).toJson();
    QNetworkReply *reply = Utils::getInstance()->syncGet(_urlOrder + QJsonDocument(data).toJson());

    if (Q_NULLPTR == reply) {
        qWarning() << "order information null";
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "order request success";
        QString response = QString(reply->readAll());
        qDebug() << "order info data: " << response;
        emit doneGetListOrderInTable(response);
    } else {
        qDebug() << "Have error while get order info " << reply->errorString();
    }

    delete reply;
}
