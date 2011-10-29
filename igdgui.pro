HEADERS     = igdgui.h igddevice.h addportmapping.h portmappingdetails.h
SOURCES     = igdgui.cpp main.cpp igddevice.cpp addportmapping.cpp portmappingdetails.cpp
FORMS       = igdgui.ui addportmapping.ui portmappingdetails.ui

# install
target.path = igdgui
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro
sources.path = .
INSTALLS += target sources
LIBS += -lminiupnpc
#CONFIG += debug
