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

    int count = ui->toolBox->count();
    for(int i=0; i<count; i++){
        delete ui->toolBox->widget(0);
    }

    for(int i=0; i<settingsCount; i++)
    {
        QString sectorInfo = tr(" (Od ") + QString::number(WindsGenerator::getRangeOfWindSector(i + 1, KPoint).first) + tr(", do ");
        if(i + 1 == WindsGenerator::calculateWindsCountByKPoint(KPoint))
            sectorInfo += tr("końca");
        else sectorInfo += QString::number(WindsGenerator::getRangeOfWindSector(i + 1, KPoint).second) + "m";
        sectorInfo += ")";

        ui->toolBox->addItem(new WindsGeneratorSettingsWidgetInputItem, tr("Sektor wiatru nr ") + QString::number(i + 1) + sectorInfo);

        dynamic_cast<WindsGeneratorSettingsWidgetInputItem *>(ui->toolBox->widget(i))->removeSubmitButton();
    }
}

QVector<WindGenerationSettings> WindsGeneratorSettingsEditorWidget::getWindsGenerationSettingsFromInputs()
{
    QVector<WindGenerationSettings> vector;

    for(int i=0; i < settingsCount; i++){
        WindsGeneratorSettingsWidgetInputItem * item = dynamic_cast<WindsGeneratorSettingsWidgetInputItem *>(ui->toolBox->widget(i));
        WindGenerationSettings settings;
        settings.setBaseWindStrength(item->getBaseWindStrengthFromInput());
        settings.setWindStrengthInstability(item->getWindStrengthChangeFromInput());
        settings.setBaseDirection(item->getBaseWindDirectionFromInput());
        settings.setWindDirectionInstability(item->getWindDirectionChangeFromInput());
        vector.push_back(settings);
    }
    return vector;
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
