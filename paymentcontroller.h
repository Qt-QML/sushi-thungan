#ifndef PAYMENTCONTROLLER_H
#define PAYMENTCONTROLLER_H

#include <QString>
#include <QObject>

class PaymentController: public QObject
{
    Q_OBJECT
public:
    PaymentController(QObject *parent = 0);
    void pay(int table_id, int staff_id, const QString invoice_id, long long amount);
signals:
    void donePay(QString data);
private:
    QString _urlPayment;
    QString merchant_code;
};

#endif // PAYMENTCONTROLLER_H
