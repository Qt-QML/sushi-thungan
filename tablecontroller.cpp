#include "tablecontroller.h"
#include "utils.h"
#include "commonfunction.h"
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

TableController::TableController(QObject * parent) : QObject(parent)
{
    _urlTable = "https://gbcstaging.zing.vn/vpos/api/table?cm=getlist&dt=";
    merchant_code = "sushi";
}

void TableController::init() {
    QJsonObject data;
    data["merchant_code"] = merchant_code;
    _urlTable = _urlTable + QJsonDocument(data).toJson();
    QNetworkReply *reply = Utils::getInstance()->syncGet(_urlTable);

    if (Q_NULLPTR == reply) {
        qWarning() << "areas/tables information null";
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        QString response = QString(reply->readAll());
        qDebug() << "areas/tables request success: " << response;
        emit areas_tables(response);
    } else {
        qDebug() << "Have error while get areas/tables info " << reply->errorString();
    }

    delete reply;
}
