SOURCES += main.cpp \
    mainwindow.cpp \
    graphicsview/graphicsitemclass.cpp \
    graphicsview/graphicsitemconnection.cpp \
    graphicsview/graphicsitemconnectionpoint.cpp
INCLUDEPATH = ../lib/
LIBS += -L$(HOME)/programming/lib \
    -lquml
unix:OBJECTS_DIR = .obj/
unix:MOC_DIR = .obj/
REL_PATH = ../lib/
include($${REL_PATH}lib.pri)
HEADERS += mainwindow.hpp \
    graphicsview/graphicsitemclass.hpp \
    graphicsview/graphicsitemconnection.hpp \
    graphicsview/graphicsitemconnectionpoint.hpp
FORMS += mainwindow.ui
