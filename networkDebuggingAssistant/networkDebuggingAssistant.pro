QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../src/openDir/openDir.cpp \
    ../src/tcpclient.cpp \
    ../src/tcpserver.cpp \
    main.cpp \
    networkdebuggingassistant.cpp \
    tcpClientTest.cpp \
    tcpServerTest.cpp \
    testForm.cpp

HEADERS += \
    ../src/openDir/openDir.h \
    ../src/tcpclient.h \
    ../src/tcpserver.h \
    networkdebuggingassistant.h \
    tcpClientTest.h \
    tcpServerTest.h \
    tcpSocketHead.h \
    testForm.h

FORMS += \
    networkdebuggingassistant.ui \
    testForm.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
