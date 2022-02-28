QT       += core gui
QT += widgets
QT += network widgets
QT += quickwidgets
QT += quick quickcontrols2
QT += quickcontrols2

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += c++11
android: include(/home/milkycookie/android_openssl/openssl.pri)
include(/home/milkycookie/ISZ/QProgressIndicator/qprogressindicator.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# QXlsx code for Application Qt project
QXLSX_PARENTPATH=./         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./source/  # current QXlsx source path is ./source/
include(./QXlsx.pri)

SOURCES += \
    BasicExcel.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    BasicExcel.hpp \
    mainwindow.h \
    schedule.h

FORMS += \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
android: include(/home/milkycookie/Android/Sdk/android_openssl/openssl.pri)

RESOURCES += \
    qml.qrc

DISTFILES +=

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_EXTRA_LIBS = \
        /home/milkycookie/android_openssl/no-asm/latest/arm/libcrypto_1_1.so \
        /home/milkycookie/android_openssl/no-asm/latest/arm/libssl_1_1.so \
        /home/milkycookie/android_openssl/no-asm/latest/arm64/libcrypto_1_1.so \
        /home/milkycookie/android_openssl/no-asm/latest/arm64/libssl_1_1.so \
        /home/milkycookie/android_openssl/no-asm/latest/x86/libcrypto_1_1.so \
        /home/milkycookie/android_openssl/no-asm/latest/x86/libssl_1_1.so \
        /home/milkycookie/android_openssl/no-asm/latest/x86_64/libcrypto_1_1.so \
        /home/milkycookie/android_openssl/no-asm/latest/x86_64/libssl_1_1.so \
        /home/milkycookie/Android/Sdk/android_openssl/no-asm/latest/arm/libcrypto_1_1.so \
        /home/milkycookie/Android/Sdk/android_openssl/no-asm/latest/arm/libssl_1_1.so \
        /home/milkycookie/Android/Sdk/android_openssl/no-asm/latest/arm64/libcrypto_1_1.so \
        /home/milkycookie/Android/Sdk/android_openssl/no-asm/latest/arm64/libssl_1_1.so \
        /home/milkycookie/Android/Sdk/android_openssl/no-asm/latest/x86/libcrypto_1_1.so \
        /home/milkycookie/Android/Sdk/android_openssl/no-asm/latest/x86/libssl_1_1.so \
        /home/milkycookie/Android/Sdk/android_openssl/no-asm/latest/x86_64/libcrypto_1_1.so \
        /home/milkycookie/Android/Sdk/android_openssl/no-asm/latest/x86_64/libssl_1_1.so
}

