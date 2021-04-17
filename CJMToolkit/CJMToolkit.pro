QT       += core gui xml widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    common/data/BaseSettings.cpp \
    common/data/LogMsg.cpp \
    common/data/Version.cpp \
    common/io/Log.cpp \
    common/qt/ButtonSelector.cpp \
    common/qt/InfoDisplay.cpp \
    common/qt/Settings.cpp \
    common/qt/StateButton.cpp \
    main.cpp \
    MainWindow.cpp \
    panels/DebugInfoPanel.cpp \
    panels/DebugPanel.cpp

HEADERS += \
    MainWindow.hpp \
    common/algorithm/utility.hpp \
    common/data/BaseSettings.hpp \
    common/data/CircularQueue.hpp \
    common/data/LogMsg.hpp \
    common/data/Version.hpp \
    common/io/Log.hpp \
    common/qt/ButtonSelector.hpp \
    common/qt/InfoDisplay.hpp \
    common/qt/Settings.hpp \
    common/qt/StateButton.hpp \
    common/version_info.hpp \
    panels/DebugInfoPanel.hpp \
    panels/DebugPanel.hpp \
    version_info.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../bin/config/base_style_sheet.qss \
    ../bin/config/settings.xml \
    .clang-format
