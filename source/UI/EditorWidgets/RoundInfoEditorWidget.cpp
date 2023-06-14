#include "RoundInfoEditorWidget.h"
#include "ui_RoundInfoEditorWidget.h"

RoundInfoEditorWidget::RoundInfoEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoundInfoEditorWidget)
{
    roundInfo = nullptr;
    ui->setupUi(this);
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

RoundInfo *RoundInfoEditorWidget::getRoundInfo() const
{
    return roundInfo;
}

void RoundInfoEditorWidget::setRoundInfo(RoundInfo *newRoundInfo)
{
    roundInfo = newRoundInfo;
}
