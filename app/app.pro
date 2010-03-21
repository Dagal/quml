SOURCES += main.cpp \
    mainwindow.cpp \
    graphicsview/graphicsitemconnection.cpp \
    graphicsview/graphicsitemconnectionpoint.cpp \
    graphicsview/graphicsitemconnectionline.cpp \
    test.cpp
INCLUDEPATH = ../lib/
LIBS += -L$(HOME)/programming/lib \
    -lquml
unix:OBJECTS_DIR = .obj/
unix:MOC_DIR = .obj/
REL_PATH = ../lib/
include($${REL_PATH}lib.pri)
HEADERS += mainwindow.hpp \
    graphicsview/graphicsitemconnection.hpp \
    graphicsview/graphicsitemconnectionpoint.hpp \
    graphicsview/graphicsitemconnectionline.hpp \
    util/iextendedgraphicsitem.hpp \
    test.hpp
FORMS += mainwindow.ui
