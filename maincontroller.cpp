#include "maincontroller.h"
#include "wsclient.h"
#include "appsetting.h"
#include "utils.h"
#include "logincontroller.h"
#include "tablecontroller.h"
#include "ordercontroller.h"
#include "commonfunction.h"
#include "foodcontroller.h"
#include "paymentcontroller.h"
#include "../common/prntdataformat.h"
#include "cmn.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <QTimerEvent>
#include <QDebug>
#include <QUuid>
#include <QVariant>

#define DT_FORMAT           "yyyy-MM-dd HH:mm:ss.z"
#define SL_WEBSOCKET        "wss://gbcstaging.zing.vn/vpos/ntf/"
#define MAX_EVENT_VIEW      2

MainController::MainController(QObject *parent) :
    QObject(parent),
    m_invce("")
{
    m_qullLUT = 0;
    cmd_appinvce        = QString("/api/invoice/");
    cmd_getsession      = QString("get_session");
    cmd_token = CommonFunction::gVPOS1();
    merchantCode        = AppSetting::getInstance()->merchantCode();
    paymentUrl          = AppSetting::getInstance()->serverUrl();
    titleEng            = AppSetting::getInstance()->description();
    deviceId            = QString(QUuid::createUuid().toRfc4122().toHex());
    appMachine          = QString("");
    machineName         = AppSetting::getInstance()->machineName();
    http2 = NULL;

    initialWsCssClient();
    wsClient = 0;
    initialWebSocket();
}

MainController::~MainController()
{
    if (wsClient)
    {
        wsClient->stop();
        wsClient->wait();
    }
}

void MainController::onNotify(const QString &message)
{
    qDebug() << "onNotify:" << message;
}

void MainController::onNotifyWS(const QString &message)
{
    qDebug() << "onNotify ws:" << message;
    emit wsConnection(message);
}

void MainController::onCenterServiceFound(const QString &ip, const int &port)
{
    centerService = QString("%1:%2").arg(ip).arg(port);
}

void MainController::initialWsCssClient()
{
    if( wscssClient ) {
        delete wscssClient;
        wscssClient = NULL;
    }

    wscssClient = new WsCssClient( 2, 4 );
    QObject::connect(wscssClient, SIGNAL(textMessageReceived(QString)), this, SLOT(onNotify(QString)));
    QObject::connect(wscssClient, SIGNAL(foundServer(QString,int)), this, SLOT(onCenterServiceFound(QString,int)));
}

void MainController::initialWebSocket()
{
    qDebug() << "initialWebSocket ...";
    if( wsClient )
    {
        wsClient->stop();
        wsClient->wait(10000);

        delete wsClient;
        wsClient = 0;
    }

    if (wsClient == 0)
    {
        QUrl wsUrl = QUrl( AppSetting::getInstance()->wsUrl() + "/ntf/");
        QUrl httpUrl = QUrl( AppSetting::getInstance()->serverUrl() + "/api/common/");

        wsClient = new WSClient(wsUrl, httpUrl, merchantCode, deviceId, true, 0);

        QObject::connect(wsClient, SIGNAL(textMessageReceived(QString)), this, SLOT(onNotifyWS(const QString&)));
        QObject::connect(wsClient, &WSClient::connected, this, [=](){emit cloudConnectionChange(true);});
        QObject::connect(wsClient, &WSClient::closed, this, [=](){emit cloudConnectionChange(false);});
        wsClient->start();
    }
}


int MainController::initialize()
{
    //login
    loginCtr = new LoginController();
    QObject::connect(loginCtr, SIGNAL(donelogin(QString)),    this, SLOT(doneLoginRequet(QString)));
    //get list area, table
    tableCtr = new TableController();
    QObject::connect(tableCtr, SIGNAL(areas_tables(QString)),    this, SLOT(areas_tables(QString)));
    tableCtr->init();
    //order
    orderCtr = new OrderController();
    QObject::connect(orderCtr, SIGNAL(doneGetListOrderInTable(QString)),    this, SLOT(doneOrderInTable(QString)));
    //food
    foodCtr = new FoodController();
    QObject::connect(foodCtr, SIGNAL(doneGetFoods(QString)),    this, SLOT(doneGetFoods(QString)));
    foodCtr->getFoods();
    //pay
    payCtr = new PaymentController();
    QObject::connect(payCtr, SIGNAL(donePay(QString)),    this, SLOT(donePay(QString)));
}


void MainController::uninitialize()
{
}

void MainController::login(const QString &u, const QString &p)
{
    qDebug() << "login -  user: " << u << ", pass: " << p;
    loginCtr->login(u,p);
}

void MainController::pay(int table_id, int staff_id, const QString invoice_id, long amount) {
    payCtr->pay(table_id, staff_id, invoice_id, amount);
}

void MainController::getOrder(int table_id) {
    orderCtr->getlistOrderInTable(table_id);
}

void MainController::doneLoginRequet(QString data) {
    qDebug() << "login data: " << data;
    emit loginConnection(data);
}

void MainController::doneOrderInTable(QString data) {
    qDebug() << "order data: " << data;
    emit getListOrderInTable(data);
}

void MainController::areas_tables(QString data) {
    qDebug() << "area-table data: " << data;
    emit area_tableConnection(data);
}

void MainController::doneGetFoods(QString data) {
    qDebug() << "foods data: " << data;
    CommonFunction::parseDataFoods(data, m_DFMan, m_qullLUT);
    QObject::connect(&m_DFMan, SIGNAL(NotifyComplete(QString)), this, SLOT(onUpdatedFoodsData(QString)), Qt::UniqueConnection);
    m_DFMan.SetData(data);
    m_DFMan.StartDownload();
}

void MainController::donePay(QString data) {
    qDebug() << "foods data: " << data;
    emit getPayConnection(data);
}

void MainController::onUpdatedFoodsData(QString data) {
    QString dataUpdated = CommonFunction::updateDataFoods(data, m_DFMan);
    emit getFoodsConnection(dataUpdated);
}

QString MainController::moneyString(long long moneyValue)
{
    qDebug() << "moneyString: " << QString::number(moneyValue);
    long long absVal = abs(moneyValue);
    QString money = QString::number(absVal);
    int loop = money.length() / 3;
    if (loop > 0)
    {
        if (money.length() % 3 == 0)
        {
            loop = loop - 1;
        }

        int index = money.length();
        for (int i = 0; i < loop ; i++)
        {
            index = index - 3;
            money.insert(index,  QString(","));
        }
    }

    if (moneyValue < 0)
        money.insert(0,  QString("-"));

    return money;
}

long long MainController::moneyNumber(const  QString &moneyString)
{
    QString money = moneyString;
    money = money.replace(QString(","),QString::null);
    if (money.isEmpty())
    {
        return 0;
    }
    else
    {
        return money.toLongLong();
    }
}

// send msg pay to payment server
QString MainController::pay(const QString& amount, const QJsonArray &foods, int payMethod)
{

    BillInfo bi;
    bi.Create(amount, foods);

    // get session id from server ...
    if (doGetPmsid(merchantCode, deviceId, bi.id, payMethod))
    {
        m_Bills.addBill(bi);
        return bi.id;
    }

    return QString("");
}

bool MainController::doGetPmsid(const QString &merchantCode, const QString &devId, const QString& biId, int payMethod) {
    if (http2 == NULL)
    {
        http2 = new HttpBase2(QString(paymentUrl + cmd_appinvce), this);
        if (payMethod == PAY_ZALO) {
            QObject::connect(http2, SIGNAL(done(QVariant)), this, SLOT(onPmsidDoneZalo(QVariant)) );
        }
        else if (payMethod == PAY_CASH) {
            QObject::connect(http2, SIGNAL(done(QVariant)), this, SLOT(onPmsidDoneCash(QVariant)) );
        }

        QObject::connect(http2, SIGNAL(error(int,QString)), this, SLOT(onPmsidError(int, QString)));

        http2->addParameter("cm", cmd_getsession, true);
        http2->addParameter("dt", CommonFunction::formatSession(merchantCode, devId, biId), false);
        http2->process(POST);

        return true;
    }
    return false;
}

void MainController::onPmsidDoneZalo(const QVariant &data)
{
    QString msg, sesId, biId;
    int err = CommonFunction::parsePmsid(data.toString(), &msg, &sesId, &biId);

    if (err == 0) {
        QObject::disconnect(http2, SIGNAL(done(QVariant)), this, SLOT(onPmsidDoneZalo(QVariant)));
        QObject::disconnect(http2, SIGNAL(error(int,QString)), this, SLOT(onPmsidError(int, QString)));

        doPayZalo(sesId, biId);
    }
    else {
        emit errorMessage(err, msg);

        http2->deleteLater();
        http2 = NULL;
    }
}

void MainController::onPmsidDoneCash(const QVariant &data)
{
    QString msg, sesId, biId;
    int err = CommonFunction::parsePmsid(data.toString(), &msg, &sesId, &biId);

    if (err == 0) {
        QObject::disconnect(http2, SIGNAL(done(QVariant)), this, SLOT(onPmsidDoneCash(QVariant)));
        QObject::disconnect(http2, SIGNAL(error(int,QString)), this, SLOT(onPmsidError(int, QString)));

        doPayCash(sesId, biId);
    }
    else {
        emit errorMessage(err, msg);

        http2->deleteLater();
        http2 = NULL;
    }
}

void MainController::onPmsidError(const int error, const QString &msg)
{
    qDebug() << "pmsid error:" << error << ", msg:" << msg;

    emit cancellBillCloseQRDialog();
    emit cancelBillItem();
    emit errorMessage(error, msg);

    http2->deleteLater();
    http2 = NULL;
}

void MainController::doPayZalo(const QString &sesId, const QString &biId)
{
    BillInfo bi;
    if( m_Bills.getById(biId, bi, sesId) )
    {
        QString dt = CommonFunction::formatRequestCreateInvoice(
                    merchantCode, machineName, bi.getAmount(),
                    deviceId, bi.id, bi.foods, titleEng, PAY_ZALO);

        QString token = CommonFunction::HMacSha256HexStringEncode(QString("%1%2")
                                                  .arg(dt)
                                                  .arg(sesId), cmd_token);
        m_Bills.setToken(biId, token);

        QObject::connect(http2, SIGNAL(done(QVariant)), this, SLOT(onPayZaloDone(QVariant)), Qt::UniqueConnection);
        QObject::connect(http2, SIGNAL(error(int,QString)), this, SLOT(onPayZaloError(int, QString)), Qt::UniqueConnection);

        QString dt2 = QString::fromUtf8(QUrl::toPercentEncoding(dt).data());
        http2->addParameter("cm", "create", true);
        http2->addParameter("tk", token);
        http2->addParameter("dt", dt2);
        http2->process(POST);
    }
}

void MainController::onPayZaloDone(const QVariant &data)
{
    QString qrcode;
    quint16 appId;
    QString token, msg;
    QString invceCode, invceSess, invceDate;
    QString oripay, discpay;
    int discount;

    if (CommonFunction::parseResponseZalopay(data.toString(), msg,
                qrcode, &token, &appId,
                &invceCode, &invceSess, &invceDate,
                &oripay, &discount, &discpay) == 0)
    {
        BillInfo bi;
        if (m_Bills.getBySessId(invceSess, bi, invceCode) == true)
        {
            //qDebug() << "found the foID ...";

            QString qrPicture = CommonFunction::generateQRCode(token, appId);

            // (QR responed) send QR & ID to GUI
            emit paySucc(invceCode, qrPicture, discount, oripay, discpay);
//            if (deviceService->isConnected()) {
//                deviceService->startReaderDevice(CommonFunction::parseDataToRemoteReaderDevice(bi.getAmount(), appId, token));
//            } else {
//                //qDebug() << "Device not connected";
//            }
        } else {
            qDebug() << "not found the foID ...";
        }

    }
    else {
        // payment error, show message here
        qDebug() << "Pay by Zalopay error, message:" << msg;
        emit payError(QString("Lỗi:%1").arg(msg));
    }

    http2->deleteLater();
    http2 = NULL;
}

void MainController::onPayZaloError(const int &error, const QString &msg)
{
    qDebug() << "on PayZalo Error: " << error << ", msg:" + msg;

    emit cancellBillCloseQRDialog();
    emit cancelBillItem();
    emit errorMessage(error, msg);

    http2->deleteLater();
    http2 = NULL;
}

void MainController::doPayCash(const QString &sesId, const QString &biId)
{
    BillInfo bi;
    if( m_Bills.getById(biId, bi, sesId) )
    {
        QString dt = CommonFunction::formatRequestCreateInvoice(
                    merchantCode, machineName, bi.getAmount(),
                    deviceId, bi.id, bi.foods, titleEng, PAY_CASH);

        QString token = CommonFunction::HMacSha256HexStringEncode(QString("%1%2")
                                                  .arg(dt)
                                                  .arg(sesId), cmd_token);
        m_Bills.setToken(biId, token);

        QObject::connect(http2, SIGNAL(done(QVariant)), this, SLOT(onPayCashDone(QVariant)), Qt::UniqueConnection);
        QObject::connect(http2, SIGNAL(error(int,QString)), this, SLOT(onPayCashError(int, QString)), Qt::UniqueConnection);

        QString dt2 = QString::fromUtf8(QUrl::toPercentEncoding(dt).data());
        http2->addParameter("cm", "create", true);
        http2->addParameter("tk", token);
        http2->addParameter("dt", dt2);
        http2->process(POST);
    }
}

void MainController::onPayCashDone(const QVariant &data)
{
    QString msg, invceCode, invceDate, barcode, balance, discount, payment;
    int err = CommonFunction::parseResponsePayCash(data.toString(), &msg, &invceCode, &invceDate, &barcode, &balance);

    invceCode = "";
    if (m_Bills.checkInvceCode(invceCode))
    {
        if (1)//notifymsg == cmd_paymentsuccess)
        {
            BillInfo bi;
            if( m_Bills.getBill(invceCode, bi) == true )
            {
                // tạo nội dung xuất ra máy in
                QString printdata = bi.toPrinter(barcode,
                                                 invceCode,
                                                 invceDate,
                                                 appMachine,
                                                 titleEng,
                                                 discount, payment);
//                printBill(invceCode, printdata);
            }

            emit notifySucc(0, invceCode, invceDate, barcode, balance); // send notify error code = 0
        }
        else {
            emit notifySucc(1, invceCode, invceDate, barcode, balance); // send notify error code = 1
        }

        // remove the bill out of list
        m_Bills.removeBill(invceCode);
    }
    else {
        emit notifySucc(err, invceCode, invceDate, barcode, balance);
    }

    http2->deleteLater();
    http2 = NULL;
}

void MainController::onPayCashError(const int &error, const QString &message)
{
    emit errorMessage(error, message);
    emit cancelBillItem();

    http2->deleteLater();
    http2 = NULL;
}

void MainController::sendCancelBillUI()
{
    emit cancelBillItem();
}

void MainController::cancelBill(const QString& foId, const QString& invcecode, const QString& total)
{

}

void MainController::printbill(QJsonObject jbinfo, QJsonObject jbills,
                               int prnId, const QString& prnName)
{
    int PAGESZ, TAB1, TAB2, TAB3, TAB4;
    PAGESZ = 48;
    TAB1 = 12;
    TAB2 = 27;
    TAB3 = 38;
    TAB4 = 48;

    // có fooditem có printer mask cho máy in prnMask
    bool foundPrintItem = false;

    if( jbills.contains("bills") && jbills["bills"].isArray() )
    {
        qDebug() << "print Payment ...";

        int PAGESZ, TAB1, TAB2, TAB3, TAB4;
        PAGESZ = 48;
        TAB1 = 12;
        TAB2 = 27;
        TAB3 = 38;
        TAB4 = 48;
        long long total_money = 0 ;

        if( jbills.contains("bills") && jbills["bills"].isArray() )
        {
            QJsonArray bills = jbills["bills"].toArray();


                Prntdataformat pdf;

                pdf.codeFontType(pdf.font_A);
                pdf.codeFontBold(true);
                pdf.codeLine( jbinfo["title"].toString(), pdf.Justify_center );
                pdf.codeFontBold(false);

                // print prefix information
                if( jbinfo.contains("prefix") )
                {
                    QStringList prefixs = jbinfo["prefix"].toString().split(";");
                    for (int i=0; i<prefixs.length(); i++) {
                        pdf.codeLine(prefixs.at(i), pdf.Justify_center);
                    }
                }

                pdf.codeFeedline();

                // print food item detail
                pdf.codeText("TEN",     pdf.Justify_left);
                pdf.codeText("\tSL",    pdf.Justify_none, TAB2);
                pdf.codeText("\tD.GIA", pdf.Justify_right, TAB3);
                pdf.codeLine("\tT.TIEN",pdf.Justify_right, TAB4);
                pdf.codeChars('-', PAGESZ, pdf.Justify_center);

//                if( bill.contains("items") && bill["items"].isArray() )
//                {
//                    QJsonArray fitms = bill["items"].toArray();
//                    for( int i=0; i<bill.size(); i++ )
//                    {
//                        QJsonObject item = bill[0].toObject();
                QString total;
                for( int b=0; b<bills.size(); b++ )
                {
                    QJsonObject bill = bills.at(b).toObject();
                        QString name = Cmn::ViettoLatin(bill["item_name"].toString());
                        if (name.length() >= TAB2)
                            pdf.codeLine(name, pdf.Justify_left);
                        else
                            pdf.codeText(name, pdf.Justify_left);

                        QString quantity = QString("\t%1").arg(bill["quantity"].toInt());
                        pdf.codeText(quantity, pdf.Justify_left,   TAB2);

                        pdf.codeText("\t" + moneyString(bill["price"].toInt()),   pdf.Justify_right,  TAB3);
                        pdf.codeLine("\t" + moneyString(bill["amount"].toInt()),  pdf.Justify_right,  TAB4);
//                    }
//                }
                         total = "\t" + moneyString( bill["amount"].toInt() );
                         total_money += bill["amount"].toInt() ;
                    }

                // print total money of bill
                pdf.codeChars('-', PAGESZ, pdf.Justify_left);



                pdf.emphasized_on();
                pdf.codeLine(QString("TONG:%1").arg( moneyString(total_money) ), pdf.Justify_right, TAB4-total.length());
                pdf.emphasized_off();

                pdf.codeChars('-', PAGESZ, pdf.Justify_left);

                pdf.codeFeedline(1);

                // print barcode if existed
                if( jbills.contains("barcode") )
                {
                    pdf.codeBarcode( jbills["barcode"].toString(), 2, 80, pdf.Justify_center );
                    pdf.codeFeedline(1);
                }

                // print postfix information
                if( jbinfo.contains("postfix") )
                {
                    QStringList postfixs = jbinfo["postfix"].toString().split(";");
                    for (int i=0; i<postfixs.length(); i++) {
                        pdf.codeLine(postfixs.at(i), pdf.Justify_center);
                    }
                }

                pdf.codeFeedline(PAGESZ < 40 ? 2 : 1);
                pdf.codeCutpage(true);

                QByteArray prntdatacode = pdf.getPrintDeviceCode();


            // broad cast to all printer to print bill
            //thungan: 4
            if( wscssClient ) {
                wscssClient->sendMsg( Cmn::broadcastPrinter(prnId, prntdatacode) );
            }
        }

    }
}
