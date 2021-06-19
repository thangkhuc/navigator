QT       += core gui
QT       += testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adress.cpp \
    anzeige_dialog.cpp \
    graph.cpp \
    main.cpp \
    navigation.cpp \
    ort.cpp \
    poi.cpp \
    routeinfor.cpp \
    test.cpp \
    vertex.cpp

HEADERS += \
    adress.h \
    anzeige_dialog.h \
    graph.h \
    navigation.h \
    ort.h \
    poi.h \
    routeinfor.h \
    test.h \
    vertex.h

FORMS += \
    anzeige_dialog.ui \
    navigation.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    maps.qrc
