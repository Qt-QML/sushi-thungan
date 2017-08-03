#ifndef APPSETTING_H
#define APPSETTING_H

#include <QObject>
#include <QDebug>

class QSettings;
class AppSetting : public QObject
{
    Q_OBJECT
public:
    static AppSetting* getInstance(){
      if (Q_NULLPTR == _instance) {
          _instance = new AppSetting();
      }
      return _instance;
    }

    explicit AppSetting(QObject *parent = 0);
    ~AppSetting();

    void load();
    Q_INVOKABLE void save();
    Q_INVOKABLE QString serverUrl() const;
    Q_INVOKABLE void setServerUrl(const QString &serverUrl);

    Q_INVOKABLE QString wsUrl() const;
    Q_INVOKABLE void setWsUrl(const QString &wsUrl);

    Q_INVOKABLE QString description() const;
    Q_INVOKABLE void setDescription(const QString &description);

    Q_INVOKABLE QString merchantCode() const;
    Q_INVOKABLE void setMerchantCode(const QString &description);

    Q_INVOKABLE QString machineName() const;
    Q_INVOKABLE void setMachineName(const QString &machine_name);
signals:

public slots:

private:
    static AppSetting* _instance;
    QSettings* _settings;
    QString _serverUrl;
    QString _wsUrl;
    QString _description;
    QString _merchant_cd;
    QString _machine_name;
};

#endif // APPSETTING_H
