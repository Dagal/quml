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
# model/propertyobject.hpp \
# model/operationobject.hpp \
# model/parameterobject.hpp \
# model/_parameterobject.hpp \
    model/datatypeobject.hpp \
    util/sortedvector.hpp \
    model/elementhelper.hpp

# model/classdiagram.hpp \
# model/_classdiagram.hpp \
# model/methodobject.hpp \
# model/_elementnamerelator.hpp \
# model/visibility.hpp \
# model/_methodobject.hpp
SOURCES += model/elementobject.cpp \
    model/umldiagram.cpp \
    model/packageobject.cpp \
	model/classobject.cpp \
# model/propertyobject.cpp \
# model/operationobject.cpp \
# model/parameterobject.cpp \
    model/datatypeobject.cpp

# model/classdiagram.cpp \
# model/methodobject.cpp \
# model/_elementnamerelator.cpp
REL_PATH = 
include($${REL_PATH}lib.pri)
