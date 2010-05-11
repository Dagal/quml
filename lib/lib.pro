TEMPLATE = lib
CONFIG += static
TARGET = quml
unix:OBJECTS_DIR = .obj/
unix:MOC_DIR = .obj/
HEADERS += model/element/umldiagram.hpp \
    model/element/elementobject.hpp \
    model/element/elementhelper.hpp \
    model/element/_elementobject.hpp \
    model/element/_umldiagram.hpp \
    util/singleton.hpp \
    util/macro.hpp \
    util/algorithm.hpp \
    model/element/_elementrelator.hpp \
    model/element/visibility.hpp \
    model/element/propertyobject.hpp \
    model/element/parameter.hpp \
    model/element/packageobject.hpp \
    model/element/operationobject.hpp \
    model/element/methodobject.hpp \
    model/element/element.hpp \
    model/element/datatypeobject.hpp \
    model/element/classobject.hpp \
    model/element/_propertyobject.hpp \
    model/element/_operationobject.hpp \
    model/element/_methodobject.hpp \
    model/element/_classobject.hpp \
    model/element/elementcontainer.hpp \
    model/element/primitiveobject.hpp \
    control/interface/ilistener.hpp \
    control/interface/icommand.hpp \
    control/controller/classdiagramcontroller.hpp \
    control/controller/controller.hpp \
    util/notifier.hpp \
    control/controller/classdiagramrules.hpp \
    control/interface/iclassdiagramrules.hpp \
    control/controller/command.hpp
SOURCES += model/element/umldiagram.cpp \
    model/element/elementobject.cpp \
    model/element/elementhelper.cpp \
    model/element/_elementrelator.cpp \
    model/element/propertyobject.cpp \
    model/element/packageobject.cpp \
    model/element/operationobject.cpp \
    model/element/methodobject.cpp \
    model/element/datatypeobject.cpp \
    model/element/classobject.cpp \
    model/element/primitiveobject.cpp \
    control/controller/classdiagramcontroller.cpp \
    control/controller/controller.cpp \
    control/controller/classdiagramrules.cpp \
    control/controller/command.cpp
REL_PATH = 
include($${REL_PATH}lib.pri)
