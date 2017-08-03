import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Material.impl 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Private 1.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQuick.Controls.Universal 2.0
import QtGraphicalEffects 1.0
import QtQuick.Extras 1.4
import Qt.labs.platform 1.0

import com.vng.gbc.smr 1.0



Page {   
    anchors.fill: parent
    property int globalPadding: 10
    property int indexRadioButton: 1
    property string url: qsTr("")
    property int total_amount: 0
    property int tableNumber: 0
    property string areaName: qsTr("")
    property string categoryName: qsTr("")
    property var areas
    property var tables
    property var categories
    property var foods
    property var orderintable
    property string img_host: qsTr("")
    property int payment_type: 1
    width: parent.width
    height: parent.height
    Connections {
        target: MainController
        onGetFoodsConnection: {
            console.log("onGetFoodsConnection: " + data)
            categories = JSON.parse(data).dt.categories
            eventListCategory.clear()
            eventListCategory.append(categories)
            foods = JSON.parse(data).dt.items
            eventListFood.clear()
            eventListFood.append(foods)
            img_host = JSON.parse(data).dt.img_host
        }
        onGetListOrderInTable: {
            console.log("onGetListOrderInTable: " + JSON.stringify(data))
            orderintable = JSON.parse(data).dt.items
            current_invoice_id = JSON.parse(data).dt.invoice_code
            orderModel.clear()
            for(var i in orderintable) {
               orderModel.jsonFooditems(
                            orderintable[i].item_name,
                            orderintable[i].quantity,
                            orderintable[i].price,
                            orderintable[i].amount)
            }
            total_amount = JSON.parse(data).dt.amount
        }
        onArea_tableConnection: {
            areas = JSON.parse(data).dt.areas
            tables = JSON.parse(data).dt.tables
            eventModelContentGroup.clear()
            eventModelContentGroup.append(areas);
        }
        onGetPayConnection: {
            var err = JSON.parse(data).err
            if (err === 0) {
                orderModel.clear()
            }
        }
        onWsConnection: {
            console.log("onWsConnection: " + JSON.stringify(data))
            var _type = JSON.parse(data).type
            var _dt = JSON.parse(data).dt
            console.log("_dt: " + JSON.stringify(_dt))
            if(_type === 3) {
                var _tbid = _dt.table_id
                MainController.getOrder(_tbid)
            }
        }
    }

    ListModel {
        id: orderModel
//        function jsonFooditems(fio) {

//            var amount = 0;
//            var fooditems = [];
//            for( var i=0; i<count; i++ )
//            {
//                var fi = {
//                    item_id:    (i+1),
//                    item_name:  get(i).item_name,
//                    quantity:   get(i).quantity,
//                    price:      get(i).price,
//                    amount:     get(i).amount,
//                    original_price:get(i).price,
//                    promotion_type:get(i).promtype,
//                    printer_mask:get(i).printer_mask
//                };

//                amount += get(i).amount;
//                fooditems.push(fi);
//            }

//            fio.amount = amount;
//            fio.items = fooditems;

//            return fooditems;
//            // JSON.stringify(fooditems);
//        }
        function jsonFooditems(name, quantity, price, amount) {
            var index = checkExisted(name)
            if (index < 0) {
                append({"item_name":name,
                        "quantity": quantity,
                        "price":    MainController.moneyString(price),
                        "amount":   MainController.moneyString(amount),
                        "cancel":   "X"})
            } else {
                var quantity2 = get(index).quantity + quantity
                var amount2 = quantity2 * price
                setProperty(index, "quantity", MainController.moneyString(quantity2))
                setProperty(index, "amount", MainController.moneyString(amount2))
            }
            //total_amount += amount
            //textThanhtienValue.text = qsTr(Number(total_amount).toLocaleString(Qt.locale("de_DE"))).replace(",00","")
        }

        function addItem(name, quantity, price, amount) {
            var index = checkExisted(name)
            if (index < 0) {
                append({"item_name":name,
                        "quantity": quantity,
                        "price":    MainController.moneyString(price),
                        "amount":   MainController.moneyString(amount),
                        "cancel":   "X"})                
            } else {
                var quantity2 = get(index).quantity + quantity
                var amount2 = quantity2 * price
                setProperty(index, "quantity", MainController.moneyString(quantity2))
                setProperty(index, "amount", MainController.moneyString(amount2))
            }
            total_amount += amount
            //textThanhtienValue.text = qsTr(Number(total_amount).toLocaleString(Qt.locale("de_DE"))).replace(",00","")
        }

        function removeOne(itmname, itmquantity) {
            var index = checkExisted(itmname)
            if( index >= 0 ) {
                var quantity2 = get(index).quantity
                var amount2 = 0
                total_amount -=  MainController.moneyNumber(get(index).price)
                if( quantity2 <= itmquantity ) {
                    // số lượng bằng 1, nên remove nguyên item luôn
                    remove(index, 1)
                }
                else {
                    // giảm số lượng đi 1
                    quantity2 -= itmquantity
                    amount2 = quantity2 * MainController.moneyNumber(get(index).price)
                    setProperty(index, "quantity", quantity2)
                    setProperty(index, "amount", MainController.moneyString(amount2))
                }                
            }
        }

        function checkExisted(name) {
            var i
            for (i = 0; i < count; i++) {
                if (get(i).item_name === name) {
                    return i;
                }
            }
            return -1;
        }

        function removeItem(row1) {
            var quantity = get(row1).quantity
            var price = MainController.moneyNumber(get(row1).price)
            var amount2 = quantity * price
            total_amount -= amount2
            remove(row1, 1)
        }

        function billDetail() {
            var items = [];

            var i
            for (i=0; i<count; i++) {
                var itemData = {
                    item_name:get(i).item_name,
                    quantity:get(i).quantity,
                    price:MainController.moneyNumber(get(i).price),
                    amount:MainController.moneyNumber(get(i).amount),
                    original_price:get(i).oriprice,
                    promotion_type:get(i).promtype
                };
                items.push(itemData);
            }

            return items;
        }
    }

    ListModel {
        id: eventModelContentGroup
    }

    ListModel {
        id: eventListTableModel
    }

    ListModel {
        id: eventListFood
    }

    ListModel {
        id: eventListCategory
    }

    // Main column
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        //content
        GridLayout {
            id: gridContent
            columns: 3
            Rectangle {
                id: rectContent
                height: parent.height
                width: parent.width
                color: "gray"

                RowLayout {
                    //-----khu vuc-----
                    Rectangle {
                        id: rectGroup
                        height: rectContent.height
                        width: rectContent.width/10*1.5
                        color: "gray"
                        AreaItemListView {
                            id: areaItemListView
                            visible: get_menu()
                            height: rectGroup.height
                            width: rectGroup.width - 0.5
                        }
                        CategoryItemListView {
                            id: categoryItemListView
                            visible: !get_menu()
                            height: rectGroup.height
                            width: rectGroup.width - 0.5
                        }
                    }
                    //-----END khu vuc-----

                    //-----danh sach ban-----
                    Rectangle {
                        id: rectListTable
                        anchors.left: rectGroup.right
                        height: rectContent.height
                        width: (rectContent.width/10)*5.5
                        color: "gray"
                        TableItemListView {
                            id: tableItemListView
                            visible: get_menu()
                            height: rectListTable.height
                            width: rectListTable.width - 0.5
                        }
                        FoodItemListView {
                            id: foodItemListView
                            visible: !get_menu()
                            height: rectListTable.height
                            width: rectListTable.width - 0.5
                        }
                    }
                    //-----END danh sach ban-----

                    //-----hoa don-----
                    Rectangle {
                        id: rectListOrders
                        anchors.left: rectListTable.right
                        height: rectContent.height
                        width: (rectContent.width/10)*3

                        //content hoa don
                        Rectangle {
                            id: rectContentListOrders
                            Layout.fillWidth: true
                            anchors.top: rectTopListOrders.bottom
                            width: rectListOrders.width
                            height: rectListOrders.height
                            ChoosenItemListView {
                                id: choosenItemListView
                                anchors.top: parent.top
                                anchors.left: parent.left
                                width: rectContentListOrders.width
                                height: rectContentListOrders.height - paymentView.height
                            }
                            PaymentView {
                                id: paymentView
                                anchors.bottom: parent.bottom
                                anchors.right: parent.right
                                anchors.left: parent.left
                                anchors.leftMargin: 1
                                anchors.bottomMargin: window.globalPadding
                            }
                        }
                    }
                    //-----END hoa don-----
                }
            }
        }
    }

    Component.onCompleted: {
        MainController.initialize();
    }

    Component.onDestruction: {
        MainController.uninitialize();
    }


}
