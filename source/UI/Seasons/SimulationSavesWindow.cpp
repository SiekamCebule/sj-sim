#include "SimulationSavesWindow.h"
#include "ui_SimulationSavesWindow.h"

#include "../../global/GlobalDatabase.h"
#include "../../seasons/SimulationSave.h"
#include "../../seasons/Season.h"
#include "../../seasons/SeasonsManager.h"
#include "NewSeasonConfiguratorWindow.h"
#include "NewSimulationSaveConfigurationWindow.h"

SimulationSavesWindow::SimulationSavesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulationSavesWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);
    listModel = new SimulationSavesListModel(&GlobalDatabase::get()->getEditableGlobalSimulationSaves(), this);
    ui->listView_simulationSaves->setModel(listModel);

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
    NewSimulationSaveConfigurationWindow * simulationSaveWindow = new NewSimulationSaveConfigurationWindow(this);
    if(simulationSaveWindow->exec() == QDialog::Accepted){
        NewSeasonConfiguratorWindow * seasonWindow = new NewSeasonConfiguratorWindow(this);
        if(seasonWindow->exec() == QDialog::Accepted){

            qDebug()<<"no";
            SimulationSave simulationSave;
            simulationSave.setName(simulationSaveWindow->getNameFromInput());
            qDebug()<<"no";

            Season season;
            season.setSeasonNumber(simulationSaveWindow->getSeasonNumberFromInput());
            qDebug()<<"no";

            SeasonsManager * manager = new SeasonsManager;
            //manager->setJumpers(seasonWindow->getJumpersFromEditor());
            //season.setCalendar(seasonWindow->getCalendarFromEditor());
            //season.setClassifications(seasonWindow->getClassificationsFromEditor());
            //season.setSettings(seasonWindow->getSettingsFromInputs());
            simulationSave.getSeasonsReference().push_back(season);
            qDebug()<<"no";



            int index = 0;
            if(ui->listView_simulationSaves->selectionModel()->selectedRows().size() > 0)
                index = ui->listView_simulationSaves->selectionModel()->selectedRows().first().row();
            qDebug()<<"no";

            GlobalDatabase::get()->getEditableGlobalSimulationSaves().insert(index, simulationSave);
            qDebug()<<"no";

            emit listModel->dataChanged(listModel->index(index), listModel->index(listModel->rowCount() - 1));
            qDebug()<<"no";

        }
    }
}

