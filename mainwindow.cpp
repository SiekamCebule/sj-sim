#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "utilities/IDGenerator.h"
#include "simulation/Jumper.h"
#include "simulation/JumperSkills.h"
#include "simulation/ConditionsInfo.h"
#include "simulation/JumpSimulator.h"

#include <QDebug>
#include <QVector>

extern IDGenerator globalIDGenerator;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    JumpSimulator simulator;
    simulator.setJumper(new Jumper("Kamil", "Stoch", "POL", new JumperSkills(38, 41, 46, 2, 14, QSet<QString>(), nullptr), 0));
    simulator.setConditionsInfo(new ConditionsInfo(new Wind(Wind::Direction::Front, 0.6)));
    simulator.simulate();

}

MainWindow::~MainWindow()
{
    delete ui;
}

