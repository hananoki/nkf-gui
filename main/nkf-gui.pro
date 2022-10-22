QT       += core gui concurrent
win32:QT += winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += lrelease

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

PRECOMPILED_HEADER = src/pch.h

INCLUDEPATH += src src/ui ../Qtlib/src ../Qtlib/src/ui ../Qtlib/include

 win32:LIBS += -L../libs
 
CONFIG( debug, debug|release ) {
    # debug
    win32:LIBS += -L../Qtlib/debug -L../Qtlib/QtLibSound/debug
} else {
    # release
    win32:LIBS += -L../Qtlib/release -L../Qtlib/QtLibSound/release
}

LIBS += nkf32.lib Qtlib.lib

RESOURCES += \
  src/res.qrc

RC_ICONS = src/icon/f_f_health_50_s16_f_health_50_1bg.ico

SOURCES += \
    src/App.cpp \
    src/Config.cpp \
    src/NkfHelper.cpp \
    src/ui/UICommandBar.cpp \
    src/ui/UIConfigIgnoreFolder.cpp \
    src/ui/UIConfigSuffixEdit.cpp \
    src/ui/UIMainWindow.cpp \
    src/ui/UIMainCaption.cpp \
    src/ui/UIPanelConfig.cpp \
    src/ui/UIPanelInfo.cpp \
    src/ui/UIPanelMain.cpp

HEADERS += \
    src/App.h \
    src/Config.h \
    src/Enums.h \
    src/NkfHelper.h \
    src/nkf32.h \
    src/Item_File.h \
    src/IConfigPage.h \
    src/ui/UICommandBar.h \
    src/ui/UIConfigIgnoreFolder.h \
    src/ui/UIConfigSuffixEdit.h \
    src/ui/UIMainWindow.h \
    src/ui/UIMainCaption.h \
    src/ui/UIPanelConfig.h \
    src/ui/UIPanelInfo.h \
    src/ui/UIPanelMain.h

FORMS += \
    src/ui/UICommandBar.ui \
    src/ui/UIConfigIgnoreFolder.ui \
    src/ui/UIConfigSuffixEdit.ui \
    src/ui/UIMainWindow.ui \
    src/ui/UIMainCaption.ui \
    src/ui/UIPanelConfig.ui \
    src/ui/UIPanelInfo.ui \
    src/ui/UIPanelMain.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
