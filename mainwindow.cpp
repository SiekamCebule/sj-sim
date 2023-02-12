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

#include <QDebug>
#include <QVector>

extern IDGenerator globalIDGenerator;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Hill * hill = new Hill("Wisła", "POL", 120, 134, 10.8, 7.24, 103, 0.395, 0.527);
    hill->insertCharacteristic(1, "takeoff-power-effect");
    hill->insertCharacteristic(-1, "takeoff-technique-effect");
    hill->insertCharacteristic(0, "takeoff-randomness-effect");
    hill->insertCharacteristic(0, "takeoff-form-effect");
    hill->insertCharacteristic(0, "flight-technique-effect");
    hill->insertCharacteristic(0, "flight-randomness-effect");
    hill->insertCharacteristic(0, "flight-form-effect");

    /*Hill * hill = new Hill("Planica", "SLO", 200, 240, 14.40, 8.64, 183, 1.2, 0.25);
    hill->insertCharacteristic(2, "takeoff-power-effect");
    hill->insertCharacteristic(0, "takeoff-technique-effect");
    hill->insertCharacteristic(0, "takeoff-randomness-effect");
    hill->insertCharacteristic(-1, "takeoff-form-effect");
    hill->insertCharacteristic(-1, "flight-technique-effect");
    hill->insertCharacteristic(2, "flight-randomness-effect");
    hill->insertCharacteristic(0, "flight-form-effect");*/

    /*Hill * hill = new Hill("Planica", "SLO", 95, 102, 8.00, 7.00, 85, 0.18, 0.41);
    hill->insertCharacteristic(1, "takeoff-power-effect");
    hill->insertCharacteristic(1, "takeoff-technique-effect");
    hill->insertCharacteristic(-1, "takeoff-randomness-effect");
    hill->insertCharacteristic(0, "takeoff-form-effect");
    hill->insertCharacteristic(0, "flight-technique-effect");
    hill->insertCharacteristic(-1, "flight-randomness-effect");
    hill->insertCharacteristic(-1, "flight-form-effect");*/

    hill->setupPointsForMeter();
    JumpSimulator simulator;
    simulator.setConditionsInfo(new ConditionsInfo(10));
    simulator.setHill(hill);

    QVector<Jumper> jumpers;
    Jumper jumper;

    jumper = Jumper("Daniel", "Huber", "AUT", new JumperSkills(38, 37, 39, 1, 26, 11, QSet<Characteristic>(), nullptr), 0);
    jumpers.push_back(jumper);
    jumper = Jumper("Aleksander", "Zniszczoł", "POL", new JumperSkills(39, 37, 33, 2, 39, 12, QSet<Characteristic>(), nullptr), 0);
    jumpers.push_back(jumper);
    jumper = Jumper("Domen", "Prevc", "SLO", new JumperSkills(34, 33, 44, 2, 37, 12.5, QSet<Characteristic>(), nullptr), 0);
    jumpers.push_back(jumper);

    for (auto & jumper : jumpers)
    {
        qDebug()<<jumper.getNameAndSurname();
        simulator.setJumper(&jumper);
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

