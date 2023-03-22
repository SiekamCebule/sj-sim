#include "CompetitionStartListDisplayWidget.h"
#include "ui_CompetitionStartListDisplayWidget.h"

#include <QInputDialog>
#include <QEventLoop>
#include <QDialog>
#include <QLayout>
#include <QMessageBox>
#include "../../../utilities/functions.h"
#include "../../EditorWidgets/JumperEditorWidget.h"
#include "../../EditorWidgets/TeamsEditing/TeamEditorWidget.h"
#include "../../Competition/CompetitionConfigWindow.h"

CompetitionStartListDisplayWidget::CompetitionStartListDisplayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompetitionStartListDisplayWidget)
{
    ui->setupUi(this);
    settings.setValue("jumperEditorWarning", true);

    connect(this, &CompetitionStartListDisplayWidget::competitiorsTypeChanged, this, [this](){
        switch(getCompetitiorsType())
        {
        case Jumpers: competitiorsActivity = &jumpersActivity;
            break;
        case Teams: competitiorsActivity = &teamsActivity;
            break;
        }
    });
}

CompetitionStartListDisplayWidget::~CompetitionStartListDisplayWidget()
{
    delete ui;
}
void CompetitionStartListDisplayWidget::fillItemsLayout()
{
    if(ui->verticalLayout_items != nullptr)
    {
        QLayoutItem * item;
        while((item = ui->verticalLayout_items->takeAt(0)) != nullptr)
        {
            if(QString(item->widget()->metaObject()->className()) == QString("CompetitionStartListDisplayItemWidget")){
                disconnect(dynamic_cast<CompetitionStartListDisplayItemWidget *>(item->widget()), &CompetitionStartListDisplayItemWidget::doubleClicked, this,
                           &CompetitionStartListDisplayWidget::askForNewIndex);
            }
            //delete item->widget();
            item->widget()->hide();
            delete item;
        }
    }
    items.clear();

    int howMany = 0;
    switch(getCompetitiorsType()){
    case Jumpers:
        howMany = jumpers->count();
        break;
    case Teams:
        howMany = teams->count();
        break;
    }
    for(int index = 0; index < howMany; index++){
        if(getCompetitiorsType() == Jumpers || getCompetitiorsType() == Teams)
        {
            CompetitionStartListDisplayItemWidget * item = new CompetitionStartListDisplayItemWidget(Null, this);
            item->setCompetitiorType(getCompetitiorsType());
            item->setIndexInParentList(index + 1);
            item->setIsActive(competitiorsActivity->at(index));
            connect(item, &CompetitionStartListDisplayItemWidget::doubleClicked, this, &CompetitionStartListDisplayWidget::askForNewIndex);
            connect(item, &CompetitionStartListDisplayItemWidget::activityChanged, this, [this, item](){
                competitiorsActivity->replace(item->getIndexInParentList() - 1, item->getIsActive());
                if(item->getIsActive() == false)
                {
                    int indexOfItem = item->getIndexInParentList() - 1;
                    switch(getCompetitiorsType())
                    {
                    case Jumpers:
                        moveItemToVectorBack(*jumpers, indexOfItem);
                        break;
                    case Teams:
                        moveItemToVectorBack(*teams, indexOfItem);
                        break;
                    }
                    moveItemToVectorBack(*competitiorsActivity, indexOfItem);
                    fillItemsLayout();
                }
            });
            connect(item, &CompetitionStartListDisplayItemWidget::selected, this, [this, item](){
                selectedItem = item->getIndexInParentList() - 1;
                updateItemsSelection();
            });
            switch(getCompetitiorsType())
            {
            case Jumpers:
                item->setJumper(const_cast<Jumper *>(&jumpers->at(index)));
                break;
            case Teams:
                item->setTeam(const_cast<Team *>(&teams->at(index)));
                break;
            }
            items.push_back(item);
            ui->verticalLayout_items->addWidget(item);
            item->fillWidget();
        }
    }
}

void CompetitionStartListDisplayWidget::setJumpers(QVector<Jumper> *newJumpers)
{
    jumpers = newJumpers;
}

void CompetitionStartListDisplayWidget::fillCompetitiorsActivity()
{
    jumpersActivity.fill(true, jumpers->size());
    teamsActivity.fill(true, teams->size());
}

void CompetitionStartListDisplayWidget::setupTeamsJumpersVectors()
{
    for(auto & team : *teams){
        team.setJumpers(Team::getJumpersFilteredByCountryCode(*jumpers, team.getCountryCode()));
    }
}

void CompetitionStartListDisplayWidget::askForNewIndex()
{
    int countOfCompetitiors = 0;
    switch(getCompetitiorsType())
    {
    case Jumpers:
        countOfCompetitiors = jumpers->count();
        break;
    case Teams:
        countOfCompetitiors = teams->count();
        break;
    }

    QInputDialog dialog;
    dialog.setWindowTitle(tr("Zmiana miejsca na liście startowej"));
    dialog.setLabelText(tr("Podaj nowy numer startowy uczestnika na liście startowej"));
    dialog.setInputMode(QInputDialog::IntInput);
    dialog.setIntMinimum(1);
    dialog.setIntMaximum(countOfCompetitiors);
    dialog.setModal(true);
    dialog.show();
    int val = dialog.exec();
    if(val == QDialog::Accepted)
    {
        int newIndex = dialog.intValue();
        if(dynamic_cast<CompetitionStartListDisplayItemWidget *>(sender())->getIndexInParentList() != newIndex){
            switch(getCompetitiorsType())
            {
            case Jumpers:
                jumpers->swapItemsAt(dynamic_cast<CompetitionStartListDisplayItemWidget *>(sender())->getIndexInParentList() - 1, newIndex - 1);
                break;
            case Teams:
                teams->swapItemsAt(dynamic_cast<CompetitionStartListDisplayItemWidget *>(sender())->getIndexInParentList() - 1, newIndex - 1);
                break;
            }

            int t = getCompetitiorsType();
            if(t == Jumpers || t == Teams)
                competitiorsActivity->swapItemsAt(dynamic_cast<CompetitionStartListDisplayItemWidget *>(sender())->getIndexInParentList() - 1, newIndex - 1);

            fillItemsLayout();
        }
    }
}

void CompetitionStartListDisplayWidget::updateItemsSelection()
{
    int index = 0;
    for(auto & item : items)
    {
        if((item->getIndexInParentList() - 1) != selectedItem)
        {
            item->setIsSelected(false);
        }
        else item->setIsSelected(true);
        index++;
    }
}

void CompetitionStartListDisplayWidget::on_pushButton_up_clicked()
{
    if((!(selectedItem < 1)) && selectedItem > (-1))
    {
        switch(getCompetitiorsType())
        {
        case Jumpers:
            jumpers->swapItemsAt(selectedItem, selectedItem - 1);
            break;
        case Teams:
            teams->swapItemsAt(selectedItem, selectedItem - 1);
            break;
        }
        competitiorsActivity->swapItemsAt(selectedItem, selectedItem - 1);
        fillItemsLayout();
        selectedItem = selectedItem - 1;
        emit items.at(selectedItem)->selected();
    }
}

void CompetitionStartListDisplayWidget::on_pushButton_down_clicked()
{
    int size = 0;
    switch(getCompetitiorsType())
    {
    case Jumpers:
        size = jumpers->size();
        break;
    case Teams:
        size = teams->size();
        break;
    }

    if((selectedItem + 1 < size) && selectedItem > (-1))
    {
        switch(getCompetitiorsType())
        {
        case Jumpers:
            jumpers->swapItemsAt(selectedItem, selectedItem + 1);
            break;
        case Teams:
            teams->swapItemsAt(selectedItem, selectedItem + 1);
            break;
        }
        competitiorsActivity->swapItemsAt(selectedItem, selectedItem + 1);
        fillItemsLayout();
        selectedItem = selectedItem + 1;
        emit items.at(selectedItem)->selected();
    }
}

short CompetitionStartListDisplayWidget::getCompetitiorsType() const
{
    return competitiorsType;
}

void CompetitionStartListDisplayWidget::setCompetitiorsType(short newCompetitiorsType)
{
    competitiorsType = newCompetitiorsType;
    emit competitiorsTypeChanged();
}

void CompetitionStartListDisplayWidget::on_pushButton_edit_clicked()
{
    if(competitiorsActivity->at(selectedItem) == true){
        QDialog * dialog = new QDialog;
        dialog->setWindowFlags(Qt::Window);
        dialog->setWindowTitle("Edytuj uczestnika na liście startowej");
        dialog->setStyleSheet("background-color: rgb(225, 225, 225);");
        dialog->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        dialog->setFixedSize(dialog->size());
        dialog->setLayout(new QVBoxLayout(dialog));

        QWidget * editorPointer = nullptr;
        switch(getCompetitiorsType())
        {
        case Jumpers:{
            JumperEditorWidget * editor = new JumperEditorWidget(const_cast<Jumper*>(&jumpers->at(selectedItem)));
            editor->fillJumperInputs();
            editorPointer = editor;
            dialog->layout()->addWidget(editor);

            connect(editor, &JumperEditorWidget::destroyed, dialog, &QDialog::accept);
            break;
        }
        case Teams:{
            TeamEditorWidget * editor = new TeamEditorWidget(teams->at(selectedItem).getCountryCode(), teams->at(selectedItem).getJumpers()); //teams->at(selectedItem).getCountryCode()), this);
            qDebug()<<"parent:    "<<parent()->objectName();
            editor->setJumpersCount(dynamic_cast<CompetitionConfigWindow *>(parent())->getJumpersCountInTeam());
            editor->fillWidgetInputs();
            editorPointer = editor;
            dialog->layout()->addWidget(editor);

            connect(editor, &TeamEditorWidget::submitted, dialog, &QDialog::accept);
            break;
        }
        }
        if(dialog->exec() == QDialog::Accepted)
        {
            switch(getCompetitiorsType())
            {
            case Jumpers:{
                JumperEditorWidget * jumEditor = dynamic_cast<JumperEditorWidget *>(editorPointer);
                if(jumEditor->getJumperFromWidgetInput().getCountryCode() != jumpers->at(selectedItem).getCountryCode())
                {
                    if(settings.value("jumperEditorWarning") == true){
                        int button = QMessageBox::warning(this, tr("Edycja zawodnika"), tr("Jeśli dokonasz zmiany kraju zawodnika, wszystkie drużyny zostaną wygenerowane od nowa!\nCzy jesteś tego pewien?"), tr("Nie"), tr("Tak"), tr("Tak (nie pokazuj ponownie)"));
                        switch(button)
                        {
                        case 0: //Nie
                            break;
                        case 1: //Tak
                            jumpers->replace(selectedItem, dynamic_cast<JumperEditorWidget *>(editorPointer)->getJumperFromWidgetInput());
                            dynamic_cast<CompetitionConfigWindow *>(parent())->setCompetitionTeams(Team::constructTeamsVectorByJumpersList(jumpers));
                            break;
                        case 2:
                            jumpers->replace(selectedItem, dynamic_cast<JumperEditorWidget *>(editorPointer)->getJumperFromWidgetInput());
                            settings.setValue("jumperEditorWarning", false);
                            dynamic_cast<CompetitionConfigWindow *>(parent())->setCompetitionTeams(Team::constructTeamsVectorByJumpersList(jumpers));
                            break;
                        }
                    }
                    else{
                        jumpers->replace(selectedItem, dynamic_cast<JumperEditorWidget *>(editorPointer)->getJumperFromWidgetInput());
                        dynamic_cast<CompetitionConfigWindow *>(parent())->setCompetitionTeams(Team::constructTeamsVectorByJumpersList(jumpers));
                    }
                    break;
                }
                jumpers->replace(selectedItem, dynamic_cast<JumperEditorWidget *>(editorPointer)->getJumperFromWidgetInput());
                break;
            }
            case Teams:{
                teams->replace(selectedItem, dynamic_cast<TeamEditorWidget *>(editorPointer)->constructTeamFromWidgetInput());
                // trzbea przeniesc zawodnika do nowej druzyny, i usunac ze starej

                dynamic_cast<CompetitionConfigWindow *>(parent())->setCompetitionTeams(*teams);
                break;
            }
            }
            fillItemsLayout();
            delete editorPointer;
        }
    }
}

QVector<Jumper> *CompetitionStartListDisplayWidget::getJumpers() const
{
    return jumpers;
}

QVector<Jumper *> CompetitionStartListDisplayWidget::convertToVectorObjectOfPointers(QVector<Jumper> * jumpers)
{
    QVector<Jumper*> vector;
    for(auto & jumper : *jumpers)
    {
        vector.push_back(&jumper);
    }
    return vector;
}

void CompetitionStartListDisplayWidget::setTeams(QVector<Team> *newTeams)
{
    teams = newTeams;
}

