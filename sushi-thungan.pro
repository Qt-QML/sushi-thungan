QT += qml quick quickcontrols2 network websockets printsupport

CONFIG += c++11

SOURCES += main.cpp \
    maincontroller.cpp \
    appsetting.cpp \
    utils.cpp \
    commonfunction.cpp \
    qqrencode.cpp \
    colorimageprovider.cpp \
    wsclient.cpp \
    logincontroller.cpp \
    tablecontroller.cpp \
    ordercontroller.cpp \
    foodcontroller.cpp \
    downloadfileman.cpp \
    billdetail.cpp \
    http/json.cpp \
    http/httptool.cpp \
    http/httpparams.cpp \
    http/httpbase.cpp \
    http/httpbase2.cpp \
    ../common/prntdataformat.cpp \
    ../common/udpbrdcastclient.cpp \
    ../common/wscssclient.cpp \
    cmn.cpp \
    paymentcontroller.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    maincontroller.h \
    appsetting.h \
    utils.h \
    version.h \
    commonfunction.h \
    qqrencode.h \
    qqrencode_p.h \
    colorimageprovider.h \
    wsclient.h \
    logincontroller.h \
    tablecontroller.h \
    ordercontroller.h \
    foodcontroller.h \
    downloadfileman.h \
    billdetail.h \
    http/json.h \
    http/httptool.h \
    http/httpparams.h \
    http/httpbase.h \
    http/httpbase2.h \
    ../common/prntdataformat.h \
    ../common/udpbrdcastclient.h \
    ../common/vposiddefine.h \
    ../common/wscssclient.h \
    cmn.h \
    paymentcontroller.h \
    vpossdk.h \
    vpossdk_global.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/src/com/vng/gbc/smr/MyActivity.java \
    android/src/com/vng/gbc/smr/BootUpReceiver.java

android {
    DEFINES += AUTO_HIDE_NAVI
    contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
#        LIBS+= -L/Users/baohavan/Dev/android/libraries/output/armeabi-v7a/lib -lssl -lcrypto
        #DEFINES += USE_BREAKPAD
        #include(qBreakpad/qBreakpad.pri)
    }
    contains(ANDROID_TARGET_ARCH,x86) {
        LIBS+= -L/Users/baohavan/Dev/android/libraries/output/x86/lib -lssl -lcrypto
    }
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS =
}

include(qrencode-3.4.4/qrencode.pri)
INCLUDEPATH += $$PWD/qrencode-3.4.4
DEFINES += HAVE_CONFIG_H


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../vpossdk-build/release/ -lvpos.1.0.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../vpossdk-build/debug/ -lvpos.1.0.0
else:unix: LIBS += -L$$PWD/../vpossdk-build/ -lvpos.1.0.0

INCLUDEPATH += $$PWD/../vpossdk-build
DEPENDPATH += $$PWD/../vpossdk-build
