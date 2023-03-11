#include "CompetitionStartListDisplayItemWidget.h"
#include "ui_CompetitionStartListDisplayItemWidget.h"
#include "../../../global/CountryFlagsManager.h"

#include <QMouseEvent>

CompetitionStartListDisplayItemWidget::CompetitionStartListDisplayItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompetitionStartListDisplayItemWidget)
{
    ui->setupUi(this);

    ui->label_index->installEventFilter(this);
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
    ui->label_name->setText(jumper->getNameAndSurname());
    ui->label_flag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumper->getCountryCode().toLower())).scaled(ui->label_flag->size()));
    ui->label_index->setText(QString::number(getIndexInParentList()));
}

void CompetitionStartListDisplayItemWidget::on_checkBox_active_stateChanged(int arg1)
{
    if(ui->checkBox_active->isChecked() == true)
    {
        ui->checkBox_active->setText("Aktywny");
    }
    else{
        ui->checkBox_active->setText("Nieaktywny");
    }
    emit activityChanged();
}

int CompetitionStartListDisplayItemWidget::getIndexInParentList() const
{
    return indexInParentList;
}

void CompetitionStartListDisplayItemWidget::setIndexInParentList(int newIndexInParentList)
{
    indexInParentList = newIndexInParentList;
}

void CompetitionStartListDisplayItemWidget::uninstallEventFilterForThis()
{
    ui->label_index->removeEventFilter(this);
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

