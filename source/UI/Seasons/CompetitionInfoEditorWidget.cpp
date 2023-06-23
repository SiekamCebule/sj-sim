#include "CompetitionInfoEditorWidget.h"
#include "ui_CompetitionInfoEditorWidget.h"

CompetitionInfoEditorWidget::CompetitionInfoEditorWidget(CompetitionInfo * competitionInfo, QVector<Hill> *hillsList, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompetitionInfoEditorWidget),
    competitionInfo(competitionInfo),
    hillsList(hillsList)
{
    ui->setupUi(this);
    rulesEditor = new CompetitionRulesEditorWidget(this);
    rulesEditor->setCompetitionRules(this->competitionInfo->getRulesPointer());
    ui->verticalLayout_competitionRules->addWidget(rulesEditor);
    setupHillsComboBox();
}

CompetitionInfoEditorWidget::~CompetitionInfoEditorWidget()
{
    delete ui;
}

void CompetitionInfoEditorWidget::fillInputs()
{
    rulesEditor->fillCompetitionRulesInputs();
    rulesEditor->fillRoundsInputs(true);
}

void CompetitionInfoEditorWidget::resetInputs()
{
    rulesEditor->resetInputs();
    rulesEditor->fillRoundsInputs(true);
}

void CompetitionInfoEditorWidget::setupHillsComboBox()
{
    ui->comboBox_hill->clear();
    ui->comboBox_hill->addItem("BRAK");
    for(const auto & hill : *hillsList){
        ui->comboBox_hill->addItem(QIcon(QPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(hill.getCountryCode().toLower())))) ,hill.getName() + " HS" + QString::number(hill.getHSPoint()));
    }
}

void CompetitionInfoEditorWidget::setReadOnly(bool ok)
{
    if(ok == true){
        ui->comboBox_hill->setDisabled(true);
        ui->comboBox_seriesType->setDisabled(true);
        rulesEditor->setDisabled(true);
    }
    else{
        ui->comboBox_hill->setEnabled(true);
        ui->comboBox_seriesType->setEnabled(true);
        rulesEditor->setEnabled(true);
    }
}

QVector<Hill> *CompetitionInfoEditorWidget::getHillsList() const
{
    return hillsList;
}

void CompetitionInfoEditorWidget::setHillsList(QVector<Hill> *newHillsList)
{
    hillsList = newHillsList;
}

CompetitionInfo *CompetitionInfoEditorWidget::getCompetitionInfo() const
{
    return competitionInfo;
}

void CompetitionInfoEditorWidget::setCompetitionInfo(CompetitionInfo *newCompetitionInfo)
{
    competitionInfo = newCompetitionInfo;
}

void CompetitionInfoEditorWidget::setRulesEditor(CompetitionRulesEditorWidget *newRulesEditor)
{
    rulesEditor = newRulesEditor;
}

CompetitionRulesEditorWidget *CompetitionInfoEditorWidget::getRulesEditor() const
{
    return rulesEditor;
}
