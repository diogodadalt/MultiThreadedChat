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
	../common/socket_wrapper.c \
	user_interface.c

HEADERS += \
	../common/socket_wrapper.h \
	user_interface.h