#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../utilities/functions.h"

#include "../simulator/Jumper.h"
#include "../simulator/JumperSkills.h"
#include "../simulator/WindsCalculator.h"
#include "../simulator/JumpSimulator.h"
#include "../simulator/Characteristic.h"
#include "../simulator/Hill.h"
#include "../simulator/wind-generation/WindsGenerator.h"
#include "../simulator/wind-generation/WindGenerationSettings.h"

#include "../global/IDGenerator.h"
#include "../global/MyRandom.h"
#include "../global/GlobalDatabase.h"
#include "../global/GlobalAppSettings.h"
#include "../global/GlobalSimulationSettings.h"

#include "../single-jumps/SingleJumpsManager.h"

#include "AppSettings/AppSettingsWindow.h"
#include "DatabaseEditor/DatabaseEditorWindow.h"
#include "SingleJumps/SingleJumpsConfigWindow.h"
#include "SingleJumps/SingleJumpsResultsWindow.h"
#include "Competition/CompetitionConfigWindow.h"
#include "EditorWidgets/JumperEditorWidget.h"
#include "EditorWidgets/HillEditorWidget.h"
#include "EditorWidgets/WindsGeneratorSettingsEditorWidget.h"

#include <QDebug>
#include <QCloseEvent>
#include <QVector>
#include <random>

extern IDGenerator globalIDGenerator;
const QString appVersion = "0.3.0 beta";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    parentApplication = nullptr;
    ui->label_version->setText(appVersion);
    //qDebug()<<"random: "<<0;
    //double random = MyRandom::reducingChancesRandom(0, (2) * 1.3, (2) / 45, 1, (1.33 - (5) / 30), MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::FromSmallerToLarger);
    //double random = MyRandom::reducingChancesRandom(0, 2.6, 0.044, 1, 1.1, MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::FromSmallerToLarger);
    //qDebug()<<"random: "<<random;

    /*int count = 300;
    double min = 500000, max = 0, avg = 0;

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(5.0, (5));
    for(int i=0; i<count; i++)
    {
        //double random = MyRandom::reducingChancesRandom(-5, 80, 0.5, 1, 1.1075, MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::FromSmallerToLarger);//min: -3, max: 30.5, avg: 9.02
        // double random = MyRandom::reducingChancesRandom(-16.5, 80, 0.5, 1, 1.0625, MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::FromSmallerToLarger); //min: -8.5, max: 50, avg: 10.75
        //double random = MyRandom::reducingChancesRandom(4, 80, 0.5, 1, 1.28, MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::FromSmallerToLarger); //min 4, max: 20.5, avg: 8.16
            //double random = MyRandom::reducingChancesRandom(0, 2.6, 0.044, 1, 1.163, MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::FromSmallerToLarger);

        //double random = 0;
        //random = distribution(generator);

        if(random < min)
            min = random;
        if(random > max)
            max = random;

        avg += random;

        qDebug()<<"random: "<<random;
    }
    avg /= count;
    qDebug()<<"Najmniejszy: "<<min;
    qDebug()<<"Największy: "<<max;
    qDebug()<<"Średnio: "<<avg;*/

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

        SingleJumpsResultsWindow resultsWindow;
        resultsWindow.setManager(&manager);
        resultsWindow.fillJumperInfo();
        resultsWindow.fillHillInfo();
        resultsWindow.fillMiniJumpsResultsLayout();
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
