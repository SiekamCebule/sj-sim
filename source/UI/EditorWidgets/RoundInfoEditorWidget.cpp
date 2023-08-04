#include "RoundInfoEditorWidget.h"
#include "ui_RoundInfoEditorWidget.h"

RoundInfoEditorWidget::RoundInfoEditorWidget(bool hideGroupsInfo, int competitiorsCount, bool sortStartList, short sortGroups, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoundInfoEditorWidget)
{
    roundInfo = nullptr;
    ui->setupUi(this);

    if(hideGroupsInfo == true){
        ui->comboBox_sortingAfterGroups->hide();
        ui->label_sortingGroups->hide();
    }
    ui->spinBox_count->setValue(competitiorsCount);
    ui->checkBox_sortStartList->setChecked(sortStartList);
    ui->comboBox_sortingAfterGroups->setCurrentIndex(sortGroups);
}

RoundInfoEditorWidget::~RoundInfoEditorWidget()
{
    delete ui;
}

void RoundInfoEditorWidget::fillRoundInfoInput()
{
    if(roundInfo == nullptr)
        qDebug()<<"roundInfo is nullptr!";
    else{
        ui->spinBox_count->setValue(roundInfo->getCount());
        ui->checkBox_sortStartList->setChecked(roundInfo->getSortStartList());
        ui->comboBox_sortingAfterGroups->setCurrentIndex(roundInfo->getSortAfterGroups());
    }
}

void RoundInfoEditorWidget::resetRoundInfoInput()
{
    ui->spinBox_count->setValue(0);
    ui->checkBox_sortStartList->setChecked(false);
    ui->comboBox_sortingAfterGroups->setCurrentIndex(3);
}

int RoundInfoEditorWidget::getCountFromInput()
{
    return ui->spinBox_count->value();
}

bool RoundInfoEditorWidget::getSortStartListFromInput()
{
    return ui->checkBox_sortStartList->isChecked();
}

int RoundInfoEditorWidget::getSortAfterGroupsFromInput()
{
    return ui->comboBox_sortingAfterGroups->currentIndex();
}

void RoundInfoEditorWidget::hideGroupsInfo()
{
    ui->comboBox_sortingAfterGroups->hide();
    ui->label_sortingGroups->hide();
}

void RoundInfoEditorWidget::showGroupsInfo()
{
    ui->comboBox_sortingAfterGroups->show();
    ui->label_sortingGroups->show();
}

RoundInfo *RoundInfoEditorWidget::getRoundInfo() const
{
    return roundInfo;
}

void RoundInfoEditorWidget::setRoundInfo(RoundInfo *newRoundInfo)
{
    roundInfo = newRoundInfo;
}
