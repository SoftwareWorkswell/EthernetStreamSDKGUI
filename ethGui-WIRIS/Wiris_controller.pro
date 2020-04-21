QT += quick
CONFIG += c++11


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/controllercore.cpp \
        src/customtimer.cpp \
        src/helperthread.cpp \
        src/main.cpp \
        src/networkclient.cpp \
        src/palettevaluewrapper.cpp \
        src/palettewrapper.cpp \
        src/streamframespropvider.cpp \
        src/streamframesprovidervisible.cpp \
        src/streamthread.cpp \
        src/thermalthread.cpp \
        src/visiblethread.cpp

RESOURCES += qml.qrc

GstreamerDir=$$(GSTREAMER_1_0_ROOT_X86_64)

INCLUDEPATH += "/Users/Workswell/Matej/Dev/Boost_1_70_0"
INCLUDEPATH += "/usr/local/include/opencv4"
LIBS +=  -L/Users/Workswell/Matej/Dev/build2/lib -lopencv_core
LIBS +=  -L/Users/Workswell/Matej/Dev/build2/lib -lopencv_highgui
LIBS +=  -L/Users/Workswell/Matej/Dev/build2/lib -lopencv_imgproc
LIBS +=  -L/Users/Workswell/Matej/Dev/build2/lib -lopencv_videoio

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    C:/Users/Workswell 2/Pictures/range.PNG

HEADERS += \
    src/controllercore.h \
    src/customtimer.h \
    src/customtools.h \
    src/helperthread.h \
    src/networkclient.h \
    src/palettevaluewrapper.h \
    src/palettewrapper.h \
    src/streamframespropvider.h \
    src/streamframesprovidervisible.h \
    src/streamthread.h \
    src/thermalthread.h \
    src/visiblethread.h
