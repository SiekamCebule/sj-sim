#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "utilities/IDGenerator.h"
#include "utilities/functions.h"
#include "simulation/Jumper.h"
#include "simulation/JumperSkills.h"
#include "simulation/ConditionsInfo.h"
#include "simulation/JumpSimulator.h"
#include "simulation/Characteristic.h"
#include "simulation/Hill.h"
#include "simulation/wind-generation/WindsGenerator.h"
#include "simulation/wind-generation/WindGenerationSettings.h"

#include <QDebug>
#include <QVector>

extern IDGenerator globalIDGenerator;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*Hill * hill = new Hill("Wisła", "POL", 120, 134, 10.8, 7.24, 103, 0.4, 0.51);
    hill->insertCharacteristic("takeoff-power-effect", 1);
    hill->insertCharacteristic("takeoff-technique-effect", -1);
    hill->insertCharacteristic("takeoff-randomness-effect", 0);
    hill->insertCharacteristic("takeoff-form-effect", 0);
    hill->insertCharacteristic("flight-technique-effect", 0);
    hill->insertCharacteristic("flight-randomness-effect", 0);
    hill->insertCharacteristic("flight-form-effect", 0);*/

    Hill * hill = new Hill("Planica", "SLO", 200, 240, 14.40, 8.64, 183, 0.22, 1.02);
    hill->insertCharacteristic("takeoff-power-effect", 2);
    hill->insertCharacteristic("takeoff-technique-effect", 1);
    hill->insertCharacteristic("takeoff-randomness-effect", 0);
    hill->insertCharacteristic("takeoff-form-effect", -1);
    hill->insertCharacteristic("flight-technique-effect", 0);
    hill->insertCharacteristic("flight-randomness-effect", -1);
    hill->insertCharacteristic("flight-form-effect", -1);

    /*Hill * hill = new Hill("Planica", "SLO", 95, 102, 8.00, 7.00, 85, 0.18, 0.41);
    hill->insertCharacteristic("takeoff-power-effect", 0);
    hill->insertCharacteristic("takeoff-technique-effect", 1);
    hill->insertCharacteristic("takeoff-randomness-effect", 0);
    hill->insertCharacteristic("takeoff-form-effect", 0);
    hill->insertCharacteristic("flight-technique-effect", 0);
    hill->insertCharacteristic("flight-randomness-effect", 0);
    hill->insertCharacteristic("flight-form-effect", 0);*/

    hill->setupPointsForMeter();
    JumpSimulator simulator;
    simulator.setConditionsInfo(new ConditionsInfo(17));
    simulator.setHill(hill);

    QVector<Jumper> jumpers;
    Jumper jumper;

    jumper = Jumper("Daniel", "Huber", "AUT", new JumperSkills(38, 37, 39, 1, 22, 11, QSet<Characteristic>(), nullptr), 0);
    jumpers.push_back(jumper);
    jumper = Jumper("Aleksander", "Zniszczoł", "POL", new JumperSkills(39, 37, 33, 2, 37, 12, QSet<Characteristic>(), nullptr), 0);
    jumpers.push_back(jumper);
    jumper = Jumper("Domen", "Prevc", "SLO", new JumperSkills(34, 33, 44, 2, 43, 12.5, QSet<Characteristic>(), nullptr), 0);
    jumper.getJumperSkills()->insertCharacteristic("takeoff-height", -3);
    jumper.getJumperSkills()->insertCharacteristic("flight-height", -2);
    jumpers.push_back(jumper);

    QVector<WindGenerationSettings> windGenerationSettings = {
        WindGenerationSettings(Wind::Back, 2, 0.5, 1.65),
        WindGenerationSettings(Wind::Back, 4, 0.4, 1.8),
        WindGenerationSettings(Wind::Back, 3, 0.9, 1.660),
        WindGenerationSettings(Wind::Back, 3.2, 1.2, 1.4),
        WindGenerationSettings(Wind::Side, 3, 1.2, 1.20),
        WindGenerationSettings(Wind::Side, 2, 1.3, 1.20),
        WindGenerationSettings(Wind::Side, 3, 1.5, 1.40)
    };
    WindsGenerator windGenerator(7, windGenerationSettings);

    for (auto & jumper : jumpers)
    {
        qDebug()<<jumper.getNameAndSurname();
        simulator.setJumper(&jumper);
        simulator.getConditionsInfo()->setWinds(windGenerator.generateWinds());
        simulator.simulateJump();
        qDebug()<<"Odległość zawodnika: "<<roundDoubleToHalf(simulator.getDistance());
        qDebug()<<"";
    }

    delete hill;
    delete simulator.getJumper()->getJumperSkills();
    delete simulator.getConditionsInfo();
}


MainWindow::~MainWindow()
{
    delete ui;
}

