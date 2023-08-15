#include "CompetitionConfigWindow.h"
#include "ui_CompetitionConfigWindow.h"

#include "../EditorWidgets/HillEditorWidget.h"
#include "../EditorWidgets/WindsGeneratorSettingsEditorWidget.h"
#include "../EditorWidgets/CompetitionRulesEditorWidget.h"
#include "../EditorWidgets/TeamsEditing/TeamEditorWidget.h"
#include "../../global/GlobalDatabase.h"
#include "../../global/GlobalSimulationSettings.h"
#include "../../global/CountryFlagsManager.h"
#include "../../competitions/CompetitionManagers/IndividualCompetitionManager.h"
#include "../../competitions/CompetitionManagers/TeamCompetitionManager.h"
#include "../../competitions/CompetitionResults.h"
#include "../../utilities/functions.h"
#include "CompetitionManagerWindow.h"

#include <QSizePolicy>
#include <QStringListModel>
#include <QStringList>
#include <QCompleter>
#include <QFileDialog>
#include <QFile>
#include <QKeyCombination>
#include <QKeySequence>
#include <QMessageBox>

CompetitionConfigWindow::CompetitionConfigWindow(short type, QWidget *parent, SimulationSave *save) :
    QDialog(parent),
    ui(new Ui::SingleCompetitionConfigWindow),
    type(type),
    simulationSave(save)
{
    ui->setupUi(this);
    if(simulationSave != nullptr)
        seasonCompetition = simulationSave->getNextCompetition();
    ui->spinBox_dsqProbability->setValue(GlobalSimulationSettings::get()->getBaseDsqProbability());
    delete ui->page_2;
    ui->toolBox->removeItem(1);
    setWindowFlags(Qt::Window);
        setupHillToolBoxItem();

    windsGeneratorSettingsEditor = new WindsGeneratorSettingsEditorWidget(this);
    windsGeneratorSettingsEditor->setRemovingSubmitButtons(true);
    windsGeneratorSettingsEditor->removeSubmitButton();
    inrunSnowGeneratorSettingsEditor = new InrunSnowGeneratorSettingsEditorWidget(this);
    inrunSnowGeneratorSettingsEditor->removeSubmitButton();
    competitionRulesEditor = new CompetitionRulesEditorWidget(this);
    competitionRulesEditor->removeSubmitButton();
    competitionRulesEditor->setCompetitionRules(new CompetitionRules(tr("Zasady")));
    competitionRulesEditor->setParent(this);

    ui->toolBox->raise();
    ui->label_title->raise();
    ui->toolBox->addItem(windsGeneratorSettingsEditor, tr("Ustawienia generatora wiatru"));
    ui->toolBox->addItem(inrunSnowGeneratorSettingsEditor, tr("Ustawienia generatora śniegu na najeździe"));
    setupCompetitionRulesToolBoxItem();

    if(getType() == SingleCompetition)
    {
        ui->comboBox_classification->hide();
        ui->comboBox_competition->hide();
        ui->pushButton_defaultStartListOrder->hide();
        ui->label_3->hide();
    }
    else
    {
        int compType = seasonCompetition->getRulesPointer()->getCompetitionType();
        for(auto & classification : Classification::getSpecificTypeClassifications(simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference(), compType))
            ui->comboBox_classification->addItem(classification->getName());

        QVector<CompetitionInfo *> competitions = CompetitionInfo::getSpecificTypeCompetitions(simulationSave->getActualSeason()->getCalendarReference().getCompetitionsReference(), compType);
        for(auto & competition : competitions)
        {
            int competitionIndex = simulationSave->getActualSeason()->getCalendarReference().getCompetitionMainIndex(simulationSave->getActualSeason()->getCalendarReference().getCompetitionsReference(), competition); //Index konkursu na głównej liście (bez treningów i serii próbnych)
            if(competitionIndex < SeasonCalendar::getCompetitionMainIndex(simulationSave->getActualSeason()->getCalendarReference().getCompetitionsReference(), simulationSave->getNextCompetition())){
                Hill * hill = competition->getHill();
                ui->comboBox_competition->addItem(QIcon(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(hill->getCountryCode().toLower()))),
                                              QString::number(competitionIndex + 1) + ". " + hill->getName() + " HS" + QString::number(hill->getHSPoint()));
            }
        }

        if(seasonCompetition->getRulesPointer()->getCompetitionType() == CompetitionRules::Team)
            ui->pushButton_defaultStartListOrder->hide();
    }

    jumpersListView = new DatabaseItemsListView(DatabaseItemsListView::SeasonJumpersItems, false, true, true, this);
    jumpersListView->getListView()->setSelectionMode(QAbstractItemView::ExtendedSelection);
    jumpersListView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    if(getType() == SingleCompetition)
        jumpersListView->setSeasonJumpers(&competitionJumpers);
    else{
        jumpersListView->setType(DatabaseItemsListView::SeasonJumpersItems);
        if(seasonCompetition->getAdvancementCompetition() != nullptr && seasonCompetition->getRulesPointer()->getCompetitionType() == CompetitionRules::Individual)
            seasonCompetitionJumpers = IndividualCompetitionManager::getFilteredJumpersAfterQualifications(seasonCompetition, simulationSave->getJumpersReference());
        else if(seasonCompetition->getAdvancementClassification() != nullptr && seasonCompetition->getRulesPointer()->getCompetitionType() == CompetitionRules::Individual)
            seasonCompetitionJumpers = IndividualCompetitionManager::getFilteredJumpersByClassification(seasonCompetition, seasonCompetition->getAdvancementClassification(), simulationSave->getJumpersReference());
        else{
            for(auto & jp : simulationSave->getJumpersReference())
            {
                seasonCompetitionJumpers.push_back(jp);
            }
        }
        jumpersListView->setSeasonJumpers(&seasonCompetitionJumpers);
    }
    jumpersListView->setupListModel();
    ui->verticalLayout_startList->addWidget(jumpersListView);

    teamsSquadsModel = new TeamsSquadsTreeModel(&competitionTeams);
    if(getType() == SingleCompetition)
        teamsSquadsModel->setJumpersInTeam(competitionRulesEditor->getJumpersCountInTeam());
    else teamsSquadsModel->setJumpersInTeam(seasonCompetition->getRulesPointer()->getJumpersInTeamCount());
    teamsSquadsModel->setupTreeItems();
    teamsTreeView = new TeamsSquadsTreeView(teamsSquadsModel, this);
    ui->verticalLayout_startList->addWidget(teamsTreeView);

    if(getType() == SeasonCompetition)
    {
        if(seasonCompetition->getRulesPointer()->getCompetitionType() == CompetitionRules::Individual){
            jumpersListView->show();
            teamsTreeView->hide();
        }
        else if(seasonCompetition->getRulesPointer()->getCompetitionType() == CompetitionRules::Team){
            jumpersListView->hide();
            teamsTreeView->show();
        }

        ui->pushButton_loadJumpers->hide();
        setWindowTitle(tr("Konfiguracja konkursu"));
        ui->toolBox->widget(0)->hide();
        ui->toolBox->removeItem(0);
        ui->toolBox->widget(2)->hide();
        ui->toolBox->removeItem(2);

        windsGeneratorSettingsEditor->setKPoint(seasonCompetition->getHill()->getKPoint());
        windsGeneratorSettingsEditor->setWindGenerationSettings(new QVector<WindGenerationSettings>());
        windsGeneratorSettingsEditor->fillWindGenerationSettingsByDefault();
        windsGeneratorSettingsEditor->fillSettingsInputs();

        if(seasonCompetition->getRulesPointer()->getCompetitionType() == CompetitionRules::Team)
        {
            if(seasonCompetition->getAdvancementCompetition() != nullptr)
                competitionTeams = TeamCompetitionManager::getFilteredTeamsAfterQualifications(seasonCompetition);
            else
            {
                competitionTeams = Team::constructTeamsVectorByJumpersList(seasonCompetitionJumpers, seasonCompetition->getRulesPointer()->getJumpersInTeamCount());
                if(seasonCompetition->getAdvancementClassification() != nullptr && seasonCompetition->getRulesPointer()->getCompetitionType() == CompetitionRules::Team)
                    competitionTeams = TeamCompetitionManager::getFilteredTeamsByClassification(seasonCompetition, seasonCompetition->getAdvancementClassification(), competitionTeams);
            }

            teamsSquadsModel->setupTreeItems();
            teamsTreeView->setModel(teamsSquadsModel);
            //teamsTreeView->getTreeView()->expandToDepth(0);
            connect(jumpersListView, &DatabaseItemsListView::remove, this, [this](){
                competitionTeams = Team::constructTeamsVectorByJumpersList(seasonCompetitionJumpers, seasonCompetition->getRulesPointer()->getJumpersInTeamCount());
                teamsSquadsModel->setupTreeItems();
                teamsTreeView->setModel(teamsSquadsModel);
                teamsTreeView->getTreeView()->expandToDepth(0);
            });
        }
    }

    else if(getType() == SingleCompetition)
    {
        checkBox_singleCompetitionQualifications = new QCheckBox(tr("Przeprowadzenie kwalifikacji"), this);
        checkBox_singleCompetitionQualifications->setStyleSheet("color: rgb(30, 30, 30);\nQCheckBox::indicator{\nwidth: 40px;\nheight: 30px;\n}");
        checkBox_singleCompetitionQualifications->setFont(QFont("Quicksand Medium", 12, 650));
        ui->formLayout_competitionOptions->addWidget(checkBox_singleCompetitionQualifications);

        setWindowTitle("Konfiguracja pojedynczego konkursu");
        competitionJumpers = MyFunctions::convertToVectorOfPointers(&GlobalDatabase::get()->getEditableGlobalJumpers());
        competitionTeams = Team::constructTeamsVectorByJumpersList(competitionJumpers, competitionRulesEditor->getJumpersCountInTeam());
    }
    connect(competitionRulesEditor, &CompetitionRulesEditorWidget::jumpersCountInTeamChanged, this, [this](){
        competitionTeams = Team::constructTeamsVectorByJumpersList(competitionJumpers, competitionRulesEditor->getJumpersCountInTeam());
        delete teamsSquadsModel;
        teamsSquadsModel = new TeamsSquadsTreeModel(&competitionTeams, competitionRulesEditor->getJumpersCountInTeam());
        teamsSquadsModel->setupTreeItems();
        teamsTreeView->setModel(teamsSquadsModel);
        teamsTreeView->getTreeView()->expandToDepth(0);
        //emit teamsSquadsModel->dataChanged(teamsSquadsModel->index(0, 0, QModelIndex()), teamsSquadsModel->createIndex());
    });

    connect(hillEditor, &HillEditorWidget::KPointInputChanged, this, [this](){
        windsGeneratorSettingsEditor->setKPoint(hillEditor->getKPointFromInput());
        windsGeneratorSettingsEditor->setWindGenerationSettings(new QVector<WindGenerationSettings>());
        windsGeneratorSettingsEditor->fillWindGenerationSettingsByDefault();
        windsGeneratorSettingsEditor->fillSettingsInputs();
    });
    connect(competitionRulesEditor, &CompetitionRulesEditorWidget::competitionTypeChanged, this, [this](){
        if(competitionRulesEditor->getCompetitionTypeFromInput() == CompetitionRules::Individual){
            jumpersListView->setHidden(false);
            teamsTreeView->setHidden(true);
        }
        else if(competitionRulesEditor->getCompetitionTypeFromInput() == CompetitionRules::Team){
            jumpersListView->setHidden(true);
            teamsTreeView->setHidden(false);
        }
    });

    connect(teamsTreeView, &TeamsSquadsTreeView::needToUpdateModel, this, [this]{
        int count = competitionRulesEditor->getJumpersCountInTeam();
        if(seasonCompetition != nullptr)
            count = seasonCompetition->getRulesPointer()->getJumpersInTeamCount();
        teamsSquadsModel = new TeamsSquadsTreeModel(&competitionTeams, count );
        teamsSquadsModel->setupTreeItems();
        teamsTreeView->setModel(teamsSquadsModel);
    });


    connect(teamsTreeView, &TeamsSquadsTreeView::treeViewDoubleClicked, this, [this](const QModelIndex & index){
        if(getType() == SeasonCompetition)
            if(seasonCompetition->getAdvancementCompetition() != nullptr || seasonCompetition->getAdvancementClassification() != nullptr)
                return;
        TreeItem * item = static_cast<TreeItem *>(index.internalPointer());
        int teamIndex = 0;
        if(item->getParentItem() == teamsSquadsModel->getRootItem()){
            teamIndex = item->row();
        }
        else{
            teamIndex = item->getParentItem()->row();
        }

        TeamEditorWidget * editor = new TeamEditorWidget(&competitionTeams[teamIndex]);
        if(getType() == SingleCompetition)
            editor->setJumpersCount(competitionRulesEditor->getJumpersCountInTeam());
        else editor->setJumpersCount(seasonCompetition->getRulesPointer()->getJumpersInTeamCount());
        editor->getModel()->setJumpersInTeamCount(editor->getJumpersCount());

        editor->setAttribute(Qt::WA_DeleteOnClose);
        editor->fillWidgetInputs();
        editor->show();
        QDialog dialog(this);
        dialog.setLayout(new QHBoxLayout());
        dialog.layout()->addWidget(editor);
        dialog.setModal(true);
        connect(editor, &TeamEditorWidget::submitted, &dialog, &QDialog::accept);
        if(dialog.exec() == QDialog::Accepted){
            competitionTeams[teamIndex] = editor->constructTeamFromWidgetInput();
            delete teamsSquadsModel;
            teamsSquadsModel = new TeamsSquadsTreeModel(&competitionTeams);
            if(getType() == SingleCompetition)
                teamsSquadsModel->setJumpersInTeam(competitionRulesEditor->getJumpersCountInTeam());
            else teamsSquadsModel->setJumpersInTeam(seasonCompetition->getRulesPointer()->getJumpersInTeamCount());
            teamsSquadsModel->setupTreeItems();
            teamsTreeView->setModel(teamsSquadsModel);
            teamsTreeView->getTreeView()->expandToDepth(0);
        }
    });

    ui->verticalLayout_startList->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Maximum));

    if(getType() == SingleCompetition)
        emit competitionRulesEditor->competitionTypeChanged();

    KOGroupsList = new KOGroupsListView(this);
    if(getType() == SingleCompetition)
    {
        KOGroupsList->setJumpersList(&competitionJumpers);
    }
    else{
        KOGroupsList->setJumpersList(&seasonCompetitionJumpers);
    }
    KOGroupsList->hide();
    comboBox_groupsSelectionType = new QComboBox(this);
    comboBox_groupsSelectionType->addItem(tr("Dobierz grupy"));
    comboBox_groupsSelectionType->addItem(tr("Klasycznie"));
    comboBox_groupsSelectionType->addItem(tr("Klasycznie (Dla dużych grup)"));
    comboBox_groupsSelectionType->addItem(tr("Losowo (Z podziałem na koszyki)"));
    comboBox_groupsSelectionType->addItem(tr("Losowo"));
    ui->verticalLayout_startList->addWidget(comboBox_groupsSelectionType);
    comboBox_groupsSelectionType->hide();
    ui->verticalLayout_startList->addWidget(KOGroupsList);

    if(getType() == SingleCompetition){
        /*RoundInfo roundInfo;//= competitionRulesEditor->getRoundsFromInput().at(0);
        roundInfo.setAdvancingFromKOGroup(1);
        roundInfo.setCountInKOGroup(2);
        roundInfo.setKO(true);
        if(competitionRulesEditor->getCompetitionTypeFromInput() == CompetitionRules::Individual){
            QVector<Jumper *> jms = MyFunctions::convertToVectorOfPointers(&competitionJumpers);
            competitionGroups = KOGroup::constructDefaultKOGroups(&roundInfo, &jms, KOGroup::Random, nullptr);
        }*/
        KOGroupsList->setKOGroups(&competitionGroups);
    }
    else{
        KOGroupsList->setKOGroups(&seasonCompetition->getKOGroupsReference());
    }

    if(getType() == SingleCompetition){
        if(competitionRulesEditor->getRoundsFromInput().count() > 0){
            if(competitionRulesEditor->getRoundsFromInput().at(0).getKO() == true)
            {
                jumpersListView->hide();
                teamsTreeView->hide();
                KOGroupsList->show();
                comboBox_groupsSelectionType->show();
                KOGroupsList->fillListLayout();
            }
            else{
                KOGroupsList->hide();
                comboBox_groupsSelectionType->hide();
                emit competitionRulesEditor->competitionTypeChanged();
            }
        }
    }
    else{
        if(seasonCompetition != nullptr){
            if(seasonCompetition->getRulesPointer()->getRoundsReference()[0].getKO() == true)
            {
                jumpersListView->hide();
                teamsTreeView->hide();
                KOGroupsList->show();
                comboBox_groupsSelectionType->show();
                KOGroupsList->fillListLayout();
            }
            else{
                KOGroupsList->hide();
                comboBox_groupsSelectionType->hide();
                emit competitionRulesEditor->competitionTypeChanged();
            }
        }
    }
    connect(competitionRulesEditor, &CompetitionRulesEditorWidget::KORoundChanged, this, [this](){
        if(competitionRulesEditor->getRoundsFromInput().count() > 0){
            if(competitionRulesEditor->getRoundsFromInput().at(0).getKO() == true)
            {
                jumpersListView->hide();
                teamsTreeView->hide();
                KOGroupsList->show();
                comboBox_groupsSelectionType->show();
                KOGroupsList->fillListLayout();
            }
            else{
                KOGroupsList->hide();
                comboBox_groupsSelectionType->hide();
                emit competitionRulesEditor->competitionTypeChanged();
            }
        }
    });
    if(getType() == SingleCompetition)
        emit competitionRulesEditor->KORoundChanged();

    connect(comboBox_groupsSelectionType, &QComboBox::activated, this, [this](int index){
        if(index > 0)
        {
            index--;
            bool singleCompetitionCondition = false;
            bool seasonCompetitionCondition = false;
            if(getType() == SingleCompetition)
            {
                if(index != KOGroup::Classic || (index == KOGroup::Classic && ((competitionJumpers.count() % 2 == 0) || (competitionRulesEditor->getRoundsFromInput().at(0).getCountInKOGroup() == 2))))
                {
                    RoundInfo roundInfo = competitionRulesEditor->getRoundsFromInput().at(0);
                    competitionGroups = KOGroup::constructKOGroups(&roundInfo, &competitionJumpers, index, nullptr);
                }
            }
            else if((seasonCompetitionJumpers.count() % 2 == 0) && (seasonCompetition->getRulesPointer()->getRoundsReference()[0].getCountInKOGroup() == 2))
            {
                if(index != KOGroup::Classic || (index == KOGroup::Classic && ((seasonCompetitionJumpers.count() % 2 == 0) || (seasonCompetition->getRulesPointer()->getRoundsReference()[0].getCountInKOGroup() == 2))))
                    competitionGroups = KOGroup::constructKOGroups(&seasonCompetition->getRulesPointer()->getRoundsReference()[0], &seasonCompetitionJumpers, index, seasonCompetition);
            }
            KOGroupsList->fillListLayout();
        }
    });
}

CompetitionConfigWindow::~CompetitionConfigWindow()
{
    delete ui;
}

int CompetitionConfigWindow::getJumpersCountInTeam() const
{
    return competitionRulesEditor->getJumpersCountInTeam();
}

int CompetitionConfigWindow::getStartingGateFromInput() const
{
    return ui->spinBox_startGate->value();
}

int CompetitionConfigWindow::getBaseDSQProbability() const
{
    return ui->spinBox_dsqProbability->value();
}

WindsGeneratorSettingsEditorWidget *CompetitionConfigWindow::getWindGeneratorSettingsWidget()
{
    return windsGeneratorSettingsEditor;
}

InrunSnowGeneratorSettingsEditorWidget *CompetitionConfigWindow::getInrunSnowGeneratorSettingsWidget()
{
    return inrunSnowGeneratorSettingsEditor;
}

void CompetitionConfigWindow::setClassificationsComboBoxIndex(int index)
{
    ui->comboBox_classification->setCurrentIndex(index);
    emit ui->comboBox_classification->activated(index);
}

void CompetitionConfigWindow::setupHillToolBoxItem()
{
    ui->comboBox_existingHill->clear();
    ui->comboBox_existingHill->addItem(tr("BRAK"));
    for(const auto & hill : GlobalDatabase::get()->getEditableGlobalHills())
    {
        ui->comboBox_existingHill->addItem(QIcon(QPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(hill.getCountryCode().toLower())))) ,hill.getName() + " HS" + QString::number(hill.getHSPoint()));
    }

    hillEditor = new HillEditorWidget(this);
    hillEditor->removeSubmitButton();
    ui->verticalLayout_hillEditorWidget->addWidget(hillEditor);

    connect(ui->comboBox_existingHill, &QComboBox::currentIndexChanged, this, [this](){
        int selected = ui->comboBox_existingHill->currentIndex();
        if(selected > 0){
            hillEditor->setHill(&GlobalDatabase::get()->getEditableGlobalHills()[selected - 1]);
            hillEditor->fillHillInputs();
            emit hillEditor->KPointInputChanged(hillEditor->getHill()->getKPoint());
        }
        else hillEditor->resetHillInputs();
    });
}

void CompetitionConfigWindow::setupCompetitionRulesToolBoxItem()
{
    existingCompetitionRulesComboBox = new QComboBox();
    QStringList rulesStringList;
    rulesStringList.push_back(tr("BRAK"));
    for(const auto & rule : GlobalDatabase::get()->getEditableCompetitionRules())
    {
        rulesStringList.push_back(rule.getName());
    }
    QStringListModel * existingCompetitionRulesModel = new QStringListModel(rulesStringList, this);
    existingCompetitionRulesComboBox->setModel(existingCompetitionRulesModel);

    competitionRulesParentWidget = new QWidget(this);
    competitionRulesToolBoxItemLayout = new QVBoxLayout(competitionRulesParentWidget);
    competitionRulesParentWidget->setLayout(competitionRulesToolBoxItemLayout);
    existingRulesLabelAndComboBoxLayout = new QVBoxLayout(competitionRulesParentWidget);
    competitionRulesToolBoxItemLayout->addLayout(existingRulesLabelAndComboBoxLayout);
    //competitionRulesToolBoxItemLayout->addSpacerItem(new QSpacerItem(35, 35, QSizePolicy::Maximum, QSizePolicy::Maximum));
    competitionRulesToolBoxItemLayout->addWidget(competitionRulesEditor);
    //competitionRulesToolBoxItemLayout->addSpacerItem(new QSpacerItem(35, 35, QSizePolicy::Expanding, QSizePolicy::Expanding));

    existingCompetitionRulesLabel = new QLabel(tr("Istniejące zasady konkursu"), competitionRulesParentWidget);
    existingCompetitionRulesLabel->setStyleSheet("color: rgb(20, 20, 20);");
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    font.setFamily("Quicksand [PfEd]");
    existingCompetitionRulesLabel->setFont(font);
    existingRulesLabelAndComboBoxLayout->addWidget(existingCompetitionRulesLabel);
    existingRulesLabelAndComboBoxLayout->addWidget(existingCompetitionRulesComboBox);

    ui->toolBox->addItem(competitionRulesParentWidget, tr("Zasady konkursu"));

    connect(existingCompetitionRulesComboBox, &QComboBox::currentIndexChanged, this, [this](){
        int selected = existingCompetitionRulesComboBox->currentIndex();
        if(selected > 0){
            CompetitionRules * rules = &GlobalDatabase::get()->getEditableCompetitionRules()[selected - 1];
            competitionRulesEditor->setCompetitionRules(rules);
            competitionRulesEditor->fillCompetitionRulesInputs();
        }
        else competitionRulesEditor->resetInputs();
    });
}

QVector<KOGroup> CompetitionConfigWindow::getCompetitionGroups() const
{
    return competitionGroups;
}

void CompetitionConfigWindow::setCompetitionGroups(const QVector<KOGroup> &newCompetitionGroups)
{
    competitionGroups = newCompetitionGroups;
}

QVector<Jumper *> CompetitionConfigWindow::getSeasonCompetitionJumpers() const
{
    return seasonCompetitionJumpers;
}

void CompetitionConfigWindow::setSeasonCompetitionJumpers(const QVector<Jumper *> &newSeasonCompetitionJumpers)
{
    seasonCompetitionJumpers = newSeasonCompetitionJumpers;
}

QVector<KOGroup> &CompetitionConfigWindow::getCompetitionGroupsReference()
{
    return competitionGroups;
}

SimulationSave *CompetitionConfigWindow::getSimulationSave() const
{
    return simulationSave;
}

void CompetitionConfigWindow::setSimulationSave(SimulationSave *newSimulationSave)
{
    simulationSave = newSimulationSave;
}

CompetitionInfo *CompetitionConfigWindow::getSeasonCompetition() const
{
    return seasonCompetition;
}

void CompetitionConfigWindow::setSeasonCompetition(CompetitionInfo *newSeasonCompetition)
{
    seasonCompetition = newSeasonCompetition;
}

TeamsSquadsTreeModel *CompetitionConfigWindow::getTeamsSquadsModel() const
{
    return teamsSquadsModel;
}

void CompetitionConfigWindow::setTeamsSquadsModel(TeamsSquadsTreeModel *newTeamsSquadsModel)
{
    teamsSquadsModel = newTeamsSquadsModel;
}

void CompetitionConfigWindow::setCompetitionJumpers(const QVector<Jumper *> &newCompetitionJumpers)
{
    competitionJumpers = newCompetitionJumpers;
}

QVector<Team> CompetitionConfigWindow::getCompetitionTeams() const
{
    return competitionTeams;
}

QVector<Team> &CompetitionConfigWindow::getCompetitionTeamsReference()
{
    return competitionTeams;
}

void CompetitionConfigWindow::setCompetitionTeams(const QVector<Team> &newCompetitionTeams)
{
    competitionTeams = newCompetitionTeams;
}

QVector<Jumper *> CompetitionConfigWindow::getCompetitionJumpers() const
{
    return competitionJumpers;
}

QVector<Jumper *> &CompetitionConfigWindow::getCompetitionJumpersReference()
{
    return competitionJumpers;
}

HillEditorWidget *CompetitionConfigWindow::getHillEditor() const
{
    return hillEditor;
}

void CompetitionConfigWindow::setHillEditor(HillEditorWidget *newHillEditor)
{
    hillEditor = newHillEditor;
}

CompetitionRulesEditorWidget *CompetitionConfigWindow::getCompetitionRulesEditor() const
{
    return competitionRulesEditor;
}

void CompetitionConfigWindow::setCompetitionRulesEditor(CompetitionRulesEditorWidget *newCompetitionRulesEditor)
{
    competitionRulesEditor = newCompetitionRulesEditor;
}

WindsGeneratorSettingsEditorWidget *CompetitionConfigWindow::getWindGeneratorSettingsEditor()
{
    return windsGeneratorSettingsEditor;
}

void CompetitionConfigWindow::setWindGeneratorSettingsEditor(WindsGeneratorSettingsEditorWidget *newWindsGeneratorSettingsEditor)
{
    windsGeneratorSettingsEditor = newWindsGeneratorSettingsEditor;
}

short CompetitionConfigWindow::getType() const
{
    return type;
}

void CompetitionConfigWindow::setType(short newType)
{
    type = newType;
}

void CompetitionConfigWindow::on_pushButton_submit_clicked()
{
    emit submitted();
    if(getType() == SeasonCompetition)
        accept();
    switch(getType())
    {
    case SingleCompetition:{
        CompetitionInfo qualsInfo;
        qualsInfo.setHill(new Hill(hillEditor->getHillFromWidgetInput()));
        qualsInfo.setRules(competitionRulesEditor->getCompetitionRulesFromWidgetInputs());
        qualsInfo.setSerieType(CompetitionInfo::Qualifications);
        qualsInfo.setExceptionalRoundsCount(1);
        int type = competitionRulesEditor->getCompetitionTypeFromInput();

        for(auto & team : competitionTeams)
            Team::cutTeamJumpers(&team, competitionRulesEditor->getJumpersCountInTeam());

        AbstractCompetitionManager * competitionManager = nullptr;
        if(type == CompetitionRules::Individual)
            competitionManager = new IndividualCompetitionManager();
        else if(type == CompetitionRules::Team)
            competitionManager = new TeamCompetitionManager();

        AbstractCompetitionManager * qualificationsManager = nullptr;
        if(checkBox_singleCompetitionQualifications->isChecked() == true){
            if(type == CompetitionRules::Individual)
                qualificationsManager = new IndividualCompetitionManager();
            else if(type == CompetitionRules::Team)
                qualificationsManager = new TeamCompetitionManager();

            qualificationsManager->getRoundsStartingGatesReference().push_back(ui->spinBox_startGate->value());

            if(type == CompetitionRules::Individual){
                static_cast<IndividualCompetitionManager *>(qualificationsManager)->getRoundsJumpersReference().push_back(competitionJumpers);
                //if(competitionRulesEditor->getRoundsFromInput().at(0).getKO() == true)
                  //  static_cast<IndividualCompetitionManager *>(qualificationsManager)->getRoundsKOGroupsReference().push_back(competitionGroups);
            }
            else if(type == CompetitionRules::Team){
                QVector<Team *> teams;
                for(auto & t : competitionTeams)
                    teams.push_back(&t);
                static_cast<TeamCompetitionManager *>(qualificationsManager)->getRoundsTeamsReference().push_back(teams);
            }

            qualificationsManager->setCompetitionInfo(&qualsInfo);
            qualificationsManager->getCompetitionInfo()->setSerieType(CompetitionInfo::Qualifications);
            qualificationsManager->setCompetitionRules(qualsInfo.getRulesPointer());
            qualificationsManager->setResults(&qualsInfo.getResultsReference());
            qualificationsManager->setActualGate(ui->spinBox_startGate->value());
            qualificationsManager->getRoundsStartingGatesReference().push_back(qualificationsManager->getActualGate());
            qualificationsManager->setActualRound(1);
            if(type == CompetitionRules::Team)
                static_cast<TeamCompetitionManager *>(qualificationsManager)->setActualGroup(1);

            if(type == CompetitionRules::Team)
                dynamic_cast<TeamCompetitionManager *>(qualificationsManager)->setupStartListStatusesForActualRound(true);
            else
                qualificationsManager->setupStartListStatusesForActualRound();
            qualificationsManager->setActualStartListIndex(0);
            qualificationsManager->setBaseDSQProbability(ui->spinBox_dsqProbability->value());
            qualificationsManager->setWindGenerationSettings(windsGeneratorSettingsEditor->getWindsGenerationSettingsFromInputs());
            QVector<RoundInfo> qualsRounds = {
                RoundInfo(0, false),
                RoundInfo(qualificationsManager->getCompetitionRules()->getRounds().at(0).getCount(), qualificationsManager->getCompetitionRules()->getRounds().at(0).getSortStartList())
            };
            qualificationsManager->getCompetitionRules()->setRounds(qualsRounds);

            CompetitionManagerWindow * qualsWindow = new CompetitionManagerWindow(qualificationsManager, this);
            qualsWindow->setInrunSnowGenerator(InrunSnowGenerator(inrunSnowGeneratorSettingsEditor->getBase(), inrunSnowGeneratorSettingsEditor->getDeviation()));
            connect(qualsWindow->getManager(), &AbstractCompetitionManager::competitionEnd, qualsWindow, [qualsWindow](){
                qualsWindow->showMessageBoxForQualificationsEnd();
            });
            if(qualsWindow->exec() == QDialog::Accepted)
            {
            }
        }

        CompetitionInfo info;
        info.setHill(new Hill(hillEditor->getHillFromWidgetInput()));
        info.setRules(competitionRulesEditor->getCompetitionRulesFromWidgetInputs());
        info.setSerieType(CompetitionInfo::Competition);

        //competitionManager->setStartingJumpers(startListDisplayWidget->getIndividualCompetitionJumpers());
        //Wypełnić

        competitionManager->setCompetitionInfo(&info);
        competitionManager->setCompetitionRules(info.getRulesPointer());
        competitionManager->setResults(&info.getResultsReference());
        competitionManager->setActualGate(ui->spinBox_startGate->value());
        competitionManager->setActualRound(1);
        if(type == CompetitionRules::Team)
            static_cast<TeamCompetitionManager *>(competitionManager)->setActualGroup(1);
        competitionManager->setBaseDSQProbability(ui->spinBox_dsqProbability->value());
        competitionManager->setWindGenerationSettings(windsGeneratorSettingsEditor->getWindsGenerationSettingsFromInputs());

        if(qualsInfo.getCancelled() == true)
            return;
        if(qualsInfo.getResultsReference().getResultsReference().count() > 0){
            if(type == CompetitionRules::Individual){
                static_cast<IndividualCompetitionManager *>(competitionManager)->getRoundsJumpersReference().push_back(static_cast<IndividualCompetitionManager *>(qualificationsManager)->getFilteredJumpersForNextRound());
            }
            else if(type == CompetitionRules::Team){
                static_cast<TeamCompetitionManager *>(competitionManager)->getRoundsTeamsReference().push_back(static_cast<TeamCompetitionManager *>(qualificationsManager)->getFilteredTeamsForNextRound());
            }
            competitionManager->setActualGate(qualificationsManager->getActualGate());
            competitionManager->setWindGenerationSettings(qualificationsManager->getWindGenerationSettingsReference());
        }
        else{
            if(type == CompetitionRules::Individual){
                if(competitionRulesEditor-> getRoundsFromInput().at(0).getKO() == true){
                    static_cast<IndividualCompetitionManager *>(competitionManager)->getRoundsJumpersReference().push_back(KOGroup::getJumpersFromGroups(competitionGroups));
                    static_cast<IndividualCompetitionManager *>(competitionManager)->getRoundsKOGroupsReference().push_back(competitionGroups);
                }
                else
                {
                    static_cast<IndividualCompetitionManager *>(competitionManager)->getRoundsJumpersReference().push_back(competitionJumpers);
                    static_cast<IndividualCompetitionManager *>(competitionManager)->getRoundsKOGroupsReference().push_back(QVector<KOGroup>());
                }
            }
            else if(type == CompetitionRules::Team){
                QVector<Team *> teams;
                for(auto & t : competitionTeams) teams.push_back(&t);
                static_cast<TeamCompetitionManager *>(competitionManager)->getRoundsTeamsReference().push_back(teams);
            }
        }
        if(type == CompetitionRules::Team)
            dynamic_cast<TeamCompetitionManager *>(competitionManager)->setupStartListStatusesForActualRound(true);
        else
            competitionManager->setupStartListStatusesForActualRound();
        competitionManager->setActualStartListIndex(0);
        competitionManager->getRoundsStartingGatesReference().push_back(competitionManager->getActualGate());

        CompetitionManagerWindow * window = new CompetitionManagerWindow(competitionManager, this);
        window->setInrunSnowGenerator(InrunSnowGenerator(inrunSnowGeneratorSettingsEditor->getBase(), inrunSnowGeneratorSettingsEditor->getDeviation()));
        if(window->exec() == QDialog::Accepted)
        {
            this->setModal(true);
        }
        competitionTeams = Team::constructTeamsVectorByJumpersList(competitionJumpers, competitionRulesEditor->getJumpersCountInTeam());
        info.setPlayed(true);

        QDate date = QDate::currentDate();
        QTime time = QTime::currentTime();
        QString dateString = QString::number(date.day()) + "-" + QString::number(date.month()) + "-" + QString::number(date.year()) + " " + QString::number(time.hour()) + "-" + QString::number(time.minute()) + "-" + QString::number(time.second());
        info.saveToFile("results/single-competitions/", info.getHill()->getName() + " HS" + QString::number(info.getHill()->getHSPoint()) + " " + dateString +".json");
        if(qualificationsManager != nullptr){
            qualsInfo.setPlayed(true);
            qualsInfo.saveToFile(QString("results/single-competitions/"), info.getHill()->getName() + " HS" + QString::number(info.getHill()->getHSPoint()) + " " + dateString + " (Q).json");
        }

        delete info.getHill();
        delete competitionManager;
        if(qualificationsManager != nullptr)
            delete qualificationsManager;
    }
        break;
    }
}

void CompetitionConfigWindow::on_pushButton_loadJumpers_clicked()
{
    if(getType() == SingleCompetition){
        QUrl fileUrl = QFileDialog::getOpenFileUrl(this, tr("Wybierz plik z zawodnikami"), QUrl(), "JSON (*.json)");

        QFile file(fileUrl.path());
        if(!(file.open(QIODevice::ReadOnly | QIODevice::Text)))
        {
            QMessageBox message(QMessageBox::Icon::Critical, tr("Nie można otworzyć pliku z zawodnikami"), tr("Nie udało się otworzyć wybranego pliku\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia"),  QMessageBox::StandardButton::Ok);
            message.setModal(true);
            message.exec();
        }
        QVector<Jumper> jumpers = Jumper::getVectorFromJson(file.readAll());
        file.close();
        if(jumpers.isEmpty() == false){
            competitionJumpers.clear();
            for(auto & j : jumpers)
                competitionJumpers.push_back(new Jumper(j));
            jumpersListView->setSeasonJumpers(&competitionJumpers);
            delete jumpersListView->getListModel();
            jumpersListView->setupListModel();

            competitionTeams = Team::constructTeamsVectorByJumpersList(competitionJumpers, competitionRulesEditor->getJumpersCountInTeam());
            delete teamsSquadsModel;
            teamsSquadsModel = new TeamsSquadsTreeModel(&competitionTeams, competitionRulesEditor->getJumpersCountInTeam());
            teamsSquadsModel->setupTreeItems();
            teamsTreeView->setModel(teamsSquadsModel);
            teamsTreeView->getTreeView()->expandToDepth(0);
        }
    }
}

void CompetitionConfigWindow::on_comboBox_competition_currentIndexChanged(int index)
{
    if(index > 0)
    {
        index--; //Nie liczymy pierwszego item'a - "Według konkursu"
        int type = seasonCompetition->getRulesPointer()->getCompetitionType();
        QVector<CompetitionInfo *> competitions = CompetitionInfo::getSpecificTypeCompetitions(simulationSave->getActualSeason()->getCalendarReference().getCompetitionsReference(), type);
        CompetitionInfo * competition = SeasonCalendar::getMainCompetitionByIndex(competitions, index);
        if(type == CompetitionRules::Individual){
            IndividualCompetitionManager::setStartListOrderByCompetitionResults(seasonCompetitionJumpers, competition);
            emit jumpersListView->getListModel()->dataChanged(jumpersListView->getListModel()->index(0, 0), jumpersListView->getListModel()->index(jumpersListView->getListModel()->rowCount() - 1));
        }
        else{
            TeamCompetitionManager::setStartListOrderByCompetitionResults(competitionTeams, competition);
            delete teamsSquadsModel;
            teamsSquadsModel = new TeamsSquadsTreeModel(&competitionTeams, seasonCompetition->getRulesPointer()->getJumpersInTeamCount());
            teamsSquadsModel->setupTreeItems();
            teamsTreeView->setModel(teamsSquadsModel);
            teamsTreeView->getTreeView()->expandToDepth(0);
        }
    }
}

void CompetitionConfigWindow::on_comboBox_classification_activated(int index)
{
    if(index > 0)
    {
        index--; //Nie liczymy pierwszego item'a - "Według klasyfikacji"
        int type = seasonCompetition->getRulesPointer()->getCompetitionType();
        Classification * classification = Classification::getSpecificTypeClassifications(simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference(), type).at(index);
        if(type == CompetitionRules::Individual)
        {
            IndividualCompetitionManager::setStartListOrderByClassification(seasonCompetitionJumpers, classification);
            emit jumpersListView->getListModel()->dataChanged(jumpersListView->getListModel()->index(0, 0), jumpersListView->getListModel()->index(jumpersListView->getListModel()->rowCount() - 1));
        }
        else{
            TeamCompetitionManager::setStartListOrderByClassification(competitionTeams, classification);
            delete teamsSquadsModel;
            teamsSquadsModel = new TeamsSquadsTreeModel(&competitionTeams, seasonCompetition->getRulesPointer()->getJumpersInTeamCount());
            teamsSquadsModel->setupTreeItems();
            teamsTreeView->setModel(teamsSquadsModel);
            teamsTreeView->getTreeView()->expandToDepth(0);
        }
    }
}


void CompetitionConfigWindow::on_pushButton_defaultStartListOrder_clicked()
{
    int type = seasonCompetition->getRulesPointer()->getCompetitionType();
    if(type == CompetitionRules::Individual)
    {
        IndividualCompetitionManager::setStartListOrderByDefault(&simulationSave->getJumpersReference(), seasonCompetitionJumpers);
        emit jumpersListView->getListModel()->dataChanged(jumpersListView->getListModel()->index(0, 0), jumpersListView->getListModel()->index(jumpersListView->getListModel()->rowCount() - 1));
    }
}

