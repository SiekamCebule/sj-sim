#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "utilities/IDGenerator.h"
#include "simulation/Jumper.h"
#include "simulation/JumperSkills.h"
#include "simulation/ConditionsInfo.h"
#include "simulation/JumpSimulator.h"
#include "simulation/Hill.h"

#include <QDebug>
#include <QVector>

extern IDGenerator globalIDGenerator;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Hill * hill = new Hill("Wisła", "POL", 120, 134, 0, 0, 0, 3.0, 84.8, 0.23);
    JumpSimulator simulator;
    Jumper * jumper = new Jumper("Kamil", "Stoch", "POL", new JumperSkills(36, 42, 2, 44, 33, 14, QSet<QString>(), nullptr), 0);
    jumper->getJumperSkills()->insertCharacteristic("slow-inrun-speed");
    //jumper->getJumperSkills()->insertCharacteristic("high-takeoff");

    simulator.setConditionsInfo(new ConditionsInfo(13));
    simulator.setHill(hill);
    simulator.setJumper(jumper);
    simulator.simulateAll();

    delete hill;
    delete simulator.getJumper();
    delete simulator.getJumper()->getJumperSkills();
    delete simulator.getConditionsInfo();

}

//SIŁA WYBICIA ZWIEKSZONA O 12
// K200 9 metrow wiecej
// K120  5 metrow wiecej
// K95, 5 metrow wiecej

//TECHNIKA WYBICIA ZWIEKSZONA O 12
// K200 5 metrow, 4 metry wiecej
// K120 3 metry, 3 metry wiecej
// K95 3 metry, 3 metry wiecej

//TECHNIKA LOTU ZWIEKSZONA O 12
// K200 5 metrow
// K120 5 metry
// K95 4 metry



MainWindow::~MainWindow()
{
    delete ui;
}

