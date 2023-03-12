#include "CompetitionStartListDisplayItemWidget.h"
#include "ui_CompetitionStartListDisplayItemWidget.h"
#include "../../../global/CountryFlagsManager.h"
#include "CompetitionStartListDisplayWidget.h"

#include <QMouseEvent>

CompetitionStartListDisplayItemWidget::CompetitionStartListDisplayItemWidget(short competitiorType, QWidget *parent) :
    competitiorType(competitiorType),
    QWidget(parent),
    ui(new Ui::CompetitionStartListDisplayItemWidget)
{
    ui->setupUi(this);

    ui->label_index->installEventFilter(this);
    ui->label_name->installEventFilter(this);

    connect(this, &CompetitionStartListDisplayItemWidget::selectionChanged, this, [this](){
        if(getIsActive() == true){
            if(getIsSelected() == true){
                QFont font = ui->label_name->font();
                font.setBold(true);
                ui->label_name->setFont(font);
                ui->label_name->setStyleSheet("color: rgb(51, 109, 184);");
            }
            else{
                QFont font = ui->label_name->font();
                font.setBold(false);
                ui->label_name->setFont(font);
                ui->label_name->setStyleSheet("color: rgb(10, 10, 10);");
            }
        }
    });
}

CompetitionStartListDisplayItemWidget::~CompetitionStartListDisplayItemWidget()
{
    ui->label_index->removeEventFilter(this);
    delete ui;
}

void CompetitionStartListDisplayItemWidget::setIsActive(bool arg)
{
    ui->checkBox_active->setChecked(arg);
}

bool CompetitionStartListDisplayItemWidget::getIsActive()
{
    return ui->checkBox_active->isChecked();
}

void CompetitionStartListDisplayItemWidget::fillWidget()
{
    switch(getCompetitiorType()){
    case CompetitionStartListDisplayWidget::Jumpers:
        ui->label_name->setText(jumper->getNameAndSurname());
        ui->label_flag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumper->getCountryCode().toLower())).scaled(ui->label_flag->size()));
        break;
    case CompetitionStartListDisplayWidget::Teams:
        break;
    }

    if(getIsActive() == true){
        ui->label_index->setText(QString::number(getIndexInParentList()));
        ui->label_name->setStyleSheet("color: rgb(0, 0, 0);");
    }
    else{
        ui->label_index->setText("X");
        ui->label_name->setStyleSheet("color: rgb(90, 90, 90);");
    }
}

void CompetitionStartListDisplayItemWidget::on_checkBox_active_stateChanged(int arg1)
{
    if(ui->checkBox_active->isChecked() == true)
    {
        ui->checkBox_active->setText(tr("Aktywny"));
        ui->label_index->setText(QString::number(getIndexInParentList()));
        ui->label_name->setStyleSheet("color: rgb(0, 0, 0);");
    }
    else{
        ui->checkBox_active->setText(tr("Nieaktywny"));
        ui->label_index->setText("X");
        ui->label_name->setStyleSheet("color: rgb(90, 90, 90);");
    }
    emit activityChanged();
}

short CompetitionStartListDisplayItemWidget::getCompetitiorType() const
{
    return competitiorType;
}

void CompetitionStartListDisplayItemWidget::setCompetitiorType(short newCompetitiorType)
{
    competitiorType = newCompetitiorType;
}

bool CompetitionStartListDisplayItemWidget::getIsSelected() const
{
    return isSelected;
}

void CompetitionStartListDisplayItemWidget::setIsSelected(bool newIsSelected)
{
    isSelected = newIsSelected;
    emit selectionChanged();
}

int CompetitionStartListDisplayItemWidget::getIndexInParentList() const
{
    return indexInParentList;
}

void CompetitionStartListDisplayItemWidget::setIndexInParentList(int newIndexInParentList)
{
    indexInParentList = newIndexInParentList;
}

bool CompetitionStartListDisplayItemWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->label_index)
    {
        if(event->type() == QEvent::MouseButtonDblClick)
        {
            emit doubleClicked();
        }
    }
    else if(watched == ui->label_name)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            if(getIsActive() == true){
                setIsSelected(true);
                emit selected();
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

Jumper *CompetitionStartListDisplayItemWidget::getJumper() const
{
    return jumper;
}

void CompetitionStartListDisplayItemWidget::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}

