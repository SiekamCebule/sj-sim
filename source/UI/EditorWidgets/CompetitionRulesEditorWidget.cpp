#include "CompetitionRulesEditorWidget.h"
#include "ui_CompetitionRulesEditorWidget.h"

#include "RoundInfoEditorWidget.h"
#include "../../utilities/functions.h"

CompetitionRulesEditorWidget::CompetitionRulesEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompetitionRulesEditorWidget)
{
    ui->setupUi(this);
    setupConnectsForWidgetChange();
    competitionRules = nullptr;

    ui->toolBox_rounds->removeItem(0);
    ui->toolBox_rounds->removeItem(0);
    delete ui->page;
    delete ui->page_2;

    connect(ui->spinBox_roundsCount, &QSpinBox::valueChanged, this, &CompetitionRulesEditorWidget::fillRoundsInputs);
    connect(ui->comboBox_competitionType, &QComboBox::currentIndexChanged, this, &CompetitionRulesEditorWidget::competitionTypeChanged);
    connect(ui->spinBox_jumpersInTeamCount, &QSpinBox::valueChanged, this, &CompetitionRulesEditorWidget::jumpersCountInTeamChanged);
}

CompetitionRulesEditorWidget::~CompetitionRulesEditorWidget()
{
    removeConnectsForWidgetChange();
    delete ui;
}

void CompetitionRulesEditorWidget::resetInputs()
{
    removeConnectsForWidgetChange();
    ui->lineEdit_name->clear();
    ui->spinBox_roundsCount->setValue(0);
    ui->checkBox_windCompensation->setChecked(true);
    ui->checkBox_gateCompensation->setChecked(true);
    ui->checkBox_95HsRule->setChecked(true);
    ui->checkBox_judgesPoints->setChecked(true);
    ui->checkBox_dsq->setChecked(true);
    ui->spinBox_jumpersInTeamCount->setValue(0);
    ui->comboBox_competitionType->setCurrentIndex(CompetitionRules::Individual);
    ui->comboBox_windAverageCalculatingType->setCurrentIndex(0);
    ui->comboBox_windCompensationDistanceEffect->setCurrentIndex(0);
    setupConnectsForWidgetChange();
}

void CompetitionRulesEditorWidget::fillCompetitionRulesInputs()
{
    removeConnectsForWidgetChange();
    ui->lineEdit_name->setText(competitionRules->getName());
    ui->spinBox_roundsCount->setValue(competitionRules->getRounds().count());
    ui->checkBox_windCompensation->setChecked(competitionRules->getHasWindCompensations());
    ui->checkBox_gateCompensation->setChecked(competitionRules->getHasGateCompensations());
    ui->checkBox_95HsRule->setChecked(competitionRules->getHas95HSRule());
    ui->checkBox_judgesPoints->setChecked(competitionRules->getHasJudgesPoints());
    ui->checkBox_dsq->setChecked(competitionRules->getHasDsq());
    if(competitionRules->getCompetitionType() == CompetitionRules::Team){
        ui->spinBox_jumpersInTeamCount->setEnabled(true);
        ui->spinBox_jumpersInTeamCount->setValue(competitionRules->getJumpersInTeamCount());
    }
    else{
        ui->spinBox_jumpersInTeamCount->setValue(0);
        ui->spinBox_jumpersInTeamCount->setEnabled(false);
    }
    ui->comboBox_competitionType->setCurrentIndex(competitionRules->getCompetitionType());
    ui->comboBox_windAverageCalculatingType->setCurrentIndex(competitionRules->getWindAverageCalculatingType());
    ui->comboBox_windCompensationDistanceEffect->setCurrentIndex(competitionRules->getWindCompensationDistanceEffect());
    fillRoundsInputs(true);
    setupConnectsForWidgetChange();
}

void CompetitionRulesEditorWidget::fillRoundsInputs(bool setup)
{
    int roundsCount = ui->spinBox_roundsCount->value();
    while (ui->toolBox_rounds->count() > roundsCount)
    {
        delete ui->toolBox_rounds->widget(ui->toolBox_rounds->count() - 1);
    }

    while(ui->toolBox_rounds->count() < roundsCount)
    {
        ui->toolBox_rounds->addItem(new RoundInfoEditorWidget, "");
    }
    for(int i=0; i<ui->toolBox_rounds->count(); i++)
    {
        ui->toolBox_rounds->setItemText(i, "Seria " + QString::number(i+1));
    }
    if(setup == true){
        disconnect(ui->spinBox_roundsCount, &QSpinBox::valueChanged, this, &CompetitionRulesEditorWidget::fillRoundsInputs);
        for(int i=0; i<competitionRules->getRounds().count(); i++){
            dynamic_cast<RoundInfoEditorWidget *>(ui->toolBox_rounds->widget(i))->setRoundInfo(const_cast<RoundInfo *>(&competitionRules->getRounds().at(i)));
            dynamic_cast<RoundInfoEditorWidget *>(ui->toolBox_rounds->widget(i))->fillRoundInfoInput();
        }
        connect(ui->spinBox_roundsCount, &QSpinBox::valueChanged, this, [this](){
            fillRoundsInputs(false);
        });
    }
}

QString CompetitionRulesEditorWidget::getNameFromInput()
{
    return ui->lineEdit_name->text();
}

QVector<RoundInfo> CompetitionRulesEditorWidget::getRoundsFromInput()
{
    QVector<RoundInfo> rounds;
    for(int i=0; i<ui->toolBox_rounds->count(); i++)
    {
        RoundInfo round;
        round.setCount(dynamic_cast<RoundInfoEditorWidget *>(ui->toolBox_rounds->widget(i))->getCountFromInput());
        round.setSortStartList(dynamic_cast<RoundInfoEditorWidget *>(ui->toolBox_rounds->widget(i))->getSortStartListFromInput());
        rounds.push_back(round);
    }
    return rounds;
}

bool CompetitionRulesEditorWidget::getHas95HSRuleFromInput()
{
    return ui->checkBox_95HsRule->isChecked();
}

bool CompetitionRulesEditorWidget::getHasWindCompensations()
{
    return ui->checkBox_windCompensation->isChecked();
}

bool CompetitionRulesEditorWidget::getHasGateCompensations()
{
    return ui->checkBox_gateCompensation->isChecked();
}

bool CompetitionRulesEditorWidget::getHasJudgesPoints()
{
    return ui->checkBox_judgesPoints->isChecked();
}

bool CompetitionRulesEditorWidget::getHasDsq()
{
    return ui->checkBox_dsq->isChecked();
}

int CompetitionRulesEditorWidget::getJumpersCountInTeam()
{
    return ui->spinBox_jumpersInTeamCount->value();
}

short CompetitionRulesEditorWidget::getCompetitionTypeFromInput()
{
    return ui->comboBox_competitionType->currentIndex();
}

short CompetitionRulesEditorWidget::getWindCompensationDistanceEffectFromInput()
{
    return ui->comboBox_windCompensationDistanceEffect->currentIndex();
}

short CompetitionRulesEditorWidget::getWindAverageCalculatingTypeFromInput()
{
    return ui->comboBox_windAverageCalculatingType->currentIndex();
}

CompetitionRules CompetitionRulesEditorWidget::getCompetitionRulesFromWidgetInputs()
{
    CompetitionRules rules;
    rules.setName(getNameFromInput());
    rules.setHas95HSRule(getHas95HSRuleFromInput());
    rules.setHasWindCompensations(getHasWindCompensations());
    rules.setHasGateCompensations(getHasGateCompensations());
    rules.setHasJudgesPoints(getHasJudgesPoints());
    rules.setHasDsq(getHasDsq());
    rules.setCompetitionType(getCompetitionTypeFromInput());
    rules.setRounds(getRoundsFromInput());
    rules.setWindAverageCalculatingType(getWindAverageCalculatingTypeFromInput());
    rules.setWindCompensationDistanceEffect(getWindCompensationDistanceEffectFromInput());
    return rules;
}

void CompetitionRulesEditorWidget::removeConnectsForWidgetChange()
{
    for(auto & w : MyFunctions::getWidgetsVector(this, "lineEdit")){
        disconnect(dynamic_cast<QLineEdit *>(w), &QLineEdit::textChanged, this, &CompetitionRulesEditorWidget::changed);
    }
    for(auto & w : MyFunctions::getWidgetsVector(this, "spinBox")){
        disconnect(dynamic_cast<QSpinBox *>(w), &QSpinBox::valueChanged, this, &CompetitionRulesEditorWidget::changed);
    }
    for(auto & w : MyFunctions::getWidgetsVector(this, "comboBox")){
        disconnect(dynamic_cast<QComboBox *>(w), &QComboBox::currentIndexChanged, this, &CompetitionRulesEditorWidget::changed);
    }
    for(auto & w : MyFunctions::getWidgetsVector(this, "checkBox")){
        disconnect(dynamic_cast<QCheckBox *>(w), &QCheckBox::stateChanged, this, &CompetitionRulesEditorWidget::changed);
    }
}

void CompetitionRulesEditorWidget::setupConnectsForWidgetChange()
{
    for(auto & w : MyFunctions::getWidgetsVector(this, "lineEdit")){
        connect(dynamic_cast<QLineEdit *>(w), &QLineEdit::textChanged, this, &CompetitionRulesEditorWidget::changed);
    }
    for(auto & w : MyFunctions::getWidgetsVector(this, "spinBox")){
        connect(dynamic_cast<QSpinBox *>(w), &QSpinBox::valueChanged, this, &CompetitionRulesEditorWidget::changed);
    }
    for(auto & w : MyFunctions::getWidgetsVector(this, "comboBox")){
        connect(dynamic_cast<QComboBox *>(w), &QComboBox::currentIndexChanged, this, &CompetitionRulesEditorWidget::changed);
    }
    for(auto & w : MyFunctions::getWidgetsVector(this, "checkBox")){
        connect(dynamic_cast<QCheckBox *>(w), &QCheckBox::stateChanged, this, &CompetitionRulesEditorWidget::changed);
    }
}

CompetitionRules *CompetitionRulesEditorWidget::getCompetitionRules() const
{
    return competitionRules;
}

void CompetitionRulesEditorWidget::setCompetitionRules(CompetitionRules *newCompetitionRules)
{
    competitionRules = newCompetitionRules;
}
