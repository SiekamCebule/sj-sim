#include "WindsGeneratorSettingsEditorWidget.h"
#include "ui_WindsGeneratorSettingsEditorWidget.h"

#include "WindsGeneratorSettingsWidgetInputItem.h"

#include <QDebug>

WindsGeneratorSettingsEditorWidget::WindsGeneratorSettingsEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WindsGeneratorSettingsEditorWidget)
{
    ui->setupUi(this);

    ui->toolBox->removeItem(0);
    ui->toolBox->removeItem(0);
    delete ui->widget;
    delete ui->page_2;
}

WindsGeneratorSettingsEditorWidget::~WindsGeneratorSettingsEditorWidget()
{
    delete ui;
}

void WindsGeneratorSettingsEditorWidget::fillSettingsInputs(double KPoint)
{
    settingsCount = WindsGenerator::calculateWindsCountByKPoint(KPoint);
    qDebug()<<"Ilosc wiatrow: "<<settingsCount;

    for(int i=0; i<ui->toolBox->count(); i++){
        ui->toolBox->removeItem(0);
        delete ui->toolBox->widget(0);
    }

    for(int i=0; i<settingsCount; i++)
    {
        ui->toolBox->addItem(new WindsGeneratorSettingsWidgetInputItem, "Sektor wiatru nr " + QString::number(i + 1));
        dynamic_cast<WindsGeneratorSettingsWidgetInputItem *>(ui->toolBox->widget(i))->removeSubmitButton();
    }
}

QVector<WindGenerationSettings> *WindsGeneratorSettingsEditorWidget::getWindGenerationSettings() const
{
    return windGenerationSettings;
}

void WindsGeneratorSettingsEditorWidget::setWindGenerationSettings(QVector<WindGenerationSettings> *newWindGenerationSettings)
{
    windGenerationSettings = newWindGenerationSettings;
}

int WindsGeneratorSettingsEditorWidget::getSettingsCount() const
{
    return settingsCount;
}

void WindsGeneratorSettingsEditorWidget::setSettingsCount(int newSettingsCount)
{
    settingsCount = newSettingsCount;
}

bool WindsGeneratorSettingsEditorWidget::getRemovingSubmitButtons() const
{
    return removingSubmitButtons;
}

void WindsGeneratorSettingsEditorWidget::setRemovingSubmitButtons(bool newRemovingSubmitButtons)
{
    removingSubmitButtons = newRemovingSubmitButtons;
}
