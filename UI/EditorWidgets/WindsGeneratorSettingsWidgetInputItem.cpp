#include "WindsGeneratorSettingsWidgetInputItem.h"
#include "ui_WindsGeneratorSettingsWidgetInputItem.h"

WindsGeneratorSettingsWidgetInputItem::WindsGeneratorSettingsWidgetInputItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WindsGeneratorSettingsWidgetInputItem)
{
    ui->setupUi(this);

    characteristicsEditor = new CharacteristicsEditor(Characteristic::ParentType::WindsGeneratorSettings);
    ui->verticalLayout_characteristicsEditor->insertWidget(0, characteristicsEditor);

    ui->comboBox_baseWindDirection->setItemIcon(0, QIcon("://img/arrow-down.png"));
    ui->comboBox_baseWindDirection->setItemIcon(1, QIcon("://img/back-side.png"));
    ui->comboBox_baseWindDirection->setItemIcon(2, QIcon("://img/horizontal.png"));
    ui->comboBox_baseWindDirection->setItemIcon(3, QIcon("://img/front-side.png"));
    ui->comboBox_baseWindDirection->setItemIcon(4, QIcon("://img/arrow-up1.png"));

    disconnect(ui->pushButton_submit, &QPushButton::clicked, this, &WindsGeneratorSettingsWidgetInputItem::on_pushButton_submit_clicked);
}

WindsGeneratorSettingsWidgetInputItem::~WindsGeneratorSettingsWidgetInputItem()
{
    delete ui;
}

void WindsGeneratorSettingsWidgetInputItem::removeSubmitButton()
{
    disconnect(ui->pushButton_submit, &QPushButton::clicked, this, &WindsGeneratorSettingsWidgetInputItem::on_pushButton_submit_clicked);
    ui->verticalLayout_characteristicsEditor->removeWidget(ui->pushButton_submit);
    delete ui->pushButton_submit;
}

double WindsGeneratorSettingsWidgetInputItem::getBaseWindStrengthFromInput()
{
    return ui->doubleSpinBox_baseWindStrength->value();
}

double WindsGeneratorSettingsWidgetInputItem::getWindStrengthChangeFromInput()
{
    return ui->doubleSpinBox_windStrengthChange->value();
}

short WindsGeneratorSettingsWidgetInputItem::getBaseWindDirectionFromInput()
{
    return ui->comboBox_baseWindDirection->currentIndex();
}

double WindsGeneratorSettingsWidgetInputItem::getWindDirectionChangeFromInput()
{
    return ui->doubleSpinBox_windDirectionChange->value();
}

WindGenerationSettings WindsGeneratorSettingsWidgetInputItem::getWindGenerationSettingsFromInputs()
{
    WindGenerationSettings settings;
    settings.setBaseWindStrength(getBaseWindStrengthFromInput());
    settings.setWindStrengthInstability(getWindStrengthChangeFromInput());
    settings.setBaseDirection(getBaseWindDirectionFromInput());
    settings.setWindDirectionInstability(getWindDirectionChangeFromInput());
    settings.setCharacteristics(characteristicsEditor->getCharacteristics());

    return settings;
}

void WindsGeneratorSettingsWidgetInputItem::fillInputs()
{
    ui->doubleSpinBox_baseWindStrength->setValue(settings->getBaseWindStrength());
    ui->doubleSpinBox_windStrengthChange->setValue(settings->getWindStrengthInstability());
    ui->comboBox_baseWindDirection->setCurrentIndex(settings->getBaseDirection());
    ui->doubleSpinBox_windDirectionChange->setValue(settings->getWindDirectionInstability());
    characteristicsEditor->setCharacteristics(settings->getCharacteristics());
}

void WindsGeneratorSettingsWidgetInputItem::on_pushButton_submit_clicked()
{
    emit submitted();
}

WindGenerationSettings *WindsGeneratorSettingsWidgetInputItem::getSettings() const
{
    return settings;
}

void WindsGeneratorSettingsWidgetInputItem::setSettings(WindGenerationSettings *newSettings)
{
    settings = newSettings;
}

