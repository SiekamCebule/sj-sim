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

void JumperEditorWidget::fillJumperInfo()
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

Jumper JumperEditorWidget::getJumperFromWidget()
{
    Jumper jumper;
    jumper.setName(ui->lineEdit_name->text());
    jumper.setSurname(ui->lineEdit_surname->text());
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
