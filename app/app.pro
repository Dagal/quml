SOURCES += main.cpp \
    mainwindow.cpp \
    graphicsview/connection/graphicsitemconnection.cpp \
    graphicsview/connection/graphicsitemconnectionpoint.cpp \
    graphicsview/connection/graphicsitemconnectionline.cpp \
    util/graphicsext.cpp \
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
    util/graphicsext.hpp \
    graphicsview/graphicsitemclass.hpp \
    graphicsview/graphicsitemrelation.hpp \
	graphicsview/_graphicsitemclass.hpp
FORMS += mainwindow.ui
