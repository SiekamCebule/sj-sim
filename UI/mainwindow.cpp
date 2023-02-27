#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../global/IDGenerator.h"
#include "../global/MyRandom.h"

#include "../utilities/functions.h"

#include "../simulator/Jumper.h"
#include "../simulator/JumperSkills.h"
#include "../simulator/WindsInfo.h"
#include "../simulator/JumpSimulator.h"
#include "../simulator/Characteristic.h"
#include "../simulator/Hill.h"
#include "../simulator/wind-generation/WindsGenerator.h"
#include "../simulator/wind-generation/WindGenerationSettings.h"

#include "../global/GlobalDatabase.h"
#include "../global/GlobalAppSettings.h"

#include "UI/AppSettings/AppSettingsWindow.h"
#include "UI/DatabaseEditor/DatabaseEditorWindow.h"
#include "UI/SingleJumps/SingleJumpsConfigWindow.h"
#include "UI/EditorWidgets/JumperEditorWidget.h"
#include "UI/EditorWidgets/HillEditorWidget.h"
#include "UI/EditorWidgets/WindsGeneratorSettingsEditorWidget.h"

#include "single-jumps/SingleJumpsManager.h"
#include "single-jumps/SingleJumpsSettings.h"

#include <QDebug>
#include <QCloseEvent>
#include <QVector>

extern IDGenerator globalIDGenerator;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GlobalDatabase::get()->loadFromJson();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_singleJumps_clicked()
{
    SingleJumpsConfigWindow singleJumpsConfig;
    singleJumpsConfig.setModal(true);
    if(singleJumpsConfig.exec() == QDialog::Accepted)
    {
        SingleJumpsManager manager;
        manager.setJumpsCount(singleJumpsConfig.getJumpsCountFromInput());
        manager.setChangeableWind(singleJumpsConfig.getChangeableWindFromInput());
        manager.setSaveResultsToFile(singleJumpsConfig.getSaveResultsToFileFromInput());
        if(manager.getSaveResultsToFile() == true)
            manager.setResultsFileName(singleJumpsConfig.getResultsFileName());
        manager.setGate(singleJumpsConfig.getGateFromInput());
        manager.setJumper(singleJumpsConfig.getJumperEditor()->getJumperFromWidgetInput());
        manager.setHill(singleJumpsConfig.getHillEditor()->getHillFromWidgetInput());
        manager.setWindsGeneratorSettings(singleJumpsConfig.getWindsGeneratorSettingsEditor()->getWindsGenerationSettingsFromInputs());
        manager.setWindAverageCalculatingType(singleJumpsConfig.getWindAverageCalculatingType());

        manager.simulate();
    }
}


void MainWindow::on_pushButton_databaseEdit_clicked()
{
    DatabaseEditorWindow databaseEditor;
    databaseEditor.setModal(true);
    if(databaseEditor.exec() == QDialog::Accepted)
    {

    }
}


void MainWindow::on_pushButton_settings_clicked()
{
    AppSettingsWindow settingsWindow;
    settingsWindow.setMainWindowParent(this);
    settingsWindow.setModal(true);
    settingsWindow.setupLanguagesComboBox();
    if(settingsWindow.exec() == QDialog::Accepted)
    {
        ui->retranslateUi(this);
    }
}

QApplication *MainWindow::getParentApplication() const
{
    return parentApplication;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    GlobalAppSettings::get()->writeToJson();
}

void MainWindow::setParentApplication(QApplication *newParentApplication)
{
    parentApplication = newParentApplication;
}
