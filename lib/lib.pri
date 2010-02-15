MY_INCLUDEPATH += model \
    view \
    view/graphicsview \
    control \
	control/command \
    util
for(dir, MY_INCLUDEPATH):INCLUDEPATH += $${REL_PATH}$${dir}

