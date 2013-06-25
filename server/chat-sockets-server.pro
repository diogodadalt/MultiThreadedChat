TEMPLATE = app
CONFIG += console
CONFIG += debug
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread

SOURCES += server.c \
	../common/socket_wrapper.c \
	command.c \
	client_info.c

HEADERS += \
	../common/socket_wrapper.h \
	command.h \
	client_info.h