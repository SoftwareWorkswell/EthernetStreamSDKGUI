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
        src/protocol.cpp \
        src/streamframespropvider.cpp \
        src/streamframesprovidervisible.cpp \
        src/streamthread.cpp \
        src/thermalthread.cpp \
        src/visiblethread.cpp

RESOURCES += qml.qrc

GstreamerDir=$$(GSTREAMER_1_0_ROOT_X86_64)



win32 {
    INCLUDEPATH += "C:\lib\Install\opencv\include"
    INCLUDEPATH += C:\local\boost_1_70_0

    LIBS += -L"C:\lib\Install\opencv\x64\vc15\lib" -lopencv_world411
    LIBS += -L"C:\lib\Install\opencv\x64\vc15\lib" -lopencv_img_hash411
    LIBS += -LC:\local\boost_1_70_0\lib64-msvc-14.1\ -lboost_date_time-vc141-mt-x64-1_70
}
macx {
    INCLUDEPATH += "$$_PRO_FILE_PWD_/libs/opencv/headers/"
    INCLUDEPATH += "/Users/Workswell/Matej/Dev/Boost_1_70_0"

    LIBS +=  -L"$$_PRO_FILE_PWD_/libs/opencv/" -lopencv_superres
    LIBS +=  -L"$$_PRO_FILE_PWD_/libs/opencv/" -lopencv_core
    LIBS +=  -L"$$_PRO_FILE_PWD_/libs/opencv/" -lopencv_imgcodecs
    LIBS +=  -L"$$_PRO_FILE_PWD_/libs/opencv/" -lopencv_imgproc
    LIBS +=  -L"$$_PRO_FILE_PWD_/libs/opencv/" -lopencv_videoio
}

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
    src/cameraconstants.h \
    src/cameraparameters.h \
    src/captureparameters.h \
    src/controllercore.h \
    src/customtimer.h \
    src/customtools.h \
    src/helperthread.h \
    src/networkclient.h \
    src/palettevaluewrapper.h \
    src/palettewrapper.h \
    src/protocol.h \
    src/streamframespropvider.h \
    src/streamframesprovidervisible.h \
    src/streamthread.h \
    src/thermalthread.h \
    src/visiblethread.h
