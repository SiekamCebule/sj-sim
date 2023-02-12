QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    simulation/Characteristic.cpp \
    simulation/ConditionsInfo.cpp \
    simulation/Hill.cpp \
    simulation/JumpData.cpp \
    simulation/JumpMistake.cpp \
    simulation/JumpSimulator.cpp \
    simulation/Jumper.cpp \
    simulation/JumperSkills.cpp \
    simulation/SimulationGlobals.cpp \
    simulation/Wind.cpp \
    utilities/ClassWithCharacteristics.cpp \
    utilities/ClassWithID.cpp \
    utilities/IDGenerator.cpp \
    utilities/MyRandom.cpp \
    utilities/WindsGenerator.cpp \
    utilities/functions.cpp

HEADERS += \
    mainwindow.h \
    simulation/Characteristic.h \
    simulation/ConditionsInfo.h \
    simulation/Hill.h \
    simulation/JumpData.h \
    simulation/JumpMistake.h \
    simulation/JumpSimulator.h \
    simulation/Jumper.h \
    simulation/JumperSkills.h \
    simulation/SimulationGlobals.h \
    simulation/Wind.h \
    utilities/ClassWithCharacteristics.h \
    utilities/ClassWithID.h \
    utilities/IDGenerator.h \
    utilities/MyRandom.h \
    utilities/WindsGenerator.h \
    utilities/functions.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
