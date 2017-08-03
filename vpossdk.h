#ifndef VPOSSDK_H
#define VPOSSDK_H

#include <QObject>
#include <QJsonObject>
#include "vpossdk_global.h"

#ifdef VPOSSDK_LIBRARY
    #include "http/httpbase2.h"
#endif

#define PAY_ZALO 1  // need ZALOPAY merchant-id
#define PAY_CARD 2  // need system support card-id
#define PAY_CASH 4  // cash money

#define VPOSSDK_PROCESS_FOOD       1   // đang thực hiện request download food data content
#define VPOSSDK_PROCESS_ZPAY       2   // đang thực hiện request thanh toán zalopay
#define VPOSSDK_PROCESS_ZCANCEL    4   // đang thực hiện request cancel zalopay/invoicecode
#define VPOSSDK_PROCESS_ULOGIN     8   // đang thực hiện request login
#define VPOSSDK_PROCESS_TABLE     16   // đang thực hiện request table
#define VPOSSDK_PROCESS_FORDER    32   // đang thực hiện request order món ăn
#define VPOSSDK_PROCESS_TABFOOD   64   // đang thực hiện request get order list
#define VPOSSDK_PROCESS_TABEXCHE 128   // đang thực hiện request merge/change table
#define VPOSSDK_PROCESS_PAYMENT  256   // đang thực hiện thanh toán
#define VPOSSDK_PROCESS_PRINTER  512   // đang thực hiện lấy ds máy in cấu hình

class VPOSSDKSHARED_EXPORT Vpossdk : public QObject
{
    Q_OBJECT

public:
    enum {
        VposErrUnknow               = 101,      // thông điệp không xác định
        VposErrProcessFood          = 102,      // đang chờ kết quả request data content
        VposErrProcessZalopay       = 103,      // đang chờ kết quả thực hiện thanh toán
        VposErrProcessPaycancel     = 104,      // đang chờ kết quả thực hiện cancel
        VposErrInvalidInvoice       = 105,      // gọi cancel với mã invoice code không tồn tại
        VposErrProcessUserlogin     = 106,      // đang gọi login
        VposErrUserLogined          = 107,      // user đã login
        VposErrUserNotLogin         = 108,      // chưa có user login
        VposErrProcessOrder         = 109,      // đang chờ kết quả order món ăn
        VposErrProcessTable         = 110,      // đang chờ kết quả request table
        VposErrProcessTableInfo     = 111,      // đang chờ kết quả các món ăn đã chọn
        VposErrProcessTableExchange = 112,      // đang chờ kết quả gộp/chuyển bàn
        VposErrProcessPayment       = 113,      // đang chờ kết quả thực hiện payment
        VposErrProcessPrinter       = 114,      // đang chờ kết quả ds máy in cấu hình
    } vpos_error;

    enum user_type {
        VposUserAll,
        VposUserWaiter,
        VposUserCashier,
    };

public:
    Vpossdk(const QString &merchantId, const QString &machineName, bool verbose=false, bool httplog=false);

public slots:

    void    userLogin(const QString &usr, const QString &pwd, user_type usrtype=VposUserAll);
    void    userLogout();
    bool    userName(int &id, QString &name);
    bool    userIslogged();

            // get food data/content & download image to local
    void    foodsRequest();

            // gọi thực hiện order món ăn
    void    foodOrderRequest(const QJsonObject &foodItems);

            // request list of table and its status
    void    tableRequest();

            // gọi lấy danh sách các món đã order
    void    tableFoodsRequest(int tableId);

            // gộp bàn từ frmtableId vào totableId
    void    tableMergeRequest(int frmtableId, int totableId);

            // chuyển bàn từ frmtableId vào totableId
    void    tableChangeRequest(int frmtableId, int totableId);

    void    printerRequest();

            // gọi thực hiện thanh toán bằng zalopay
    void    zalopayRequest(long money, const QString &description, const QString& itemDetail);

            // gọi thanh toán zalopay để nhận QR code, invoice code
    void    zalopayTransfers(const QString &sesId, long money, const QString &description, const QString& itemDetail);

            // gọi hủy thanh toán
    void    zalopayCancel(const QString invoiceCode);

            // gọi thanh toán cho bàn, amount không cần do lấy trên server
    void    paymentRequest(int tableId, int amount);


public slots:
            // notify từ server / dock service về sdk
    void    wssNotify(const QString &message);
    void    onNetworkError(int error, const QString &message);
    void    transPermisionRequest(int paytype, long money, const QString &description, const QString& itemDetail);
    void    onFoodsReadyDownload(const QVariant &data);
            // kết quả gọi thanh toán zalopay, nhận được QR code và invoice code
    void    onZalopayQRInvoiceCode(const QVariant &data);

    void    tableExchangeRequest(int frmtableId, int totableId, int type=0);

signals:

    void    cloudConnectionChange(bool connected);

            // general network error
    void    networkError(int error, const QString &message);

            // vpos app request error
    void    vposappError(int error, const QString &message);

            // thông điệp về từ server vpos
    void    notify(const QString &msg);

    void    userLoginDone(int error, const QString &msg);
    void    userLogoutDone(int error, const QString &msg);
    void    userIdname(int id, const QString &name);

            // request food/data download to local done
    void    foodsRequestDone(const QString &data);

            // result of food order request
    void    foodOrderRequestDone(const QString &data);

            // result of table request
    void    tableRequestDone(const QString &data);

            // result of table info request
    void    tableFoodsDone(const QString &data);

    void    tableMergeDone(const QString &data);
    void    tableChangeDone(const QString &data);

    void    printerDone(const QString &data);

            // zalopay gửi mã QR code để thực hiện thanh toán ZALOPAY-APP
    void    zaloQRInvoiceCode(const QString &data);

            // kết thúc đợt thanh toán
    void    zalopayRequestDone(const QString &data);

            // kết thúc đợt hủy thanh toán cho hóa đơn có invoicecode/qrcode
    void    zalopayCancelDone(const QString &data);

    void    paymentDone(const QString &data);

private:

    QString     m_vposhotname;  // vpos server hotname 'gbc.zing.vn'
    QString     m_vposurl;      // https://gbc.zing.vn

    QString     m_merchantId;   // cantinvng, ffvietthai ...
    QString     m_machineName;  // MAY-1, MAY-2 ...
    QString     m_deviceId;     // quui-hex

    long        m_process;      // các tác vụ đang thực hiện
    bool        m_showlog;      // verbose logging
    bool        m_httplog;      // http log

    QString     m_invceCode;    // current working invoice code

    QString     m_usrname;      // user logined Name
    int         m_usrid;        // user logined ID

#ifdef VPOSSDK_LIBRARY
    HttpBase2   *http2;
#endif
};

#endif // VPOSSDK_H
