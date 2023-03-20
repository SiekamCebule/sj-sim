#include "SimulationSettingsWindow.h"
#include "ui_SimulationSettingsWindow.h"

#include "../../global/GlobalSimulationSettings.h"

SimulationSettingsWindow::SimulationSettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulationSettingsWindow)
{
    ui->setupUi(this);
    setFixedSize(size());

    connect(ui->pushButton_submit, &QPushButton::clicked, this, &SimulationSettingsWindow::submitted);
}

SimulationSettingsWindow::~SimulationSettingsWindow()
{
    delete ui;
}

void SimulationSettingsWindow::fillInputs()
{
    GlobalSimulationSettings * s = GlobalSimulationSettings::get();
    ui->doubleSpinBox_tkr_takeoffHeightMultiplier->setValue(s->getTakeoffRatingTakeoffHeightAbsValue());
    ui->doubleSpinBox_tkr_baseRandomBase->setValue(s->getTakeoffRatingBaseRandomBaseValue());
    ui->doubleSpinBox_tkr_baseDevBase->setValue(s->getTakeoffRatingBaseRandomDeviationBaseValue());
    ui->doubleSpinBox_tkr_baseDevDivider->setValue(s->getTakeoffRatingBaseRandomDeviationSubstractCharacteristicDivider());
    ui->doubleSpinBox_tkr_logRandomBase->setValue(s->getTakeoffRatingLogRandomBaseValue());
    ui->doubleSpinBox_tkr_logDevBase->setValue(s->getTakeoffRatingLogRandomDeviationBaseValue());
    ui->doubleSpinBox_tkr_logDevDivider->setValue(s->getTakeoffRatingLogRandomDeviationJumpsEqualityDivider());
    //
    ui->doubleSpinBox_flr_takeoffHeightMultiplier->setValue(s->getFlightRatingFlightHeightAbsValue());
    ui->doubleSpinBox_flr_baseRandomBase->setValue(s->getFlightRatingBaseRandomBaseValue());
    ui->doubleSpinBox_flr_baseDevBase->setValue(s->getFlightRatingBaseRandomDeviationBaseValue());
    ui->doubleSpinBox_flr_baseDevDivider->setValue(s->getFlightRatingBaseRandomDeviationSubstractCharacteristicDivider());
    ui->doubleSpinBox_flr_logRandomBase->setValue(s->getFlightRatingLogRandomBaseValue());
    ui->doubleSpinBox_flr_logDevBase->setValue(s->getFlightRatingLogRandomDeviationBaseValue());
    ui->doubleSpinBox_flr_logDevDivider->setValue(s->getFlightRatingLogRandomDeviationJumpsEqualityDivider());
}

void SimulationSettingsWindow::resetInputs()
{
    ui->doubleSpinBox_tkr_takeoffHeightMultiplier->setValue(0);
    ui->doubleSpinBox_tkr_baseRandomBase->setValue(0);
    ui->doubleSpinBox_tkr_baseDevBase->setValue(0);
    ui->doubleSpinBox_tkr_baseDevDivider->setValue(0);
    ui->doubleSpinBox_tkr_logRandomBase->setValue(0);
    ui->doubleSpinBox_tkr_logDevBase->setValue(0);
    ui->doubleSpinBox_tkr_logDevDivider->setValue(0);
    //
    ui->doubleSpinBox_flr_takeoffHeightMultiplier->setValue(0);
    ui->doubleSpinBox_flr_baseRandomBase->setValue(0);
    ui->doubleSpinBox_flr_baseDevBase->setValue(0);
    ui->doubleSpinBox_flr_baseDevDivider->setValue(0);
    ui->doubleSpinBox_flr_logRandomBase->setValue(0);
    ui->doubleSpinBox_flr_logDevBase->setValue(0);
    ui->doubleSpinBox_flr_logDevDivider->setValue(0);
}

double SimulationSettingsWindow::getTakeoffRatingTakeoffHeightAbsValueFromInput() const
{
    return ui->doubleSpinBox_tkr_takeoffHeightMultiplier->value();
}

double SimulationSettingsWindow::getTakeoffRatingBaseRandomBaseValueFromInput() const
{
    return ui->doubleSpinBox_tkr_baseRandomBase->value();
}

double SimulationSettingsWindow::getTakeoffRatingBaseRandomDeviationBaseValueFromInput() const
{
    return ui->doubleSpinBox_tkr_baseDevBase->value();
}

double SimulationSettingsWindow::getTakeoffRatingBaseRandomDeviationSubstractCharacteristicDividerFromInput() const
{
    return ui->doubleSpinBox_tkr_baseDevDivider->value();
}

double SimulationSettingsWindow::getTakeoffRatingLogRandomBaseValueFromInput() const
{
    return ui->doubleSpinBox_tkr_logRandomBase->value();
}

double SimulationSettingsWindow::getTakeoffRatingLogRandomDeviationBaseValueFromInput() const
{
    return ui->doubleSpinBox_tkr_logDevBase->value();
}

double SimulationSettingsWindow::getTakeoffRatingLogRandomDeviationJumpsEqualityDividerFromInput() const
{
    return ui->doubleSpinBox_tkr_logDevDivider->value();
}

double SimulationSettingsWindow::getFlightRatingFlightHeightAbsValueFromInput() const
{
    return ui->doubleSpinBox_flr_takeoffHeightMultiplier->value();
}

double SimulationSettingsWindow::getFlightRatingBaseRandomBaseValueFromInput() const
{
    return ui->doubleSpinBox_flr_baseRandomBase->value();
}

double SimulationSettingsWindow::getFlightRatingBaseRandomDeviationBaseValueFromInput() const
{
    return ui->doubleSpinBox_flr_baseDevBase->value();
}

double SimulationSettingsWindow::getFlightRatingBaseRandomDeviationSubstractCharacteristicDividerFromInput() const
{
    return ui->doubleSpinBox_flr_baseDevDivider->value();
}

double SimulationSettingsWindow::getFlightRatingLogRandomBaseValueFromInput() const
{
    return ui->doubleSpinBox_flr_logRandomBase->value();
}

double SimulationSettingsWindow::getFlightRatingLogRandomDeviationBaseValueFromInput() const
{
    return ui->doubleSpinBox_flr_logDevBase->value();
}

double SimulationSettingsWindow::getFlightRatingLogRandomDeviationJumpsEqualityDividerFromInput() const
{
    return ui->doubleSpinBox_flr_logDevDivider->value();
}

void SimulationSettingsWindow::on_pushButton_submit_2_clicked()
{
    resetInputs();
}

