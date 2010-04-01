SOURCES += main.cpp \
    mainwindow.cpp \
    graphicsview/connection/graphicsitemconnection.cpp \
    graphicsview/connection/graphicsitemconnectionpoint.cpp \
    graphicsview/connection/graphicsitemconnectionline.cpp \
    util/iextendedgraphicsitem.cpp \
    graphicsview/graphicsitemclass.cpp \
    graphicsview/graphicsitemrelation.cpp

unix:OBJECTS_DIR = .obj/
unix:MOC_DIR = .obj/
REL_PATH = ../lib/
include($${REL_PATH}lib.pri)
HEADERS += mainwindow.hpp \
    graphicsview/connection/graphicsitemconnection.hpp \
    graphicsview/connection/graphicsitemconnectionpoint.hpp \
    graphicsview/connection/graphicsitemconnectionline.hpp \
    util/iextendedgraphicsitem.hpp \
    util/iextendedgraphicsitem.hpp \
    graphicsview/graphicsitemclass.hpp \
    graphicsview/graphicsitemrelation.hpp
FORMS += mainwindow.ui
