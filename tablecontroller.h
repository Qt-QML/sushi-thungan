#ifndef TABLECONTROLLER_H
#define TABLECONTROLLER_H
#include <QObject>
#include <QString>

class TableController : public QObject
{
    Q_OBJECT
public:
    TableController(QObject *parent = 0);
    void init();
signals:
    void areas_tables(QString data);
private:
    QString _urlTable;
    QString merchant_code;
};

#endif // TABLECONTROLLER_H
