MY_INCLUDEPATH += model \
    view \
    view/graphicsview \
    control \
	control/interface \
	control/action \
    util
for(dir, MY_INCLUDEPATH):INCLUDEPATH += $${REL_PATH}$${dir}

