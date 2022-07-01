













include(meshsetting/meshsetting.pri)
include(materialsetting/materialsetting.pri)

HEADERS += \
    $$PWD/abstractcomponentssettingwidget.h \
    $$PWD/layout.h \
    $$PWD/materialsetting.h \
    $$PWD/meshsetting.h \
    $$PWD/transformsetting.h

SOURCES += \
    $$PWD/abstractcomponentssettingwidget.cpp \
    $$PWD/layout.cpp \
    $$PWD/materialsetting.cpp \
    $$PWD/meshsetting.cpp \
    $$PWD/transformsetting.cpp
