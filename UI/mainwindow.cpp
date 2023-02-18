#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../utilities/IDGenerator.h"
#include "../utilities/functions.h"
#include "../utilities/MyRandom.h"

#include "../simulator/Jumper.h"
#include "../simulator/JumperSkills.h"
#include "../simulator/ConditionsInfo.h"
#include "../simulator/JumpSimulator.h"
#include "../simulator/Characteristic.h"
#include "../simulator/Hill.h"
#include "../simulator/wind-generation/WindsGenerator.h"
#include "../simulator/wind-generation/WindGenerationSettings.h"

#include "../global/GlobalDatabase.h"

#include "UI/SingleJumps/SingleJumpsConfigWindow.h"
#include "UI/DatabaseEditor/DatabaseEditorWindow.h"

#include <QDebug>
#include <QVector>

extern IDGenerator globalIDGenerator;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(GlobalDatabase::get()->loadFromJson() == false)
        exit(0);


    /*Hill * hill = new Hill("Wisła", "POL", 120, 134, 10.8, 7.24, 0.35, 0.375);
    hill->insertCharacteristic("real-hs-point", -1);
    hill->insertCharacteristic("takeoff-power-effect", 1);
    hill->insertCharacteristic("takeoff-technique-effect", -1);
    hill->insertCharacteristic("takeoff-randomness-effect", 0);
    hill->insertCharacteristic("takeoff-form-effect", 0);
    hill->insertCharacteristic("flight-technique-effect", 0);
    hill->insertCharacteristic("flight-randomness-effect", 0);
    hill->insertCharacteristic("flight-form-effect", 0);*/

    /*Hill * hill = new Hill("Planica", "SLO", 200, 240, 14.40, 8.64, 0.185, 0.965);
    hill->insertCharacteristic("real-hs-point", -1.5);
    hill->insertCharacteristic("takeoff-power-effect", 2);
    hill->insertCharacteristic("takeoff-technique-effect", 1);
    hill->insertCharacteristic("takeoff-randomness-effect", 0);
    hill->insertCharacteristic("takeoff-form-effect", -1);
    hill->insertCharacteristic("flight-technique-effect", 0);
    hill->insertCharacteristic("flight-randomness-effect", -1);
    hill->insertCharacteristic("flight-form-effect", -1);*/

    /*Hill * hill = new Hill("Oberstdorf", "GER", 95, 106, 8.00, 7.00, 0.345, 0.1);
    hill->insertCharacteristic("takeoff-power-effect", 0);
    hill->insertCharacteristic("takeoff-technique-effect", 1);
    hill->insertCharacteristic("takeoff-randomness-effect", -1);
    hill->insertCharacteristic("takeoff-form-effect", 1);
    hill->insertCharacteristic("flight-technique-effect", 0);
    hill->insertCharacteristic("flight-randomness-effect", 0);
    hill->insertCharacteristic("flight-form-effect", 0);*/

    /*hill->setupPointsForMeter();
    JumpSimulator simulator;
    simulator.getConditionsInfo().setGate(15);
    simulator.setHill(hill);

    QVector<Jumper> jumpers;
    Jumper jumper;

    jumper = Jumper("Daniel", "Huber", "AUT", new JumperSkills(38, 37, 39, 1, 20, 11.5, QSet<Characteristic>(), nullptr), 0);
    jumpers.push_back(jumper);

    jumper = Jumper("Aleksander", "Zniszczoł", "POL", new JumperSkills(39, 32.5, 34, 2, 38, 11.75, QSet<Characteristic>(), nullptr), 0);
    jumpers.push_back(jumper);

    jumper = Jumper("Domen", "Prevc", "SLO", new JumperSkills(34, 33, 43.8, 2, 35, 12.25, QSet<Characteristic>(), nullptr), 0);
    jumper.getJumperSkills()->insertCharacteristic("takeoff-height", -2.5);
    jumper.getJumperSkills()->insertCharacteristic("flight-height", -2);
    jumpers.push_back(jumper);

    jumper = Jumper("Clemens", "Aigner", "AUT", new JumperSkills(40, 36.8, 29.5, 1, 38, 12, QSet<Characteristic>(), nullptr), 0);
    jumper.getJumperSkills()->insertCharacteristic("takeoff-height", 1);
    jumper.getJumperSkills()->insertCharacteristic("flight-height", 2);
    jumpers.push_back(jumper);

    jumper = Jumper("Karl", "Geiger", "SLO", new JumperSkills(45, 46, 43, 1, 37, 12.75, QSet<Characteristic>(), nullptr), 0);
    jumper.getJumperSkills()->insertCharacteristic("landing-skill", 5);
    jumper.getJumperSkills()->insertCharacteristic("takeoff-height", 1.5);
    jumper.getJumperSkills()->insertCharacteristic("flight-height", 0.5);
    jumpers.push_back(jumper);

    QVector<WindGenerationSettings> windGenerationSettings = {
        WindGenerationSettings(Wind::Front, 1.45, 0.45, 4),
        WindGenerationSettings(Wind::Front, 1.2, 0.2, 4),
        WindGenerationSettings(Wind::FrontSide, 0.2, 3, 3),
        WindGenerationSettings(Wind::Side, 1.4, 2.7, 1),
        WindGenerationSettings(Wind::Side, 2.2, 2.4, 2),
        WindGenerationSettings(Wind::Side, 1.7, 0.7, 1.5),
        WindGenerationSettings(Wind::FrontSide, 2, 0.3, 2)
    };
    WindsGenerator windGenerator(7, windGenerationSettings);

    QVector<JumpData> jumps;
    for(auto & jumper : jumpers)
    {
        simulator.setJumper(&jumper);
        simulator.getConditionsInfo().setWinds(windGenerator.generateWinds());
        simulator.simulateJump();
        jumps.push_back(simulator.getJumpData());
    }
    for(const auto & jump : jumps)
    {
        qDebug()<<jump.getJumper()->getNameAndSurname();
        qDebug()<<"Odległość zawodnika: "<<jump.getDistance();
        qDebug()<<"Lądowanie: "<<jump.getLanding().getTextLandingType();
        qDebug()<<"Uśredniony odczyt wiatru: "<<jump.getConditionsInfo().getAveragedWind().getValueToAveragedWind()<<" m/s";
        qDebug()<<"Rekompensata za wiatr: "<<jump.getWindCompensation();
        qDebug()<<"Noty sędziowskie: "<<"|"<<jump.getJudges().at(0)<<"|"<<jump.getJudges().at(1)<<"|"<<jump.getJudges().at(2)<<"|"<<jump.getJudges().at(3)<<"|"<<jump.getJudges().at(4)<<"|";
        qDebug()<<"Punkty łącznie: "<<jump.getPoints()<<"\n\n";
    }

    delete hill;*/
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_singleJumps_clicked()
{

}


void MainWindow::on_pushButton_databaseEdit_clicked()
{
    DatabaseEditorWindow databaseEditor;
    databaseEditor.setModal(true);
    if(databaseEditor.exec() == QDialog::Accepted)
    {
        qDebug()<<"accepted";
    }
}

