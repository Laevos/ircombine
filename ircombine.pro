TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    ../../Users/Dell/pgrm/ircombine/logfile.h \
    logfile.h

OTHER_FILES += \
    kita.log \
    kitafox.log

