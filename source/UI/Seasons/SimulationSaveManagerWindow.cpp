#include "SimulationSaveManagerWindow.h"
#include "ui_SimulationSaveManagerWindow.h"

SimulationSaveManagerWindow::SimulationSaveManagerWindow(SimulationSave *simulationSave, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulationSaveManagerWindow),
    simulationSave(simulationSave)
{
    ui->setupUi(this);

    jumpersListView = new DatabaseItemsListView(DatabaseItemsListView::JumperItems, true, this);
    jumpersListView->setJumpers(&simulationSave->getJumpersReference());
    jumpersListView->setupListModel();
    ui->verticalLayout_jumpersList->addWidget(jumpersListView);

    jumperEditor = new JumperEditorWidget();
    jumperEditor->hide();
    ui->verticalLayout_jumperEditor->addWidget(jumperEditor);
    connect(jumpersListView, &DatabaseItemsListView::listViewDoubleClicked, this, [this, simulationSave](const QModelIndex & index){
        jumperEditor->show();
        jumperEditor->setJumper(&simulationSave->getJumpersReference()[index.row()]);
        jumperEditor->fillJumperInputs();
    });
    connect(jumperEditor, &JumperEditorWidget::submitted, this, [this, simulationSave](){
        int idx = jumpersListView->getLastDoubleClickedIndex();
        simulationSave->getJumpersReference()[idx] = jumperEditor->getJumperFromWidgetInput();
        emit jumpersListView->getListModel()->dataChanged(jumpersListView->getListModel()->index(idx), jumpersListView->getListModel()->index(idx));
    });

    ui->label_saveName->setText(simulationSave->getName() + ": ");
    ui->label_seasonNumber->setText(tr("Sezon ") + QString::number(simulationSave->getActualSeason()->getSeasonNumber()));
}

SimulationSaveManagerWindow::~SimulationSaveManagerWindow()
{
    delete ui;
}
