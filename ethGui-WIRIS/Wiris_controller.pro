QT += quick
CONFIG += c++11
CONFIG += resources_big

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
        src/graphic/palettewrapper.cpp \
        src/graphic/streamframesprovider.cpp \
        src/main.cpp \
        src/stream/camerastreamer.cpp \
        src/camera/camerautils.cpp \
        src/communication/connectionthread.cpp \
        src/communication/controllercore.cpp \
        src/graphic/controllercoreinterface.cpp \
        src/communication/helperthread.cpp \
        src/communication/networkclient.cpp \
        src/graphic/palettevaluewrapper.cpp \
        src/communication/protocol.cpp \
        src/graphic/streamframesprovidervisible.cpp \
        src/stream/thermalthread.cpp \
        src/stream/visiblethread.cpp

RESOURCES += qml.qrc

win32 {
    GstreamerDir= "$$_PRO_FILE_PWD_/libs/gstreamer/1.0/x86_64"
    INCLUDEPATH += "$$_PRO_FILE_PWD_/libs/gstreamer/1.0/x86_64/lib"
    INCLUDEPATH += "$$_PRO_FILE_PWD_/libs/gstreamer/1.0/x86_64/bin"
    INCLUDEPATH += "$$_PRO_FILE_PWD_/libs/gstreamer/1.0/x86_64/lib/gstreamer-1.0"
    INCLUDEPATH += "$$_PRO_FILE_PWD_/libs/opencv/include"
    INCLUDEPATH += "$$_PRO_FILE_PWD_/libs/boost_1_70_0"

    # libs for release build
    Release:LIBS += -L"$$_PRO_FILE_PWD_/libs/opencv/x64/vc15/lib" -lopencv_world411 -lopencv_img_hash411
    Release:LIBS += -L"$$_PRO_FILE_PWD_/libs/boost_1_70_0/lib64-msvc-14.1" -lboost_date_time-vc142-mt-x64-1_70

    # libs for debug build
    Debug:LIBS += -L"$$_PRO_FILE_PWD_/libs/opencv/x64/vc15/lib" -lopencv_world411d
    Debug:LIBS += -L"$$_PRO_FILE_PWD_/libs/opencv/x64/vc15/lib" -lopencv_img_hash411d
    Debug:LIBS += -L"$$_PRO_FILE_PWD_/libs/boost_1_70_0/lib64-msvc-14.1" -lboost_date_time-vc142-mt-gd-x64-1_70
}
macx {
    GstreamerDir=$$(GSTREAMER_1_0_ROOT_X86_64)
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

HEADERS += \
    src/camera/cameraconnection.h \
    src/camera/cameraconstants.h \
    src/camera/cameraparameters.h \
    src/stream/camerastreamer.h \
    src/camera/camerautils.h \
    src/communication/connectionthread.h \
    src/communication/controllercore.h \
    src/graphic/controllercoreinterface.h \
    src/graphic/customtimer.h \
    src/communication/helperthread.h \
    src/graphic/streamframesprovider.h \
    src/communication/networkclient.h \
    src/graphic/palettevaluewrapper.h \
    src/graphic/palettewrapper.h \
    src/communication/protocol.h \
    src/graphic/streamframesprovidervisible.h \
    src/stream/streamthread.h \
    src/stream/thermalthread.h \
    src/stream/visiblethread.h
