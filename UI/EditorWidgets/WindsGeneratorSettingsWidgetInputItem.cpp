#include "WindsGeneratorSettingsWidgetInputItem.h"
#include "ui_WindsGeneratorSettingsWidgetInputItem.h"

WindsGeneratorSettingsWidgetInputItem::WindsGeneratorSettingsWidgetInputItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WindsGeneratorSettingsWidgetInputItem)
{
    ui->setupUi(this);

    characteristicsEditor = new CharacteristicsEditor(Characteristic::ParentType::WindsGeneratorSettings);
    ui->verticalLayout_characteristicsEditor->insertWidget(0, characteristicsEditor);

    ui->comboBox_baseWindDirection->setItemIcon(0, QIcon("://img/arrow-up1"));
    ui->comboBox_baseWindDirection->setItemIcon(1, QIcon("://img/front-side"));
    ui->comboBox_baseWindDirection->setItemIcon(2, QIcon("://img/horizontal.png"));
    ui->comboBox_baseWindDirection->setItemIcon(3, QIcon("://img/back-side.png"));
    ui->comboBox_baseWindDirection->setItemIcon(4, QIcon("://img/arrow-down.png"));

    disconnect(ui->pushButton_submit, &QPushButton::clicked, this, &WindsGeneratorSettingsWidgetInputItem::when_submitButtonClicked);
}

WindsGeneratorSettingsWidgetInputItem::~WindsGeneratorSettingsWidgetInputItem()
{
    delete ui;
}

void WindsGeneratorSettingsWidgetInputItem::removeSubmitButton()
{
    disconnect(ui->pushButton_submit, &QPushButton::clicked, this, &WindsGeneratorSettingsWidgetInputItem::when_submitButtonClicked);
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

void WindsGeneratorSettingsWidgetInputItem::when_submitButtonClicked()
{
    emit submitted();
}
