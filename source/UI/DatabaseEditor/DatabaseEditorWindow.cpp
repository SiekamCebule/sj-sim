#include "DatabaseEditorWindow.h"
#include "ui_DatabaseEditorWindow.h"

#include "../../global/GlobalDatabase.h"
#include "../../global/CountryFlagsManager.h"
#include "../../simulator/Hill.h"
#include "../EditorWidgets/JumperEditorWidget.h"
#include "../EditorWidgets/HillEditorWidget.h"
#include "../EditorWidgets/CompetitionRulesEditorWidget.h"
#include "ListModels/JumpersListModel.h"
#include "ListModels/HillsListModel.h"
#include "ListModels/CompetitionRulesListModel.h"
#include "DatabaseItemsListView.h"

#include <QFont>
#include <QScrollArea>
#include <QCloseEvent>
#include <QMessageBox>

DatabaseEditorWindow::DatabaseEditorWindow(JumperEditorWidget * jumperEditor, HillEditorWidget *hillEditor, CompetitionRulesEditorWidget * competitionRulesEditor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseEditorWindow),
    jumperEditor(jumperEditor),
    hillEditor(hillEditor),
    competitionRulesEditor(competitionRulesEditor),
    actualElementType(0),
    actualElementIndex(0)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tabWidget_main->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    tempGlobalJumpers = GlobalDatabase::get()->getGlobalJumpers();
    tempGlobalHills = GlobalDatabase::get()->getGlobalHills();
    tempGlobalCompetitionRules = GlobalDatabase::get()->getGlobalCompetitionsRules();

    if(this->jumperEditor == nullptr)
        this->jumperEditor = new JumperEditorWidget;
    ui->tab_jumpers->layout()->addWidget(this->jumperEditor);
    this->jumperEditor->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    this->jumperEditor->hide();

    if(this->hillEditor == nullptr)
        this->hillEditor = new HillEditorWidget();
    ui->tab_hills->layout()->addWidget(this->hillEditor);
    this->hillEditor->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    this->hillEditor->hide();

    if(this->competitionRulesEditor == nullptr)
        this->competitionRulesEditor = new CompetitionRulesEditorWidget();
    ui->tab_competitionRules->layout()->addWidget(this->competitionRulesEditor);
    this->competitionRulesEditor->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    this->competitionRulesEditor->hide();

    jumpersListView = new DatabaseItemsListView(DatabaseItemsListView::JumperItems, true, this);
    jumpersListView->setJumpers(&GlobalDatabase::get()->getEditableGlobalJumpers());
    jumpersListView->setupListModel();
    ui->verticalLayout_jumpersList->addWidget(jumpersListView);

    hillsListView = new DatabaseItemsListView(DatabaseItemsListView::HillItems, true, this);
    hillsListView->setHills(&GlobalDatabase::get()->getEditableGlobalHills());
    hillsListView->setupListModel();
    ui->verticalLayout_hillsList->addWidget(hillsListView);

    competitionRulesListView = new DatabaseItemsListView(DatabaseItemsListView::CompetitionRulesItems, true, this);
    competitionRulesListView->setCompetitionRules(&GlobalDatabase::get()->getEditableCompetitionRules());
    competitionRulesListView->setupListModel();
    ui->verticalLayout_competitionRulesList->addWidget(competitionRulesListView);

    connect(ui->tabWidget_main, &QTabWidget::currentChanged, this, [this](){
        setActualElementType(ui->tabWidget_main->currentIndex());
    });
    connect(this->jumperEditor, &JumperEditorWidget::submitted, this, &DatabaseEditorWindow::replaceJumperByJumperEditor);
    connect(this->hillEditor, &HillEditorWidget::submitted, this, &DatabaseEditorWindow::replaceHillByHillEditor);
    connect(this->competitionRulesEditor, &CompetitionRulesEditorWidget::submitted, this, &DatabaseEditorWindow::replaceCompetitionRuleByCompetitionRulesEditor);

    connect(this->jumpersListView, &DatabaseItemsListView::listViewDoubleClicked, this, &DatabaseEditorWindow::onJumpersListViewDoubleClicked);
    connect(this->hillsListView, &DatabaseItemsListView::listViewDoubleClicked, this, &DatabaseEditorWindow::onHillsListViewDoubleClicked);
    connect(this->competitionRulesListView, &DatabaseItemsListView::listViewDoubleClicked, this, &DatabaseEditorWindow::onCompetitionRulesListViewDoubleClicked);

    connect(this->jumpersListView, &DatabaseItemsListView::up, this, [this](){
        actualElementIndex--;
    });
    connect(this->hillsListView, &DatabaseItemsListView::up, this, [this](){
        actualElementIndex--;
    });
    connect(this->competitionRulesListView, &DatabaseItemsListView::up, this, [this](){
        actualElementIndex--;
    });
    connect(this->jumpersListView, &DatabaseItemsListView::down, this, [this](){
        actualElementIndex++;
    });
    connect(this->hillsListView, &DatabaseItemsListView::down, this, [this](){
        actualElementIndex++;
    });
    connect(this->competitionRulesListView, &DatabaseItemsListView::down, this, [this](){
        actualElementIndex++;
    });
}

DatabaseEditorWindow::~DatabaseEditorWindow()
{
    delete ui;
}

void DatabaseEditorWindow::replaceJumperByJumperEditor()
{
    GlobalDatabase::get()->getEditableGlobalJumpers().replace(actualElementIndex, jumperEditor->getJumperFromWidgetInput());
    JumpersListModel * jumpersListModel = dynamic_cast<JumpersListModel *>(jumpersListView->getListModel());
    emit jumpersListModel->dataChanged(jumpersListModel->index(actualElementIndex), jumpersListModel->index(actualElementIndex));
}

void DatabaseEditorWindow::replaceHillByHillEditor()
{
    GlobalDatabase::get()->getEditableGlobalHills().replace(actualElementIndex, hillEditor->getHillFromWidgetInput());
    HillsListModel * hillsListModel = dynamic_cast<HillsListModel *>(hillsListView->getListModel());
    emit hillsListModel->dataChanged(hillsListModel->index(actualElementIndex), hillsListModel->index(actualElementIndex));
}

void DatabaseEditorWindow::replaceCompetitionRuleByCompetitionRulesEditor()
{
    GlobalDatabase::get()->getEditableCompetitionRules().replace(actualElementIndex, competitionRulesEditor->getCompetitionRulesFromWidgetInputs());
    CompetitionRulesListModel * competitionRulesListModel = dynamic_cast<CompetitionRulesListModel *>(competitionRulesListView->getListModel());
    emit competitionRulesListModel->dataChanged(competitionRulesListModel->index(actualElementIndex), competitionRulesListModel->index(actualElementIndex));
}

void DatabaseEditorWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox message;
    message.setStyleSheet("background-color: white; color: black;");
    QMessageBox::StandardButton button;
    button = message.question(this, "Wyjście z edytora bazy danych", "Czy zapisać zmiany w bazie danych?", QMessageBox::StandardButton::No | QMessageBox::StandardButton::Cancel | QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::Yes);
    if(button == QMessageBox::Yes)
    {
        GlobalDatabase::get()->writeToJson();
        event->accept();
    }
    else if(button == QMessageBox::Cancel)
    {
        event->ignore();
    }
    else if(button == QMessageBox::No)
    {
        GlobalDatabase::get()->setGlobalJumpers(tempGlobalJumpers);
        GlobalDatabase::get()->setGlobalHills(tempGlobalHills);
        GlobalDatabase::get()->setGlobalCompetitionsRules(tempGlobalCompetitionRules);
        event->accept();
    }
    else
        qDebug()<<"invalid button in database editor close event";
}

void DatabaseEditorWindow::onJumpersListViewDoubleClicked(const QModelIndex &index)
{
    int row = index.row();
    actualElementIndex = row;
    if(jumperEditor->isHidden()){
        ui->tabWidget_main->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        jumperEditor->show();
    }
    jumperEditor->setJumper(const_cast<Jumper *>(&GlobalDatabase::get()->getEditableGlobalJumpers().at(row)));
    jumperEditor->fillJumperInputs();
}

void DatabaseEditorWindow::onHillsListViewDoubleClicked(const QModelIndex &index)
{
    int row = index.row();
    actualElementIndex = row;
    if(hillEditor->isHidden()){
        ui->tabWidget_main->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        hillEditor->show();
    }
    hillEditor->setHill(const_cast<Hill *>(&GlobalDatabase::get()->getEditableGlobalHills().at(row)));
    hillEditor->fillHillInputs();
}

void DatabaseEditorWindow::onCompetitionRulesListViewDoubleClicked(const QModelIndex &index)
{
    int row = index.row();
    actualElementIndex = row;
    if(competitionRulesEditor->isHidden()){
        ui->tabWidget_main->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        competitionRulesEditor->show();
    }
    competitionRulesEditor->setCompetitionRules(const_cast<CompetitionRules *>(&GlobalDatabase::get()->getEditableCompetitionRules().at(row)));
    competitionRulesEditor->fillCompetitionRulesInputs();
}

short DatabaseEditorWindow::getActualElementType() const
{
    return actualElementType;
}

void DatabaseEditorWindow::setActualElementType(short newActualElementType)
{
    actualElementType = newActualElementType;
}

QVector<CompetitionRules> DatabaseEditorWindow::getTempGlobalCompetitionRules() const
{
    return tempGlobalCompetitionRules;
}

void DatabaseEditorWindow::setTempGlobalCompetitionRules(const QVector<CompetitionRules> &newTempGlobalCompetitionRules)
{
    tempGlobalCompetitionRules = newTempGlobalCompetitionRules;
}

QVector<Hill> DatabaseEditorWindow::getTempGlobalHills() const
{
    return tempGlobalHills;
}

void DatabaseEditorWindow::setTempGlobalHills(const QVector<Hill> &newTempGlobalHills)
{
    tempGlobalHills = newTempGlobalHills;
}

QVector<Jumper> DatabaseEditorWindow::getTempGlobalJumpers() const
{
    return tempGlobalJumpers;
}

void DatabaseEditorWindow::setTempGlobalJumpers(const QVector<Jumper> &newTempGlobalJumpers)
{
    tempGlobalJumpers = newTempGlobalJumpers;
}

HillsListModel *DatabaseEditorWindow::getHillsListModel() const
{
    return hillsListModel;
}

void DatabaseEditorWindow::setHillsListModel(HillsListModel *newHillsListModel)
{
    hillsListModel = newHillsListModel;
}

int DatabaseEditorWindow::getActualElementIndex() const
{
    return actualElementIndex;
}

void DatabaseEditorWindow::setActualElementIndex(int newActualElementIndex)
{
    actualElementIndex = newActualElementIndex;
}

JumpersListModel *DatabaseEditorWindow::getJumpersListModel() const
{
    return jumpersListModel;
}

void DatabaseEditorWindow::setJumpersListModel(JumpersListModel *newJumpersListModel)
{
    jumpersListModel = newJumpersListModel;
}

/*void DatabaseEditorWindow::on_pushButton_add_clicked()
{
    switch(actualElementType){
    case JumperElement:{
        if(ui->listView_jumpers->selectionModel()->selectedRows().count() > 0){
            int row = ui->listView_jumpers->selectionModel()->selectedRows().first().row() + 1;
            GlobalDatabase::get()->getEditableGlobalJumpers().insert(row, Jumper("Name", "Surname", "XXX"));
            jumpersListModel->insertRows(row, 1);
            emit jumpersListModel->dataChanged(jumpersListModel->index(row), jumpersListModel->index(row));
        }
        break;
    }
    case HillElement:{
        if(ui->listView_hills->selectionModel()->selectedRows().count() > 0){
            int row = ui->listView_hills->selectionModel()->selectedRows().first().row() + 1;
            GlobalDatabase::get()->getEditableGlobalHills().insert(row, Hill("Hill", "XXX"));
            hillsListModel->insertRows(row, 1);
            emit hillsListModel->dataChanged(hillsListModel->index(row), hillsListModel->index(row));
        }
        break;
    }
    case CompetitionRulesElement:{
        if(ui->listView_competitionRules->selectionModel()->selectedRows().count() > 0){
            int row = ui->listView_competitionRules->selectionModel()->selectedRows().first().row() + 1;
            GlobalDatabase::get()->getEditableCompetitionRules().insert(row, CompetitionRules("Rules"));
            competitionRulesListModel->insertRows(row, 1);
            emit competitionRulesListModel->dataChanged(competitionRulesListModel->index(row), competitionRulesListModel->index(row));
        }
        break;
    }
    }
}


void DatabaseEditorWindow::on_pushButton_remove_clicked()
{
    switch(actualElementType){
    case JumperElement:{
        if(ui->listView_jumpers->selectionModel()->selectedRows().count() > 0){
            int row = ui->listView_jumpers->selectionModel()->selectedRows().first().row();
            GlobalDatabase::get()->getEditableGlobalJumpers().remove(row);
            jumpersListModel->removeRows(row, 1);
            emit jumpersListModel->dataChanged(jumpersListModel->index(row), jumpersListModel->index(jumpersListModel->rowCount()));
        }
        break;
    }
    case HillElement:{
        if(ui->listView_hills->selectionModel()->selectedRows().count() > 0){
            int row = ui->listView_hills->selectionModel()->selectedRows().first().row();
            GlobalDatabase::get()->getEditableGlobalHills().remove(row);
            hillsListModel->removeRows(row, 1);
            emit hillsListModel->dataChanged(hillsListModel->index(row), hillsListModel->index(hillsListModel->rowCount()));
        }
        break;
    }
    case CompetitionRulesElement:{
        if(ui->listView_competitionRules->selectionModel()->selectedRows().count() > 0){
            int row = ui->listView_competitionRules->selectionModel()->selectedRows().first().row() + 1;
            GlobalDatabase::get()->getEditableCompetitionRules().remove(row);
            competitionRulesListModel->removeRows(row, 1);
            emit competitionRulesListModel->dataChanged(competitionRulesListModel->index(row), competitionRulesListModel->index(competitionRulesListModel->rowCount()));
        }
        break;
    }
    }
}


void DatabaseEditorWindow::on_pushButton_up_clicked()
{
    switch(actualElementType){
    case JumperElement:{
        if(ui->listView_jumpers->selectionModel()->selectedRows().count() > 0){
            int row = ui->listView_jumpers->selectionModel()->selectedRows().first().row();
            if(row > 0){
                GlobalDatabase::get()->getEditableGlobalJumpers().swapItemsAt(row, row - 1);
                emit jumpersListModel->dataChanged(jumpersListModel->index(row), jumpersListModel->index(row - 1));
                ui->listView_jumpers->selectionModel()->clear();
                ui->listView_jumpers->selectionModel()->select(jumpersListModel->index(row - 1), QItemSelectionModel::Select);
                ui->listView_jumpers->scrollTo(jumpersListModel->index(row - 1));
                actualElementIndex--;
            }
        }
        break;
    }
    case HillElement:{
        if(ui->listView_hills->selectionModel()->selectedRows().count() > 0){
            int row = ui->listView_hills->selectionModel()->selectedRows().first().row();
            if(row > 0){
                GlobalDatabase::get()->getEditableGlobalHills().swapItemsAt(row, row - 1);
                emit hillsListModel->dataChanged(hillsListModel->index(row), hillsListModel->index(row - 1));
                ui->listView_hills->selectionModel()->clear();
                ui->listView_hills->selectionModel()->select(hillsListModel->index(row - 1), QItemSelectionModel::Select);
                ui->listView_hills->scrollTo(hillsListModel->index(row - 1));
                actualElementIndex--;
            }
        }
        break;
    }
    case CompetitionRulesElement:{
        if(ui->listView_competitionRules->selectionModel()->selectedRows().count() > 0){
            int row = ui->listView_competitionRules->selectionModel()->selectedRows().first().row();
            if(row > 0){
                GlobalDatabase::get()->getEditableCompetitionRules().swapItemsAt(row, row - 1);
                emit competitionRulesListModel->dataChanged(competitionRulesListModel->index(row), competitionRulesListModel->index(row - 1));
                ui->listView_competitionRules->selectionModel()->clear();
                ui->listView_competitionRules->selectionModel()->select(competitionRulesListModel->index(row - 1), QItemSelectionModel::Select);
                ui->listView_competitionRules->scrollTo(competitionRulesListModel->index(row - 1));
                actualElementIndex--;
            }
        }
        break;
    }
    }
}


void DatabaseEditorWindow::on_pushButton_down_clicked()
{
    switch(actualElementType){
    case JumperElement:{
        if(ui->listView_jumpers->selectionModel()->selectedRows().count() > 0){
            int row = ui->listView_jumpers->selectionModel()->selectedRows().first().row();
            if(row < jumpersListModel->rowCount() - 1){
                GlobalDatabase::get()->getEditableGlobalJumpers().swapItemsAt(row, row + 1);
                emit jumpersListModel->dataChanged(jumpersListModel->index(row), jumpersListModel->index(row + 1));
                ui->listView_jumpers->selectionModel()->clear();
                ui->listView_jumpers->selectionModel()->select(jumpersListModel->index(row + 1), QItemSelectionModel::Select);
                ui->listView_jumpers->scrollTo(jumpersListModel->index(row + 1));
                actualElementIndex++;
            }
        }
        break;
    }
    case HillElement:{
        if(ui->listView_hills->selectionModel()->selectedRows().count() > 0){
            int row = ui->listView_hills->selectionModel()->selectedRows().first().row();
            if(row < hillsListModel->rowCount() - 1){
                GlobalDatabase::get()->getEditableGlobalHills().swapItemsAt(row, row + 1);
                emit hillsListModel->dataChanged(hillsListModel->index(row), hillsListModel->index(row + 1));
                ui->listView_hills->selectionModel()->clear();
                ui->listView_hills->selectionModel()->select(hillsListModel->index(row + 1), QItemSelectionModel::Select);
                ui->listView_hills->scrollTo(hillsListModel->index(row + 1));
                actualElementIndex++;
            }
        }
        break;
    }
    case CompetitionRulesElement:{
        if(ui->listView_competitionRules->selectionModel()->selectedRows().count() > 0){
            int row = ui->listView_competitionRules->selectionModel()->selectedRows().first().row();
            if(row < competitionRulesListModel->rowCount() - 1){
                GlobalDatabase::get()->getEditableCompetitionRules().swapItemsAt(row, row + 1);
                emit competitionRulesListModel->dataChanged(competitionRulesListModel->index(row), competitionRulesListModel->index(row + 1));
                ui->listView_competitionRules->selectionModel()->clear();
                ui->listView_competitionRules->selectionModel()->select(competitionRulesListModel->index(row + 1), QItemSelectionModel::Select);
                ui->listView_competitionRules->scrollTo(competitionRulesListModel->index(row + 1));
                actualElementIndex++;
            }
        }
        break;
    }
    }
}*/
