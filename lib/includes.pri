MY_INCLUDEPATH += model \
    model/element \
    control/command \
    control/controller \
    control/interface \
    util
for(dir, MY_INCLUDEPATH):INCLUDEPATH += $${REL_PATH}$${dir}
