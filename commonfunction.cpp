#include "commonfunction.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDataStream>
#include <QByteArray>
#include <QtEndian>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>
#include <QCryptographicHash>
#include <QMessageAuthenticationCode>
CommonFunction::CommonFunction()
{

}

QString CommonFunction::gVPOS1()
{
    QString s;
    s.reserve(32);

    // 7VShsAFE3S4pS3lijpCkIxCDpzi7ljdS

    s.append('7');
    s.append('V');
    s.append('S');
    s.append('h');
    s.append('s');
    s.append('A');
    s.append('F');
    s.append('E');

    s.append('3');
    s.append('S');
    s.append('4');
    s.append('p');
    s.append('S');
    s.append('3');
    s.append('l');
    s.append('i');

    s.append('j');
    s.append('p');
    s.append('C');
    s.append('k');
    s.append('I');
    s.append('x');
    s.append('C');
    s.append('D');

    s.append('p');
    s.append('z');
    s.append('i');
    s.append('7');
    s.append('l');
    s.append('j');
    s.append('d');
    s.append('S');

    return s;
}

QString CommonFunction::generateQRCode(const QString &url, const QString &_datetime, int _roomId)
{
    return NULL;
}


int CommonFunction::parseMsgType(const QString& msg) {
    QJsonObject jsonMain;
    QJsonDocument jsonDoc;

    jsonDoc = QJsonDocument::fromJson(QByteArray(msg.toUtf8()));
    jsonMain = jsonDoc.object();

    return jsonMain["msg_type"].toInt();
}

bool CommonFunction::parseMsgSensors(const QString &msg, QString &devId, QString &gatewayId,
                              bool &havepersion, int &temperature, int &humidity, QString &area_id) {
    QJsonDocument jdoc = QJsonDocument::fromJson(QByteArray(msg.toUtf8()));
    QJsonObject jso = jdoc.object();

    if( jso.contains("dt"))
    {
        QJsonObject jdt = jso["dt"].toObject();

        if( jdt.contains("device_id")) {
            devId = jdt["device_id"].toString();
        }

        if( jdt.contains("area_id") ) {
            area_id = jdt["area_id"].toString();
        }

        if( jdt.contains("gateway_id")) {
            gatewayId = jdt["gateway_id"].toString();
        }

        int sensor = -1;
        if( jdt.contains("sensor_data")) {
            sensor = jdt["sensor_data"].toInt();
        }

        QString cmd;
        if( jdt.contains("command")) {
            cmd = jdt["command"].toString();
        }

        if( cmd.compare("humidity_sensor") == 0 ) {
            humidity = sensor;
        }

        if( cmd.compare("temperature_sensor") == 0 ) {
            temperature = sensor;
        }        

        return true;
    }

    return false;
}

QVariant CommonFunction::initJson(QString &devId, QString &gatewayId,
                            int &temperature, int &humidity, QString &area_id) {
    QJsonObject jso;
    jso["devId"] = devId;
    jso["gatewayId"] = gatewayId;
    jso["temperature"] = temperature;
    jso["humidity"] = humidity;
    jso["area_id"] = area_id;
//    QJsonDocument doc(jso);
    return jso;
//    return doc.toJson(QJsonDocument::Compact);
}
QString CommonFunction::dataLogin(const QString &merchant_code, const QString &u, const QString &p) {
    QJsonObject jso;
    jso["merchant_code"] = merchant_code;
    jso["u"] = u;
    jso["p"] = p;
    QJsonDocument doc(jso);
    return doc.toJson(QJsonDocument::Compact);
}

int CommonFunction::getAreaId(const QString &data) {
    QJsonDocument jdoc = QJsonDocument::fromJson(QByteArray(data.toUtf8()));
    QJsonObject jso = jdoc.object();
    int area_id = 0;
    if(jso.contains("dt")) {
        QJsonObject jdt = jso["dt"].toObject();
        if( jdt.contains("area_id") ) {
            area_id = jdt["area_id"].toInt();
        }
    }
    return area_id;
}

QString CommonFunction::getCurrentTimeInServer(const QString &data) {
    QJsonDocument jdoc = QJsonDocument::fromJson(QByteArray(data.toUtf8()));
    QJsonObject jso = jdoc.object();
    QString _time = "";
    if(jso.contains("dt")) {
        _time = jso["dt"].toString();
    }
    return _time;
}


QString CommonFunction::ViettoLatin(const QString& strIn)
{
    QString strOut;
    strOut=strIn;
    //strOut=strOut.toLower();
    strOut=strOut.replace(QRegExp("à|á|ạ|ả|ã|â|ầ|ấ|ậ|ẩ|ẫ|ă|ằ|ắ|ặ|ẳ|ẵ"),"a");
    strOut=strOut.replace(QRegExp("À|Á|Ạ|Ả|Ã|Ă|Ằ|Ắ|Ặ|Ẳ|Ẵ|Â|Ấ|Ầ|Ậ|Ẩ|Ẫ"),"A");
    strOut=strOut.replace(QRegExp("è|é|ẹ|ẻ|ẽ|ê|ề|ế|ệ|ể|ễ"),"e");
    strOut=strOut.replace(QRegExp("Ể|Ế|Ệ|Ể|Ê|Ễ|É|È|Ẻ|Ẽ|Ẹ"),"E");
    strOut=strOut.replace(QRegExp("ì|í|ị|ỉ|ĩ"),"i");
    strOut=strOut.replace(QRegExp("Ì|Í|Ị|Ỉ|Ĩ"),"I");
    strOut=strOut.replace(QRegExp("ò|ó|ọ|ỏ|õ|ô|ồ|ố|ộ|ổ|ỗ|ơ|ờ|ớ|ợ|ở|ỡ"),"o");
    strOut=strOut.replace(QRegExp("Ò|Ó|Ọ|Ỏ|Õ|Ơ|Ờ|Ớ|Ợ|Ở|Ỡ|Ô|Ố|Ổ|Ộ|Ồ|Ỗ"),"O");
    strOut=strOut.replace(QRegExp("ù|ú|ụ|ủ|ũ|ư|ừ|ứ|ự|ử|ữ"),"u");
    strOut=strOut.replace(QRegExp("Ừ|Ứ|Ự|Ử|Ư|Ữ|Ù|Ú|Ụ|Ủ|Ũ"),"U");
    strOut=strOut.replace(QRegExp("ỳ|ý|ỵ|ỷ|ỹ"),"y");
    strOut=strOut.replace(QRegExp("Ỳ|Ý|Ỵ|Ỷ|Ỹ"),"y");
    strOut=strOut.replace(QRegExp("đ"),"d");
    strOut=strOut.replace(QRegExp("Đ"),"D");
    return strOut;
}

void  CommonFunction::parseDataFoods(const QString &qstrData, CDownloadFileMan& dfMan, qulonglong& qullLUT)
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObjMain;
    QJsonObject jsonObjSub;
    QString     qstrHost;
    DOWNFILE_INFO dfi;
    int i, nNumItem, nSearch;
    QString qstrImagePath;
    QDir qStrDir;

    qstrImagePath = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];
    qStrDir = qstrImagePath;
    qstrImagePath = qStrDir.absolutePath();
    //qDebug() << "=== Path contains images: " << qstrImagePath;

    dfMan.Reset();
    jsonDoc = QJsonDocument::fromJson(QByteArray(qstrData.toUtf8()));
    jsonObjMain = jsonDoc.object();

    qstrHost = jsonObjMain["dt"].toObject()["img_host"].toString();
    qullLUT = jsonObjMain["dt"].toObject()["last_update"].toDouble();
    nNumItem = jsonObjMain["dt"].toObject()["items"].toArray().count();

    for (i = 0; i < nNumItem; i++){
        jsonObjSub = jsonObjMain["dt"].toObject()["items"].toArray()[i].toObject();
        dfi.qstrUrl = qstrHost +  jsonObjSub["img_path"].toString();
        dfi.nIdImage = jsonObjSub["item_id"].toInt();

        dfi.qstrLocalFile = qstrImagePath + QString("/") + FOLDER_IMAGE + QString("/") + QString::number(dfi.nIdImage);
        nSearch = dfi.qstrUrl.lastIndexOf('.');
        if (nSearch >= 0)
            dfi.qstrLocalFile =  dfi.qstrLocalFile + dfi.qstrUrl.right(dfi.qstrUrl.length() - nSearch);
        dfi.qullLMTime = jsonObjSub["img_crc"].toString().toULongLong();
        dfi.bResult = false;
        dfMan.AddDownLoadFile(dfi);
    }
}

QString CommonFunction::updateDataFoods(const QString &qstrData, CDownloadFileMan& dfMan){
    QJsonDocument jsonDoc;
    QJsonObject jsonObjMain, jsonSubObj, jsonSubObj1;
    QJsonArray jsonArray;
    int i;
    QList<DOWNFILE_INFO>& ListFile = dfMan.GetListFile();

    jsonDoc = QJsonDocument::fromJson(QByteArray(qstrData.toUtf8()));
    jsonObjMain = jsonDoc.object();
    jsonSubObj  = jsonObjMain["dt"].toObject();
    jsonArray   = jsonSubObj["items"].toArray();
    for (i = 0; i < ListFile.length(); i++){
        jsonSubObj1 = jsonArray[i].toObject();
        if (ListFile[i].bResult){
            jsonSubObj1["img_path"] = "file:///" + QFileInfo(ListFile[i].qstrLocalFile).absoluteFilePath();
        }else{
            jsonSubObj1["img_path"] = "qrc:/icons/images/defaultfood.png";
        }
        jsonSubObj1["img_cache"] = ListFile[i].bSetCache;
        jsonArray[i] = jsonSubObj1;
    }
    jsonSubObj["items"] = jsonArray;
    jsonObjMain["dt"] = jsonSubObj;
    QJsonDocument jsonDocOut(jsonObjMain);
    QByteArray data = jsonDocOut.toJson();
    return QString::fromUtf8(data.data());
}

int CommonFunction::parsePmsid(const QString &respone, QString *msg, QString *sid, QString *biid)
{
    int err = -1;

    QJsonDocument   jsd = QJsonDocument::fromJson(respone.toUtf8());
    QJsonObject     jso = jsd.object();

    if (jso.contains("err")) {
        err = jso["err"].toInt();
    }

    if (msg && jso.contains("msg")) {
        *msg = jso["msg"].toString();
    }

    if (jso.contains("dt") == true)
    {
        QJsonObject jdt = jso["dt"].toObject();

        if (sid && jdt.contains("invoice_session") == true) {
            *sid = jdt["invoice_session"].toString();
        }

        if (biid && jdt.contains("food_order") == true) {
            *biid = jdt["food_order"].toString();
        }
    }

    return err;
}

QString CommonFunction::formatSession(const QString& merchantCode, const QString& devId, const QString& biId)
{
    QJsonObject jso;
    jso["merchant_code"] = merchantCode;
    jso["food_order"] = biId;
    jso["devid"] = devId;

    QJsonDocument jsd(jso);

    return QString::fromUtf8(jsd.toJson().data());
}

QString CommonFunction::HMacSha256HexStringEncode(const QString& data, const QString& key)
{
    return QMessageAuthenticationCode::hash(data.toUtf8(), key.toUtf8(), QCryptographicHash::Sha256).toHex();
}

QString CommonFunction::formatRequestCreateInvoice(const QString &merchantCode,
                                                const QString &machineName,
                                                long amount,
                                                const QString& deviceId,
                                                const QString& biId,
                                                const QJsonArray &billItems,
                                                const QString &labTitle,
                                                   int paymentMethod)
{
    QJsonObject jsonObj;
    jsonObj["merchant_code"] = merchantCode;
    jsonObj["foodsorder_id"] = biId;
    jsonObj["machine_name"] = machineName;
    jsonObj["description"] = QString("%1 - %2").arg(labTitle).arg(machineName);
    jsonObj["appuser"] = merchantCode;
    jsonObj["amount"] = QString::number(amount);
    jsonObj["devid"] = deviceId;
    jsonObj["items"] = billItems;
    jsonObj["payment_method"] = QString::number(paymentMethod);

    QJsonDocument jsonDoc(jsonObj);
    QByteArray data = jsonDoc.toJson();

    return QString::fromUtf8(data.data());
}

int CommonFunction::parseResponseZalopay(const QString& response, QString &msg,
                                         QString& qrcode, QString* token, quint16* appId,
                                         QString* ivceCode, QString *ivceSess, QString* ivceDate,
                                         QString* oripay, int* discount, QString* discpay)
{
    int err = -1;

    QJsonDocument   jsd = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject     jso = jsd.object();

    if (jso.contains("err")) {
        err = jso["err"].toInt();
    }

    if (jso.contains("msg")) {
        msg = jso["msg"].toString();
    }

    if (jso.contains("dt") == true)
    {
        QJsonObject jsonDt = jso["dt"].toObject();

        QJsonDocument jsdQr(jsonDt);
        qrcode = QString::fromUtf8(jsdQr.toJson().data());

        if (token && jsonDt.contains("zptranstoken") == true) {
            *token = jsonDt["zptranstoken"].toString();
        }

        if (appId && jsonDt.contains("appid") == true) {
            *appId = jsonDt["appid"].toInt();
        }

        if (ivceCode && jsonDt.contains("invoice_code") == true) {
            *ivceCode = jsonDt["invoice_code"].toString();
        }

        if (ivceSess && jsonDt.contains("invoice_session") == true) {
            *ivceSess = jsonDt["invoice_session"].toString();
        }

        if (ivceDate && jsonDt.contains("invoice_datetime") == true) {
            *ivceDate = jsonDt["invoice_datetime"].toString();
        }

        if (oripay && jsonDt.contains("amount") == true) {
            *oripay = formatMoney(jsonDt["amount"].toInt());
        }

        if (discount && jsonDt.contains("discount") == true) {
            *discount = jsonDt["discount"].toInt();
        }

        if (discpay && jsonDt.contains("payment") == true) {
            *discpay = formatMoney(jsonDt["payment"].toInt());
        }
    }

    return err;
}

QString CommonFunction::generateQRCode(const QString &token, int appId)
{
    QJsonObject jso;
    jso["zptranstoken"] = token;
    jso["appid"] = appId;

    QJsonDocument jsd(jso);

    return QString::fromUtf8(jsd.toJson().data());
}

QByteArray CommonFunction::parseDataToRemoteReaderDevice(quint32 amount, quint16 appId, const QString &token)
{
    QByteArray dataArray;
    QDataStream dataStream(&dataArray, QIODevice::WriteOnly);
    dataStream.setByteOrder(QDataStream::LittleEndian);

    // Header
    //dataStream << (qint8)0xA0;

    // Len
    //dataStream << (qint8)0x1C; //28

    // Cmd
    //dataStream << (qint8)0x01;

    // SeqID
    //dataStream << (qint8)0x01;

    // Direction Flag
    dataStream << (qint8)0x07;

    dataStream << amount;
    //qDebug() << "Amount data: " << dataArray.toHex();

    dataStream << appId;
    //dataArray.append((char*)outAppId,2);
    //qDebug() << "AppId data: " << dataArray.toHex();
    //qDebug() << "Token:" << QByteArray::fromBase64(token.toLocal8Bit(), QByteArray::Base64UrlEncoding).toHex();

    //dataStream << QByteArray::fromBase64(token.toUtf8(), QByteArray::Base64UrlEncoding);

    dataArray.append(QByteArray::fromBase64(token.toLocal8Bit(), QByteArray::Base64UrlEncoding));

    QByteArray dataArray2;
    QDataStream dataStream2(&dataArray2, QIODevice::WriteOnly);
    dataStream2.setByteOrder(QDataStream::LittleEndian);
    //quint16 checksum = deviceDataChecksum(dataArray);
    quint16 checksum = makeCrc16Kermit(dataArray.right(dataArray.length()-1));
    dataStream2 << checksum;

    //dataStream2 << (qint8)0xFF;

    dataArray.append(dataArray2);
    //qDebug() << "Compose data: " << dataArray.toHex() << " checksum " << checksum << " lenght " << QByteArray::fromBase64(token.toLocal8Bit(), QByteArray::Base64UrlEncoding).length();
    return dataArray;
}

int CommonFunction::parseResponsePayCash(const QString &respone, QString *msg,
                                     QString *ivceCode, QString *ivceTime,
                                     QString *barcode, QString *balance)
{
    int err = -1;

    QJsonDocument   jsd = QJsonDocument::fromJson(respone.toUtf8());
    QJsonObject     jso = jsd.object();

    if (jso.contains("err")) {
        err = jso["err"].toInt();
    }

    if (msg && jso.contains("msg")) {
        *msg = jso["msg"].toString();
    }

    if (jso.contains("dt") == true)
    {
        QJsonObject jsonDt = jso["dt"].toObject();

        if (ivceCode && jsonDt.contains("invoice_code") == true) {
            *ivceCode = jsonDt["invoice_code"].toString();
        }

        if (ivceTime && jsonDt.contains("invoice_datetime") == true) {
            *ivceTime = jsonDt["invoice_datetime"].toString();
        }

        if (barcode && jsonDt.contains("barcode") == true) {
            *barcode = jsonDt["barcode"].toString();
        }

        if (balance && jsonDt.contains("balance") == true) {
            *balance = QString::number(jsonDt["balance"].toInt());
        }
    }

    return err;
}

QString CommonFunction::formatMoney(const QString& money)
{
    QString m = money;
    m = m.replace(QString(","), QString::null);
    return formatMoney(m.toInt());
}
QString CommonFunction::formatMoney(int moneyValue)
{
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

quint16 CommonFunction::makeCrc16Kermit(const QByteArray &data)
{
    quint16         valuehex = 0;
    quint16         CRC = 0;
    int             size = data.size();
    const quint8    *pData  = reinterpret_cast<const quint8*>(data.constData());

    CRC = 0;
    for(int i=0; i<size; ++i) {
        valuehex = ((*pData ^ CRC) & 0x0fu) * 0x1081;
        CRC >>= 4;
        CRC ^= valuehex;
        valuehex = ( (*pData >> 4) ^ (CRC & 0x00ffu) ) & 0x0fu;
        CRC >>= 4;
        CRC ^= (valuehex * 0x1081u);

        ++pData;
    }
    quint16 ret = ( (CRC & 0x00ffu) << 8) | ((CRC & 0xff00u) >> 8);
    return ret;
}

QString CommonFunction::formatWsSession(const QString& merchantCode, const QString& devId)
{
    QJsonObject jso;
    jso["merchant_code"] = merchantCode;
    jso["devid"] = devId;

    QJsonDocument jsd(jso);

    return QString::fromUtf8(jsd.toJson().data());
}

int CommonFunction::parseWssid(const QString &respone, QString *sid)
{
    int err = -1;

    QJsonDocument   jsd = QJsonDocument::fromJson(respone.toUtf8());
    QJsonObject     jso = jsd.object();

    if (jso.contains("err")) {
        err = jso["err"].toInt();
    }

    if( (err == 0) && (jso.contains("dt") == true) )
    {
        QJsonObject jdt = jso["dt"].toObject();

        if (sid && jdt.contains("ws_session") == true) {
            *sid = jdt["ws_session"].toString();
        }
    }
    else {
        err = -1;
    }

    return err;
}

QString CommonFunction::formatWSConnectString( const QString& url,
                                    const QString& merchantCode,
                                    const QString& deviceId,
                                    const QString& wsSession)
{
    QString request, token;

    token = HMacSha256HexStringEncode(merchantCode + deviceId + wsSession, gVPOS1());
    request = url + "?appuser=" + merchantCode + "&devid=" + deviceId + "&ws_session=" + wsSession + "&tk=" + token;

    return request;
}

