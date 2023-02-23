#include "HillEditorWidget.h"
#include "ui_HillEditorWidget.h"

#include "../../simulator/Hill.h"
#include "../../simulator/Characteristic.h"

#include "CharacteristicsEditor.h"

HillEditorWidget::HillEditorWidget(CharacteristicsEditor * characteristicsEditor, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HillEditorWidget)
{
    ui->setupUi(this);

    if(this->characteristicsEditor == nullptr)
        this->characteristicsEditor = new CharacteristicsEditor;

    ui->verticalLayout_characteristicsEditor->insertWidget(0, getCharacteristicsEditor());
}

HillEditorWidget::~HillEditorWidget()
{
    delete ui;
}

void HillEditorWidget::fillHillInputs()
{
    if(hill == nullptr)
    {
        qDebug()<<"Hill is nullptr!";
        return;
    }
    ui->lineEdit_name->setText(hill->getName());
    ui->lineEdit_countryCode->setText(hill->getCountryCode());
    ui->doubleSpinBox_KPoint->setValue(hill->getKPoint());
    ui->doubleSpinBox_HSPoint->setValue(hill->getHSPoint());
    ui->doubleSpinBox_pointsForKPoint->setValue(hill->getPointsForKPoint());
    ui->doubleSpinBox_pointsForMeter->setValue(hill->getPointsForMeter());
    ui->doubleSpinBox_gatePoints->setValue(hill->getPointsForGate());
    ui->doubleSpinBox_frontWindPoints->setValue(hill->getPointsForFrontWind());
    ui->doubleSpinBox_backWindPoints->setValue(hill->getPointsForBackWind());
    ui->doubleSpinBox_takeoffEffect->setValue(hill->getTakeoffEffect());
    ui->doubleSpinBox_flightEffect->setValue(hill->getFlightEffect());
}

Hill HillEditorWidget::getHillFromWidgetInput() const
{
    Hill hill;
    hill.setName(ui->lineEdit_name->text());
    hill.setCountryCode(ui->lineEdit_countryCode->text());
    hill.setKPoint(ui->doubleSpinBox_KPoint->value());
    hill.setHSPoint(ui->doubleSpinBox_HSPoint->value());
    hill.setPointsForKPoint(ui->doubleSpinBox_pointsForKPoint->value());
    hill.setPointsForMeter(ui->doubleSpinBox_pointsForMeter->value());
    hill.setPointsForGate(ui->doubleSpinBox_gatePoints->value());
    hill.setPointsForFrontWind(ui->doubleSpinBox_frontWindPoints->value());
    hill.setPointsForBackWind(ui->doubleSpinBox_backWindPoints->value());
    hill.setTakeoffEffect(ui->doubleSpinBox_takeoffEffect->value());
    hill.setFlightEffect(ui->doubleSpinBox_flightEffect->value());
    return hill;
}

void HillEditorWidget::on_checkBox_autoPointsForKPoint_stateChanged(int arg1)
{
    if(arg1 == 1)
    {
        ui->doubleSpinBox_pointsForKPoint->setValue(Hill::calculatePointsForKPoint(ui->doubleSpinBox_KPoint->value()));
        ui->doubleSpinBox_pointsForKPoint->setReadOnly(true);
    }
    else
    {
        ui->doubleSpinBox_pointsForKPoint->setReadOnly(false);
    }
}


void HillEditorWidget::on_checkBox_autoPointsForMeter_stateChanged(int arg1)
{
    if(arg1 == 1)
    {
        ui->doubleSpinBox_pointsForMeter->setValue(Hill::calculatePointsForMeter(ui->doubleSpinBox_KPoint->value()));
        ui->doubleSpinBox_pointsForMeter->setReadOnly(true);
    }
    else
    {
        ui->doubleSpinBox_pointsForMeter->setReadOnly(false);
    }
}


void HillEditorWidget::on_checkBox_autoBackWindPoints_stateChanged(int arg1)
{
    if(arg1 == 1)
    {
        ui->doubleSpinBox_backWindPoints->setValue(Hill::calculatePointsForBackWindBy21PercentsOfFrontWind(ui->doubleSpinBox_frontWindPoints->value()));
        ui->doubleSpinBox_backWindPoints->setReadOnly(true);
    }
    else
    {
        ui->doubleSpinBox_backWindPoints->setReadOnly(false);
    }
}

CharacteristicsEditor *HillEditorWidget::getCharacteristicsEditor() const
{
    return characteristicsEditor;
}

void HillEditorWidget::setCharacteristicsEditor(CharacteristicsEditor *newCharacteristicsEditor)
{
    characteristicsEditor = newCharacteristicsEditor;
}

Hill *HillEditorWidget::getHill() const
{
    return hill;
}

void HillEditorWidget::setHill(Hill *newHill)
{
    hill = newHill;
}

