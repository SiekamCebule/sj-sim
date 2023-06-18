#include "RoundInfoEditorWidget.h"
#include "ui_RoundInfoEditorWidget.h"

RoundInfoEditorWidget::RoundInfoEditorWidget(bool hideGroupsInfo, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoundInfoEditorWidget)
{
    roundInfo = nullptr;
    ui->setupUi(this);

    if(hideGroupsInfo == true)
        ui->comboBox_sortingAfterGroups->hide();
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
