#include "CompetitionManagerWindow.h"
#include "ui_CompetitionManagerWindow.h"
#include "../../competitions/IndividualCompetitions/IndividualCompetitionManager.h"

CompetitionManagerWindow::CompetitionManagerWindow(AbstractCompetitionManager *manager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompetitionManagerWindow),
    manager(manager)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    if(manager != nullptr)
        type = manager->getType();

    startListModel = new StartListModel(this);
    startListModel->setType(getType());
    switch(type)
    {
    case CompetitionRules::Individual:
        startListModel->setJumpers(dynamic_cast<IndividualCompetitionManager *>(this->manager)->getActualRoundJumpersPointer());
    }
    startListModel->fillCompletedJumpsWithFalse();
    ui->listView_startList->setModel(startListModel);
}

CompetitionManagerWindow::~CompetitionManagerWindow()
{
    delete ui;
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
