SOURCES += main.cpp
INCLUDEPATH = ../lib/
LIBS += -L$(HOME)/programming/lib \
	-lquml
unix:OBJECTS_DIR = .obj/
unix:MOC_DIR = .obj/
REL_PATH = ../lib/
include($${REL_PATH}lib.pri)
