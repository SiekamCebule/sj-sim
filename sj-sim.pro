QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    UI/DatabaseEditor/DatabaseEditorWindow.cpp \
    UI/DatabaseEditor/DatabaseListItemWidget.cpp \
    UI/SingleJumps/SingleJumpsConfigWindow.cpp \
    global/GlobalDatabase.cpp \
    main.cpp \
    UI/mainwindow.cpp \
    simulator/Characteristic.cpp \
    simulator/ConditionsInfo.cpp \
    simulator/Hill.cpp \
    simulator/JumpData.cpp \
    simulator/JumpMistake.cpp \
    simulator/JumpSimulator.cpp \
    simulator/Jumper.cpp \
    simulator/JumperSkills.cpp \
    simulator/Landing.cpp \
    simulator/Wind.cpp \
    simulator/wind-generation/WindGenerationSettings.cpp \
    simulator/wind-generation/WindsGenerator.cpp \
    utilities/ClassWithCharacteristics.cpp \
    utilities/ClassWithID.cpp \
    utilities/IDGenerator.cpp \
    utilities/MyRandom.cpp \
    utilities/functions.cpp

HEADERS += \
    UI/DatabaseEditor/DatabaseEditorWindow.h \
    UI/DatabaseEditor/DatabaseListItemWidget.h \
    UI/SingleJumps/SingleJumpsConfigWindow.h \
    UI/mainwindow.h \
    global/GlobalDatabase.h \
    simulator/Characteristic.h \
    simulator/ConditionsInfo.h \
    simulator/Hill.h \
    simulator/JumpData.h \
    simulator/JumpMistake.h \
    simulator/JumpSimulator.h \
    simulator/Jumper.h \
    simulator/JumperSkills.h \
    simulator/Landing.h \
    simulator/Wind.h \
    simulator/wind-generation/WindGenerationSettings.h \
    simulator/wind-generation/WindsGenerator.h \
    utilities/ClassWithCharacteristics.h \
    utilities/ClassWithID.h \
    utilities/IDGenerator.h \
    utilities/MyRandom.h \
    utilities/functions.h

FORMS += \
    UI/DatabaseEditor/DatabaseEditorWindow.ui \
    UI/DatabaseEditor/DatabaseListItemWidget.ui \
    UI/SingleJumps/SingleJumpsConfigWindow.ui \
    UI/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

copydata.commands = $(COPY_DIR) $$PWD/userData $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

DISTFILES += \
    flaticon_authors \
    userData/GlobalDatabase/globalJumpers.json

RESOURCES += \
    assets.qrc
