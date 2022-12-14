QT += core gui widgets opengl 

TARGET = run
TEMPLATE = app

SOURCES += main.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp \
    joint.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    joint.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target
