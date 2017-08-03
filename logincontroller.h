#ifndef LOGIN_H
#define LOGIN_H
#include <QString>
#include <QObject>

class LoginController: public QObject
{
    Q_OBJECT
public:
    LoginController(QObject *parent = 0);
    void login(const QString &u, const QString &p);
signals:
    void donelogin(QString data);
private:
    QString _urlLogin;
    QString merchant_code;
};

#endif // LOGIN_H
