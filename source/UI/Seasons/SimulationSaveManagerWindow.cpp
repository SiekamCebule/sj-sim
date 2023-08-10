#include "SimulationSaveManagerWindow.h"
#include "ui_SimulationSaveManagerWindow.h"
#include "../../global/GlobalAppSettings.h"
#include "../../global/CountryFlagsManager.h"
#include "../../utilities/functions.h"
#include "../Competition/CompetitionConfigWindow.h"
#include "../Competition/CompetitionManagerWindow.h"
#include "../EditorWidgets/WindsGeneratorSettingsEditorWidget.h"
#include "../EditorWidgets/InrunSnowGeneratorSettingsEditorWidget.h"
#include "../../competitions/CompetitionManagers/IndividualCompetitionManager.h"
#include "../../competitions/CompetitionManagers/TeamCompetitionManager.h"
#include <QMessageBox>
#include <QTimer>
#include <QPushButton>

SimulationSaveManagerWindow::SimulationSaveManagerWindow(SimulationSave *save, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulationSaveManagerWindow),
    simulationSave(save)
{
    ui->setupUi(this);
    ui->label_saveName->setText(simulationSave->getName());
    ui->label_4->setText(QString::number(simulationSave->getActualSeason()->getSeasonNumber()));

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
    jumpersListView->setInsertLast(true);
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
    });
    connect(jumperEditor, &JumperEditorWidget::submitted, this, [this](){
        int idx = jumpersListView->getLastDoubleClickedIndex();
        *simulationSave->getJumpersReference()[idx] = jumperEditor->getJumperFromWidgetInput();
        emit jumpersListView->getListModel()->dataChanged(jumpersListView->getListModel()->index(idx), jumpersListView->getListModel()->index(idx));
    });

    //-----//

    hillsListView = new DatabaseItemsListView(DatabaseItemsListView::SeasonHillsItems, true, false, true, this);
    hillsListView->setInsertLast(true);
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
        simulationSave->getActualSeason()->getCalendarReference().fixCompetitionsHills(&simulationSave->getHillsReference());
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

    calendarTableModel = new CalendarEditorTableModel(&simulationSave->getActualSeason()->getCalendarReference(), &simulationSave->getHillsReference(), &simulationSave->getCompetitionRulesReference(), simulationSave->getNextCompetitionIndex(), this);
    calendarEditor = new CalendarEditorWidget(calendarTableModel, &simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference());
    ui->verticalLayout_calendar->addWidget(calendarEditor);

    //-----//

    classificationEditor = new ClassificationEditorWidget();
    classificationEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    classificationEditor->setParent(this);
    ui->verticalLayout_classificationEditor->addWidget(classificationEditor);

    classificationsListView = new DatabaseItemsListView(DatabaseItemsListView::ClassificationItems, true, false, false, this);
    classificationsListView->setInsertLast(true);
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

void SimulationSaveManagerWindow::whenClassificationsComboBoxIndexChanged(int index)
{
    classificationResultsTableView->setClassification(simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference()[index]);
    classificationResultsTableView->fillTable();
}

SimulationSave *SimulationSaveManagerWindow::getSimulationSave() const
{
    return simulationSave;
}

void SimulationSaveManagerWindow::on_pushButton_competitionConfig_clicked()
{
    // To teraz trzeba ustawić ten limit jeżeli skonfigurowany konkurs jest dla czyjegoś konkursu kwalifikacjami. Jeśli tak to bierzemy 1 z brzegu'
    // i ustalamy altLimit jako limit tamtego koknkursu w 1 rundzie.

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

        if(type == CompetitionRules::Individual)
            static_cast<IndividualCompetitionManager *>(competitionManager)->getRoundsJumpersReference().push_back(configWindow->getSeasonCompetitionJumpers());
        else if(type == CompetitionRules::Team){
            competition->setTeams(configWindow->getCompetitionTeamsReference());
            static_cast<TeamCompetitionManager *>(competitionManager)->getRoundsTeamsReference().push_back(MyFunctions::convertToVectorOfPointers(&competition->getTeamsReference()));
        }
\
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
                        if(competition->getRulesPointer()->getCompetitionType()== CompetitionRules::Team &&
                            classification->getPunctationType() == Classification::PointsForPlaces)
                            break;

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
                        if(competition->getRulesPointer()->getCompetitionType() == CompetitionRules::Individual &&
                            classification->getPunctationType() == Classification::PointsForPlaces)
                            break;

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
            //classificationResultsTableView->setClassification(simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference()[ui->comboBox_classifications->currentIndex()]);
            //classificationResultsTableView->fillTable();
            simulationSave->updateNextCompetitionIndex();
            fillNextCompetitionInformations();
            simulationSave->saveToFile("simulationSaves/");
        }
    }
}

void SimulationSaveManagerWindow::on_pushButton_saveToFile_clicked()
{
    simulationSave->saveToFile("simulationSaves/");
    QMessageBox::information(this, tr("Zapis do pliku"), tr("Pomyślnie zapisano aktualny zapis symulacji do pliku"), QMessageBox::Ok);
}

