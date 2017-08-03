#include "billdetail.h"
#include "commonfunction.h"
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

void BillInfo::Create(const QString& money, const QJsonArray &foodItems)
{
    id = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
    amount = money;
    amount.replace(",", "");
    ivceCode = "";
    foods = foodItems;
}

quint32 BillInfo::getAmount()
{
    return amount.toULong();
}

QString BillInfo::toPrinter(const QString &barcode, const QString &receipt,
                               const QString &datetime, const QString &machine,
                               const QString &title,
                            const QString &discount, const QString &payment)
{
    QJsonObject jso;
    jso["barcode"] = barcode;
    jso["datetime"] = datetime;
    jso["machine"] = machine;
    jso["receiptNum"] = receipt;
    jso["title"] = title;
    jso["total"] = CommonFunction::formatMoney(amount);

    QJsonArray jsarr;
    for( int i=0; i<foods.size(); i++ )
    {
        QJsonObject item = foods[i].toObject();

        QJsonObject jso;
        jso["item_name"] = item["item_name"].toString();
        jso["quantity"] = QString("%1").arg(item["quantity"].toInt());
        jso["amount"] = CommonFunction::formatMoney(item["amount"].toInt());
        jso["price"] = CommonFunction::formatMoney(item["price"].toInt());

        jsarr.append(jso);
    }
    jso["item"] = jsarr;

    int dct = discount.toInt();
    if( !discount.isEmpty() && (dct > 0) )
    {
        QJsonArray jsarr;
        QJsonObject namevalue;

        int thucthu = payment.toInt();

        namevalue["name"] = QString("GIAM %1\%").arg(dct);
        namevalue["value"] = CommonFunction::formatMoney( qRound(getAmount()*dct/100.));
        jsarr.append(namevalue);

        namevalue["name"] = QString("THANH TOAN");
        namevalue["value"] = CommonFunction::formatMoney(thucthu);
        jsarr.append(namevalue);

        jso["ext"] = jsarr;
    }

    QJsonDocument jsdoc(jso);
    return QString::fromUtf8(jsdoc.toJson().data());
}

CBillDetail::CBillDetail()
{
}

bool CBillDetail::checkInvceCode(const QString& invceCode)
{
    for (int i = 0; i < BillList.length();i++)
    {
        if (BillList[i].ivceCode == invceCode){
            return true;
        }
    }
    return false;
}

bool CBillDetail::getAt(int idx, BillInfo& bi)
{
    if( (idx >=0) && (idx<BillList.length()) ) {
        bi = BillList[idx];
        return true;
    }

    return false;
}

bool CBillDetail::getBill(const QString& invoiceCode, BillInfo& bi)
{
    for (int i = 0; i < BillList.length();i++){
        if (BillList[i].ivceCode == invoiceCode){
            bi = BillList[i];
            return true;
        }
    }
    return false;
}

bool CBillDetail::getById(const QString& id, BillInfo& bi, const QString& sesId)
{
    for (int i = 0; i < BillList.length();i++) {
        if (BillList[i].id == id) {
            BillList[i].sesId = sesId;
            bi = BillList[i];
            return true;
        }
    }
    return false;
}

bool CBillDetail::getBySessId(const QString& sesId, BillInfo& bi, const QString &invceCode)
{
    for (int i = 0; i < BillList.length();i++) {
        if (BillList[i].sesId == sesId) {
            BillList[i].ivceCode = invceCode;
            bi = BillList[i];
            return true;
        }
    }
    return false;
}

// get billInfo by id and set back invceCode to billlist
bool CBillDetail::getById(const QString& id, const QString &invceCode, BillInfo& bi)
{
    for (int i = 0; i < BillList.length();i++) {
        if (BillList[i].id == id) {
            BillList[i].ivceCode = invceCode;
            bi = BillList[i];
            return true;
        }
    }
    return false;
}

bool CBillDetail::removeById(const QString &id)
{
    for (int i = 0; i < BillList.length();i++){
        if (BillList[i].id == id){
            BillList.removeAt(i);
            return true;
        }
    }
    return false;
}

bool CBillDetail::removeBill(const QString& invoiceCode){
    for (int i = 0; i < BillList.length();i++){
        if (BillList[i].ivceCode == invoiceCode){
            BillList.removeAt(i);
            return true;
        }
    }
    return false;
}

void CBillDetail::setToken(const QString& id, const QString& token)
{
    for (int i = 0; i < BillList.length();i++) {
        if (BillList[i].id == id) {
            BillList[i].token = token;
            return;
        }
    }
}

void CBillDetail::addBill(const BillInfo& billInfo){
    removeBill(billInfo.ivceCode);
    BillList.append(billInfo);
}

void CBillDetail::cleanupBill() {
    BillList.clear();
}

// get invoice_code by index
QString CBillDetail::getBillInvoiceCode(int idx, const QString &invceDefault)
{
    if (idx<BillList.count()) {
        return QString("\"%1\"").arg(BillList[idx].ivceCode);
    }

    return QString("\"%1\"").arg(invceDefault);
}
