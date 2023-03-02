#include "JumpDataDetailedInfoWindow.h"
#include "ui_JumpDataDetailedInfoWindow.h"

#include "../../global/CountryFlagsManager.h"

JumpDataDetailedInfoWindow::JumpDataDetailedInfoWindow(JumpData *jumpData, QWidget *parent) :
    jumpData(jumpData),
    QWidget(parent),
    ui(new Ui::JumpDataDetailedInfoWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
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
    }
    else qDebug()<<"jumpData is nullptr!";
}

JumpData *JumpDataDetailedInfoWindow::getJumpData() const
{
    return jumpData;
}

void JumpDataDetailedInfoWindow::setJumpData(JumpData *newJumpData)
{
    jumpData = newJumpData;
}
