#ifndef ORDERCONTROLLER_H
#define ORDERCONTROLLER_H
#include <QObject>
#include <QString>
class OrderController : public QObject
{
    Q_OBJECT
public:
    OrderController(QObject *parent = 0);
public:
    void getlistOrderInTable(int table_it);
signals:
    void doneGetListOrderInTable(QString data);
private:
    QString _urlOrder;
    QString merchant_code;
};

#endif // ORDERCONTROLLER_H
