#include "SimulationSaveInfoWidget.h"
#include "ui_SimulationSaveInfoWidget.h"

SimulationSaveInfoWidget::SimulationSaveInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulationSaveInfoWidget)
{
    ui->setupUi(this);
}

SimulationSaveInfoWidget::~SimulationSaveInfoWidget()
{
    delete ui;
}

void SimulationSaveInfoWidget::resetInputs()
{
    ui->label_name->setText("");
    ui->label_jumpersCount->setText("");
    ui->label_currentSeason->setText("");
    ui->label_playedCompetitions->setText("");
    ui->label_playedSeries->setText("");
}

void SimulationSaveInfoWidget::fillInputs()
{
    ui->label_name->setText(simulationSave->getName());
    ui->label_jumpersCount->setText("");
    ui->label_currentSeason->setText("");
    ui->label_playedCompetitions->setText("");
    ui->label_playedSeries->setText("");
}

SimulationSave *SimulationSaveInfoWidget::getSimulationSave() const
{
    return simulationSave;
}

void SimulationSaveInfoWidget::setSimulationSave(SimulationSave *newSimulationSave)
{
    simulationSave = newSimulationSave;
}
