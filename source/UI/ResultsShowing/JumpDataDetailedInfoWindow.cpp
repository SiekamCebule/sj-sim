#include "JumpDataDetailedInfoWindow.h"
#include "ui_JumpDataDetailedInfoWindow.h"

#include "../../global/CountryFlagsManager.h"
#include "../../global/GlobalAppSettings.h"
#include "../../utilities/functions.h"

JumpDataDetailedInfoWindow::JumpDataDetailedInfoWindow(JumpData *jumpData, QWidget *parent) :
    jumpData(jumpData),
    QWidget(parent),
    ui(new Ui::JumpDataDetailedInfoWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
    windInfoWidget = new JumpWindInfoWidget;
    windInfoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->verticalLayout_windsInfo->addWidget(windInfoWidget);
}

JumpDataDetailedInfoWindow::~JumpDataDetailedInfoWindow()
{
    delete ui;
}

void JumpDataDetailedInfoWindow::fillJumpInformations()
{
    if(jumpData != nullptr)
    {
        Jumper * jumper;
        jumper = jumpData->getJumper();

        ui->label_nameAndSurname->setText(jumper->getNameAndSurname());
        ui->label_flag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumper->getCountryCode().toLower())).scaled(ui->label_flag->size()));
        ui->label_distance->setText(QString::number(jumpData->getDistance()) + "m");
        ui->label_points->setText(QString::number(jumpData->getPoints()) + tr("pkt"));
        ui->label_gate->setText(QString::number(jumpData->getGate()));
        ui->label_gatePoints->setText(QString::number(jumpData->getGateCompensation()) + tr("pkt"));
        ui->label_gatePoints->setStyleSheet(Wind::getStyleSheetForAveragedWind(jumpData->getGateCompensation()));
        ui->label_averageWind->setText(QString::number(jumpData->getAveragedWind()) + tr("m/s"));
        ui->label_averageWind->setStyleSheet(Wind::getStyleSheetForAveragedWind(jumpData->getAveragedWind()));
        ui->label_windPoints->setText(QString::number(jumpData->getWindCompensation()) + tr("pkt"));
        ui->label_windPoints->setStyleSheet(Wind::getStyleSheetForAveragedWind(jumpData->getWindCompensation()));
        ui->label_totalCompensation->setText(QString::number(jumpData->getTotalCompensation()) + tr("pkt"));
        ui->label_totalCompensation->setStyleSheet(Wind::getStyleSheetForAveragedWind(jumpData->getTotalCompensation()));
        ui->label_judges->setText("| " + QString::number(jumpData->getJudges().at(0)) + " | ");
        for(int i=0; i < 4; i++) //jedno zostalo juz wykonane
        {
            ui->label_judges->setText(ui->label_judges->text() + QString::number(jumpData->getJudges().at(i+1)) + " | ");
        }
        ui->label_judgesPoints->setText(QString::number(jumpData->getJudgesPoints()) + tr("pkt"));
        ui->label_landingType->setText(jumpData->getLanding().getTextLandingType());
        ui->label_landingType->setStyleSheet(Landing::getStyleSheetForLandingTypeTextInfo(jumpData->getLanding().getType()));

        ui->label_takeoffRating->setText(QString::number(jumpData->getSimulationDataReference().getTakeoffRating()));
        ui->label_flightRating->setText(QString::number(jumpData->getSimulationDataReference().getFlightRating()));
        ui->label_landingRating->setText(QString::number(jumpData->getLanding().getRating()));
        ui->label_snowRain->setText(QString::number(jumpData->getSimulationDataReference().getInrunSnow()));

        if(jumpData->getHasCoachGate()){
            ui->label_beats95HSPercents->show();
            if(jumpData->getBeats95HSPercents()){
                ui->label_beats95HSPercents->setText(tr("Zawodnik osiągnął 95% punktu HS"));
                ui->label_beats95HSPercents->setStyleSheet("color: rgb(49, 76, 44);");
            }
            else{
                ui->label_beats95HSPercents->setText(tr("Zawodnik nie osiągnął 95% punktu HS"));
                ui->label_beats95HSPercents->setStyleSheet("color: rgb(112, 37, 31);");
            }
            ui->label_beats95HSPercents->setText(ui->label_beats95HSPercents->text() + " (" + QString::number(roundDoubleToHalf(jumpData->getHill()->getHSPoint() * 0.95)) + "m)");
        }
        else{
            ui->label_beats95HSPercents->hide();
        }

        windInfoWidget->setWinds(jumpData->getWindsPointer());
        windInfoWidget->setKPoint(jumpData->getHill()->getKPoint());
        windInfoWidget->fillItemsLayout();

        showOrHideByAppSettings();

        if(jumpData->getCompetition() != nullptr){
        if(jumpData->getCompetition()->getRulesPointer()->getCompetitionType() == CompetitionRules::Individual)
        {
            double round = MyFunctions::getIndexOfItemInVector(jumpData->getCompetition()->getResultsReference().getResultOfIndividualJumper(jumpData->getJumper())->getJumpsReference(), jumpData) + 1;
            double points = jumpData->getPoints();
            double position = 1;
            CompetitionResults results = jumpData->getCompetition()->getResultsReference();
            results.getResultsReference().detach();
            for(auto & res : results.getResultsReference())
            {
                if(res.getJumpsReference().count() > round)
                    res.getJumpsReference().remove(round, res.getJumpsReference().count() - round);
                res.updatePointsSum();
            }
            results.sortInDescendingOrder();

            for(auto & res : results.getResultsReference())
            {
                    if(res.getJumpsReference()[round - 1].getPoints() > points)
                        position++;
            }
            ui->label_positionInRound->setText(QString::number(position));
        }
        else
        {
            ui->label_positionInRoundTitle->hide();
            ui->label_positionInRoundTitle->hide();
        }
        }
        else
        {
            ui->label_positionInRoundTitle->hide();
            ui->label_positionInRoundTitle->hide();
        }
    }
    else qDebug()<<"jumpData is nullptr!";
}

void JumpDataDetailedInfoWindow::showOrHideByAppSettings()
{
    JumpDataInfoChoice * c = &GlobalAppSettings::get()->getJumpDataInfoChoiceReference();
    ui->label_distanceTitle->setHidden(!c->getDistance());
    ui->label_distance->setHidden(!c->getDistance());
    ui->label_pointsTitle->setHidden(!c->getPoints());
    ui->label_points->setHidden(!c->getPoints());
    ui->label_gateTitle->setHidden(!c->getGate());
    ui->label_gate->setHidden(!c->getGate());
    ui->label_averageWindTitle->setHidden(!c->getAveragedWind());
    ui->label_averageWind->setHidden(!c->getAveragedWind());
    ui->label_gatePointsTitle->setHidden(!c->getGateCompensation());
    ui->label_gatePoints->setHidden(!c->getGateCompensation());
    ui->label_windCompensationTitle->setHidden(!c->getWindCompensation());
    ui->label_windPoints->setHidden(!c->getWindCompensation());
    ui->label_totalCompensationTitle->setHidden(!c->getTotalCompensation());
    ui->label_totalCompensation->setHidden(!c->getTotalCompensation());
    ui->label_judgesTItle->setHidden(!c->getJudges());
    ui->label_judges->setHidden(!c->getJudges());
    ui->label_judgesPointsTitle->setHidden(!c->getJudgesPoints());
    ui->label_judgesPoints->setHidden(!c->getJudgesPoints());
    ui->label_landingTypeTitle->setHidden(!c->getLandingType());
    ui->label_landingType->setHidden(!c->getLandingType());
    windInfoWidget->setHidden(!c->getSpecificWind());
    ui->label_takeoffRatingTitle->setHidden(!c->getTakeoffRating());
    ui->label_takeoffRating->setHidden(!c->getTakeoffRating());
    ui->label_flightRatingTitle->setHidden(!c->getFlightRating());
    ui->label_flightRating->setHidden(!c->getFlightRating());
    ui->label_landingRatingTitle->setHidden(!c->getLandingRating());
    ui->label_landingRating->setHidden(!c->getLandingRating());
    ui->label_snowRainTitle->setHidden(!c->getInrunSnow());
    ui->label_snowRain->setHidden(!c->getInrunSnow());
    ui->label_positionInRoundTitle->setHidden(!c->getJumpPositionInRound());
    ui->label_positionInRound->setHidden(!c->getJumpPositionInRound());
}

void JumpDataDetailedInfoWindow::removeJumperInfoTitle()
{
    if(ui->horizontalLayout_nameSurnameAndFlag != nullptr){
        QLayoutItem * item;
        while((item = ui->horizontalLayout_nameSurnameAndFlag->takeAt(0)) != NULL)
        {
            delete item->widget();
            delete item;
        }
    }
    delete ui->horizontalLayout_nameSurnameAndFlag;
    if(ui->horizontalLayout_jumperInfoTitle != nullptr){
        QLayoutItem * item;
        while((item = ui->horizontalLayout_jumperInfoTitle->takeAt(0)) != NULL)
        {
            delete item->widget();
            delete item;
        }
    }
    delete ui->horizontalLayout_jumperInfoTitle;
}

void JumpDataDetailedInfoWindow::removeWindsInfoLayout()
{
    if (ui->verticalLayout_windsInfo){
        while(ui->verticalLayout_windsInfo->count() > 0){
            QLayoutItem *item = ui->verticalLayout_windsInfo->takeAt(0);
            delete item->widget();
            delete item;
        }
    }
    delete ui->verticalLayout_windsInfo;
}

JumpData *JumpDataDetailedInfoWindow::getJumpData() const
{
    return jumpData;
}

void JumpDataDetailedInfoWindow::setJumpData(JumpData *newJumpData)
{
    jumpData = newJumpData;
}
