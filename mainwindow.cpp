#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "utilities/IDGenerator.h"
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

    Hill * hill = new Hill("Wisła", "POL", 120, 134, 10.8, 7.24, 103, 0.4035, 0.5465);
    hill->insertCharacteristic(1, "takeoff-power-effect");
    hill->insertCharacteristic(-1, "takeoff-technique-effect");
    hill->setupPointsForMeter();
    JumpSimulator simulator;

    //QVector<Jumper>jumpers;

    Jumper * jumper;

    simulator.setConditionsInfo(new ConditionsInfo(10));
    simulator.setHill(hill);
    jumper = new Jumper("Daniel", "Huber", "AUT", new JumperSkills(38, 42, 42, 1, 40, 14, QSet<Characteristic>(), nullptr), 0);
    simulator.setJumper(jumper);

    qDebug()<<"SKOCZNIA: "<<hill->getName()<<" ("<<hill->getCountry()<<")    K"<<hill->getKPoint()<<"  HS"<<hill->getHSPoint();
    for(int i=0; i<10; i++){
        simulator.simulateJump();
        qDebug()<<"";
    }
    /*for(auto & jum : jumpers)
    {
        qDebug()<<"";
        qDebug()<<jum.getNameAndSurname();
        simulator.setJumper(&jum);
        simulator.simulateJump();
        simulator.resetTemporaryParameters();
    }*/

    delete jumper;
    delete hill;
    delete simulator.getJumper()->getJumperSkills();
    delete simulator.getConditionsInfo();

}


MainWindow::~MainWindow()
{
    delete ui;
}

