include(includes.pri)
LIBS += ../lib/libquml.a
POST_TARGETDEPS += ../lib/libquml.a
contains(PROFILE, true):QMAKE_CXXFLAGS += -o0 -g
