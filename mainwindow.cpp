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

    Hill * hill = new Hill("Wisła", "POL", 120, 134, 0, 0, 3.0, 84.5, 0.55, 103);
    hill->setupPointsForMeter();
    JumpSimulator simulator;  
    Jumper * jumper = new Jumper("Markus", "Eisenbichler", "GER", new JumperSkills(37, 41, 2, 45, 38, 14, QSet<Characteristic>(), nullptr), 0);
    jumper->getJumperSkills()->insertCharacteristic(Characteristic(0, "inrun-speed"));
    simulator.setConditionsInfo(new ConditionsInfo(10));
    simulator.setHill(hill);
    simulator.setJumper(jumper);
    simulator.simulateJump();

    /*for(int i=0; i<5; i++){
        qDebug()<<"ZAWODNIK: "<<jumper->getNameAndSurname();
    simulator.simulateJump();
    simulator.resetTemporaryParameters();
    qDebug()<<"\n";
    }*/

    delete hill;
    delete simulator.getJumper();
    delete simulator.getJumper()->getJumperSkills();
    delete simulator.getConditionsInfo();

}


MainWindow::~MainWindow()
{
    delete ui;
}

