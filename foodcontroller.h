#ifndef FOODCONTROLLER_H
#define FOODCONTROLLER_H

#include <QObject>
#include <QString>
class FoodController : public QObject
{
    Q_OBJECT
public:
    FoodController(QObject *parent = 0);
public:
    void getFoods();
signals:
    void doneGetFoods(QString data);
private:
    QString _urlFood;
    QString merchant_code;
};

#endif // FOODCONTROLLER_H
