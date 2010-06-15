#####################################################################
# Velasco web browser 
# Copyright (C) 2008 Sascha Peilicke <sasch.pe@gmx.de>
#
# qmake project file
######################################################################

TEMPLATE      = app
QT           += webkit
TARGET        = velasco
DEPENDPATH   +=
INCLUDEPATH  +=
LIBS         += 
CONFIG       += qt thread \
                warn_on \                # show all compiler warnings
                debug_and_release \
#                build_all                # build debug and release targets

HEADERS      += src/config.h \
                src/main.h \
                src/minibrowser.h \
                src/browsertab.h \
                src/browsertabwidget.h \
                src/browserwindow.h
SOURCES      += src/main.cpp \
                src/minibrowser.cpp \
                src/browsertab.cpp \
                src/browsertabwidget.cpp \
                src/browserwindow.cpp
RESOURCES    += resources/velasco.qrc
TRANSLATIONS +=
