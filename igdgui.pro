HEADERS     = igdgui.h igddevice.h addportmapping.h
SOURCES     = igdgui.cpp main.cpp igddevice.cpp addportmapping.cpp
FORMS       = igdgui.ui addportmapping.ui

# install
target.path = igdgui
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro
sources.path = .
INSTALLS += target sources
LIBS += -lminiupnpc
