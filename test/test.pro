unix:OBJECTS_DIR = .obj/
unix:MOC_DIR = .moc/
SOURCES += main.cpp \
    elementdiagramtest.cpp
HEADERS += elementdiagramtest.hpp

REL_PATH = ../lib/
include($${REL_PATH}lib.pri)
QT += testlib
