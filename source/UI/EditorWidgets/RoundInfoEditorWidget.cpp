#include "RoundInfoEditorWidget.h"
#include "ui_RoundInfoEditorWidget.h"

RoundInfoEditorWidget::RoundInfoEditorWidget(bool hideGroupsInfo, int competitiorsCount, bool sortStartList, short sortGroups, bool KO, int KOCount, int KOAdvance, QWidget *parent) :
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

    connect(ui->checkBox_KORound, &QCheckBox::stateChanged, this, [this](){
        if(ui->checkBox_KORound->isChecked() == true)
        {
            ui->spinBox_KOGroupCount->show();
            ui->spinBox_AdvancingFromKOGroup->show();
            ui->label_KOCount->show();
            ui->label_KOAdvanceCount->show();
        }
        else
        {
            ui->spinBox_KOGroupCount->hide();
            ui->spinBox_AdvancingFromKOGroup->hide();
            ui->label_KOCount->hide();
            ui->label_KOAdvanceCount->hide();
        }
    });

    ui->checkBox_KORound->setChecked(KO);
    ui->spinBox_KOGroupCount->setValue(KOCount);
    ui->spinBox_AdvancingFromKOGroup->setValue(KOAdvance);
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

        ui->checkBox_KORound->setChecked(roundInfo->getKO());
        ui->spinBox_KOGroupCount->setValue(roundInfo->getCountInKOGroup());
        ui->spinBox_AdvancingFromKOGroup->setValue(roundInfo->getAdvancingFromKOGroup());
    }
}

void RoundInfoEditorWidget::resetRoundInfoInput()
{
    ui->spinBox_count->setValue(0);
    ui->checkBox_sortStartList->setChecked(false);
    ui->comboBox_sortingAfterGroups->setCurrentIndex(3);

    ui->checkBox_KORound->setChecked(false);
    ui->spinBox_KOGroupCount->setValue(0);
    ui->spinBox_AdvancingFromKOGroup->setValue(0);
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

bool RoundInfoEditorWidget::getKORoundFromInput()
{
    return ui->checkBox_KORound->isChecked();
}

int RoundInfoEditorWidget::getKOGroupCountFromInput()
{
    return ui->spinBox_KOGroupCount->value();
}

int RoundInfoEditorWidget::getAdvancingFromKOGroup()
{
    return ui->spinBox_AdvancingFromKOGroup->value();
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
