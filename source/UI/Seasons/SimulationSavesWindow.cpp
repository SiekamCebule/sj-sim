#include "SimulationSavesWindow.h"
#include "ui_SimulationSavesWindow.h"

#include "../../global/GlobalDatabase.h"
#include "../../seasons/SimulationSave.h"
#include "../../seasons/Season.h"
#include "NewSeasonConfiguratorWindow.h"
#include "NewSimulationSaveConfigurationWindow.h"
#include "SimulationSaveManagerWindow.h"
#include <QTimer>

SimulationSavesWindow::SimulationSavesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulationSavesWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);
    listModel = new SimulationSavesListModel(&GlobalDatabase::get()->getEditableGlobalSimulationSaves(), this);
    ui->listView_simulationSaves->setModel(listModel);
    ui->listView_simulationSaves->setSelectionMode(QAbstractItemView::SingleSelection);

}

SimulationSavesWindow::~SimulationSavesWindow()
{
    delete ui;
}

void SimulationSavesWindow::on_listView_simulationSaves_doubleClicked(const QModelIndex &index)
{
}

void SimulationSavesWindow::on_pushButton_add_clicked()
{
    QStringList otherNames;
    for(auto & save : GlobalDatabase::get()->getEditableGlobalSimulationSaves()){
        otherNames.push_back(save.getName());
    }
    NewSimulationSaveConfigurationWindow * simulationSaveWindow = new NewSimulationSaveConfigurationWindow(otherNames, this);
    if(simulationSaveWindow->exec() == QDialog::Accepted){
        NewSeasonConfiguratorWindow * seasonWindow = new NewSeasonConfiguratorWindow(this);
        connect(seasonWindow->getToolBox(), &QToolBox::currentChanged, this, [this, seasonWindow](){
            if(seasonWindow->getToolBox()->currentIndex() == 3){ //edytor kalendarzy
                QTimer::singleShot(200, [this, seasonWindow](){
                    seasonWindow->showCalendarEditorHelp();
                });
            }
        });
        if(seasonWindow->exec() == QDialog::Accepted){
            SimulationSave simulationSave;
            simulationSave.setName(simulationSaveWindow->getNameFromInput());
            simulationSave.setJumpers(seasonWindow->getJumpersReference());
            simulationSave.setHills(seasonWindow->getHillsReference());
            simulationSave.setCompetitionRules(seasonWindow->getCompetitionsRulesReference());

            Season season;
            season.setSeasonNumber(simulationSaveWindow->getSeasonNumberFromInput());
            season.setCalendar(seasonWindow->getCalendar());
            season.getCalendarReference().updateCompetitionsQualifyingCompetitions();

            simulationSave.getSeasonsReference().push_back(season);
            simulationSave.setActualSeason(&simulationSave.getSeasonsReference().first());

            int index = 0;
            if(ui->listView_simulationSaves->selectionModel()->selectedRows().size() > 0)
                index = ui->listView_simulationSaves->selectionModel()->selectedRows().first().row();

            GlobalDatabase::get()->getEditableGlobalSimulationSaves().insert(index, simulationSave);

            emit listModel->dataChanged(listModel->index(index), listModel->index(listModel->rowCount() - 1));

            simulationSave.saveToFile("simulationSaves/");
        }
    }
}


void SimulationSavesWindow::on_pushButton_OK_clicked()
{
    if(ui->listView_simulationSaves->selectionModel()->selectedRows().count() > 0){
        SimulationSaveManagerWindow * manager = new SimulationSaveManagerWindow(&GlobalDatabase::get()->getEditableGlobalSimulationSaves()[ui->listView_simulationSaves->selectionModel()->selectedRows().first().row()], this);
        if(manager->exec() == QDialog::Accepted)
        {

        }
    }
}

