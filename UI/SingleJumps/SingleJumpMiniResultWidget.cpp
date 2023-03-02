#include "SingleJumpMiniResultWidget.h"
#include "ui_SingleJumpMiniResultWidget.h"

#include "../ResultsShowing/JumpDataDetailedInfoWindow.h"
#include <QMouseEvent>

SingleJumpMiniResultWidget::SingleJumpMiniResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleJumpMiniResultWidget)
{
    ui->setupUi(this);
}

SingleJumpMiniResultWidget::~SingleJumpMiniResultWidget()
{
    delete ui;
}

void SingleJumpMiniResultWidget::fillUi()
{
    ui->label_index->setText(QString::number(getIndexInList()) + ". ");
    ui->label_distance->setText(QString::number(jumpData->getDistance()) + "m");
    ui->label_points->setText(QString::number(jumpData->getPoints()) + "pts");

    double wind = jumpData->getAveragedWind();
    ui->label_wind->setText(QString::number(wind) + "m/s");

    if(wind > 0)
        ui->label_wind->setStyleSheet("QLabel{\n	color: rgb(28, 132, 105);\n}");
    else if(wind < 0)
        ui->label_wind->setStyleSheet("QLabel{\n	color: rgb(197, 11, 19);\n}");
    else ui->label_wind->setStyleSheet("QLabel{\n	color: rgb(50, 50, 50);\n}");

    ui->label_landingInfo->setText(Landing::getShortLandingTypeTextInfo(jumpData->getLanding().getType()));
    ui->label_landingInfo->setStyleSheet(Landing::getStyleSheetForLandingTypeTextInfo(jumpData->getLanding().getType()));
}

JumpData *SingleJumpMiniResultWidget::getJumpData() const
{
    return jumpData;
}

void SingleJumpMiniResultWidget::setJumpData(JumpData *newJumpData)
{
    jumpData = newJumpData;
}

int SingleJumpMiniResultWidget::getIndexInList() const
{
    return indexInList;
}

void SingleJumpMiniResultWidget::setIndexInList(int newIndexInList)
{
    indexInList = newIndexInList;
}

void SingleJumpMiniResultWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit pressed();
    }
}
