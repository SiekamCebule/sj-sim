QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    source/UI/Competition/CompetitionManagerWindow.cpp \
    source/UI/Competition/Results/ResultsTableModel.cpp \
    source/UI/Competition/StartList/CompetitionStartListDisplayItemWidget.cpp \
    source/UI/Competition/StartList/CompetitionStartListDisplayWidget.cpp \
    source/UI/Competition/StartList/StartListModel.cpp \
    source/UI/EditorWidgets/CompetitionRulesEditorWidget.cpp \
    source/UI/EditorWidgets/OtherCompetitionSettingsWidget.cpp \
    source/UI/EditorWidgets/RoundInfoEditorWidget.cpp \
    source/UI/EditorWidgets/TeamsEditing/TeamEditorWidget.cpp \
    source/UI/EditorWidgets/WindsGeneratorSettingsEditorWidget.cpp \
    source/UI/EditorWidgets/WindsGeneratorSettingsWidgetInputItem.cpp \
    source/UI/ResultsShowing/JumpDataDetailedInfoWindow.cpp \
    source/UI/Competition/CompetitionConfigWindow.cpp \
    source/UI/SingleJumps/SingleJumpMiniResultWidget.cpp \
    source/UI/SingleJumps/SingleJumpsConfigWindow.cpp \
    source/UI/AppSettings/AppSettingsWindow.cpp \
    source/UI/DatabaseEditor/DatabaseEditorWindow.cpp \
    source/UI/DatabaseEditor/DatabaseListItemWidget.cpp \
    source/UI/EditorWidgets/CharacteristicsEditor.cpp \
    source/UI/EditorWidgets/Dialogs/CharacteristicInputDialog.cpp \
    source/UI/EditorWidgets/HillEditorWidget.cpp \
    source/UI/EditorWidgets/JumperEditorWidget.cpp \
    source/UI/SingleJumps/SingleJumpsResultsWindow.cpp \
    source/competitions/AbstractCompetitionManager.cpp \
    source/competitions/AbstractCompetitionResults.cpp \
    source/competitions/AbstractCompetitionSingleResult.cpp \
    source/competitions/CompetitionInfo.cpp \
    source/competitions/CompetitionResults.cpp \
    source/competitions/CompetitionRules.cpp \
    source/competitions/IndividualCompetitions/IndividualCompetitionManager.cpp \
    source/competitions/IndividualCompetitions/IndividualCompetitionResults.cpp \
    source/competitions/IndividualCompetitions/IndividualCompetitionSingleResult.cpp \
    source/competitions/RoundInfo.cpp \
    source/global/CountryFlagsManager.cpp \
    source/global/GlobalAppSettings.cpp \
    source/global/GlobalDatabase.cpp \
    source/global/GlobalSimulationSettings.cpp \
    source/global/GlobalTranslators.cpp \
    source/main.cpp \
    source/UI/mainwindow.cpp \
    source/simulator/Characteristic.cpp \
    source/simulator/Hill.cpp \
    source/simulator/JumpData.cpp \
    source/simulator/JumpManipulator.cpp \
    source/simulator/JumpSimulationData.cpp \
    source/simulator/JumpSimulator.cpp \
    source/simulator/Jumper.cpp \
    source/simulator/JumperSkills.cpp \
    source/simulator/Landing.cpp \
    source/simulator/Team.cpp \
    source/simulator/Wind.cpp \
    source/simulator/WindsCalculator.cpp \
    source/simulator/wind-generation/WindGenerationSettings.cpp \
    source/simulator/wind-generation/WindsGenerator.cpp \
    source/single-jumps/SingleJumpsManager.cpp \
    source/utilities/ClassWithCharacteristics.cpp \
    source/utilities/ClassWithID.cpp \
    source/global/IDGenerator.cpp \
    source/global/MyRandom.cpp \
    source/utilities/functions.cpp

HEADERS += \
    source/UI/Competition/CompetitionManagerWindow.h \
    source/UI/Competition/Results/ResultsTableModel.h \
    source/UI/Competition/StartList/CompetitionStartListDisplayItemWidget.h \
    source/UI/Competition/StartList/CompetitionStartListDisplayWidget.h \
    source/UI/Competition/StartList/StartListModel.h \
    source/UI/EditorWidgets/CompetitionRulesEditorWidget.h \
    source/UI/EditorWidgets/OtherCompetitionSettingsWidget.h \
    source/UI/EditorWidgets/RoundInfoEditorWidget.h \
    source/UI/EditorWidgets/TeamsEditing/TeamEditorWidget.h \
    source/UI/EditorWidgets/WindsGeneratorSettingsEditorWidget.h \
    source/UI/EditorWidgets/WindsGeneratorSettingsWidgetInputItem.h \
    source/UI/ResultsShowing/JumpDataDetailedInfoWindow.h \
    source/UI/Competition/CompetitionConfigWindow.h \
    source/UI/SingleJumps/SingleJumpMiniResultWidget.h \
    source/UI/SingleJumps/SingleJumpsConfigWindow.h \
    source/UI/AppSettings/AppSettingsWindow.h \
    source/UI/DatabaseEditor/DatabaseEditorWindow.h \
    source/UI/DatabaseEditor/DatabaseListItemWidget.h \
    source/UI/EditorWidgets/CharacteristicsEditor.h \
    source/UI/EditorWidgets/Dialogs/CharacteristicInputDialog.h \
    source/UI/EditorWidgets/HillEditorWidget.h \
    source/UI/EditorWidgets/JumperEditorWidget.h \
    source/UI/SingleJumps/SingleJumpsResultsWindow.h \
    source/UI/mainwindow.h \
    source/competitions/AbstractCompetitionManager.h \
    source/competitions/AbstractCompetitionResults.h \
    source/competitions/AbstractCompetitionSingleResult.h \
    source/competitions/CompetitionInfo.h \
    source/competitions/CompetitionResults.h \
    source/competitions/CompetitionRules.h \
    source/competitions/IndividualCompetitions/IndividualCompetitionManager.h \
    source/competitions/IndividualCompetitions/IndividualCompetitionResults.h \
    source/competitions/IndividualCompetitions/IndividualCompetitionSingleResult.h \
    source/competitions/RoundInfo.h \
    source/global/CountryFlagsManager.h \
    source/global/GlobalAppSettings.h \
    source/global/GlobalDatabase.h \
    source/global/GlobalSimulationSettings.h \
    source/global/GlobalTranslators.h \
    source/simulator/Characteristic.h \
    source/simulator/Hill.h \
    source/simulator/JumpData.h \
    source/simulator/JumpManipulator.h \
    source/simulator/JumpSimulationData.h \
    source/simulator/JumpSimulator.h \
    source/simulator/Jumper.h \
    source/simulator/JumperSkills.h \
    source/simulator/Landing.h \
    source/simulator/Team.h \
    source/simulator/Wind.h \
    source/simulator/WindsCalculator.h \
    source/simulator/wind-generation/WindGenerationSettings.h \
    source/simulator/wind-generation/WindsGenerator.h \
    source/single-jumps/SingleJumpsManager.h \
    source/utilities/ClassWithCharacteristics.h \
    source/utilities/ClassWithID.h \
    source/global/IDGenerator.h \
    source/global/MyRandom.h \
    source/utilities/functions.h

FORMS += \
    source/UI/Competition/CompetitionManagerWindow.ui \
    source/UI/Competition/StartList/CompetitionStartListDisplayItemWidget.ui \
    source/UI/Competition/StartList/CompetitionStartListDisplayWidget.ui \
    source/UI/EditorWidgets/CompetitionRulesEditorWidget.ui \
    source/UI/EditorWidgets/OtherCompetitionSettingsWidget.ui \
    source/UI/EditorWidgets/RoundInfoEditorWidget.ui \
    source/UI/EditorWidgets/TeamsEditing/TeamEditorWidget.ui \
    source/UI/EditorWidgets/WindsGeneratorSettingsEditorWidget.ui \
    source/UI/EditorWidgets/WindsGeneratorSettingsWidgetInputItem.ui \
    source/UI/ResultsShowing/JumpDataDetailedInfoWindow.ui \
    source/UI/Competition/CompetitionConfigWindow.ui \
    source/UI/SingleJumps/SingleJumpMiniResultWidget.ui \
    source/UI/SingleJumps/SingleJumpsConfigWindow.ui \
    source/UI/AppSettings/AppSettingsWindow.ui \
    source/UI/DatabaseEditor/DatabaseEditorWindow.ui \
    source/UI/DatabaseEditor/DatabaseListItemWidget.ui \
    source/UI/EditorWidgets/CharacteristicsEditor.ui \
    source/UI/EditorWidgets/Dialogs/CharacteristicInputDialog.ui \
    source/UI/EditorWidgets/HillEditorWidget.ui \
    source/UI/EditorWidgets/JumperEditorWidget.ui \
    source/UI/SingleJumps/SingleJumpsResultsWindow.ui \
    source/UI/mainwindow.ui

TRANSLATIONS += \
    translations/translation_en.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#copydata.commands = $(COPY_DIR) $$PWD/translations $$PWD/flags $$OUT_PWD
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
