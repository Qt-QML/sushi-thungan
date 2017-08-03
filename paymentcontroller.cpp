#include "paymentcontroller.h"
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

PaymentController::PaymentController(QObject *parent): QObject(parent)
{
    _urlPayment = "https://gbcstaging.zing.vn/vpos/api/invoice?cm=payment";
    merchant_code = "sushi";
}
void PaymentController::pay(int table_id, int staff_id, const QString invoice_id, long long amount) {
    QJsonObject data;
    data["merchant_code"] = merchant_code;
    data["table_id"] = table_id;
    data["staff_id"] = staff_id;
    data["invoice_id"] = invoice_id;
    data["amount"] = amount;
    QString url;
    url = _urlPayment + "&dt=" + QJsonDocument(data).toJson();
    qDebug() << "pay url: " << url;
    QNetworkReply *reply = Utils::getInstance()->syncGet(url);

    if (Q_NULLPTR == reply) {
        qWarning() << "pay information null";
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        QString response = QString(reply->readAll());
        qDebug() << "pay info data: " << response;
        emit donePay(response);
    } else {
        qDebug() << "Have error while get pay info " << reply->errorString();
    }
    delete reply;
}
