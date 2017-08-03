#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QVariant>
#include "downloadfileman.h"
#include "http/httpbase.h"
#include "http/httpbase2.h"
#include "billdetail.h"
#include "../common/wscssclient.h"
#include "wsclient.h"

#define PAY_ZALO 1
#define PAY_CARD 2
#define PAY_CASH 4

#define LOGIN_NONE          0
#define LOGIN_WAITLOGIN     1
#define LOGIN_READY         2
#define BAOCAO_WAITING      3
#define BAOCAO_READY        4
#define FUPDATE_NONE        0
#define FUPDATE_PROG1       1
#define FUPDATE_PROG2       2
#define FUPDATE_BUSY        (FUPDATE_NONE + FUPDATE_PROG1 + FUPDATE_PROG2)
#define FOOD_SEARCHING      4

class WSClient;
class LoginController;
class TableController;
class OrderController;
class FoodController;
class PaymentController;
class ConfigSetting;
class DeviceClientService;
class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = 0);
    ~MainController();
    Q_INVOKABLE int initialize();
    Q_INVOKABLE void uninitialize();
public:
    Q_INVOKABLE void login(const QString &u, const QString &p);
    Q_INVOKABLE void printbill(QJsonObject jbinfo, QJsonObject jbills,
                               int prnId, const QString& prnName);
    Q_INVOKABLE void getOrder(int table_id);
    Q_INVOKABLE QString moneyString(long long moneyValue);
    Q_INVOKABLE long long moneyNumber(const QString &moneyString);
    Q_INVOKABLE QString pay(const QString& amount, const QJsonArray &foods, int payMethod);
    Q_INVOKABLE void sendCancelBillUI();
    Q_INVOKABLE void cancelBill(const QString& foId, const QString &invcecode, const QString& total);
    Q_INVOKABLE void pay(int table_id, int staff_id, const QString invoice_id, long amount);
signals:
    //login
    void    loginConnection(const QString &data);
    //areas, tables
    void    area_tableConnection(const QString &data);
    //order
    void    getListOrderInTable(const QString &data);
    //foods
    void    getFoodsConnection(const QString &data);
    //pay
    void    getPayConnection(const QString &data);
    void    wsConnection(const QString &data);

    void errorMessage(int err, const QString &msg);
    void cancelBillItem();
    void cancellBillCloseQRDialog();
    void paySucc(const QString& invcecode, const QString& qrcode, int discount, const QString& oripay, const QString& discpay);
    void payError(const QString& msg);
    void notifySucc(int errcode, const QString& invcecode, const QString& invcetime, const QString& barcode, const QString &balance);
    void cloudConnectionChange(bool connected);
public slots:
    //login
    void    doneLoginRequet(QString data);
    //area, table
    void    areas_tables(QString data);
    //order
    void    doneOrderInTable(QString data);
    //foods
    void    doneGetFoods(QString data);
    //pay
    void    donePay(QString data);
    void    onUpdatedFoodsData(QString data);

    bool doGetPmsid(const QString &merchantCode, const QString& devId, const QString& biId, int payMethod);
    void doPayZalo(const QString &sesId, const QString &biId);
    void doPayCash(const QString &sesId, const QString &biId);
    void onPmsidDoneZalo(const QVariant &data);
    void onPmsidDoneCash(const QVariant &data);
    void onPmsidError(const int error, const QString &msg);
    void onPayZaloDone(const QVariant &data);
    void onPayZaloError(const int &error, const QString &msg);
    void onPayCashDone(const QVariant &data);
    void onPayCashError(const int &error, const QString &message);

    void    initialWsCssClient();
    void    initialWebSocket();
    void    onNotify(const QString &message);
    void    onNotifyWS(const QString &message);
    void    onCenterServiceFound(const QString &ip, const int &port);
public:
    QString paymentUrl;
    QString centerService;
    QString wsTranspro;     // ws / wss
    QString appTitle;       // "Canteen VNG"
    QString merchantCode;   // "vpos"
    QString machineName;    // "May-1"
    bool    cardWaitingPay; // đang chờ thanh toán ...
private:
    WsCssClient         *wscssClient;
    WSClient            *wsClient;
    LoginController     *loginCtr;
    TableController     *tableCtr;
    OrderController     *orderCtr;
    FoodController      *foodCtr;
    PaymentController   *payCtr;
    CDownloadFileMan    m_DFMan;
    QString             m_invce;
    qulonglong          m_qullLUT;
    QString             deviceId;
    QString             titleEng;
    QString             appMachine;
private:
    QString cmd_appinvce;
    QString cmd_token;
    QString cmd_getsession;
private:
    HttpBase2*          http2;
    CBillDetail m_Bills;
};

#endif // MAINCONTROLLER_H
