TEMPLATE = lib
CONFIG += dll
TARGET = quml
unix:OBJECTS_DIR = .obj/
unix:MOC_DIR = .obj/
DESTDIR = /home/tfannes/programming/lib/
HEADERS += model/elementobject.hpp \
    model/_elementobject.hpp \
    model/umldiagram.hpp \
    model/_umldiagram.hpp \
    model/defines.hpp \
    model/packageobject.hpp \
    util/algorithm.hpp \
    util/elementcontainer.hpp \
    model/classobject.hpp \
    model/_classobject.hpp \
    model/propertyobject.hpp \
    model/operationobject.hpp \
    model/parameterobject.hpp \
    model/_parameterobject.hpp \
    model/datatypeobject.hpp \
    model/elementhelper.hpp \
    model/_elementhelper.hpp \
    model/methodobject.hpp \
    model/visibility.hpp \
    model/_methodobject.hpp \
    model/_propertyobject.hpp \
    model/elementcontainer.hpp \
    model/_operationobject.hpp \
    control/classdiagramcontroller.hpp \
    util/singleton.hpp \
    control/controller.hpp \
    control/_controller.hpp \
    control/interface/iclassdiagramcontroller.hpp \
    control/interface/idiagramcontroller.hpp \
    control/interface/iaction.hpp \
    control/interface/ilistener.hpp \
    control/interface/_idiagramcontroller.hpp \
    control/action/classaction.hpp \
    util/macro.hpp \
    control/action/_action.hpp
SOURCES += model/elementobject.cpp \
    model/umldiagram.cpp \
    model/packageobject.cpp \
    model/classobject.cpp \
    model/propertyobject.cpp \
    model/operationobject.cpp \
    model/parameterobject.cpp \
    model/datatypeobject.cpp \
    model/methodobject.cpp \
    model/elementhelper.cpp \
    control/classdiagramcontroller.cpp \
    control/controller.cpp \
    control/interface/idiagramcontroller.cpp \
    control/action/classaction.cpp
REL_PATH = 
include($${REL_PATH}lib.pri)
