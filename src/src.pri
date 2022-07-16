
SUBDIRS += Qt3DScene.pro

HEADERS += $$PWD/mainwindow.h \
    $$PWD/3dwindow.h \
    $$PWD/componentmanager.h \
    $$PWD/ecstruct.h \
    $$PWD/scenemanager.h \
    $$PWD/utility.h

SOURCES += $$PWD/main.cpp \
           $$PWD/3dwindow.cpp \
           $$PWD/componentmanager.cpp \
           $$PWD/mainwindow.cpp \
           $$PWD/scenemanager.cpp \
           $$PWD/utility.cpp

RESOURCES +=

include(components/components.pri)
include(extrasentity/extrasentity.pri)
