TEMPLATE = lib
CONFIG += dll
TARGET = quml
unix:OBJECTS_DIR = .obj/
unix:MOC_DIR = .obj/
DESTDIR = /home/tfannes/programming/lib/
HEADERS +=  \
	 model/element/umldiagram.hpp \
	model/element/elementobject.hpp \
	model/element/_umldiagram.hpp \
	util/singleton.hpp \
    util/macro.hpp \
	util/algorithm.hpp
SOURCES +=   model/element/umldiagram.cpp \
	model/element/elementobject.cpp
REL_PATH = 
include($${REL_PATH}lib.pri)
