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
    src/ItemFile.cpp \
    src/ItemFolder.cpp \
    src/NkfHelper.cpp \
    src/ContextMenu.cpp \
    src/utils.cpp \
    src/ActionFile.cpp \
    src/ActionFolder.cpp \
    src/ui/UICommandBar.cpp \
    src/ui/UIConfigIgnoreFolder.cpp \
    src/ui/UIConfigSuffixEdit.cpp \
    src/ui/UIDialogConfig.cpp \
    src/ui/UIDialogInfo.cpp \
    src/ui/UIMainWindow.cpp

HEADERS += \
    src/ItemFile.h \
    src/ItemFolder.h \
    src/revinfo.inc \
    src/App.h \
    src/Config.h \
    src/NkfHelper.h \
    src/ContextMenu.h \
    src/utils.h \
    src/ActionFile.h \
    src/ActionFolder.h \
    src/nkf32.h \
    src/ui/UICommandBar.h \
    src/ui/UIConfigIgnoreFolder.h \
    src/ui/UIConfigSuffixEdit.h \
    src/ui/UIDialogConfig.h \
    src/ui/UIDialogInfo.h \
    src/ui/UIMainWindow.h

FORMS += \
    src/ui/UICommandBar.ui \
    src/ui/UIConfigIgnoreFolder.ui \
    src/ui/UIConfigSuffixEdit.ui \
    src/ui/UIDialogConfig.ui \
    src/ui/UIDialogInfo.ui \
    src/ui/UIMainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
