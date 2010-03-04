TEMPLATE = lib
CONFIG += dll
TARGET = quml
unix:OBJECTS_DIR = .obj/
unix:MOC_DIR = .obj/
DESTDIR = /home/tfannes/programming/lib/
HEADERS += model/element/umldiagram.hpp \
    model/element/elementobject.hpp \
    model/element/elementhelper.hpp \
    model/element/_elementobject.hpp \
    model/element/_umldiagram.hpp \
    util/singleton.hpp \
    util/macro.hpp \
    util/algorithm.hpp \
    control/command/error.hpp \
    model/element/_elementrelator.hpp \
    model/element/visibility.hpp \
    model/element/propertyobject.hpp \
    model/element/parameterobject.hpp \
    model/element/packageobject.hpp \
    model/element/operationobject.hpp \
    model/element/methodobject.hpp \
    model/element/defines.hpp \
    model/element/datatypeobject.hpp \
    model/element/classobject.hpp \
    model/element/_propertyobject.hpp \
    model/element/_parameterobject.hpp \
    model/element/_operationobject.hpp \
    model/element/_methodobject.hpp \
    model/element/_classobject.hpp \
    model/element/elementcontainer.hpp \
    model/element/primitiveobject.hpp \
    control/controller/diagramcontroller.hpp \
    control/controller/_diagramcontroller.hpp \
    control/interface/ilistener.hpp \
    control/command/event.hpp \
    control/controller/classdiagramcontroller.hpp \
    control/controller/_classdiagramcontroller.hpp
SOURCES += model/element/umldiagram.cpp \
    model/element/elementobject.cpp \
    model/element/elementhelper.cpp \
    model/element/_elementrelator.cpp \
    model/element/propertyobject.cpp \
    model/element/parameterobject.cpp \
    model/element/packageobject.cpp \
    model/element/operationobject.cpp \
    model/element/methodobject.cpp \
    model/element/datatypeobject.cpp \
    model/element/classobject.cpp \
    model/element/primitiveobject.cpp \
    control/controller/diagramcontroller.cpp \
    control/controller/classdiagramcontroller.cpp
REL_PATH = 
include($${REL_PATH}lib.pri)
