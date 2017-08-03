#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H
#include <QString>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include "downloadfileman.h"

#define FOLDER_IMAGE "images"

class CommonFunction
{
public:
    CommonFunction();
    static QString gVPOS1();
    static QString generateQRCode(const QString &url, const QString &_datetime, int _roomId);
    static int parseMsgType(const QString& msg);
    static bool parseMsgSensors(const QString &msg, QString &devId, QString &gatewayId,
                                  bool &havepersion, int &temperature, int &humidity, QString &area_id);
    static QVariant initJson(QString &devId, QString &gatewayId,
                            int &temperature, int &humidity, QString &area_id);
    static QString dataLogin(const QString &merchant_code, const QString &u, const QString &p);
    static int getAreaId(const QString& data);
    static QString getCurrentTimeInServer(const QString& data);
    static QString ViettoLatin(const QString& strIn);
    static void parseDataFoods(const QString &qstrData, CDownloadFileMan& dfMan, qulonglong& qullLUT);
    static QString updateDataFoods(const QString &qstrData, CDownloadFileMan& dfMan);
    static int parsePmsid(const QString &respone, QString *msg, QString *sid, QString *biid);
    static QString formatSession(const QString& merchantCode, const QString& devId, const QString &biId);
    static QString HMacSha256HexStringEncode(const QString& data, const QString& key);
    static QString formatRequestCreateInvoice(const QString &merchantCode,
                                              const QString &machineName,
                                              long amount,
                                              const QString& deviceId,
                                              const QString& biId,
                                              const QJsonArray& billItems,
                                              const QString& labTitle,
                                              int paymentMethod);
    static int parseResponseZalopay(const QString& response, QString &msg,
                                        QString& qrcode, QString* token, quint16* appId,
                                        QString* ivceCode, QString *ivceSess, QString* ivceDate,
                                        QString* oripay, int *discount, QString* discpay);
    static QString generateQRCode(const QString &token, int appId);
    static QByteArray parseDataToRemoteReaderDevice(quint32 amount, quint16 appId, const QString& token);
    static int parseResponsePayCash(const QString &respone, QString *msg,
                                            QString *ivceCode,
                                            QString *ivceTime,
                                            QString *barcode,
                                            QString *balance);
    static QString formatMoney(const QString& money);
    static QString formatMoney(int money);
    static quint16 makeCrc16Kermit(const QByteArray &data);
    static QString formatWsSession(const QString& merchantCode, const QString& devId);
    static int parseWssid(const QString &respone, QString *sid);
    static QString formatWSConnectString( const QString& url,
                                        const QString& merchantCode,
                                        const QString& deviceId,
                                        const QString& wsSession);
};

#endif // COMMONFUNCTION_H
