TEMPLATE = lib
CONFIG += dll
TARGET = quml
unix:OBJECTS_DIR = .obj/
unix:MOC_DIR = .obj/
HEADERS += model/elementobject.hpp \
    model/_elementobject.hpp \
    model/classobject.hpp \
    model/propertyobject.hpp \
    model/operationobject.hpp \
    model/parameterobject.hpp \
    model/_classobject.hpp \
    model/_parameterobject.hpp \
    model/datatypeobject.hpp \
    model/classdiagram.hpp \
    model/defines.hpp \
    model/_classdiagram.hpp
SOURCES += model/elementobject.cpp \
    model/classobject.cpp \
    model/propertyobject.cpp \
    model/operationobject.cpp \
    model/parameterobject.cpp \
    model/datatypeobject.cpp \
    model/classdiagram.cpp
REL_PATH = 
include($${REL_PATH}qumllib.pri)
