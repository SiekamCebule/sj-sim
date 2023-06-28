#include "CalendarEditorWidget.h"
#include "ui_CalendarEditorWidget.h"
#include "../../../global/CountryFlagsManager.h"
#include "../../../utilities/functions.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>

CalendarEditorWidget::CalendarEditorWidget(CalendarEditorTableModel *model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarEditorWidget),
    calendarModel(model),
    calendar(model->getCalendar()),
    actualCompetitionIndex(-1)
{
    ui->setupUi(this);
    ui->tableView->setModel(this->calendarModel);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();

    action_add = new QAction(this);
    action_add->setShortcut(Qt::CTRL | Qt::Key_A);
    this->addAction(action_add);
    connect(action_add, &QAction::triggered, this, &CalendarEditorWidget::addActionTriggered);

    action_remove = new QAction(this);
    action_remove->setShortcut(Qt::CTRL | Qt::Key_D);
    this->addAction(action_remove);
    connect(action_remove, &QAction::triggered, this, &CalendarEditorWidget::removeActionTriggered);

    action_up = new QAction(this);
    action_up->setShortcut(Qt::CTRL | Qt::Key_Up);
    this->addAction(action_up);
    connect(action_up, &QAction::triggered, this, &CalendarEditorWidget::upActionTriggered);

    action_down = new QAction(this);
    action_down->setShortcut(Qt::CTRL | Qt::Key_Down);
    this->addAction(action_down);
    connect(action_down, &QAction::triggered, this, &CalendarEditorWidget::downActionTriggered);

    defaultHill.setName("Hill");

    competitionInfoEditor = new CompetitionInfoEditorWidget();
    competitionInfoEditor->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    competitionInfoEditor->setParent(this);
    competitionInfoEditor->setHillsList(model->getHillsList());
    competitionInfoEditor->setupHillsComboBox();
    competitionInfoEditor->setRulesList(model->getRulesList());
    competitionInfoEditor->setupRulesComboBox();
    competitionInfoEditor->setDefaultHill(&defaultHill);
    ui->verticalLayout_competitionInfo->addWidget(competitionInfoEditor);
    competitionInfoEditor->hide();
    connect(competitionInfoEditor, &CompetitionInfoEditorWidget::submitted, this, [this](){
        if(actualCompetitionIndex > (-1)){
            qDebug()<<"Przed usuwaniem:";
            debugCalendar();
            updateActualCompetitionByID();
            calendar->getCompetitionsReference().reserve(calendar->getCompetitionsReference().size() + competitionInfoEditor->getTrainingsCount() + 3);
            if(actualCompetition->getTrialRound() != nullptr){
                calendar->getCompetitionsReference().remove(actualCompetitionIndex - 1);
                delete actualCompetition->getTrialRound();
            }
            for(auto & training : actualCompetition->getTrainingsReference()){
                MyFunctions::removeFromVector(calendar->getCompetitionsReference(), training);
                delete training;
            }
            updateActualCompetitionByID();

            qDebug()<<"Przed dodawaniem:";
            debugCalendar();
            CompetitionInfo * competition = new CompetitionInfo(competitionInfoEditor->getCompetitionInfoFromInputs());
            if(competitionInfoEditor->getTrialRoundChecked()){
                CompetitionInfo * trialRound = new CompetitionInfo(competition->getHill());
                trialRound->setRules(competitionInfoEditor->getTrialRoundRules());
                trialRound->setSerieType(CompetitionInfo::TrialRound);
                calendar->getCompetitionsReference().insert(actualCompetitionIndex, trialRound);
                updateActualCompetitionByID();
                competition->setTrialRound(calendar->getCompetitionsReference()[actualCompetitionIndex - 1]);
            }
            for(int i=0; i<competitionInfoEditor->getTrainingsCount(); i++){
                CompetitionInfo * training = new CompetitionInfo(competition->getHill());
                training->setRules(competitionInfoEditor->getTrainingsRules());
                training->setSerieType(CompetitionInfo::Training);
                if(competitionInfoEditor->getTrialRoundChecked()){
                    calendar->getCompetitionsReference().insert(actualCompetitionIndex-1, training);
                    updateActualCompetitionByID();
                    competition->getTrainingsReference().push_back(calendar->getCompetitionsReference()[actualCompetitionIndex - 2]);
                }
                else{
                    calendar->getCompetitionsReference().insert(actualCompetitionIndex, training);
                    updateActualCompetitionByID();
                    competition->getTrainingsReference().push_back(calendar->getCompetitionsReference()[actualCompetitionIndex - 1]);
                }
            }
            qDebug()<<"Po dodawaniu:";
            debugCalendar();
            delete calendar->getCompetitionsReference()[actualCompetitionIndex];
            calendar->getCompetitionsReference()[actualCompetitionIndex] = competition;
            actualCompetitionID = competition->getID();
            emit calendarModel->dataChanged(calendarModel->index(0, 0), calendarModel->index(calendarModel->rowCount() - 1, calendarModel->columnCount() - 1));
            ui->tableView->resizeColumnsToContents();
        }
    });
}

CalendarEditorWidget::~CalendarEditorWidget()
{
    delete ui;
}

void CalendarEditorWidget::updateActualCompetitionByID()
{
    if(actualCompetitionID > 0){
        int i=0;
        for(auto & comp : calendar->getCompetitionsReference()){
            if(comp->getID() == actualCompetitionID){
                actualCompetitionIndex = i;
                actualCompetition = comp;
                break;
            }
            i++;
        }
    }
    else{
        actualCompetition = nullptr;
        actualCompetitionIndex = (-1);
    }
}

void CalendarEditorWidget::debugCalendar()
{
    int i=1;
    for(auto & comp : calendar->getCompetitionsReference()){
        QString string = QString::number(i);
        string += ". " + comp->getHill()->getName() + " HS" + QString::number(comp->getHill()->getHSPoint()) + " -> ";
        switch(comp->getSerieType()){
        case CompetitionInfo::Competition:
            string += "KONKURS"; break;
        case CompetitionInfo::Qualifications:
            string += "KWALIFIKACJE"; break;
        case CompetitionInfo::TrialRound:
            string += "Seria próbna"; break;
        case CompetitionInfo::Training:
            string += "Trening"; break;
        default: break;
        }

        qDebug()<<string<<" ("<<comp<<") (ID: "<<comp->getID()<<")";
        i++;
    }
}

CalendarEditorTableModel *CalendarEditorWidget::getModel() const
{
    return calendarModel;
}

void CalendarEditorWidget::setModel(CalendarEditorTableModel *newModel)
{
    calendarModel = newModel;
}

QTableView *CalendarEditorWidget::getTableView()
{
    return ui->tableView;
}

void CalendarEditorWidget::addActionTriggered()
{
    calendar->getCompetitionsReference().push_back(new CompetitionInfo(&defaultHill));

    int insertIndex = 0;
    if(ui->tableView->selectionModel()->selectedIndexes().count() > 0)
        insertIndex = ui->tableView->selectionModel()->selectedIndexes().at(0).row();
    else if(calendarModel->rowCount() > 0)
        insertIndex = calendarModel->rowCount() - 1;
    else
        insertIndex = 0;
    calendarModel->insertRow(insertIndex);

    emit calendarModel->dataChanged(calendarModel->index(0, 0), calendarModel->index(calendarModel->rowCount() - 1, calendarModel->columnCount() - 1));
    ui->tableView->resizeColumnsToContents();
    updateActualCompetitionByID();
}

void CalendarEditorWidget::removeActionTriggered()
{
    if(ui->tableView->selectionModel()->selectedIndexes().count() > 0){
        int removeIndex = ui->tableView->selectionModel()->selectedIndexes().at(0).row();

        int i=0;
        CompetitionInfo * competitionToRemove = nullptr;
        for(auto & comp : calendar->getCompetitionsReference()){ //znajdujemy odpowiedni konkurs z listy (Bo nie pokazujemy treningów i serii próbnych)
            if(comp->getSerieType() == CompetitionInfo::Qualifications || comp->getSerieType() == CompetitionInfo::Competition){
                if(i == removeIndex){
                    competitionToRemove = comp;
                    break;
                }
                i++;
            }
        }
        if(competitionToRemove != nullptr){
            if(competitionToRemove->getTrialRound() != nullptr){
                MyFunctions::removeFromVector(calendar->getCompetitionsReference(), competitionToRemove->getTrialRound());
                delete competitionToRemove->getTrialRound();
            }
            for(auto & training : competitionToRemove->getTrainingsReference()){
                MyFunctions::removeFromVector(calendar->getCompetitionsReference(), training);
                delete training;
            }
            MyFunctions::removeFromVector(calendar->getCompetitionsReference(), competitionToRemove);
            delete competitionToRemove;
            calendarModel->removeRow(removeIndex);
        }
        updateActualCompetitionByID();
    }
}

void CalendarEditorWidget::upActionTriggered()
{
    if(ui->tableView->selectionModel()->selectedIndexes().count() > 0){
        int moveIndex = ui->tableView->selectionModel()->selectedIndexes().at(0).row();
        if(moveIndex == 0) return;
        int i=0;
        int competitionToMoveIndexInCalendar = 0;
        CompetitionInfo * competitionToMove = nullptr;
        for(auto & comp : calendar->getCompetitionsReference()){ //znajdujemy odpowiedni konkurs z listy (Bo nie pokazujemy treningów i serii próbnych)
            if(comp->getSerieType() == CompetitionInfo::Qualifications || comp->getSerieType() == CompetitionInfo::Competition){
                if(i == moveIndex){
                    competitionToMove = comp;
                    break;
                }
                i++;
            }
            competitionToMoveIndexInCalendar++;
        }
        if(competitionToMove != nullptr){
            int previousCompetitionIndexInCalendar = -1;
            CompetitionInfo * previousCompetition = nullptr;
            previousCompetitionIndexInCalendar = competitionToMoveIndexInCalendar - 1;
            if(competitionToMove->getTrialRound() != nullptr)
                previousCompetitionIndexInCalendar -= 1;
            previousCompetitionIndexInCalendar -= competitionToMove->getTrainingsReference().count();

            previousCompetition = calendar->getCompetitionsReference()[previousCompetitionIndexInCalendar];

            QVector<CompetitionInfo *> competitionToMoveCompetitions;
            if(competitionToMove->getTrialRound() != nullptr){
                MyFunctions::removeFromVector(calendar->getCompetitionsReference(), competitionToMove->getTrialRound());
                competitionToMoveCompetitions.push_back(competitionToMove->getTrialRound());
            }
            for(auto & training : competitionToMove->getTrainingsReference()){
                MyFunctions::removeFromVector(calendar->getCompetitionsReference(), training);
                competitionToMoveCompetitions.push_back(training);
            }

            QVector<CompetitionInfo *> previousCompetitionCompetitions;
            if(previousCompetition->getTrialRound() != nullptr){
                MyFunctions::removeFromVector(calendar->getCompetitionsReference(), previousCompetition->getTrialRound());
                previousCompetitionCompetitions.push_back(previousCompetition->getTrialRound());
            }
            for(auto & training : previousCompetition->getTrainingsReference()){
                MyFunctions::removeFromVector(calendar->getCompetitionsReference(), training);
                previousCompetitionCompetitions.push_back(training);
            }

            competitionToMoveIndexInCalendar = 0;
            for(auto & comp : calendar->getCompetitionsReference()){ //znajdujemy odpowiedni konkurs z listy (Bo nie pokazujemy treningów i serii próbnych)
                if(comp->getSerieType() == CompetitionInfo::Qualifications || comp->getSerieType() == CompetitionInfo::Competition){
                    if(competitionToMove == comp){
                        break;
                    }
                }
                competitionToMoveIndexInCalendar++;
            }

            calendar->getCompetitionsReference().swapItemsAt(competitionToMoveIndexInCalendar, competitionToMoveIndexInCalendar - 1);
            competitionToMoveIndexInCalendar -= 1;
            //qSwap(competitionToMoveIndexInCalendar, previousCompetitionIndexInCalendar);

            qDebug()<<"KALENDARZ:";
            debugCalendar();

            for(auto it = competitionToMoveCompetitions.begin(); it != competitionToMoveCompetitions.end(); it++){
                calendar->getCompetitionsReference().insert(competitionToMoveIndexInCalendar, *it);
            }
            qDebug()<<"PO competitionToMoveCompetitions:";
            debugCalendar();
            previousCompetitionIndexInCalendar = 0;
            for(auto & comp : calendar->getCompetitionsReference()){ //znajdujemy odpowiedni konkurs z listy (Bo nie pokazujemy treningów i serii próbnych)
                if(comp->getSerieType() == CompetitionInfo::Qualifications || comp->getSerieType() == CompetitionInfo::Competition){
                    if(previousCompetition == comp){
                        break;
                    }
                }
                previousCompetitionIndexInCalendar++;
            }
            for(auto it = previousCompetitionCompetitions.begin(); it != previousCompetitionCompetitions.end(); it++){
                calendar->getCompetitionsReference().insert(previousCompetitionIndexInCalendar, *it);
            }
            qDebug()<<"PO previousCompetitionIndexInCalendar:";
            debugCalendar();
            ui->tableView->clearSelection();
            ui->tableView->selectionModel()->select(calendarModel->index(moveIndex - 1, 0), QItemSelectionModel::Select);
            emit calendarModel->dataChanged(calendarModel->index(0, 0), calendarModel->index(calendarModel->rowCount() - 1, calendarModel->columnCount() - 1));
        }
        updateActualCompetitionByID();
    }
}

void CalendarEditorWidget::downActionTriggered()
{
    if(ui->tableView->selectionModel()->selectedIndexes().count() > 0){
        int moveIndex = ui->tableView->selectionModel()->selectedIndexes().at(0).row();
        if(moveIndex + 1 >= calendarModel->rowCount()) return;
        int i=0;
        int competitionToMoveIndexInCalendar = 0;
        CompetitionInfo * competitionToMove = nullptr;
        for(auto & comp : calendar->getCompetitionsReference()){ //znajdujemy odpowiedni konkurs z listy (Bo nie pokazujemy treningów i serii próbnych)
            if(comp->getSerieType() == CompetitionInfo::Qualifications || comp->getSerieType() == CompetitionInfo::Competition){
                if(i == moveIndex){
                    competitionToMove = comp;
                    break;
                }
                i++;
            }
            competitionToMoveIndexInCalendar++;
        }
        if(competitionToMove != nullptr){
            int nextCompetitionIndexInCalendar = -1;
            CompetitionInfo * nextCompetition = nullptr;
            for(int i=competitionToMoveIndexInCalendar + 1; i<calendar->getCompetitionsReference().count(); i++){
                if(calendar->getCompetitionsReference()[i]->getSerieType() == CompetitionInfo::Qualifications || calendar->getCompetitionsReference()[i]->getSerieType() == CompetitionInfo::Competition){
                    nextCompetition = calendar->getCompetitionsReference()[i];
                    nextCompetitionIndexInCalendar = i;
                    break;
                }
            }

            nextCompetition = calendar->getCompetitionsReference()[nextCompetitionIndexInCalendar];

            QVector<CompetitionInfo *> competitionToMoveCompetitions;
            if(competitionToMove->getTrialRound() != nullptr){
                MyFunctions::removeFromVector(calendar->getCompetitionsReference(), competitionToMove->getTrialRound());
                competitionToMoveCompetitions.push_back(competitionToMove->getTrialRound());
            }
            for(auto & training : competitionToMove->getTrainingsReference()){
                MyFunctions::removeFromVector(calendar->getCompetitionsReference(), training);
                competitionToMoveCompetitions.push_back(training);
            }

            QVector<CompetitionInfo *> nextCompetitionCompetitions;
            if(nextCompetition->getTrialRound() != nullptr){
                MyFunctions::removeFromVector(calendar->getCompetitionsReference(), nextCompetition->getTrialRound());
                nextCompetitionCompetitions.push_back(nextCompetition->getTrialRound());
            }
            for(auto & training : nextCompetition->getTrainingsReference()){
                MyFunctions::removeFromVector(calendar->getCompetitionsReference(), training);
                nextCompetitionCompetitions.push_back(training);
            }

            competitionToMoveIndexInCalendar = 0;
            for(auto & comp : calendar->getCompetitionsReference()){ //znajdujemy odpowiedni konkurs z listy (Bo nie pokazujemy treningów i serii próbnych)
                if(comp->getSerieType() == CompetitionInfo::Qualifications || comp->getSerieType() == CompetitionInfo::Competition){
                    if(competitionToMove == comp){
                        break;
                    }
                }
                competitionToMoveIndexInCalendar++;
            }

            calendar->getCompetitionsReference().swapItemsAt(competitionToMoveIndexInCalendar, competitionToMoveIndexInCalendar + 1);
            competitionToMoveIndexInCalendar += 1;
            //qSwap(competitionToMoveIndexInCalendar, nextCompetitionIndexInCalendar);

            qDebug()<<"KALENDARZ:";
            debugCalendar();

            for(auto it = competitionToMoveCompetitions.begin(); it != competitionToMoveCompetitions.end(); it++){
                calendar->getCompetitionsReference().insert(competitionToMoveIndexInCalendar, *it);
            }
            qDebug()<<"PO competitionToMoveCompetitions:";
            debugCalendar();
            nextCompetitionIndexInCalendar = 0;
            for(auto & comp : calendar->getCompetitionsReference()){ //znajdujemy odpowiedni konkurs z listy (Bo nie pokazujemy treningów i serii próbnych)
                if(comp->getSerieType() == CompetitionInfo::Qualifications || comp->getSerieType() == CompetitionInfo::Competition){
                    if(nextCompetition == comp){
                        break;
                    }
                }
                nextCompetitionIndexInCalendar++;
            }
            for(auto it = nextCompetitionCompetitions.begin(); it != nextCompetitionCompetitions.end(); it++){
                calendar->getCompetitionsReference().insert(nextCompetitionIndexInCalendar, *it);
            }
            qDebug()<<"PO nextCompetitionIndexInCalendar:";
            debugCalendar();
            ui->tableView->clearSelection();
            ui->tableView->selectionModel()->select(calendarModel->index(moveIndex + 1, 0), QItemSelectionModel::Select);
            emit calendarModel->dataChanged(calendarModel->index(0, 0), calendarModel->index(calendarModel->rowCount() - 1, calendarModel->columnCount() - 1));
        }
        updateActualCompetitionByID();
    }
}
/*{
    if(ui->tableView->selectionModel()->selectedIndexes().count() > 0){
        int moveIndex = ui->tableView->selectionModel()->selectedIndexes().at(0).row();
        if(moveIndex+1 == calendarModel->rowCount()) return;
        int competitionToMoveIndex = -1;
        int i=0;
        CompetitionInfo * competitionToMove = nullptr;
        for(auto & comp : calendar->getCompetitionsReference()){ //znajdujemy odpowiedni konkurs z listy (Bo nie pokazujemy treningów i serii próbnych)
            if(comp->getSerieType() == CompetitionInfo::Qualifications || comp->getSerieType() == CompetitionInfo::Competition){
                if(i == moveIndex){
                    competitionToMove = comp;
                    competitionToMoveIndex = i;
                    break;
                }
            }
            i++;
        }
        if(competitionToMove != nullptr){
            int nextCompetitionIndex = -1;
            CompetitionInfo * nextCompetition = nullptr;
            nextCompetitionIndex = competitionToMoveIndex + 1;
            if(calendar->getCompetitionsReference().count() > nextCompetitionIndex)
                nextCompetition = calendar->getCompetitionsReference()[nextCompetitionIndex];

            QVector<CompetitionInfo *> competitionToMoveCompetitions;
            if(competitionToMove->getTrialRound() != nullptr){
                calendar->getCompetitionsReference().remove(competitionToMoveIndex - 1);
                competitionToMoveCompetitions.push_back(competitionToMove->getTrialRound());
            }
            for(auto & training : competitionToMove->getTrainingsReference()){
                calendar->getCompetitionsReference().remove(competitionToMoveIndex - 1);
                competitionToMoveCompetitions.push_back(training);
            }

            QVector<CompetitionInfo *> nextCompetitionCompetitions;
            if(nextCompetition->getTrialRound() != nullptr){
                calendar->getCompetitionsReference().remove(nextCompetitionIndex - 1);
                nextCompetitionCompetitions.push_back(nextCompetition->getTrialRound());
            }
            for(auto & training : nextCompetition->getTrainingsReference()){
                calendar->getCompetitionsReference().remove(nextCompetitionIndex - 1);
                nextCompetitionCompetitions.push_back(training);
            }

            calendar->getCompetitionsReference().swapItemsAt(competitionToMoveIndex, nextCompetitionIndex);
            qSwap(competitionToMoveIndex, nextCompetitionIndex);

            for(auto it = competitionToMoveCompetitions.rbegin(); it != competitionToMoveCompetitions.rend(); it++){
                calendar->getCompetitionsReference().insert(competitionToMoveIndex, *it);
            }
            for(auto it = nextCompetitionCompetitions.rbegin(); it != nextCompetitionCompetitions.rend(); it++){
                calendar->getCompetitionsReference().insert(nextCompetitionIndex, *it);
            }
        }
        ui->tableView->clearSelection();
        ui->tableView->selectionModel()->select(calendarModel->index(competitionToMoveIndex, 0), QItemSelectionModel::Select);
        emit calendarModel->dataChanged(calendarModel->index(0, 0), calendarModel->index(calendarModel->rowCount(), calendarModel->columnCount()));
        updateActualCompetitionByID();
    }
}*/

void CalendarEditorWidget::on_tableView_doubleClicked(const QModelIndex &index)
{
    CompetitionInfo * competition = nullptr;
    int i=0;
    for(auto & comp : calendar->getCompetitionsReference()){
        if(comp->getSerieType() == CompetitionInfo::Qualifications || comp->getSerieType() == CompetitionInfo::Competition){
            if(i == index.row()){
                competition = comp;
                actualCompetitionID = competition->getID();
                qDebug()<<"actualCompetitionID: "<<actualCompetitionID;
                updateActualCompetitionByID();
                break;
            }
            i++;
        }
    }
    if(competition != nullptr){
        competitionInfoEditor->setCompetitionInfo(competition);
        competitionInfoEditor->fillInputs();
        if(competitionInfoEditor->isHidden())
            competitionInfoEditor->show();
    }
    else{
        actualCompetitionID = (-1);
        updateActualCompetitionByID();
    }
}
