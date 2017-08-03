#include "appsetting.h"
#include <QSettings>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


AppSetting* AppSetting::_instance = Q_NULLPTR;

AppSetting::AppSetting(QObject *parent) : QObject(parent)
{
    _settings = new QSettings();
    load();
}

AppSetting::~AppSetting()
{
    delete _settings;
}

void AppSetting::load()
{
    _serverUrl = _settings->value("settings/serverurl", "https://gbcstaging.zing.vn/vpos").toString();
    _wsUrl = _settings->value("settings/wsurl", "wss://gbcstaging.zing.vn/vpos").toString();
    _description = _settings->value("settings/description", "").toString();
    _merchant_cd = _settings->value("settings/merchantcode", "").toString();
    _machine_name = _settings->value("settings/machinename", "").toString();
}

void AppSetting::save()
{
    _settings->setValue("settings/serverurl", _serverUrl);
    _settings->setValue("settings/wsurl", _wsUrl);
    _settings->setValue("settings/description", _description);
    _settings->setValue("settings/merchantcode", _merchant_cd);
    _settings->setValue("settings/machinename", _machine_name);
    _settings->sync();
}

QString AppSetting::serverUrl() const
{
    return _serverUrl;
}

void AppSetting::setServerUrl(const QString &serverUrl)
{
    _serverUrl = serverUrl;
}

QString AppSetting::wsUrl() const
{
    return _wsUrl;
}

void AppSetting::setWsUrl(const QString &wsUrl)
{
    _wsUrl = wsUrl;
}

QString AppSetting::description() const
{
    return _description;
}

void AppSetting::setDescription(const QString &decription)
{
    _description = decription;
}

QString AppSetting::merchantCode() const
{
    return _merchant_cd;
}

void AppSetting::setMerchantCode(const QString &merchant_cd)
{
    _merchant_cd = merchant_cd;
}

QString AppSetting::machineName() const
{
    return _machine_name;
}

void AppSetting::setMachineName(const QString &machine_name)
{
    _machine_name = machine_name;
}
