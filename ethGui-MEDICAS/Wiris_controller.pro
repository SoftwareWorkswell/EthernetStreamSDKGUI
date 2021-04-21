QT += quick
QT += qml
CONFIG += c++11
CONFIG += resources_big

# target .exe name
TARGET = "MEDICAS Ethernet Stream SDK GUI"

# applicaton version
VERSION = 0.1.2

# maximum time, for which logs will be kept (currently 7 days)
DEFINES += "KEEP_LOG_DAYS=7"

# force to require administrator privilege - use only when compiling for final Windows installer
#QMAKE_LFLAGS_WINDOWS += "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""

# target .exe icon
RC_ICONS += "$$_PRO_FILE_PWD_/img/icon.ico"


# export name and version to c++ defines
DEFINES += "APP_NAME=\"\\\"$$TARGET\\\"\""
DEFINES += "APP_VERSION=\"\\\"$$VERSION\\\"\""



win32 {
    GstreamerDir= "$$_PRO_FILE_PWD_/libs/gstreamer/1.0/x86_64"
    INCLUDEPATH += "$$_PRO_FILE_PWD_/libs/gstreamer/1.0/x86_64/lib"
    INCLUDEPATH += "$$_PRO_FILE_PWD_/libs/gstreamer/1.0/x86_64/bin"
    INCLUDEPATH += "$$_PRO_FILE_PWD_/libs/gstreamer/1.0/x86_64/lib/gstreamer-1.0"
    INCLUDEPATH += "$$_PRO_FILE_PWD_/libs/opencv/include"
    INCLUDEPATH += "$$_PRO_FILE_PWD_/libs/boost_1_70_0"

    # libs for release build
    Release:LIBS += -L"$$_PRO_FILE_PWD_/libs/opencv/x64/vc15/lib" -lopencv_world411 -lopencv_img_hash411
    Release:LIBS += -L"$$_PRO_FILE_PWD_/libs/boost_1_70_0/lib64-msvc-14.1" -lboost_date_time-vc141-mt-x64-1_70


    # libs for debug build
    Debug:LIBS += -L"$$_PRO_FILE_PWD_/libs/opencv/x64/vc15/lib" -lopencv_world411d
    Debug:LIBS += -L"$$_PRO_FILE_PWD_/libs/opencv/x64/vc15/lib" -lopencv_img_hash411d
    Debug:LIBS += -L"$$_PRO_FILE_PWD_/libs/boost_1_70_0/lib64-msvc-14.1" -lboost_date_time-vc141-mt-gd-x64-1_70
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

HEADERS += \
    src/cameraconstants.h \
    src/cameraparameters.h \
    src/captureparameters.h \
    src/connectionthread.h \
    src/controllercore.h \
    src/customtimer.h \
    src/customtools.h \
    src/diowrapper.h \
    src/helperthread.h \
    src/networkclient.h \
    src/palettevaluewrapper.h \
    src/palettewrapper.h \
    src/protocol.h \
    src/streamframespropvider.h \
    src/streamframesprovidervisible.h \
    src/streamthread.h \
    src/thermalthread.h \
    src/translator.h \
    src/usersettings.h \
    src/visiblethread.h

SOURCES += \
        src/connectionthread.cpp \
        src/controllercore.cpp \
        src/customtimer.cpp \
        src/diowrapper.cpp \
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
        src/translator.cpp \
        src/usersettings.cpp \
        src/visiblethread.cpp

RESOURCES += qml.qrc

TRANSLATIONS = translations/ts_cz.ts

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

