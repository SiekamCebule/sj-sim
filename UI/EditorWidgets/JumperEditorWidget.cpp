#include "JumperEditorWidget.h"
#include "ui_JumperEditorWidget.h"

JumperEditorWidget::JumperEditorWidget(Jumper *jumper, CharacteristicsEditor *characteristicsEditor, QWidget *parent) :
    jumper(jumper),
    characteristicsEditor(characteristicsEditor),
    QWidget(parent),
    ui(new Ui::JumperEditorWidget)
{
    ui->setupUi(this);

    if(this->characteristicsEditor == nullptr)
        this->characteristicsEditor = new CharacteristicsEditor;

    ui->verticalLayout_characteristicsEditor->insertWidget(0, getCharacteristicsEditor());
}

JumperEditorWidget::~JumperEditorWidget()
{
    delete ui;
}

void JumperEditorWidget::fillJumperInputs()
{
    if(jumper == nullptr){
        qDebug()<<"Jumper is nullptr!";
        return;
    }
    ui->lineEdit_name->setText(jumper->getName());
    ui->lineEdit_surname->setText(jumper->getSurname());
    ui->lineEdit_countryCode->setText(jumper->getCountryCode());
    ui->doubleSpinBox_takeoffPower->setValue(jumper->getJumperSkills().getTakeoffPower());
    ui->doubleSpinBox_takeoffTechnique->setValue(jumper->getJumperSkills().getTakeoffTechnique());
    ui->doubleSpinBox_flightTechnique->setValue(jumper->getJumperSkills().getFlightTechnique());
    ui->comboBox_flightStyle->setCurrentIndex(jumper->getJumperSkills().getFlightStyle());
    ui->doubleSpinBox_landingStyle->setValue(jumper->getJumperSkills().getLandingStyle());
    ui->doubleSpinBox_form->setValue(jumper->getJumperSkills().getForm());
    characteristicsEditor->setCharacteristics(jumper->getJumperSkills().getCharacteristics());
}

Jumper JumperEditorWidget::getJumperFromWidgetInput()
{
    Jumper jumper;
    jumper.setName(ui->lineEdit_name->text());
    jumper.setSurname(ui->lineEdit_surname->text());
    jumper.setCountryCode(ui->lineEdit_countryCode->text());
    jumper.getJumperSkillsPointer()->setTakeoffPower(ui->doubleSpinBox_takeoffPower->value());
    jumper.getJumperSkillsPointer()->setTakeoffTechnique(ui->doubleSpinBox_takeoffTechnique->value());
    jumper.getJumperSkillsPointer()->setFlightTechnique(ui->doubleSpinBox_flightTechnique->value());
    jumper.getJumperSkillsPointer()->setFlightStyle(ui->comboBox_flightStyle->currentIndex());
    jumper.getJumperSkillsPointer()->setLandingStyle(ui->doubleSpinBox_landingStyle->value());
    jumper.getJumperSkillsPointer()->setForm(ui->doubleSpinBox_form->value());
    jumper.getJumperSkillsPointer()->setCharacteristics(characteristicsEditor->getCharacteristics());
    return jumper;
}

Jumper *JumperEditorWidget::getJumper() const
{
    return jumper;
}

void JumperEditorWidget::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}

CharacteristicsEditor *JumperEditorWidget::getCharacteristicsEditor() const
{
    return characteristicsEditor;
}

void JumperEditorWidget::setCharacteristicsEditor(CharacteristicsEditor *newCharacteristicsEditor)
{
    characteristicsEditor = newCharacteristicsEditor;
}

void JumperEditorWidget::on_pushButton_clicked()
{
    emit submitted();
}

