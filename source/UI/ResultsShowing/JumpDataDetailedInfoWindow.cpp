#include "JumpDataDetailedInfoWindow.h"
#include "ui_JumpDataDetailedInfoWindow.h"

#include "../../global/CountryFlagsManager.h"
#include "../../utilities/functions.h"

JumpDataDetailedInfoWindow::JumpDataDetailedInfoWindow(JumpData *jumpData, QWidget *parent) :
    jumpData(jumpData),
    QWidget(parent),
    ui(new Ui::JumpDataDetailedInfoWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
    windInfoWidget = new JumpWindInfoWidget;
    windInfoWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
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

        ui->label_takeoffRating->setText(QString::number(jumpData->getSimulationData().getTakeoffRating()));
        ui->label_flightRating->setText(QString::number(jumpData->getSimulationData().getFlightRating()));
        ui->label_judgesRating->setText(QString::number(jumpData->getSimulationData().getJudgesRating()));
        ui->label_landingInstability->setText(QString::number(jumpData->getLanding().getImbalance()));

        if(jumpData->getHasCoachGate()){
            ui->label_beats95HSPercents->show();
            if(jumpData->getBeats95HSPercents()){
                ui->label_beats95HSPercents->setText("Zawodnik osiągnął 95% punktu HS");
                ui->label_beats95HSPercents->setStyleSheet("color: rgb(49, 76, 44);");
            }
            else{
                ui->label_beats95HSPercents->setText("Zawodnik nie osiągnął 95% punktu HS");
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
    }
    else qDebug()<<"jumpData is nullptr!";
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

void JumpDataDetailedInfoWindow::removeSimulationInformationsLayouts()
{
    if (ui->formLayout_simulationRating) {
        while(ui->formLayout_simulationRating->count() > 0){
            QLayoutItem *item = ui->formLayout_simulationRating->takeAt(0);
            delete item->widget();
            delete item;
        }
    }
    delete ui->formLayout_simulationRating;

    if (ui->formLayout_weatherInfos) {
        while(ui->formLayout_weatherInfos->count() > 0){
            QLayoutItem *item = ui->formLayout_weatherInfos->takeAt(0);
            delete item->widget();
            delete item;
        }
    }
    delete ui->formLayout_weatherInfos;
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
