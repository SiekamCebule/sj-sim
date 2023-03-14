#include "TeamEditorWidget.h"
#include "ui_TeamEditorWidget.h"

#include "../../../global/CountryFlagsManager.h"

TeamEditorWidget::TeamEditorWidget(const QString &teamCode, const QVector<Jumper *> & jumpers, QWidget * parent) : teamCode(teamCode),
    jumpers(jumpers),
    ui(new Ui::TeamEditorWidget),
    QWidget(parent)
{
    ui->setupUi(this);

    model = new QStandardItemModel(this);
    ui->listView_jumpers->setModel(model);

    connect(this, &TeamEditorWidget::needToUpdateModel, this, &TeamEditorWidget::updateModelByItemsListVector);
    connect(ui->pushButton_submit, &QPushButton::clicked, this, &TeamEditorWidget::when_submitButtonClicked);
}

TeamEditorWidget::~TeamEditorWidget()
{
    delete ui;
}

void TeamEditorWidget::fillWidgetInputs()
{
    ui->label_teamCountryCode->setText(getTeamCode());
    ui->label_flagPixmap->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(getTeamCode().toLower())).scaled(ui->label_flagPixmap->size()));
    setupItemsList();
}

void TeamEditorWidget::setupItemsList()
{
    QPixmap flagPixmap = CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(getTeamCode().toLower())).scaled(ui->label_flagPixmap->size());
    for(auto & item : itemsList)
        delete item;
    itemsList.clear();

    int index = 0;
    for(const auto & jumper : jumpers)
    {
        QString addition = "";
        if((index) < getJumpersCount())
            addition = tr("   --> Wybrany do składu");

        itemsList.push_back(new QStandardItem(QPixmap(flagPixmap), jumper->getNameAndSurname() + addition));
        index++;
    }
    emit needToUpdateModel();
}

void TeamEditorWidget::removeSubmitButton()
{
    disconnect(ui->pushButton_submit, &QPushButton::clicked, this, &TeamEditorWidget::when_submitButtonClicked);
    ui->verticalLayout->removeWidget(ui->pushButton_submit);
    delete ui->pushButton_submit;
}

Team TeamEditorWidget::constructTeamFromWidgetInput()
{
    Team team;
    return team;
}

int TeamEditorWidget::getJumpersCount() const
{
    return jumpersCount;
}

void TeamEditorWidget::setJumpersCount(int newJumpersCount)
{
    jumpersCount = newJumpersCount;
}

void TeamEditorWidget::when_submitButtonClicked()
{
    emit submitted();
}

void TeamEditorWidget::updateModelByItemsListVector()
{
    int i = 0;
    for(auto & item : itemsList){
        model->setItem(i, 0, item);
        i++;
    }
}

QString TeamEditorWidget::getTeamCode() const
{
    return teamCode;
}

void TeamEditorWidget::setTeamCode(const QString &newTeamCode)
{
    teamCode = newTeamCode;
}

void TeamEditorWidget::on_pushButton_up_clicked()
{
    int index = -1;
    if(ui->listView_jumpers->selectionModel()->selectedRows().size() > 0)
        index = ui->listView_jumpers->selectionModel()->selectedRows().first().row();

    if(index > (0) && index < itemsList.count())
    {
        jumpers.swapItemsAt(index, index - 1);
        setupItemsList();
        ui->listView_jumpers->clearSelection();
        ui->listView_jumpers->setCurrentIndex(model->index(index - 1, 0));
    }
}


void TeamEditorWidget::on_pushButton_down_clicked()
{
    int index = -1;
    if(ui->listView_jumpers->selectionModel()->selectedRows().size() > 0)
        index = ui->listView_jumpers->selectionModel()->selectedRows().first().row();

    if(index > (0) && index < itemsList.count())
    {
        itemsList.swapItemsAt(index, index + 1);
        emit needToUpdateModel();
    }
    setupItemsList();
}
