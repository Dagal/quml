MY_INCLUDEPATH += model \
	model/element \
	control/command \
    util
for(dir, MY_INCLUDEPATH):INCLUDEPATH += $${REL_PATH}$${dir}

