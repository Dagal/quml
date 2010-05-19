TEMPLATE=subdirs
SUBDIRS= lib \
	app \
	test
CONFIG += ordered
contains(PROFILE, true):QMAKE_CXXFLAGS += -00 -g
