MY_INCLUDEPATH += model \
    view \
    view/graphicsview \
    control \
	control/interface \
	control/event \
	control/controller \
	control/action \
	control/command \
    util
for(dir, MY_INCLUDEPATH):INCLUDEPATH += $${REL_PATH}$${dir}

