#include "SimulationSaveManagerWindow.h"
#include "ui_SimulationSaveManagerWindow.h"
#include "../../global/GlobalAppSettings.h"
#include "../../global/CountryFlagsManager.h"
#include "../../utilities/functions.h"
#include "../../competitions/CompetitionManagers/IndividualCompetitionManager.h"
#include "../../competitions/CompetitionManagers/TeamCompetitionManager.h"
#include "../Competition/CompetitionConfigWindow.h"
#include "../Competition/CompetitionManagerWindow.h"
#include "../EditorWidgets/WindsGeneratorSettingsEditorWidget.h"
#include "../EditorWidgets/InrunSnowGeneratorSettingsEditorWidget.h"
#include "../EditorWidgets/JumpersListEditorWindow.h"
#include "../ResultsShowing/JumpDataDetailedInfoWindow.h"
#include "../Competition/JumperCompetitionResultsWidget.h"
#include "../Competition/Results/ResultsTableModel.h"
#include "../Competition/Results/TeamResultsTreeModel.h"
#include "../FormGenerator/JumpersFormGeneratorConfigWindow.h"
#include "Stats/JumperStatsWindow.h"
#include "Stats/ApperanceInClassificationWindow.h"
#include "Stats/SimulationRatingsWindow.h"
#include "NewSeasonConfiguratorWindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QProgressDialog>
#include <QPushButton>
#include <QTableView>
#include <QTreeView>

SimulationSaveManagerWindow::SimulationSaveManagerWindow(SimulationSave *save, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulationSaveManagerWindow),
    simulationSave(save)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);
    ui->toolBox->setCurrentIndex(0);
    ui->toolBox_2->setCurrentIndex(0);
    ui->label_saveName->setText(simulationSave->getName());
    ui->label_seasonNumber->setText(QString::number(simulationSave->getActualSeason()->getSeasonNumber()));
    ui->comboBox_archiveSeason->setCurrentIndex(0);

    connect(ui->toolBox, &QToolBox::currentChanged, this, [this](){
        if(ui->toolBox->currentIndex() == 0 || ui->toolBox->currentIndex() == 1)
        {
            QTimer::singleShot(276, this, [this](){
                showJumperAndHillsEditingHelp();
            });
        }
    });
    emit ui->toolBox->currentChanged(0);

    //-----//

    jumpersListView = new DatabaseItemsListView(DatabaseItemsListView::SeasonJumpersItems, true, false, true, this);
    jumpersListView->setSeasonJumpers(&simulationSave->getJumpersReference());
    jumpersListView->setupListModel();
    ui->verticalLayout_jumpersList->addWidget(jumpersListView);

    jumperEditor = new JumperEditorWidget();
    jumperEditor->hide();
    ui->verticalLayout_jumperEditor->addWidget(jumperEditor);
    connect(jumpersListView, &DatabaseItemsListView::listViewDoubleClicked, this, [this](const QModelIndex & index){
        jumperEditor->show();
        jumperEditor->setJumper(simulationSave->getJumpersReference()[index.row()]);
        jumperEditor->fillJumperInputs();
        jumperEditor->setShowForm(simulationSave->getShowForm());
    });
    connect(jumperEditor, &JumperEditorWidget::submitted, this, [this](){
        int idx = jumpersListView->getLastDoubleClickedIndex();
        *simulationSave->getJumpersReference()[idx] = jumperEditor->getJumperFromWidgetInput();
        emit jumpersListView->getListModel()->dataChanged(jumpersListView->getListModel()->index(idx), jumpersListView->getListModel()->index(idx));
    });

    //-----//

    hillsListView = new DatabaseItemsListView(DatabaseItemsListView::SeasonHillsItems, true, false, true, this);
    hillsListView->setSeasonHills(&simulationSave->getHillsReference());
    hillsListView->setupListModel();
    ui->verticalLayout_hillsList->addWidget(hillsListView);

    hillEditor = new HillEditorWidget();
    hillEditor->hide();
    ui->verticalLayout_hillEditor->addWidget(hillEditor);
    connect(hillsListView, &DatabaseItemsListView::listViewDoubleClicked, this, [this](const QModelIndex & index){
        hillEditor->show();
        hillEditor->setHill(simulationSave->getHillsReference()[index.row()]);
        hillEditor->fillHillInputs();
    });
    connect(hillEditor, &HillEditorWidget::submitted, this, [this](){
        int idx = hillsListView->getLastDoubleClickedIndex();
        *simulationSave->getHillsReference()[idx] = hillEditor->getHillFromWidgetInput();
        emit hillsListView->getListModel()->dataChanged(hillsListView->getListModel()->index(idx), hillsListView->getListModel()->index(idx));
    });
    connect(hillsListView, &DatabaseItemsListView::insert, this, [this](){
        simulationSave->getActualSeason()->getCalendarReference().fixCompetitionsHills(&simulationSave->getHillsReference(), simulationSave->getHillsReference().first());
    });

    //-----//

    rulesListView = new DatabaseItemsListView(DatabaseItemsListView::CompetitionRulesItems, true, true, true, this);
    rulesListView->setCompetitionRules(&simulationSave->getCompetitionRulesReference());
    rulesListView->setupListModel();
    ui->verticalLayout_rulesList->addWidget(rulesListView);

    rulesEditor = new CompetitionRulesEditorWidget();
    rulesEditor->hide();
    ui->verticalLayout_rulesEditor->addWidget(rulesEditor);
    connect(rulesListView, &DatabaseItemsListView::listViewDoubleClicked, this, [this](const QModelIndex & index){
        rulesEditor->show();
        rulesEditor->setCompetitionRules(&simulationSave->getCompetitionRulesReference()[index.row()]);
        rulesEditor->fillCompetitionRulesInputs();
    });
    connect(rulesEditor, &CompetitionRulesEditorWidget::submitted, this, [this](){
        int idx = rulesListView->getLastDoubleClickedIndex();
        simulationSave->getCompetitionRulesReference()[idx] = rulesEditor->getCompetitionRulesFromWidgetInputs();
        emit rulesListView->getListModel()->dataChanged(rulesListView->getListModel()->index(idx), rulesListView->getListModel()->index(idx));
    });

    //-----//

    simulationSave->getActualSeason()->getCalendarReference().fixCompetitionsHills(&simulationSave->getHillsReference(), simulationSave->getHillsReference().first());
    calendarTableModel = new CalendarEditorTableModel(&simulationSave->getActualSeason()->getCalendarReference(), &simulationSave->getHillsReference(), &simulationSave->getCompetitionRulesReference(), simulationSave->getNextCompetitionIndex(), this);
    calendarEditor = new CalendarEditorWidget(calendarTableModel, &simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference());
    ui->verticalLayout_calendar->addWidget(calendarEditor);

    //-----//

    classificationEditor = new ClassificationEditorWidget();
    classificationEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    classificationEditor->setParent(this);
    ui->verticalLayout_classificationEditor->addWidget(classificationEditor);

    classificationsListView = new DatabaseItemsListView(DatabaseItemsListView::ClassificationItems, true, false, false, this);
    classificationsListView->setClassifications(&simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference());
    classificationsListView->setupListModel();
    classificationsListView->selectOnlyFirstRow();
    ui->verticalLayout_classificationsList->addWidget(classificationsListView);
    connect(classificationsListView, &DatabaseItemsListView::listViewDoubleClicked, this, [this](const QModelIndex &index){
        Classification * classification = simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference()[index.row()];
        if(classificationEditor->getClassification() != nullptr)
            classificationEditor->resetInputs();
        classificationEditor->setClassification(classification);
        classificationEditor->fillInputs();
    });
    connect(classificationEditor, &ClassificationEditorWidget::submitted, this, [this](){
        bool classificationWasUsed = false;
        for(auto & comp : simulationSave->getActualSeason()->getCalendarReference().getCompetitionsReference())
        {
            if(comp->getSerieType() == CompetitionInfo::Qualifications || comp->getSerieType() == CompetitionInfo::Competition){
                if(comp->getPlayed() == false)
                    break;
                if(MyFunctions::vectorContainsByID(comp->getClassificationsReference(), classificationEditor->getClassification()))
                {
                    classificationWasUsed = true;
                    break;
                }
            }
        }
        if(classificationWasUsed == false){
            if(classificationsListView->getListView()->selectionModel()->selectedRows().count() > 0)
            {
                int row = classificationsListView->getLastDoubleClickedIndex();//->getListView()->selectionModel()->selectedRows().at(0).row();
                simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference()[row] = classificationEditor->getClassificationFromInputs();
            }
        }
        else
        {
            QMessageBox::information(this, tr("Edycja klasyfikacji"), tr("Nie możesz edytować tej klasyfikacji, ponieważ wcześniej został rozegrany konkurs z jej użyciem.\nMożesz ją tylko całkowicie usunąć"), QMessageBox::Ok);
        }
    });
    connect(classificationsListView, &DatabaseItemsListView::insert, this, [this](){
        simulationSave->getActualSeason()->getCalendarReference().fixCompetitionsClassifications();
        setupClassificationsComboBox();
        emit ui->comboBox_classifications->currentIndexChanged(0);
        emit calendarTableModel->dataChanged(calendarTableModel->index(0, 0), calendarTableModel->index(calendarTableModel->rowCount() - 1, 6));
    });
    connect(classificationsListView, &DatabaseItemsListView::remove, this, [this](){
        simulationSave->getActualSeason()->getCalendarReference().fixCompetitionsClassifications();
        setupClassificationsComboBox();
        if(ui->comboBox_classifications->count() > 0)
            emit ui->comboBox_classifications->currentIndexChanged(0);
        emit calendarTableModel->dataChanged(calendarTableModel->index(0, 0), calendarTableModel->index(calendarTableModel->rowCount() - 1, 6));
    });
    connect(calendarEditor, &CalendarEditorWidget::changed, this, [this](){
        simulationSave->updateNextCompetitionIndex();
        fillNextCompetitionInformations();
        simulationSave->getActualSeason()->getCalendarReference().updateCompetitionsQualifyingCompetitions();
    });

    classificationResultsTableView = new ClassificationResultsTableView(false, nullptr, this);
    classificationResultsTableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    classificationResultsTableView->setFixedHeight(500);
    ui->verticalLayout_classificationResults->addWidget(classificationResultsTableView);

    setupClassificationsComboBox();
    if(ui->comboBox_classifications->count() > 0)
    {
        emit ui->comboBox_classifications->currentIndexChanged(0);
    }

    competitionsArchiveModel = new CompetitionsArchiveListModel(&simulationSave->getActualSeason()->getCalendarReference().getCompetitionsReference());
    classificationsArchiveModel = new ClassificationsArchiveListModel(&simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference());

    ui->comboBox_archiveSeason->clear();
    for(auto & season : simulationSave->getSeasonsReference())
    {
         ui->comboBox_archiveSeason->insertItem(0, QString::number(season.getSeasonNumber()));
    }
    ui->listView_competitionsArchive->setModel(competitionsArchiveModel);
    ui->listView_classificationsArchive->setModel(classificationsArchiveModel);

    archiveClassificationResults = new ClassificationResultsTableView(false, nullptr, this);
    ui->verticalLayout_archiveClassificationResults->addWidget(archiveClassificationResults);

    connect(classificationResultsTableView->getTableView(), &QTableView::doubleClicked, this, [this](const QModelIndex & index){
        showClassificationApperanceWindowAfterListClick(index, classificationResultsTableView->getClassification());
    });
    connect(archiveClassificationResults->getTableView(), &QTableView::doubleClicked, this, [this](const QModelIndex & index){
        showClassificationApperanceWindowAfterListClick(index, archiveClassificationResults->getClassification());
    });

    jumpersListsListView = new DatabaseItemsListView(DatabaseItemsListView::JumpersListsItems, true, true, true, this);
    jumpersListsListView->setMinimumWidth(600);
    jumpersListsListView->setJumpersLists(&simulationSave->getJumpersListsReference());
    jumpersListsListView->setupListModel();
    jumpersListsListView->selectOnlyFirstRow();
    ui->verticalLayout_jumpersListsListView->addWidget(jumpersListsListView);
    connect(jumpersListsListView, &DatabaseItemsListView::listViewDoubleClicked, this, [this](const QModelIndex & index){
        SaveJumpersList * list = &simulationSave->getJumpersListsReference()[index.row()];
        JumpersListEditorWindow * editor = new JumpersListEditorWindow(this, list->getName(), list->getIsDefault());
        editor->setAllJumpers(simulationSave->getJumpersReference());
        editor->setSelectedJumpers(list->getJumpersReference());
        editor->updateUnselectedJumpers();
        editor->getSelectedJumpersListView()->setSeasonJumpers(&editor->getSelectedJumpersReference());
        editor->getSelectedJumpersListView()->setupListModel();
        editor->getSelectedJumpersListView()->hideInstructions();
        editor->getUnselectedJumpersListView()->setSeasonJumpers(&editor->getUnselectedJumpersReference());
        editor->getUnselectedJumpersListView()->setupListModel();
        editor->getUnselectedJumpersListView()->hideInstructions();

        if(editor->exec() == QDialog::Accepted)
        {
            list->setJumpers(editor->getSelectedJumpers());
            list->setName(editor->getNameFromLineEdit());
            list->setIsDefault(editor->getIsDefaultFromCheckBox());
            jumpersListsListView->setupListModel();
            emit jumpersListsListView->getListModel()->dataChanged(jumpersListsListView->getListModel()->index(0), jumpersListsListView->getListModel()->index(jumpersListsListView->getListModel()->rowCount() - 1));
        }
    });
}

void SimulationSaveManagerWindow::showClassificationApperanceWindowAfterListClick(const QModelIndex &index, Classification * classification)
{
    if(index.column() == 1)
    {
         ApperanceInClassificationWindow * window = new ApperanceInClassificationWindow(this);
         if(classification->getClassificationType() == Classification::Individual){
             Jumper * jumper = classification->getResultByIndex(index.row())->getJumper();
             window->setJumper(jumper);
         }
         else{
             QString teamCode = classification->getResultByIndex(index.row())->getTeamCode();
             window->setTeamCode(teamCode);
         }
         window->setClassification(classification);

         Season * classificationSeason = nullptr;
         for(auto & season : simulationSave->getSeasonsReference())
         {
             for(auto & classification : season.getCalendarReference().getClassificationsReference())
                 if(classification == window->getClassification())
                     classificationSeason = &season;
         }
         window->setSeason(classificationSeason);

         if(classification->getClassificationType() == Classification::Individual){
             QHash<Jumper *, QHash<CompetitionInfo *, int>> archiveResults; //Tu są pozycje danego zawodnika z danych etapów klasyfikacji.
             archiveResults = classification->constructJumpersArchiveResults(classificationSeason);
             window->setArchiveResults(archiveResults.value(window->getJumper())); //Daliśmy do windowa informacje o pozycjach Kamila Stocha w różnych etapach klasyfikacji
         }
         else{
             QHash<QString, QHash<CompetitionInfo *, int>> archiveResults;
             archiveResults = classification->constructTeamsArchiveResults(classificationSeason);
             window->setArchiveResults(archiveResults.value(window->getTeamCode()));
        }
         window->fillWindow();

         if(window->exec() == QDialog::Accepted){

         }
    }
}

SimulationSaveManagerWindow::~SimulationSaveManagerWindow()
{
    delete ui;
}

void SimulationSaveManagerWindow::showJumperAndHillsEditingHelp()
{
    if(GlobalAppSettings::get()->getShowSeasonJumpersAndHillsHelp() == true){
    QMessageBox box;
    box.setWindowTitle(tr("Edycja zawodników i skoczni"));
    box.setText(tr("BARDZO WAŻNE: Zawodników i skoczni które dodano, nie można już później usunąć. Bądź ostrożny podczas dodawania!"));
    QAbstractButton *pButtonDontShow = box.addButton(tr("Nie pokazuj ponownie"), QMessageBox::ActionRole);
    box.setStandardButtons(QMessageBox::Ok);
    box.setDefaultButton(QMessageBox::Ok);
    box.exec();
    if(box.clickedButton() == pButtonDontShow)
    {
        GlobalAppSettings::get()->setShowSeasonJumpersAndHillsHelp(false);
        GlobalAppSettings::get()->writeToJson();
    }
    }
}

void SimulationSaveManagerWindow::fillNextCompetitionInformations()
{
    SeasonCalendar * calendar = &simulationSave->getActualSeason()->getCalendarReference();
    CompetitionInfo * competition = nullptr;
    int competitionIndex = 0;
    int indexInCalendar=0;
    for(auto & comp : calendar->getCompetitionsReference())
    {
        if(indexInCalendar == simulationSave->getNextCompetitionIndex()){
            competition = comp;
            break;
        }
        if(comp->getSerieType() == CompetitionInfo::Qualifications || comp->getSerieType() == CompetitionInfo::Competition){
            competitionIndex++;
        }
        indexInCalendar++;
    }
    if(competition != nullptr){
        QString text = QString::number(competitionIndex + 1) + ". " + competition->getHill()->getName() + " HS" + QString::number(competition->getHill()->getHSPoint());
        ui->label_nextCompetitionIndexAndHill->setText(text);
        ui->label_hillFlag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(competition->getHill()->getCountryCode().toLower())).scaled(ui->label_hillFlag->size()));
        switch(competition->getSerieType())
        {
        case CompetitionInfo::Competition:
            ui->label_serieTypeAndTrainingIndex->setText(tr("Konkurs"));
            break;
        case CompetitionInfo::Qualifications:
            ui->label_serieTypeAndTrainingIndex->setText(tr("Kwalifikacje"));
            break;
        case CompetitionInfo::TrialRound:
            ui->label_serieTypeAndTrainingIndex->setText(tr("Seria próbna"));
            break;
        case CompetitionInfo::Training:
            CompetitionInfo * mainCompetition = nullptr;
            for(int i=indexInCalendar; i<calendar->getCompetitionsReference().count(); i++)
            {
                CompetitionInfo * c = calendar->getCompetitionsReference()[i];
                if(c->getSerieType() == CompetitionInfo::Qualifications || c->getSerieType() == CompetitionInfo::Competition)
                {
                    mainCompetition = calendar->getCompetitionsReference()[i];
                    break;
                }
            }
            int trainingIndex = 0;
            qDebug()<<"competition: "<<competition;
            if(mainCompetition != nullptr)
                trainingIndex = mainCompetition->getTrainingsReference().indexOf(competition) + 1;
            ui->label_serieTypeAndTrainingIndex->setText(tr("Trening ") + QString::number(trainingIndex));
            break;
        }
        calendarTableModel->setDontModifyBefore(simulationSave->getNextCompetitionIndex());
        emit calendarTableModel->dataChanged(calendarTableModel->index(0, 0), calendarTableModel->index(calendarTableModel->rowCount() - 1, calendarTableModel->columnCount() - 1));
    }
    else{
        ui->label_nextCompetitionIndexAndHill->setText("BRAK");
        ui->label_hillFlag->setText("");
        ui->label_serieTypeAndTrainingIndex->setText("");
    }
}

void SimulationSaveManagerWindow::setupClassificationsComboBox()
{
    disconnectComboBox();
    ui->comboBox_classifications->clear();
    for(auto & classification : simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference())
    {
        QString text = classification->getName();
        ui->comboBox_classifications->addItem(text);
    }
    connectComboBox();
    qDebug()<<"a";
}

void SimulationSaveManagerWindow::connectComboBox()
{
    connect(ui->comboBox_classifications, &QComboBox::currentIndexChanged, this, &SimulationSaveManagerWindow::whenClassificationsComboBoxIndexChanged);
}

void SimulationSaveManagerWindow::disconnectComboBox()
{
    disconnect(ui->comboBox_classifications, &QComboBox::currentIndexChanged, this, &SimulationSaveManagerWindow::whenClassificationsComboBoxIndexChanged);
}

void SimulationSaveManagerWindow::setupNextSeasonConfigButton()
{
    ui->pushButton_competitionConfig->setText(tr("Konfiguruj nowy sezon"));
}

void SimulationSaveManagerWindow::configNextSeason()
{
    NewSeasonConfiguratorWindow * newSeasonConfig = new NewSeasonConfiguratorWindow(true, this);
    newSeasonConfig->setJumpers(simulationSave->getJumpersReference());
    newSeasonConfig->setHills(simulationSave->getHillsReference());
    newSeasonConfig->setCompetitionsRules(simulationSave->getCompetitionRulesReference());
    if(newSeasonConfig->exec() == QDialog::Accepted)
    {
        Season season;
        season.setSeasonNumber(simulationSave->getSeasonsReference().last().getSeasonNumber() + 1);
        season.setCalendar(newSeasonConfig->getCalendar());
        season.getCalendarReference().updateCompetitionsQualifyingCompetitions();

        simulationSave->getSeasonsReference().push_back(season);
        simulationSave->setActualSeason(&simulationSave->getSeasonsReference().last());
        simulationSave->updateNextCompetitionIndex();
        simulationSave->saveToFile("simulationSaves/");

        ui->pushButton_competitionConfig->setText(tr("Konfiguruj konkurs"));
        ui->label_seasonNumber->setText(QString::number(simulationSave->getActualSeason()->getSeasonNumber()));

        delete calendarTableModel;
        calendarTableModel = new CalendarEditorTableModel(&simulationSave->getActualSeason()->getCalendarReference(), &simulationSave->getHillsReference(), &simulationSave->getCompetitionRulesReference(), simulationSave->getNextCompetitionIndex(), this);
        /*calendarTableModel->setCalendar(&simulationSave->getActualSeason()->getCalendarReference());
        calendarTableModel->setHillsList(&simulationSave->getHillsReference());
        calendarTableModel->setRulesList(&simulationSave->getCompetitionRulesReference());
        calendarTableModel->setDontModifyBefore(simulationSave->getNextCompetitionIndex());
        */
        delete calendarEditor;
        calendarEditor = new CalendarEditorWidget(calendarTableModel, &simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference(), this);
calendarEditor->setClassificationsList(&simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference());
    ui->verticalLayout_calendar->addWidget(calendarEditor);

        classificationsListView->setClassifications(&simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference());
        classificationsListView->setupListModel();

        jumpersListView->getListView()->reset();
        hillsListView->getListView()->reset();
        rulesListView->getListView()->reset();

        fillNextCompetitionInformations();

        competitionsArchiveModel->setSeasonCompetitions(&simulationSave->getActualSeason()->getCalendarReference().getCompetitionsReference());
        classificationsArchiveModel->setSeasonClassifications(&simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference());
        ui->comboBox_archiveSeason->clear();
        for(auto & season : simulationSave->getSeasonsReference())
        {
            ui->comboBox_archiveSeason->insertItem(0, QString::number(season.getSeasonNumber()));
        }
        ui->comboBox_archiveSeason->setCurrentIndex(0);

        ui->listView_competitionsArchive->setModel(competitionsArchiveModel);
        ui->listView_classificationsArchive->setModel(classificationsArchiveModel);

        setupClassificationsComboBox();
        ui->comboBox_archiveSeason->setCurrentIndex(0);
        emit ui->comboBox_classifications->currentIndexChanged(0);
    }
}

void SimulationSaveManagerWindow::whenClassificationsComboBoxIndexChanged(int index)
{
    if(simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference().count() > 0){
        classificationResultsTableView->setClassification(simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference()[index]);
        classificationResultsTableView->fillTable();
    }
}

SimulationSave *SimulationSaveManagerWindow::getSimulationSave() const
{
    return simulationSave;
}

void SimulationSaveManagerWindow::on_pushButton_competitionConfig_clicked()
{
    if(simulationSave->getNextCompetition() == nullptr)
    {
        configNextSeason();
    }
    else{
        CompetitionConfigWindow * configWindow = new CompetitionConfigWindow(CompetitionConfigWindow::SeasonCompetition, this, simulationSave);
        configWindow->setModal(true);

        if(Classification::getSpecificTypeClassifications(simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference(), simulationSave->getNextCompetition()->getRulesPointer()->getCompetitionType()).count() > 0)
            configWindow->setClassificationsComboBoxIndex(1);

        if(configWindow->exec() == QDialog::Accepted)
        {
            CompetitionInfo * competition = simulationSave->getNextCompetition();
            int type = competition->getRulesPointer()->getCompetitionType();

            for (auto & team : configWindow->getCompetitionTeamsReference())
                Team::cutTeamJumpers(&team, competition->getRulesPointer()->getJumpersInTeamCount());

            AbstractCompetitionManager * competitionManager = nullptr;
            if(type == CompetitionRules::Individual)
                competitionManager = new IndividualCompetitionManager();
            else if(type == CompetitionRules::Team)
                competitionManager = new TeamCompetitionManager();

            if(competition->getQualifyingCompetitionsReference().count() > 0)
                competitionManager->setAltQualifiersLimit(competition->getQualifyingCompetitionsReference()[0]->getRulesPointer()->getRoundsReference()[0].getCount());

            competitionManager->setCompetitionInfo(competition);
            competitionManager->setCompetitionRules(competition->getRulesPointer());
            competitionManager->setResults(&competition->getResultsReference());
            competitionManager->setActualGate(configWindow->getStartingGateFromInput());
            competitionManager->setActualRound(1);
            if(type == CompetitionRules::Team)
                static_cast<TeamCompetitionManager *>(competitionManager)->setActualGroup(1);
            competitionManager->setBaseDSQProbability(configWindow->getBaseDSQProbability());
            competitionManager->setWindGenerationSettings(configWindow->getWindGeneratorSettingsEditor()->getWindsGenerationSettingsFromInputs());

            if(type == CompetitionRules::Individual){
                if(competition->getRulesPointer()->getRoundsReference()[0].getKO())
                {
                    competition->getRoundsKOGroupsReference().push_back(configWindow->getSeasonCompetitionGroups());
                    static_cast<IndividualCompetitionManager *>(competitionManager)->getRoundsKOGroupsReference().push_back(&competition->getRoundsKOGroupsReference().last());
                    static_cast<IndividualCompetitionManager *>(competitionManager)->getRoundsJumpersReference().push_back(KOGroup::getJumpersFromGroups(&competition->getRoundsKOGroupsReference().first()));
                }
                else{
                    static_cast<IndividualCompetitionManager *>(competitionManager)->getRoundsJumpersReference().push_back(configWindow->getSeasonCompetitionJumpers());
                    competition->getRoundsKOGroupsReference().push_back(QVector<KOGroup>());
                    static_cast<IndividualCompetitionManager *>(competitionManager)->getRoundsKOGroupsReference().push_back(&competition->getRoundsKOGroupsReference().last());
                }

            }
            else if(type == CompetitionRules::Team){
                competition->setTeams(configWindow->getCompetitionTeamsReference());
                static_cast<TeamCompetitionManager *>(competitionManager)->getRoundsTeamsReference().push_back(MyFunctions::convertToVectorOfPointers(&competition->getTeamsReference()));
            }

                if(type == CompetitionRules::Team)
                dynamic_cast<TeamCompetitionManager *>(competitionManager)->setupStartListStatusesForActualRound(true);
            else
                competitionManager->setupStartListStatusesForActualRound();
            competitionManager->setActualStartListIndex(0);
            competitionManager->getRoundsStartingGatesReference().push_back(competitionManager->getActualGate());

            CompetitionManagerWindow * managerWindow = new CompetitionManagerWindow(competitionManager, this);
            managerWindow->setInrunSnowGenerator(InrunSnowGenerator(configWindow->getInrunSnowGeneratorSettingsWidget()->getBase(), configWindow->getInrunSnowGeneratorSettingsWidget()->getDeviation()));
            if(managerWindow->exec() == QDialog::Accepted)
            {
                competition->setPlayed(true);
                for(auto & classification : competition->getClassificationsReference())
                {
                    for(auto & competitionSingleResult : competition->getResultsReference().getResultsReference())
                    {
                        if(classification->getClassificationType() == Classification::Individual)
                        {
                            int count = 1;
                            if(competitionSingleResult.getTeam() != nullptr)
                                count = competitionSingleResult.getTeam()->getJumpersCount();

                            for(int i=0; i<count; i++)
                            {
                                Jumper * jumper = competitionSingleResult.getJumper();
                                if(competitionSingleResult.getTeam() != nullptr)
                                    jumper = competitionSingleResult.getTeam()->getJumpersReference()[i];

                                ClassificationSingleResult * classificationResult = classification->getResultOfIndividualJumper(jumper);
                                if(classificationResult == nullptr){
                                    classification->getResultsReference().push_back(new ClassificationSingleResult(classification, jumper));
                                    classificationResult = classification->getResultOfIndividualJumper(jumper);
                                }
                                if(MyFunctions::vectorContains(classificationResult->getCompetitionsResultsReference(), &competition->getResultsReference()) == false){
                                    classificationResult->getCompetitionsResultsReference().push_back(&competition->getResultsReference());
                                    classificationResult->updateSingleResults();
                                    classificationResult->updatePointsSum();
                                }
                            }
                        }
                        else if(classification->getClassificationType() == Classification::Team)
                        {
                            QString countryCode;
                            if(competitionSingleResult.getJumper() != nullptr)
                                countryCode = competitionSingleResult.getJumper()->getCountryCode();
                            else if(competitionSingleResult.getTeam() != nullptr)
                                countryCode = competitionSingleResult.getTeam()->getCountryCode();

                            ClassificationSingleResult * classificationResult = classification->getResultOfTeam(countryCode);
                            if(classificationResult == nullptr){
                                classification->getResultsReference().push_back(new ClassificationSingleResult(classification, countryCode));
                                classificationResult = classification->getResultOfTeam(countryCode);
                            }
                            if(MyFunctions::vectorContains(classificationResult->getCompetitionsResultsReference(), &competition->getResultsReference()) == false){
                                classificationResult->getCompetitionsResultsReference().push_back(&competition->getResultsReference());
                                classificationResult->updateSingleResults();
                                classificationResult->updatePointsSum();
                            }
                        }
                    }
                    classification->sortInDescendingOrder();
                }
                if(ui->comboBox_classifications->count() > 0)
                {
                    emit ui->comboBox_classifications->currentIndexChanged(ui->comboBox_classifications->currentIndex());
                }

                for(auto & season : simulationSave->getSeasonsReference())
                    for(auto & comp : season.getCalendarReference().getCompetitionsReference())
                    {
                        comp->getResultsReference().fixSingleResultsJumps();
                    }

                simulationSave->updateNextCompetitionIndex();
                fillNextCompetitionInformations();
                ui->listView_competitionsArchive->reset();
                simulationSave->saveToFile("simulationSaves/");
                if(configWindow->getCSVFileName() != "")
                    competition->saveToCsvFile("results/season-competitions/csv/", simulationSave->getName() + " " + configWindow->getCSVFileName() + ".csv");

                if(simulationSave->getNextCompetition() == nullptr)
                {
                    QMessageBox::information(this, tr("Koniec sezonu"), tr("Sezon dobiegł końca! Aby skonfigurować kolejny sezon, wciśnij odpowiedni przycisk w oknie."), QMessageBox::Ok);
                    setupNextSeasonConfigButton();
                }
            }
        }
    }
}

void SimulationSaveManagerWindow::on_pushButton_saveToFile_clicked()
{
    simulationSave->saveToFile("simulationSaves/");
    QMessageBox::information(this, tr("Zapis do pliku"), tr("Pomyślnie zapisano aktualny zapis symulacji do pliku"), QMessageBox::Ok);
}


void SimulationSaveManagerWindow::on_pushButton_repairDatabase_clicked()
{
    QProgressDialog dialog;
    dialog.setStyleSheet("QProgressDialog{background-color: white; color: black;}");
    dialog.setMinimum(0);
    dialog.setMaximum(11);
    dialog.setValue(0);
    dialog.setMinimumDuration(0);
    dialog.setWindowTitle(QObject::tr("Naprawia zapisów symulacji "));
    //dialog.setLabelText(QString(QObject::tr("Postęp naprawy zapisu symulacji: %1 z %2")).arg(QString::number(dialog.value())).arg(QString::number(dialog.maximum())));
    dialog.setWindowModality(Qt::WindowModal);
    dialog.show();

    SeasonCalendar * calendar = &simulationSave->getActualSeason()->getCalendarReference();
    calendar->fixCompetitionsClassifications();
    dialog.setValue(dialog.value() + 1);
    QCoreApplication::processEvents();
    calendar->fixAdvancementClassifications();
    dialog.setValue(dialog.value() + 1);
    QCoreApplication::processEvents();
    calendar->fixAdvancementCompetitions();
    dialog.setValue(dialog.value() + 1);
    QCoreApplication::processEvents();
    calendar->fixCompetitionsHills(&simulationSave->getHillsReference(), simulationSave->getHillsReference().first());
    dialog.setValue(dialog.value() + 1);
    QCoreApplication::processEvents();
    simulationSave->repairDatabase();
    dialog.setValue(dialog.value() + 1);
    QCoreApplication::processEvents();
    simulationSave->saveToFile("simulationSaves/");
    dialog.setValue(dialog.value() + 1);
    QCoreApplication::processEvents();

    QMessageBox::information(this, tr("Naprawa bazy danych"), tr("Naprawiono bazę danych tego zapisu symulacji i zapisano do pliku."), QMessageBox::Ok);
}


void SimulationSaveManagerWindow::on_comboBox_archiveSeason_currentIndexChanged(int index)
{
    if(index >= 0){
        Season * season = &simulationSave->getSeasonsReference()[simulationSave->getSeasonsReference().count() - 1 - index];

        competitionsArchiveModel->setSeasonCompetitions(&season->getCalendarReference().getCompetitionsReference());
        ui->listView_competitionsArchive->reset();

        classificationsArchiveModel->setSeasonClassifications(&season->getCalendarReference().getClassificationsReference());
        ui->listView_classificationsArchive->reset();
    }
}

void SimulationSaveManagerWindow::on_listView_competitionsArchive_doubleClicked(const QModelIndex &index)
{
    CompetitionInfo * competition = competitionsArchiveModel->getSeasonCompetitions()->at(index.row());
    int compType = competition->getRulesPointer()->getCompetitionType();

    QDialog * dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Wyniki konkursu"));
    dialog->resize(1400, 800);
    QHBoxLayout * mainLayout = new QHBoxLayout(this);
    dialog->setLayout(mainLayout);

    QTableView * resultsTableView = new QTableView(this);
    ResultsTableModel * resultsModel = new ResultsTableModel(competition->getRulesPointer()->getCompetitionType(), &competition->getResultsReference(), nullptr, this);
    resultsTableView->setModel(resultsModel);
    resultsTableView->resizeColumnsToContents();
    if(compType == CompetitionRules::Individual)
        mainLayout->addWidget(resultsTableView);

    QTreeView * teamResultsTreeView = new QTreeView(this);
    TeamResultsTreeModel * teamResultsModel = new TeamResultsTreeModel(nullptr, this);
    if(compType == CompetitionRules::Team){
        QVector<Team *> teams = MyFunctions::convertToVectorOfPointers(&competition->getTeamsReference());
        teamResultsModel->setTeams(&teams);
        teamResultsModel->setResults(&competition->getResultsReference());
        teamResultsModel->setupTreeItems();
        teamResultsTreeView->setModel(teamResultsModel);
        teamResultsTreeView->expandToDepth(0);
        mainLayout->addWidget(teamResultsTreeView);
    }

    JumperCompetitionResultsWidget * jumperResultWidget = new JumperCompetitionResultsWidget(this);
    mainLayout->addWidget(jumperResultWidget);
    mainLayout->setStretch(0, 1);

    connect(resultsTableView, &QListView::doubleClicked, this, [jumperResultWidget, competition](const QModelIndex index){
        jumperResultWidget->setJumperResult(competition->getResultsReference().getResultByIndex(index.row()));
        jumperResultWidget->fillWidget();
    });
    connect(teamResultsTreeView, &QTreeView::doubleClicked, this, [jumperResultWidget, competition, teamResultsModel](const QModelIndex index){
        TreeItem * item = static_cast<TreeItem *>(index.internalPointer());
        if(item->getParentItem() == teamResultsModel->getRootItem())
            return;
        int teamIndex = item->getParentItem()->row();
        int jumperIndex = item->row();
        jumperResultWidget->setJumperResult(&competition->getResultsReference().getResultsReference()[teamIndex].getTeamJumpersResultsReference()[jumperIndex]);
        jumperResultWidget->setPositionShowing(false);
        jumperResultWidget->fillWidget();
    });

    dialog->show();
}


void SimulationSaveManagerWindow::on_listView_classificationsArchive_doubleClicked(const QModelIndex &index)
{
    Classification * classification = classificationsArchiveModel->getSeasonClassifications()->at(index.row());
    archiveClassificationResults->setClassification(classification);
    archiveClassificationResults->fillTable();
}

void SimulationSaveManagerWindow::on_pushButton_jumperStats_clicked()
{
    if(jumpersListView->getListView()->selectionModel()->selectedRows().count() == 0)
    {
        QMessageBox::warning(this, tr("Statystyki skoczka"), tr("Najpierw zaznacz jakiegoś zawodnika!"), QMessageBox::Ok);
    }
    else
    {
        int index = jumpersListView->getListView()->selectionModel()->selectedRows().first().row();
        Jumper * jumper = simulationSave->getJumpersReference()[index];
        JumperStatsWindow statsWindow;
        statsWindow.setSave(simulationSave);
        statsWindow.setupComboBox();
        statsWindow.setJumper(jumper);
        statsWindow.getRangeComboBoxes()->setSeasonsList(&simulationSave->getSeasonsReference());
        statsWindow.getRangeComboBoxes()->setupComboBoxes();
        statsWindow.getRangeComboBoxes()->setupConnections();
        statsWindow.getClassificationsCheckBoxes()->setClassificationsList(&simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference());
        statsWindow.getClassificationsCheckBoxes()->setupCheckBoxes();
        statsWindow.fillWindow();
        statsWindow.setupConnections();
        statsWindow.getShowFormCheckBox()->setChecked(simulationSave->getShowForm());
        if(statsWindow.exec() == QDialog::Accepted)
        {

        }
    }
}

void SimulationSaveManagerWindow::on_pushButton_clicked()
{
    SimulationRatingsWindow statsWindow(simulationSave, this);
    statsWindow.getRangeComboBoxes()->setSeasonsList(&simulationSave->getSeasonsReference());
    statsWindow.getRangeComboBoxes()->setupComboBoxes();
    statsWindow.getRangeComboBoxes()->setupConnections();
    statsWindow.getClassificationsCheckBoxes()->setClassificationsList(&simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference());
    statsWindow.getClassificationsCheckBoxes()->setupCheckBoxes();
    statsWindow.setFilteredJumpers(simulationSave->getJumpersReference());
    statsWindow.setupComboBox();
    statsWindow.fillWindow();   
    statsWindow.setupConnections();
    statsWindow.getShowFormCheckBox()->setChecked(simulationSave->getShowForm());
    if(statsWindow.exec() == QDialog::Accepted)
    {

    }
}

void SimulationSaveManagerWindow::on_pushButton_formGenerator_clicked()
{
    JumpersFormGeneratorConfigWindow window(this);
    window.getTableModel()->setGeneratorsSettings(JumperFormGeneratorSettings::constructJumperFormGeneratorsSettingsVector(simulationSave->getJumpersReference()));
    window.setSave(simulationSave);
    window.updateTable();
    window.on_doubleSpinBox_tendenceVariability_editingFinished();
    window.on_doubleSpinBox_tendBonus_editingFinished();
    window.on_doubleSpinBox_tendAlignment_editingFinished();
    window.on_doubleSpinBox_minTend_editingFinished();
    window.on_doubleSpinBox_maxTend_editingFinished();
    window.on_doubleSpinBox_tendVariability_editingFinished();
    window.on_doubleSpinBox_formBonus_editingFinished();
    window.on_doubleSpinBox_minForm_editingFinished();
    window.on_doubleSpinBox_maxForm_editingFinished();
    if(window.exec() == QDialog::Accepted)
    {

    }
}


void SimulationSaveManagerWindow::on_checkBox_showForm_stateChanged(int arg1)
{
    simulationSave->setShowForm(arg1);
}


void SimulationSaveManagerWindow::on_checkBox_compactSaveFile_stateChanged(int arg1)
{
    simulationSave->setSaveFileSizeReduce(arg1);
}

