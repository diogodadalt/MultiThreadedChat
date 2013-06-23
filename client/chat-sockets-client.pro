TEMPLATE = app
CONFIG += console
CONFIG += debug
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0

LIBS += -lpthread
LIBS += -lcdk
LIBS += -lncurses

SOURCES += client.c \
    user_interface.c

HEADERS += \
    user_interface.h