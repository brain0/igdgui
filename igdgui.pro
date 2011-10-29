HEADERS     = igdgui.h igddevice.h
SOURCES     = igdgui.cpp main.cpp igddevice.cpp
FORMS       = igdgui.ui

# install
target.path = igdgui
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro
sources.path = .
INSTALLS += target sources
LIBS += -lminiupnpc
