QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    UI/EditorWidgets/CompetitionRulesEditorWidget.cpp \
    UI/EditorWidgets/RoundInfoEditorWidget.cpp \
    UI/EditorWidgets/WindsGeneratorSettingsEditorWidget.cpp \
    UI/EditorWidgets/WindsGeneratorSettingsWidgetInputItem.cpp \
    UI/ResultsShowing/JumpDataDetailedInfoWindow.cpp \
    UI/SingleJumps/SingleJumpMiniResultWidget.cpp \
    UI/SingleJumps/SingleJumpsConfigWindow.cpp \
    UI/AppSettings/AppSettingsWindow.cpp \
    UI/DatabaseEditor/DatabaseEditorWindow.cpp \
    UI/DatabaseEditor/DatabaseListItemWidget.cpp \
    UI/EditorWidgets/CharacteristicsEditor.cpp \
    UI/EditorWidgets/Dialogs/CharacteristicInputDialog.cpp \
    UI/EditorWidgets/HillEditorWidget.cpp \
    UI/EditorWidgets/JumperEditorWidget.cpp \
    UI/SingleJumps/SingleJumpsResultsWindow.cpp \
    competitions/CompetitionRules.cpp \
    competitions/RoundInfo.cpp \
    global/CountryFlagsManager.cpp \
    global/GlobalAppSettings.cpp \
    global/GlobalDatabase.cpp \
    global/GlobalTranslators.cpp \
    main.cpp \
    UI/mainwindow.cpp \
    simulator/Characteristic.cpp \
    simulator/Hill.cpp \
    simulator/JumpData.cpp \
    simulator/JumpMistake.cpp \
    simulator/JumpSimulationData.cpp \
    simulator/JumpSimulator.cpp \
    simulator/Jumper.cpp \
    simulator/JumperSkills.cpp \
    simulator/Landing.cpp \
    simulator/Wind.cpp \
    simulator/WindsInfo.cpp \
    simulator/wind-generation/WindGenerationSettings.cpp \
    simulator/wind-generation/WindsGenerator.cpp \
    single-jumps/SingleJumpsManager.cpp \
    utilities/ClassWithCharacteristics.cpp \
    utilities/ClassWithID.cpp \
    global/IDGenerator.cpp \
    global/MyRandom.cpp \
    utilities/functions.cpp

HEADERS += \
    UI/EditorWidgets/CompetitionRulesEditorWidget.h \
    UI/EditorWidgets/RoundInfoEditorWidget.h \
    UI/EditorWidgets/WindsGeneratorSettingsEditorWidget.h \
    UI/EditorWidgets/WindsGeneratorSettingsWidgetInputItem.h \
    UI/ResultsShowing/JumpDataDetailedInfoWindow.h \
    UI/SingleJumps/SingleJumpMiniResultWidget.h \
    UI/SingleJumps/SingleJumpsConfigWindow.h \
    UI/AppSettings/AppSettingsWindow.h \
    UI/DatabaseEditor/DatabaseEditorWindow.h \
    UI/DatabaseEditor/DatabaseListItemWidget.h \
    UI/EditorWidgets/CharacteristicsEditor.h \
    UI/EditorWidgets/Dialogs/CharacteristicInputDialog.h \
    UI/EditorWidgets/HillEditorWidget.h \
    UI/EditorWidgets/JumperEditorWidget.h \
    UI/SingleJumps/SingleJumpsResultsWindow.h \
    UI/mainwindow.h \
    competitions/CompetitionRules.h \
    competitions/RoundInfo.h \
    global/CountryFlagsManager.h \
    global/GlobalAppSettings.h \
    global/GlobalDatabase.h \
    global/GlobalTranslators.h \
    simulator/Characteristic.h \
    simulator/Hill.h \
    simulator/JumpData.h \
    simulator/JumpMistake.h \
    simulator/JumpSimulationData.h \
    simulator/JumpSimulator.h \
    simulator/Jumper.h \
    simulator/JumperSkills.h \
    simulator/Landing.h \
    simulator/Wind.h \
    simulator/WindsInfo.h \
    simulator/wind-generation/WindGenerationSettings.h \
    simulator/wind-generation/WindsGenerator.h \
    single-jumps/SingleJumpsManager.h \
    utilities/ClassWithCharacteristics.h \
    utilities/ClassWithID.h \
    global/IDGenerator.h \
    global/MyRandom.h \
    utilities/functions.h

FORMS += \
    UI/EditorWidgets/CompetitionRulesEditorWidget.ui \
    UI/EditorWidgets/RoundInfoEditorWidget.ui \
    UI/EditorWidgets/WindsGeneratorSettingsEditorWidget.ui \
    UI/EditorWidgets/WindsGeneratorSettingsWidgetInputItem.ui \
    UI/ResultsShowing/JumpDataDetailedInfoWindow.ui \
    UI/SingleJumps/SingleJumpMiniResultWidget.ui \
    UI/SingleJumps/SingleJumpsConfigWindow.ui \
    UI/AppSettings/AppSettingsWindow.ui \
    UI/DatabaseEditor/DatabaseEditorWindow.ui \
    UI/DatabaseEditor/DatabaseListItemWidget.ui \
    UI/EditorWidgets/CharacteristicsEditor.ui \
    UI/EditorWidgets/Dialogs/CharacteristicInputDialog.ui \
    UI/EditorWidgets/HillEditorWidget.ui \
    UI/EditorWidgets/JumperEditorWidget.ui \
    UI/SingleJumps/SingleJumpsResultsWindow.ui \
    UI/mainwindow.ui

TRANSLATIONS += \
    translations/translation_en.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

copydata.commands = $(COPY_DIR) $$PWD/translations $$PWD/flags $$OUT_PWD
#copydata.commands = $(COPY_DIR) $$PWD/userData $$PWD/translations $$PWD/flags $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

DISTFILES += \
    flaticon_authors \
    flags_author \
    userData/GlobalDatabase/globalCompetitionsRules.json \
    userData/GlobalDatabase/globalHills.json \
    userData/GlobalDatabase/globalJumpers.json \
    userData/Settings/appSettings.json \
    README.md

RESOURCES += \
assets.qrc
