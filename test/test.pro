unix:OBJECTS_DIR = .obj/
unix:MOC_DIR = .moc/
SOURCES += main.cpp \
    elementdiagramtest.cpp \
    elementrelatortest.cpp
HEADERS += elementdiagramtest.hpp \
    elementrelatortest.hpp
REL_PATH = ../lib/
include($${REL_PATH}lib.pri)
QT += testlib
