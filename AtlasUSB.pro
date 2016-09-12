#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T12:51:49
#
#-------------------------------------------------

QT       += core gui
QT      += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = AtlasUSB
TEMPLATE = app

# Tell the qcustomplot header that it will be used as library:
# DEFINES += QCUSTOMPLOT_USE_LIBRARY

# Link with debug version of qcustomplot if compiling in debug mode, else with release library:
CONFIG(debug, release|debug) {
  win32:QCPLIB = qcustomplotd1.dll
  else: QCPLIB = qcustomplotd
} else {
  win32:QCPLIB = qcustomplot1.dll
  else: QCPLIB = qcustomplot
}

win32: LIBS += "../$$QCPLIB"
unix:  LIBS += -L"../" -l$$QCPLIB
#LIBS += -L"$$_PRO_FILE_PWD_" -l$$QCPLIB

win32: INCLUDEPATH += "D:\Projects_LTD\Qt\qcustomplot"
unix:  INCLUDEPATH += "/home/pvk/Projects/Qt/qcustomplot"

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    atlasdialog.cpp \
    qatlasusb.cpp \
    ledindicator.cpp \
    about.cpp \
    ezoframe.cpp \
    plotframe.cpp \
    serialdialog.cpp \
    qcustomplot.cpp \
    atlasusbreceiver.cpp

HEADERS += \
    mainwindow.h \
    atlasdialog.h \
    qatlasusb.h \
    ledindicator.h \
    about.h \
    ezoframe.h \
    plotframe.h \
    serialdialog.h \
    qcustomplot.h \
    atlasusbreceiver.h

FORMS += \
    mainwindow.ui \
    atlasdialog.ui \
    about.ui \
    ezoframe.ui \
    plotframe.ui \
    serialdialog.ui

RESOURCES += \
     atlasusb.qrc

CONFIG += c++11
CONFIG += warn_on

QMAKE_CXXFLAGS += -Wextra

INCLUDEPATH += ..

#qwt is not necessary any more
#win32: include ( C:\qwt-6.1.2\features\qwt.prf )
#unix: include ( /usr/lib/qt/mkspecs/features/qwt.prf )
