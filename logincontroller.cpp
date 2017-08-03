#include "logincontroller.h"
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

LoginController::LoginController(QObject *parent): QObject(parent)
{
    _urlLogin = "https://gbcstaging.zing.vn/vpos/api/login?cm=cashier_login";
    merchant_code = "sushi";
}

void LoginController::login(const QString &u, const QString &p) {
    QJsonObject data;
    data["merchant_code"] = merchant_code;
    data["u"] = u;
    data["p"] = p;
    _urlLogin = _urlLogin + "&dt=" + QJsonDocument(data).toJson();
    qDebug() << "login url: " << _urlLogin;
    QNetworkReply *reply = Utils::getInstance()->syncGet(_urlLogin);

    if (Q_NULLPTR == reply) {
        qWarning() << "login information null";
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "login request success";
        QString response = QString(reply->readAll());
        qDebug() << "login info data: " << response;
        emit donelogin(response);
    } else {
        qDebug() << "Have error while get login info " << reply->errorString();
    }

    delete reply;
}



