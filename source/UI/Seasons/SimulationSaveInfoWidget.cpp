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
    ui->label_name->clear();
    ui->label_jumpersCount->clear();
    ui->label_currentSeason->clear();
    ui->label_playedCompetitions->clear();
    ui->label_classifications->clear();
}

void SimulationSaveInfoWidget::fillInputs()
{
    ui->label_name->setText(simulationSave->getName());
    ui->label_jumpersCount->setText(QString::number(simulationSave->getJumpersReference().count()));
    ui->label_currentSeason->setText(QString::number(simulationSave->getActualSeason()->getSeasonNumber()));
    ui->label_playedCompetitions->setText(QString::number(simulationSave->getNextCompetitionIndex()) + "/" + QString::number(simulationSave->getActualSeason()->getCalendarReference().getCompetitionsReference().count()));
    ui->label_classifications->setText(QString::number(simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference().count()));
}

SimulationSave *SimulationSaveInfoWidget::getSimulationSave() const
{
    return simulationSave;
}

void SimulationSaveInfoWidget::setSimulationSave(SimulationSave *newSimulationSave)
{
    simulationSave = newSimulationSave;
}
