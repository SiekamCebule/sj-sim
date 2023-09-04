#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../simulator/Hill.h"
#include "../simulator/wind-generation/WindGenerationSettings.h"

#include "../global/IDGenerator.h"
#include "../global/GlobalDatabase.h"
#include "../global/GlobalAppSettings.h"
#include "../global/GlobalSimulationSettings.h"
#include "../global/DatabaseObjectsManager.h"

#include "../single-jumps/SingleJumpsManager.h"

#include "AppSettings/AppSettingsWindow.h"
#include "DatabaseEditor/DatabaseEditorWindow.h"
#include "SingleJumps/SingleJumpsConfigWindow.h"
#include "SingleJumps/SingleJumpsResultsWindow.h"
#include "Competition/CompetitionConfigWindow.h"
#include "EditorWidgets/JumperEditorWidget.h"
#include "EditorWidgets/HillEditorWidget.h"
#include "EditorWidgets/WindsGeneratorSettingsEditorWidget.h"
#include "Seasons/SimulationSavesWindow.h"

#include <QDebug>
#include <QCloseEvent>
#include <QVector>
#include <QDesktopServices>
#include <random>

extern IDGenerator globalIDGenerator;
const QString appVersion = "0.9.4b";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    parentApplication = nullptr;
    ui->label_version->setText(appVersion);
    setFixedSize(size());

    GlobalDatabase::get()->loadFromJson();
    GlobalAppSettings::get()->loadFromJson();
    GlobalSimulationSettings::get()->loadFromFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_singleJumps_clicked()
{
    SingleJumpsConfigWindow singleJumpsConfig(this);
    singleJumpsConfig.setModal(true);
    if(singleJumpsConfig.exec() == QDialog::Accepted)
    {
        SingleJumpsManager manager;
        manager.setJumpsCount(singleJumpsConfig.getJumpsCountFromInput());
        manager.setChangeableWind(singleJumpsConfig.getChangeableWindFromInput());
        manager.setResultsFileName(singleJumpsConfig.getResultsFileName());
        manager.setResultsFormat(singleJumpsConfig.getResultsFormatFromInput());
        manager.setGate(singleJumpsConfig.getGateFromInput());
        manager.setJumper(singleJumpsConfig.getJumperEditor()->getJumperFromWidgetInput());
        manager.setHill(singleJumpsConfig.getHillEditor()->getHillFromWidgetInput());
        manager.setWindsGeneratorSettings(singleJumpsConfig.getWindsGeneratorSettingsEditor()->getWindsGenerationSettingsFromInputs());
        if(singleJumpsConfig.getDSQProbabilityFromInput() > (-1)){
            manager.getRules().setHasDsq(true);
            manager.setDSQProbability(singleJumpsConfig.getDSQProbabilityFromInput());
        }
        else manager.getRules().setHasDsq(false);

        manager.getRules().setHasJudgesPoints(singleJumpsConfig.getHasJudgesFromInput());
        manager.getRules().setHasWindCompensations(singleJumpsConfig.getHasWindCompensationFromInput());
        manager.getRules().setHasGateCompensations(false);
        manager.getRules().setWindAverageCalculatingType(singleJumpsConfig.getWindAverageCalculatingType());
        manager.getRules().setWindCompensationDistanceEffect(singleJumpsConfig.getWindCompensationDistanceEffect());
        manager.simulate();

        SingleJumpsResultsWindow resultsWindow(&manager);
        resultsWindow.fillJumperInfo();
        resultsWindow.fillHillInfo();
        resultsWindow.fillDistancesChart();
        resultsWindow.fillPointsChart();
        resultsWindow.fillJudgesChart();
        resultsWindow.fillLandingsChart();
        resultsWindow.fillWindsChart();
        resultsWindow.installShortcuts();
        if(resultsWindow.exec() == QDialog::Rejected)
        {
            if(manager.getResultsFileName().isNull() == false){
                manager.saveResultsToFile(SingleJumpsManager::Json);
            }
        }
    }
}

void MainWindow::on_pushButton_singleCompetition_clicked()
{
    CompetitionConfigWindow * configWindow = new CompetitionConfigWindow(CompetitionConfigWindow::Type::SingleCompetition, this);
    configWindow->setModal(true);
    if(configWindow->exec() == QDialog::Accepted)
    {

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

void MainWindow::on_pushButton_reportIssue_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/SiekamCebule/sj-sim/issues/new"));
}

void MainWindow::on_pushButton_season_clicked()
{
    if(GlobalDatabase::get()->getLoadedSimulationSaves() == false){
        GlobalDatabase::get()->loadSimulationSaves(true);
        GlobalDatabase::get()->setLoadedSimulationSaves(true);
    }

    SimulationSavesWindow savesWindow;
    savesWindow.setModal(true);
    if(savesWindow.exec() == QDialog::Accepted){

    }
}

