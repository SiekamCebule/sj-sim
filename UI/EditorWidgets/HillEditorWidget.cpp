#include "HillEditorWidget.h"
#include "ui_HillEditorWidget.h"

#include "../../global/CountryFlagsManager.h"
#include "../../simulator/Hill.h"
#include "../../simulator/Characteristic.h"

#include "CharacteristicsEditor.h"

HillEditorWidget::HillEditorWidget(CharacteristicsEditor * characteristicsEditor, QWidget *parent) :
    characteristicsEditor(characteristicsEditor),
    QWidget(parent),
    ui(new Ui::HillEditorWidget)
{
    ui->setupUi(this);

    if(this->characteristicsEditor == nullptr)
        this->characteristicsEditor = new CharacteristicsEditor(Characteristic::Hill);

    ui->verticalLayout_characteristicsEditor->insertWidget(0, getCharacteristicsEditor());

    connect(ui->doubleSpinBox_KPoint, &QDoubleSpinBox::valueChanged, this, [this](){
        if(ui->doubleSpinBox_pointsForKPoint->isReadOnly() == true)
            ui->doubleSpinBox_pointsForKPoint->setValue(Hill::calculatePointsForKPoint(ui->doubleSpinBox_KPoint->value()));
        if(ui->doubleSpinBox_pointsForMeter->isReadOnly() == true)
            ui->doubleSpinBox_pointsForMeter->setValue(Hill::calculatePointsForMeter(ui->doubleSpinBox_KPoint->value()));
    });
    connect(ui->doubleSpinBox_frontWindPoints, &QDoubleSpinBox::valueChanged, this, [this](){
        if(ui->doubleSpinBox_backWindPoints->isReadOnly() == true)
            ui->doubleSpinBox_backWindPoints->setValue(Hill::calculatePointsForBackWindBy21PercentsOfFrontWind(ui->doubleSpinBox_frontWindPoints->value()));
    });

    connect(ui->pushButton_submit, &QPushButton::clicked, this, &HillEditorWidget::when_submitButtonClicked);
    connect(ui->doubleSpinBox_KPoint, &QDoubleSpinBox::editingFinished, this, [this](){
       emit KPointInputChanged(ui->doubleSpinBox_KPoint->value());
    });

    ui->label_countryFlag->setGeometry(ui->label_countryFlag->x(), ui->label_countryFlag->y(), CountryFlagsManager::getFlagPixmapSize().width(), CountryFlagsManager::getFlagPixmapSize().height());
}

HillEditorWidget::~HillEditorWidget()
{
    delete ui;
}

void HillEditorWidget::resetHillInputs()
{
    ui->lineEdit_name->setText("");
    ui->lineEdit_countryCode->setText("");
    ui->label_countryFlag->setPixmap(QPixmap());
    ui->doubleSpinBox_KPoint->setValue(0);
    ui->doubleSpinBox_HSPoint->setValue(0);

    ui->doubleSpinBox_pointsForKPoint->setValue(0);
    ui->checkBox_autoPointsForKPoint->setChecked(false);

    ui->doubleSpinBox_pointsForMeter->setValue(0);
    ui->checkBox_autoPointsForMeter->setChecked(false);

    ui->doubleSpinBox_gatePoints->setValue(0);
    ui->doubleSpinBox_frontWindPoints->setValue(0);

    ui->doubleSpinBox_backWindPoints->setValue(0);
    ui->checkBox_autoBackWindPoints->setChecked(false);

    ui->doubleSpinBox_takeoffEffect->setValue(0);
    ui->doubleSpinBox_flightEffect->setValue(0);
    characteristicsEditor->setCharacteristics(QSet<Characteristic>());
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
    ui->label_countryFlag->setPixmap(hill->getFlagPixmap().scaled(CountryFlagsManager::getFlagPixmapSize()));
    ui->doubleSpinBox_KPoint->setValue(hill->getKPoint());
    ui->doubleSpinBox_HSPoint->setValue(hill->getHSPoint());

    ui->doubleSpinBox_pointsForKPoint->setValue(hill->getPointsForKPoint());
    ui->checkBox_autoPointsForKPoint->setChecked(hill->getAutoPointsForKPoint());

    ui->doubleSpinBox_pointsForMeter->setValue(hill->getPointsForMeter());
    ui->checkBox_autoPointsForMeter->setChecked(hill->getAutoPointsForMeter());

    ui->doubleSpinBox_gatePoints->setValue(hill->getPointsForGate());
    ui->doubleSpinBox_frontWindPoints->setValue(hill->getPointsForFrontWind());

    ui->doubleSpinBox_backWindPoints->setValue(hill->getPointsForBackWind());
    ui->checkBox_autoBackWindPoints->setChecked(hill->getAutoPointsForBackWind());

    ui->doubleSpinBox_takeoffEffect->setValue(hill->getTakeoffEffect());
    ui->doubleSpinBox_flightEffect->setValue(hill->getFlightEffect());
    characteristicsEditor->setCharacteristics(hill->getCharacteristics());
}

void HillEditorWidget::removeSubmitButton()
{
    disconnect(ui->pushButton_submit, &QPushButton::clicked, this, &HillEditorWidget::when_submitButtonClicked);
    ui->verticalLayout_characteristicsEditor->removeWidget(ui->pushButton_submit);
    delete ui->pushButton_submit;
}

Hill HillEditorWidget::getHillFromWidgetInput() const
{
    Hill hill;
    hill.setName(ui->lineEdit_name->text());
    hill.setCountryCode(ui->lineEdit_countryCode->text());
    hill.setFlagPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(hill.getCountryCode().toLower())));
    hill.setKPoint(ui->doubleSpinBox_KPoint->value());
    hill.setHSPoint(ui->doubleSpinBox_HSPoint->value());

    hill.setPointsForKPoint(ui->doubleSpinBox_pointsForKPoint->value());
    hill.setAutoPointsForKPoint(ui->checkBox_autoPointsForKPoint->isChecked());

    hill.setPointsForMeter(ui->doubleSpinBox_pointsForMeter->value());
    hill.setAutoPointsForMeter(ui->checkBox_autoPointsForMeter->isChecked());

    hill.setPointsForGate(ui->doubleSpinBox_gatePoints->value());
    hill.setPointsForFrontWind(ui->doubleSpinBox_frontWindPoints->value());

    hill.setPointsForBackWind(ui->doubleSpinBox_backWindPoints->value());
    hill.setAutoPointsForBackWind(ui->checkBox_autoBackWindPoints->isChecked());

    hill.setTakeoffEffect(ui->doubleSpinBox_takeoffEffect->value());
    hill.setFlightEffect(ui->doubleSpinBox_flightEffect->value());
    hill.setCharacteristics(characteristicsEditor->getCharacteristics());
    return hill;
}

void HillEditorWidget::on_checkBox_autoPointsForKPoint_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->doubleSpinBox_pointsForKPoint->setValue(Hill::calculatePointsForKPoint(ui->doubleSpinBox_KPoint->value()));
        ui->doubleSpinBox_pointsForKPoint->setReadOnly(true);
        ui->doubleSpinBox_pointsForKPoint->setStyleSheet(ui->doubleSpinBox_pointsForKPoint->styleSheet());
    }
    else
    {
        ui->doubleSpinBox_pointsForKPoint->setReadOnly(false);
        ui->doubleSpinBox_pointsForKPoint->setStyleSheet(ui->doubleSpinBox_pointsForKPoint->styleSheet());
    }
}


void HillEditorWidget::on_checkBox_autoPointsForMeter_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->doubleSpinBox_pointsForMeter->setValue(Hill::calculatePointsForMeter(ui->doubleSpinBox_KPoint->value()));
        ui->doubleSpinBox_pointsForMeter->setReadOnly(true);
        ui->doubleSpinBox_pointsForMeter->setStyleSheet(ui->doubleSpinBox_pointsForMeter->styleSheet());
    }
    else
    {
        ui->doubleSpinBox_pointsForMeter->setReadOnly(false);
        ui->doubleSpinBox_pointsForMeter->setStyleSheet(ui->doubleSpinBox_pointsForMeter->styleSheet());
    }
}


void HillEditorWidget::on_checkBox_autoBackWindPoints_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->doubleSpinBox_backWindPoints->setValue(Hill::calculatePointsForBackWindBy21PercentsOfFrontWind(ui->doubleSpinBox_frontWindPoints->value()));
        ui->doubleSpinBox_backWindPoints->setReadOnly(true);
        ui->doubleSpinBox_backWindPoints->setStyleSheet(ui->doubleSpinBox_backWindPoints->styleSheet());
    }
    else
    {
        ui->doubleSpinBox_backWindPoints->setReadOnly(false);
        ui->doubleSpinBox_backWindPoints->setStyleSheet(ui->doubleSpinBox_backWindPoints->styleSheet());
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

void HillEditorWidget::on_lineEdit_countryCode_textChanged(const QString &arg1)
{
    if(arg1.length() == 3)
        ui->label_countryFlag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(ui->lineEdit_countryCode->text().toLower())).scaled(CountryFlagsManager::getFlagPixmapSize()));
}

void HillEditorWidget::when_submitButtonClicked()
{
    emit submitted();
}

