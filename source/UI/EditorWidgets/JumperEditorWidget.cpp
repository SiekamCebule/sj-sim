#include "JumperEditorWidget.h"
#include "ui_JumperEditorWidget.h"

#include "../../global/GlobalDatabase.h"
#include "../../global/CountryFlagsManager.h"

#include <QIcon>

JumperEditorWidget::JumperEditorWidget(Jumper *jumper, CharacteristicsEditor *characteristicsEditor, QWidget *parent) :
    jumper(jumper),
    characteristicsEditor(characteristicsEditor),
    QWidget(parent),
    ui(new Ui::JumperEditorWidget)
{
    ui->setupUi(this);

    if(this->characteristicsEditor == nullptr)
        this->characteristicsEditor = new CharacteristicsEditor(Characteristic::Jumper);

    ui->verticalLayout_characteristicsEditor->insertWidget(0, getCharacteristicsEditor());
    ui->label_countryFlag->setGeometry(ui->label_countryFlag->x(), ui->label_countryFlag->y(), CountryFlagsManager::getFlagPixmapSize().width(), CountryFlagsManager::getFlagPixmapSize().height());
    connect(ui->pushButton_submit, &QPushButton::clicked, this, &JumperEditorWidget::when_submittButtonClicked);
}

JumperEditorWidget::~JumperEditorWidget()
{
    delete ui;
}

void JumperEditorWidget::resetJumperInputs()
{
    ui->lineEdit_name->setText("");
    ui->lineEdit_surname->setText("");
    ui->lineEdit_countryCode->setText("");
    ui->label_countryFlag->setPixmap(QPixmap());
    ui->doubleSpinBox_takeoffPower->setValue(0);
    ui->doubleSpinBox_takeoffTechnique->setValue(0);
    ui->doubleSpinBox_flightTechnique->setValue(0);
    ui->comboBox_flightStyle->setCurrentIndex(0);
    ui->doubleSpinBox_landingStyle->setValue(0);
    ui->doubleSpinBox_form->setValue(0);
    ui->doubleSpinBox_jumpsEquality->setValue(0);;
    characteristicsEditor->setCharacteristics(QSet<Characteristic>());
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
    ui->label_countryFlag->setPixmap(jumper->getFlagPixmap().scaled(CountryFlagsManager::getFlagPixmapSize()));
    ui->doubleSpinBox_takeoffPower->setValue(jumper->getJumperSkills().getTakeoffPower());
    ui->doubleSpinBox_takeoffTechnique->setValue(jumper->getJumperSkills().getTakeoffTechnique());
    ui->doubleSpinBox_flightTechnique->setValue(jumper->getJumperSkills().getFlightTechnique());
    ui->comboBox_flightStyle->setCurrentIndex(jumper->getJumperSkills().getFlightStyle());
    ui->doubleSpinBox_landingStyle->setValue(jumper->getJumperSkills().getLandingStyle());
    ui->doubleSpinBox_form->setValue(jumper->getJumperSkills().getForm());
    ui->doubleSpinBox_jumpsEquality->setValue(jumper->getJumperSkills().getJumpsEquality());
    characteristicsEditor->setCharacteristics(jumper->getJumperSkills().getCharacteristics());
}

void JumperEditorWidget::removeSubmitButton()
{
    disconnect(ui->pushButton_submit, &QPushButton::clicked, this, &JumperEditorWidget::when_submittButtonClicked);
    ui->verticalLayout_characteristicsEditor->removeWidget(ui->pushButton_submit);
    delete ui->pushButton_submit;
}

Jumper JumperEditorWidget::getJumperFromWidgetInput() const
{
    Jumper jumper;
    jumper.setName(ui->lineEdit_name->text());
    jumper.setSurname(ui->lineEdit_surname->text());
    jumper.setCountryCode(ui->lineEdit_countryCode->text());
    jumper.setFlagPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumper.getCountryCode().toLower())));
    jumper.getJumperSkillsPointer()->setTakeoffPower(ui->doubleSpinBox_takeoffPower->value());
    jumper.getJumperSkillsPointer()->setTakeoffTechnique(ui->doubleSpinBox_takeoffTechnique->value());
    jumper.getJumperSkillsPointer()->setFlightTechnique(ui->doubleSpinBox_flightTechnique->value());
    jumper.getJumperSkillsPointer()->setFlightStyle(ui->comboBox_flightStyle->currentIndex());
    jumper.getJumperSkillsPointer()->setLandingStyle(ui->doubleSpinBox_landingStyle->value());
    jumper.getJumperSkillsPointer()->setForm(ui->doubleSpinBox_form->value());
    jumper.getJumperSkillsPointer()->setJumpsEquality(ui->doubleSpinBox_jumpsEquality->value());
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

void JumperEditorWidget::on_lineEdit_countryCode_textChanged(const QString &arg1)
{
    if(arg1.length() > 2)
        ui->label_countryFlag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(ui->lineEdit_countryCode->text().toLower())).scaled(CountryFlagsManager::getFlagPixmapSize()));
}

void JumperEditorWidget::when_submittButtonClicked()
{
    emit submitted();
}
