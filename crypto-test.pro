QT -=  widget

CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += main.cpp \
    crypto.cpp

LIBS += -L/usr/lib -lcrypto


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    crypto.h


