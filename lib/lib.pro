TEMPLATE = lib
CONFIG += dll
TARGET = quml
unix:OBJECTS_DIR = .obj/
unix:MOC_DIR = .obj/
DESTDIR = /home/tfannes/programming/lib/
HEADERS += control/command/event.hpp \
    control/command/error.hpp \
    control/command/command.hpp \
    control/command/classcommand.hpp \
    control/command/_command.hpp \
    control/controller/diagramcontroller.hpp \
    control/controller/controller.hpp \
    control/controller/classdiagramcontroller.hpp \
    control/controller/_diagramcontroller.hpp \
    control/controller/_controller.hpp \
    control/interface/ilistener.hpp \
    model/visibility.hpp \
    model/umldiagramhelper.hpp \
    model/umldiagram.hpp \
    model/propertyobject.hpp \
    model/parameterobject.hpp \
    model/packageobject.hpp \
    model/operationobject.hpp \
    model/methodobject.hpp \
    model/elementobject.hpp \
    model/elementhelper.hpp \
    model/elementcontainer.hpp \
    model/defines.hpp \
    model/datatypeobject.hpp \
    model/classobject.hpp \
    model/_umldiagram.hpp \
    model/_propertyobject.hpp \
    model/_parameterobject.hpp \
    model/_operationobject.hpp \
    model/_methodobject.hpp \
    model/_elementobject.hpp \
    model/_elementhelper.hpp \
    model/_classobject.hpp \
    util/singleton.hpp \
    util/macro.hpp \
    util/algorithm.hpp \
    control/command/elementcommand.hpp
SOURCES += control/command/command.cpp \
    control/command/classcommand.cpp \
    control/command/_command.cpp \
    control/controller/diagramcontroller.cpp \
    control/controller/controller.cpp \
    control/controller/classdiagramcontroller.cpp \
    model/umldiagram.cpp \
    model/propertyobject.cpp \
    model/parameterobject.cpp \
    model/packageobject.cpp \
    model/operationobject.cpp \
    model/methodobject.cpp \
    model/elementobject.cpp \
    model/elementhelper.cpp \
    model/datatypeobject.cpp \
    model/classobject.cpp \
    control/command/elementcommand.cpp
REL_PATH = 
include($${REL_PATH}lib.pri)
