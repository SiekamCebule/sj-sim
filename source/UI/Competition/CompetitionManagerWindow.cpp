#include "CompetitionManagerWindow.h"
#include "ui_CompetitionManagerWindow.h"
#include "../../competitions/IndividualCompetitions/IndividualCompetitionManager.h"
#include "../../global/CountryFlagsManager.h"

CompetitionManagerWindow::CompetitionManagerWindow(AbstractCompetitionManager *manager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompetitionManagerWindow),
    manager(manager)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    if(manager != nullptr)
        type = manager->getType();

    startListModel = new StartListModel(manager, this);
    startListModel->setType(getType());
    switch(type)
    {
    case CompetitionRules::Individual:
        IndividualCompetitionManager * indManager = dynamic_cast<IndividualCompetitionManager *>(this->manager);
        startListModel->setJumpers(indManager->getActualRoundJumpersPointer());
        connect(indManager, &IndividualCompetitionManager::actualJumperIndexChanged, this, [this, indManager](){
            ui->label_nameAndSurname->setText(indManager->getActualJumper()->getNameAndSurname());
            ui->label_flag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(indManager->getActualJumper()->getCountryCode().toLower())).scaled(ui->label_flag->size()));
        });
        break;
    }
    ui->listView_startList->setModel(startListModel);

    resultsTableModel = new ResultsTableModel(getType(), manager->getResults(), manager->getCompetitionInfo(), this);
    ui->tableView_results->setModel(resultsTableModel);
    ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

CompetitionManagerWindow::~CompetitionManagerWindow()
{
    delete ui;
}

JumpManipulator CompetitionManagerWindow::getCurrentInputJumpManipulator() const
{
    return currentInputJumpManipulator;
}

void CompetitionManagerWindow::setCurrentInputJumpManipulator(const JumpManipulator &newCurrentInputJumpManipulator)
{
    currentInputJumpManipulator = newCurrentInputJumpManipulator;
}

short CompetitionManagerWindow::getType() const
{
    return type;
}

void CompetitionManagerWindow::setType(short newType)
{
    type = newType;
}

AbstractCompetitionManager *CompetitionManagerWindow::getManager() const
{
    return manager;
}

void CompetitionManagerWindow::on_pushButton_jump_clicked()
{
    switch(getType())
    {
    case CompetitionRules::Individual:{
        IndividualCompetitionManager * m = dynamic_cast<IndividualCompetitionManager *>(manager);
        m->simulateNext(JumpManipulator());
        emit startListModel->dataChanged(startListModel->index(m->getActualJumperIndex() - 1), startListModel->index(m->getActualJumperIndex() - 1));
        ui->tableView_results->setModel(nullptr);
        ui->tableView_results->setModel(resultsTableModel);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        break;
    }
    }
}

