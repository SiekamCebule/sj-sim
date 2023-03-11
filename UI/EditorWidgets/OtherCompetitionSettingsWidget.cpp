#include "OtherCompetitionSettingsWidget.h"
#include "ui_OtherCompetitionSettingsWidget.h"

OtherCompetitionSettingsWidget::OtherCompetitionSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OtherCompetitionSettingsWidget)
{
    ui->setupUi(this);
}

OtherCompetitionSettingsWidget::~OtherCompetitionSettingsWidget()
{
    delete ui;
}

double OtherCompetitionSettingsWidget::getInrunSnowFromInput()
{
    return ui->doubleSpinBox_inrunSnow->value();
}

double OtherCompetitionSettingsWidget::getSuitsWetFromInput()
{
    return ui->doubleSpinBox_suitsWet->value();
}
