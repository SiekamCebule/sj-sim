#include "JumperCompetitionResultsWidget.h"
#include "ui_JumperCompetitionResultsWidget.h"

#include "../ResultsShowing/JumpDataDetailedInfoWindow.h"
#include "../../global/CountryFlagsManager.h"

JumperCompetitionResultsWidget::JumperCompetitionResultsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JumperCompetitionResultsWidget)
{
    ui->setupUi(this);
    for(int i=0; i<ui->tabWidget_jumps->count(); i++){
        delete ui->tabWidget_jumps->widget(0);
        ui->tabWidget_jumps->removeTab(0);
    }
}

JumperCompetitionResultsWidget::~JumperCompetitionResultsWidget()
{
    delete ui;
}

void JumperCompetitionResultsWidget::fillWidget()
{
    ui->label_nameAndSurname->setText(jumperResult->getJumper()->getNameAndSurname());
    ui->label_flag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumperResult->getJumper()->getCountryCode().toLower())).scaled(ui->label_flag->size()));
    ui->label_pointsSum->setText(QString::number(jumperResult->getPointsSum()) + tr(" punktów"));
    //qDebug()<<jumperResult->getPosition()<<" pos";
    qDebug()<<jumperResult->getJumper()->getNameAndSurname()<<", "<<jumperResult->getPointsSum()<<", "<<jumperResult->getPosition();
    ui->label_actualPosition->setText("(" + QString::number(jumperResult->getPosition()) + tr(" miejsce)"));

    for(int i=0; i<ui->tabWidget_jumps->count(); i++){
        delete ui->tabWidget_jumps->widget(0);
        ui->tabWidget_jumps->removeTab(0);
    }
    int i = 0;
    for(auto & jump : jumperResult->getJumpsReference()){
        JumpDataDetailedInfoWindow * jumpInfo = new JumpDataDetailedInfoWindow(&jump, this);
        jumpInfo->fillJumpInformations();
        jumpInfo->removeSimulationInformationsLayouts();
        jumpInfo->removeJumperInfoTitle();
        jumpInfo->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        ui->tabWidget_jumps->addTab(jumpInfo, tr("Seria ") + QString::number(i + 1));

        i++;
    }
    ui->tabWidget_jumps->setCurrentIndex(ui->tabWidget_jumps->count() - 1);
}

CompetitionSingleResult *JumperCompetitionResultsWidget::getJumperResult() const
{
    return jumperResult;
}

void JumperCompetitionResultsWidget::setJumperResult(CompetitionSingleResult *newJumperResult)
{
    jumperResult = newJumperResult;
}
