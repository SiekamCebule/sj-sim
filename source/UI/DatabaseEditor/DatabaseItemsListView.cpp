#include "DatabaseItemsListView.h"
#include "ui_DatabaseItemsListView.h"

DatabaseItemsListView::DatabaseItemsListView(int type, bool allowInserting, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseItemsListView),
    type(type),
    allowInserting(allowInserting)
{
    ui->setupUi(this);
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    jumpers = nullptr;
    hills = nullptr;
    competitionRules = nullptr;
    listModel = nullptr;

    insertAction = new QAction(this);
    insertAction->setShortcut(Qt::CTRL | Qt::Key_A);
    this->addAction(insertAction);
    connect(insertAction, &QAction::triggered, this, &DatabaseItemsListView::onInsertActionTriggered);

    removeAction = new QAction(this);
    removeAction->setShortcut(Qt::CTRL | Qt::Key_D);
    this->addAction(removeAction);
    connect(removeAction, &QAction::triggered, this, &DatabaseItemsListView::onRemoveActionTriggered);

    upAction = new QAction(this);
    upAction->setShortcut(Qt::CTRL | Qt::Key_Up);
    this->addAction(upAction);
    connect(upAction, &QAction::triggered, this, &DatabaseItemsListView::onUpActionTriggered);

    downAction = new QAction(this);
    downAction->setShortcut(Qt::CTRL | Qt::Key_Down);
    this->addAction(downAction);
    connect(downAction, &QAction::triggered, this, &DatabaseItemsListView::onDownActionTriggered);

    connect(ui->listView, &QListView::doubleClicked, this, &DatabaseItemsListView::listViewDoubleClicked);
}

DatabaseItemsListView::~DatabaseItemsListView()
{
    delete ui;
}

void DatabaseItemsListView::setupListModel()
{
    switch(type){
    case JumperItems:
        listModel = new JumpersListModel(this->jumpers);
        break;
    case HillItems:
        listModel = new HillsListModel(this->hills);
        break;
    case CompetitionRulesItems:
        listModel = new CompetitionRulesListModel(this->competitionRules);
        break;
    }
    ui->listView->setModel(listModel);
}

QAbstractListModel *DatabaseItemsListView::getListModel()
{
    return listModel;
}

void DatabaseItemsListView::onInsertActionTriggered()
{
    if(allowInserting == true){

        QVector<QModelIndex> rows = ui->listView->selectionModel()->selectedRows();
        if(rows.size() == 1){
            switch(type){
            case JumperItems:{
                JumpersListModel * jumpersListModel = dynamic_cast<JumpersListModel *>(listModel);
                int rowToInsert = ui->listView->selectionModel()->selectedRows().first().row();
                jumpersListModel->insertRows(rowToInsert, 1);
                jumpersListModel->getJumpersVectorPointer()->insert(rowToInsert + 1, 1, Jumper("Name", "Surname", "XXX"));
                emit jumpersListModel->dataChanged(jumpersListModel->index(rowToInsert), jumpersListModel->index(jumpersListModel->rowCount() - 1));
                break;
            }
            case HillItems:{
                HillsListModel * hillsListModel = dynamic_cast<HillsListModel *>(listModel);
                int rowToInsert = ui->listView->selectionModel()->selectedRows().first().row();
                hillsListModel->insertRows(rowToInsert, 1);
                hillsListModel->getHillsVectorPointer()->insert(rowToInsert + 1, 1, Hill("Hill", "XXX"));
                emit hillsListModel->dataChanged(hillsListModel->index(rowToInsert), hillsListModel->index(hillsListModel->rowCount() - 1));
                break;
            }
            case CompetitionRulesItems:{
                CompetitionRulesListModel * competitionRulesListModel = dynamic_cast<CompetitionRulesListModel *>(listModel);
                int rowToInsert = ui->listView->selectionModel()->selectedRows().first().row();
                competitionRulesListModel->insertRows(rowToInsert, 1);
                competitionRulesListModel->getCompetitonRulesVectorPointer()->insert(rowToInsert + 1, 1, CompetitionRules("Rules"));
                emit competitionRulesListModel->dataChanged(competitionRulesListModel->index(rowToInsert), competitionRulesListModel->index(competitionRulesListModel->rowCount() - 1));
                break;
            }
            default: break;
            }
        }

    }
}

void DatabaseItemsListView::onRemoveActionTriggered()
{
    QVector<QModelIndex> rows = ui->listView->selectionModel()->selectedRows();
    if(rows.size() > 0){
        int firstRow = rows.first().row();
        QItemSelectionModel * selectionModel = ui->listView->selectionModel();
        switch(type){
        case JumperItems:{
            JumpersListModel * jumpersListModel = dynamic_cast<JumpersListModel *>(listModel);
            while(ui->listView->selectionModel()->selectedRows().size() > 0){
                int rowToRemove = ui->listView->selectionModel()->selectedRows().first().row();
                jumpersListModel->removeRows(rowToRemove, 1);
                jumpersListModel->getJumpersVectorPointer()->remove(rowToRemove, 1);
                selectionModel->select(jumpersListModel->index(rowToRemove), QItemSelectionModel::Deselect);
            }
            emit jumpersListModel->dataChanged(jumpersListModel->index(firstRow), jumpersListModel->index(jumpersListModel->rowCount() - 1));
            break;
        }
        case HillItems:{
            HillsListModel * hillsListModel = dynamic_cast<HillsListModel *>(listModel);
            while(ui->listView->selectionModel()->selectedRows().size() > 0){
                int rowToRemove = ui->listView->selectionModel()->selectedRows().first().row();
                hillsListModel->removeRows(rowToRemove, 1);
                hillsListModel->getHillsVectorPointer()->remove(rowToRemove, 1);
                selectionModel->select(hillsListModel->index(rowToRemove), QItemSelectionModel::Deselect);
            }
            emit hillsListModel->dataChanged(hillsListModel->index(firstRow), hillsListModel->index(hillsListModel->rowCount() - 1));
            break;
        }
        case CompetitionRulesItems:{
            CompetitionRulesListModel * competitionRulesListModel = dynamic_cast<CompetitionRulesListModel *>(listModel);
            while(ui->listView->selectionModel()->selectedRows().size() > 0){
                int rowToRemove = ui->listView->selectionModel()->selectedRows().first().row();
                competitionRulesListModel->removeRows(rowToRemove, 1);
                competitionRulesListModel->getCompetitonRulesVectorPointer()->remove(rowToRemove, 1);
                selectionModel->select(competitionRulesListModel->index(rowToRemove), QItemSelectionModel::Deselect);
            }
            emit competitionRulesListModel->dataChanged(competitionRulesListModel->index(firstRow), competitionRulesListModel->index(competitionRulesListModel->rowCount() - 1));
            break;
        }
        default: break;
        }
    }
}

void DatabaseItemsListView::onUpActionTriggered()
{
    QVector<QModelIndex> rows = ui->listView->selectionModel()->selectedRows();
    if(rows.size() > 0){
        int firstRow = rows.first().row();
        if(firstRow == 0){
            QItemSelectionModel * selectionModel = ui->listView->selectionModel();
            QVector<QModelIndex> rows = selectionModel->selectedRows();
            rows.remove(0);
            selectionModel->clearSelection();
            for(auto & index : rows){
                selectionModel->select(index, QItemSelectionModel::Select);
            }
            ui->listView->setSelectionModel(selectionModel);
            if(ui->listView->selectionModel()->selectedRows().size() > 0)
                firstRow = ui->listView->selectionModel()->selectedRows().first().row();
            else return;
        }
        rows = ui->listView->selectionModel()->selectedRows();
        ui->listView->clearSelection();
        switch(type){
        case JumperItems:{
            JumpersListModel * jumpersListModel = dynamic_cast<JumpersListModel *>(listModel);
            for(auto & index : rows)
                ui->listView->setCurrentIndex(jumpersListModel->index(index.row() - 1));
            std::sort(rows.begin(), rows.end());
            for(auto & index : rows)
                jumpersListModel->getJumpersVectorPointer()->swapItemsAt(index.row(), index.row() - 1);
            emit jumpersListModel->dataChanged(jumpersListModel->index(firstRow), jumpersListModel->index(jumpersListModel->rowCount() - 1));
            break;
        }
        case HillItems:{
            HillsListModel * hillsListModel = dynamic_cast<HillsListModel *>(listModel);
            for(auto & index : rows)
                ui->listView->setCurrentIndex(hillsListModel->index(index.row() - 1));
            std::sort(rows.begin(), rows.end());
            for(auto & index : rows)
                hillsListModel->getHillsVectorPointer()->swapItemsAt(index.row(), index.row() - 1);
            emit hillsListModel->dataChanged(hillsListModel->index(firstRow), hillsListModel->index(hillsListModel->rowCount() - 1));
            break;
        }
        case CompetitionRulesItems:{
            CompetitionRulesListModel * competitionRulesListModel = dynamic_cast<CompetitionRulesListModel *>(listModel);
            for(auto & index : rows)
                ui->listView->setCurrentIndex(competitionRulesListModel->index(index.row() - 1));
            std::sort(rows.begin(), rows.end());
            for(auto & index : rows)
                competitionRulesListModel->getCompetitonRulesVectorPointer()->swapItemsAt(index.row(), index.row() - 1);
            emit competitionRulesListModel->dataChanged(competitionRulesListModel->index(firstRow), competitionRulesListModel->index(competitionRulesListModel->rowCount() - 1));
            break;
        }
        default: break;
        }
    }
}

void DatabaseItemsListView::onDownActionTriggered()
{
    int rowCount = listModel->rowCount();

    QVector<QModelIndex> rows = ui->listView->selectionModel()->selectedRows();
    if(rows.size() > 0){
        int lastRow = rows.last().row();
        if(lastRow == rowCount - 1){
            QItemSelectionModel * selectionModel = ui->listView->selectionModel();
            QVector<QModelIndex> rows = selectionModel->selectedRows();
            rows.removeLast();
            selectionModel->clearSelection();
            for(auto & index : rows){
                selectionModel->select(index, QItemSelectionModel::Select);
            }
            ui->listView->setSelectionModel(selectionModel);
            if(ui->listView->selectionModel()->selectedRows().size() > 0)
                lastRow = ui->listView->selectionModel()->selectedRows().last().row();
            else return;
        }
        rows = ui->listView->selectionModel()->selectedRows();
        ui->listView->clearSelection();
        switch(type){
        case JumperItems:{
            JumpersListModel * jumpersListModel = dynamic_cast<JumpersListModel *>(listModel);
            for(auto & index : rows)
                ui->listView->setCurrentIndex(jumpersListModel->index(index.row() + 1));
            std::sort(rows.begin(), rows.end());
            for(auto & index : rows)
                jumpersListModel->getJumpersVectorPointer()->swapItemsAt(index.row(), index.row() + 1);
            emit jumpersListModel->dataChanged(jumpersListModel->index(lastRow), jumpersListModel->index(0));
            break;
        }
        case HillItems:{
            HillsListModel * hillsListModel = dynamic_cast<HillsListModel *>(listModel);
            for(auto & index : rows)
                ui->listView->setCurrentIndex(hillsListModel->index(index.row() + 1));
            std::sort(rows.begin(), rows.end());
            for(auto & index : rows)
                hillsListModel->getHillsVectorPointer()->swapItemsAt(index.row(), index.row() + 1);
            emit hillsListModel->dataChanged(hillsListModel->index(lastRow), hillsListModel->index(0));
            break;
        }
        case CompetitionRulesItems:{
            CompetitionRulesListModel * competitionRulesListModel = dynamic_cast<CompetitionRulesListModel *>(listModel);
            for(auto & index : rows)
                ui->listView->setCurrentIndex(competitionRulesListModel->index(index.row() + 1));
            std::sort(rows.begin(), rows.end());
            for(auto & index : rows)
                competitionRulesListModel->getCompetitonRulesVectorPointer()->swapItemsAt(index.row(), index.row() + 1);
            emit competitionRulesListModel->dataChanged(competitionRulesListModel->index(lastRow), competitionRulesListModel->index(0));
            break;
        }
        default: break;
        }
    }
}

QVector<CompetitionRules> *DatabaseItemsListView::getCompetitionRules() const
{
    return competitionRules;
}

void DatabaseItemsListView::setCompetitionRules(QVector<CompetitionRules> *newCompetitionRules)
{
    competitionRules = newCompetitionRules;
}

QListView *DatabaseItemsListView::getListView()
{
    return ui->listView;
}

QVector<Hill> *DatabaseItemsListView::getHills() const
{
    return hills;
}

void DatabaseItemsListView::setHills(QVector<Hill> *newHills)
{
    hills = newHills;
}

QVector<Jumper> *DatabaseItemsListView::getJumpers() const
{
    return jumpers;
}

void DatabaseItemsListView::setJumpers(QVector<Jumper> *newJumpers)
{
    jumpers = newJumpers;
}

bool DatabaseItemsListView::getAllowInserting() const
{
    return allowInserting;
}

void DatabaseItemsListView::setAllowInserting(bool newAllowInserting)
{
    allowInserting = newAllowInserting;
}
