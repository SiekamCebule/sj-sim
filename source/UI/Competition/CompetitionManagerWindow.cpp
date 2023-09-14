#include "CompetitionManagerWindow.h"
#include "ui_CompetitionManagerWindow.h"
#include "../../competitions/CompetitionManagers/IndividualCompetitionManager.h"
#include "../../competitions/CompetitionManagers/TeamCompetitionManager.h"
#include "../../global/CountryFlagsManager.h"
#include "../../utilities/functions.h"
#include "../EditorWidgets/WindsGeneratorSettingsEditorWidget.h"
#include "../EditorWidgets/InrunSnowGeneratorSettingsEditorWidget.h"
#include "../EditorWidgets/KOSystem/KOGroupsListView.h"
#include "../JumpManipulation/JumpManipulatorConfigWindow.h"
#include <QInputDialog>
#include <QComboBox>
#include <QMessageBox>
#include <QStringListModel>

CompetitionManagerWindow::CompetitionManagerWindow(AbstractCompetitionManager *manager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompetitionManagerWindow),
    manager(manager),
    KOManager(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    if(manager != nullptr){
        type = manager->getType();
        ui->spinBox_actualGate->setValue(manager->getActualGate());
    }

    ui->pushButton_goToNext->hide();

    startListModel = new StartListModel(this->manager, this);
    startListModel->setType(getType());
    startListModel->setStartListStatuses(&this->manager->getStartListStatusesReference());
    if(dynamic_cast<IndividualCompetitionManager *>(manager) != nullptr){
        if(dynamic_cast<IndividualCompetitionManager *>(manager)->getRoundsKOGroupsReference()[0]->count() > 0){
            startListModel->setKOGroups(dynamic_cast<IndividualCompetitionManager *>(manager)->getRoundsKOGroupsReference()[manager->getActualRound() - 1]);

            KOManager = new KORoundManager(startListModel->getKOGroups(), manager->getResults(), &manager->getCompetitionRules()->getRoundsReference()[manager->getActualRound() - 1], dynamic_cast<IndividualCompetitionManager *>(manager));
            KOManager->updateJumpersSortedByResults();
            if(manager->getCompetitionRules()->getRoundsReference().count() > manager->getActualRound())
                KOManager->setLuckyLosersCount(manager->getCompetitionRules()->getRoundsReference()[manager->getActualRound()].getCount() - (KOManager->getGroups()->count() * KOManager->getRoundInfo()->getAdvancingFromKOGroup()));
            else
                KOManager->setLuckyLosersCount(0);
            dynamic_cast<IndividualCompetitionManager *>(manager)->setKOManager(KOManager);
            KOManager->updateActualGroup(manager->getActualJumper());
            KOManager->setIndManager(dynamic_cast<IndividualCompetitionManager *>(manager));
            KOGroupsResultsModel = new KOGroupResultsTableModel(KOManager, &dynamic_cast<IndividualCompetitionManager *>(manager)->getRoundsKOGroupsReference()[0]->first(), this);
            ui->tableView_KOGroupResults->setModel(KOGroupsResultsModel);
            ui->tableView_KOGroupResults->resizeColumnsToContents();
        }
        else
        {
            ui->tableView_KOGroupResults->hide();
        }
    }
    else
    {
        ui->tableView_KOGroupResults->hide();
    }


    ui->label_nameAndSurname->setText(this->manager->getActualJumper()->getNameAndSurname());
    ui->label_flag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(this->manager->getActualJumper()->getCountryCode().toLower())).scaled(ui->label_flag->size()));

    connect(this->manager, &AbstractCompetitionManager::actualJumperChanged, this, [this](){
        if(this->manager->getActualStartListIndex() < this->manager->getStartListStatusesReference().count()){
            ui->label_nameAndSurname->setText(this->manager->getActualJumper()->getNameAndSurname());
            ui->label_flag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(this->manager->getActualJumper()->getCountryCode().toLower())).scaled(ui->label_flag->size()));
        }
    });


    ui->listView_startList->setModel(startListModel);

    if(type == CompetitionRules::Team){
        teamResultsTreeModel = new TeamResultsTreeModel(dynamic_cast<TeamCompetitionManager *>(manager));
        teamResultsTreeView = new QTreeView();
        teamResultsTreeView->setModel(teamResultsTreeModel);
        teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->horizontalLayout_5->insertWidget(ui->horizontalLayout_5->count() - 2, teamResultsTreeView); //jako przedostatni

        connect(teamResultsTreeView, &QTreeView::doubleClicked, this, [this](const QModelIndex & index){
            TreeItem * item = static_cast<TreeItem *>(index.internalPointer());
            if(item->getParentItem() == teamResultsTreeModel->getRootItem())
                return;
            int teamIndex = item->getParentItem()->row();
            int jumperIndex = item->row();

            jumperResultsWidget->setJumperResult(&this->manager->getResults()->getResultsReference()[teamIndex].getTeamJumpersResultsReference()[jumperIndex]);
            jumperResultsWidget->setPositionShowing(false);
            jumperResultsWidget->fillWidget();
        });

        ui->horizontalLayout_5->removeWidget(ui->tableView_results);
        delete ui->tableView_results;
    }
    else{
        resultsTableModel = new ResultsTableModel(getType(), manager->getResults(), manager, this);
        resultsTableModel->setStartListStatuses(&manager->getStartListStatusesReference());
        ui->tableView_results->setModel(resultsTableModel);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }


    jumperResultsWidget = new JumperCompetitionResultsWidget(this);
    jumperResultsWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    ui->verticalLayout_jumperResult->addWidget(jumperResultsWidget);

    ui->label_toBeatDistance->setText("0m");
    ui->label_toAdvancementDistance->setText("-");
    windsGenerator.setGenerationSettings(manager->getWindGenerationSettingsReference());
    setActualWinds(windsGenerator.generateWinds());
    updateAvgWindLabel();

    ui->label_pointsBehindLeader->setText("");

    Hill * hill = manager->getCompetitionInfo()->getHill();

    ui->label_hillNameAndHSPoint->setText(hill->getName() + " HS" + QString::number(hill->getHSPoint()));
    ui->label_hillFlag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(hill->getCountryCode().toLower())).scaled(ui->label_hillFlag->size()));

    jumperResultsWidget->hide();

    connect(ui->spinBox_actualGate, &QSpinBox::valueChanged, this, [this, manager](){
        if(manager->getActualStartListIndex() == 0){
            manager->getRoundsStartingGatesReference().pop_back();
            manager->getRoundsStartingGatesReference().push_back(ui->spinBox_actualGate->value());
            manager->setActualGate(ui->spinBox_actualGate->value());
        }
        else{
            manager->setActualGate(ui->spinBox_actualGate->value());
            manager->updateLast10Judges();
            manager->updateToBeatLineDistance();
            updateToAdvanceDistanceLabel();
            updateToBeatDistanceLabel();
        }
    });

    action_cancelCompetition = new QAction(tr("Odwołaj konkurs"), this);
    action_cancelRound = new QAction(tr("Odwołaj aktualną rundę"), this);
    cancelActionsList = {action_cancelCompetition, action_cancelRound};
    cancelActionsMenu = new QMenu(this);
    cancelActionsMenu->setStyleSheet("QMenu{background-color: rgb(230, 230, 230); color: rgb(70, 70, 70);}");
    cancelActionsMenu->addActions(cancelActionsList);
    ui->toolButton_cancelActions->setMenu(cancelActionsMenu);

    action_autoSimulateCompetition = new QAction(tr("Auto-symulacja konkursu"), this);
    action_autoSimulateRound = new QAction(tr("Auto-symulacja serii"), this);
    action_autoSimulateJumps = new QAction(tr("Auto-symulacja skoków"), this);
    autoSimulationActionsList = {action_autoSimulateCompetition, action_autoSimulateRound, action_autoSimulateJumps};
    if(getType() == CompetitionRules::Team){
        action_autoSimulateGroup = new QAction(tr("Auto-symulacja grupy"), this);
        autoSimulationActionsList.insert(autoSimulationActionsList.count() - 2, action_autoSimulateGroup); // jako przedostatnie
    }
    autoSimulationActionsMenu = new QMenu();
    autoSimulationActionsMenu->setStyleSheet("QMenu{background-color: rgb(230, 230, 230); color: rgb(70, 70, 70);}");
    autoSimulationActionsMenu->addActions(autoSimulationActionsList);
    ui->toolButton_autoSimulationActions->setMenu(autoSimulationActionsMenu);

    connect(action_cancelCompetition, &QAction::triggered, this, &CompetitionManagerWindow::cancelCompetition);
    connect(action_cancelRound, &QAction::triggered, this, &CompetitionManagerWindow::cancelActualRound);
    connect(action_autoSimulateCompetition, &QAction::triggered, this, &CompetitionManagerWindow::autoSimulateCompetition);
    connect(action_autoSimulateRound, &QAction::triggered, this, &CompetitionManagerWindow::autoSimulateRound);
    if(getType() == CompetitionRules::Team)
        connect(action_autoSimulateGroup, &QAction::triggered, this, &CompetitionManagerWindow::autoSimulateGroup);
    connect(action_autoSimulateJumps, &QAction::triggered, this, &CompetitionManagerWindow::autoSimulateJumps);
}

CompetitionManagerWindow::~CompetitionManagerWindow()
{
    delete ui;
}

void CompetitionManagerWindow::updateToBeatDistanceLabel()
{
    manager->updateToBeatLineDistance();
    ui->label_toBeatDistance->setText(QString::number(manager->getToBeatLineDistance()) + "m");
}

void CompetitionManagerWindow::updateToAdvanceDistanceLabel()
{
    manager->updateToAdvanceLineDistance();
    if(manager->getToAdvanceLineDistance() == (-1))
        ui->label_toAdvancementDistance->setText("-");
    else ui->label_toAdvancementDistance->setText(QString::number(manager->getToAdvanceLineDistance()) + "m");
}

void CompetitionManagerWindow::updatePointsToTheLeaderLabel()
{
    manager->updateActualCompetitorPointsToTheLeader();
    if(manager->getActualCompetitorPointsToTheLeader() == (-1))
        ui->label_pointsBehindLeader->setText("");
    else ui->label_pointsBehindLeader->setText(" + " + QString::number(manager->getActualCompetitorPointsToTheLeader()) + "pkt");
}

void CompetitionManagerWindow::updateAvgWindLabel()
{
    double wind = WindsCalculator::getAveragedWind(actualWinds, manager->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind();
    ui->label_actualAvgWind->setText(QString::number(wind) + " m/s");
    if(wind > 0)
        ui->label->setStyleSheet("color: rgb(68, 130, 26);");
    else if(wind < 0)
        ui->label->setStyleSheet("color: rgb(145, 35, 28);");
    else ui->label->setStyleSheet("color: rgb(50, 50, 50);");
}

void CompetitionManagerWindow::disableCompetitionManagementButtons()
{
    ui->pushButton_coachGate->setDisabled(true);
    ui->pushButton_generateNewWinds->setDisabled(true);
    ui->pushButton_jump->setDisabled(true);
    ui->pushButton_manipulateJump->setDisabled(true);
    ui->pushButton_windsGeneratorSettings->setDisabled(true);
}

void CompetitionManagerWindow::enableCompetitionManagementButtons()
{
    ui->pushButton_coachGate->setEnabled(true);
    ui->pushButton_generateNewWinds->setEnabled(true);
    ui->pushButton_jump->setEnabled(true);
    ui->pushButton_manipulateJump->setEnabled(true);
    ui->pushButton_windsGeneratorSettings->setEnabled(true);
}

void CompetitionManagerWindow::showMessageBoxForNextGroup()
{
    if(getType() == CompetitionRules::Team){
        QMessageBox * box = new QMessageBox(this);
        box->setStyleSheet("QMessageBox{color: black; background-color: white;}");
        box->setIcon(QMessageBox::Information);
        box->setWindowTitle(tr("Zakończenie ") + QString::number(dynamic_cast<TeamCompetitionManager *>(manager)->getActualGroup()) + tr(" grupy zawodników"));
        box->setText(tr("Aby przejść do następnej grupy zawodników, wciśnij przycisk na dole okna konkursu"));
        QPushButton *btnOk = box->addButton("OK", QMessageBox::AcceptRole);
        box->setModal(true);
        box->show();
    }
}

void CompetitionManagerWindow::setupGoToNextButtonForNextGroup()
{
    if(getType() == CompetitionRules::Team){
        TeamCompetitionManager * tmManager = dynamic_cast<TeamCompetitionManager *>(manager);
        ui->pushButton_goToNext->show();
        ui->pushButton_goToNext->setText(tr("Przejdź do ") + QString::number(tmManager->getActualGroup() + 1) + tr(" grupy"));
        disableCompetitionManagementButtons();

        QMetaObject::Connection * const connection = new QMetaObject::Connection;
        *connection = connect(ui->pushButton_goToNext, &QPushButton::clicked, this, [this, connection, tmManager](){
            tmManager->setupNextGroup();
            currentInputJumpManipulator = JumpManipulator();
            enableCompetitionManagementButtons();
            ui->pushButton_goToNext->hide();

            QObject::disconnect(*connection);
            delete connection;
        });
    }
}

void CompetitionManagerWindow::showMessageBoxForNextRound()
{
    QMessageBox * box = new QMessageBox(this);
    box->setStyleSheet("QMessageBox{color: black; background-color: white;}");
    box->setIcon(QMessageBox::Information);
    box->setWindowTitle(tr("Zakończenie ") + QString::number(manager->getActualRound()) + tr(" serii"));
    box->setText(tr("Aby przejść do następnej serii, wciśnij przycisk na dole okna konkursu"));
    QPushButton *btnOk = box->addButton("OK", QMessageBox::AcceptRole);
    box->setModal(true);
    box->show();
}

JumpManipulator CompetitionManagerWindow::getCurrentInputJumpManipulator() const
{
    return currentInputJumpManipulator;
}

void CompetitionManagerWindow::setCurrentInputJumpManipulator(const JumpManipulator &newCurrentInputJumpManipulator)
{
    currentInputJumpManipulator = newCurrentInputJumpManipulator;
}

short CompetitionManagerWindow::getType() const
{
    return type;
}

void CompetitionManagerWindow::setType(short newType)
{
    type = newType;
}

AbstractCompetitionManager *CompetitionManagerWindow::getManager() const
{
    return manager;
}

QVector<KOGroup> CompetitionManagerWindow::getManualKOGroupsFromDialogInputs()
{
    IndividualCompetitionManager * indManager = dynamic_cast<IndividualCompetitionManager *>(manager);
    QDialog * dialog = new QDialog(this);
    QHBoxLayout * mainLayout = new QHBoxLayout(this);
    dialog->setLayout(mainLayout);
    dialog->setFixedSize(600, 820);

    QVBoxLayout * groupsListLayout = new QVBoxLayout(this);
    QComboBox * selectionComboBox = new QComboBox(this);
    selectionComboBox->setModel(new QStringListModel({tr("Dobór grup"), tr("Klasyczne"), tr("Dla dużych grup"), tr("Losowe (Z podziałem na koszyki)"), tr("Losowe")}, this));

    KOGroupsListView groupsListView = KOGroupsListView();
    QVector<Jumper *> jumpers = indManager->getFilteredJumpersForNextRound();
    QVector<KOGroup> groups = KOGroup::constructKOGroups(&manager->getCompetitionRules()->getRoundsReference()[manager->getActualRound()], &jumpers, KOGroup::Random, manager->getCompetitionInfo());
    groupsListView.setKOGroups(&groups);
    groupsListView.setJumpersList(&indManager->getActualRoundJumpersReference());
    groupsListView.fillListLayout();
    groupsListLayout->addWidget(selectionComboBox);
    groupsListLayout->addWidget(&groupsListView);

    connect(selectionComboBox, &QComboBox::activated, this, [this, & groupsListView, & jumpers, & groups](int index){
        if(index != KOGroup::Classic || (index == KOGroup::Classic || (manager->getCompetitionRules()->getRoundsReference()[manager->getActualRound()].getCountInKOGroup() == 2)))
        {
            if(index > 0){
                index--;
                RoundInfo * roundInfo = &manager->getCompetitionRules()->getRoundsReference()[manager->getActualRound()];
                groups = KOGroup::constructKOGroups(roundInfo, &jumpers, index, manager->getCompetitionInfo());
                groupsListView.setKOGroups(&groups);
                groupsListView.fillListLayout();
            }
        }
    });

    QVBoxLayout * submitButtonLayout = new QVBoxLayout(this);
    QPushButton * submitButton = new QPushButton("OK", this);
    submitButton->setFixedSize(111, 111);
    submitButton->setStyleSheet("QPushButton{\nborder: 1px solid rgb(0, 59, 23);\nborder-radius: 6px;\ncolor: rgb(0, 0, 0);\nbackground-color: rgb(123, 220, 144);\n}\nQPushButton:hover{\nbackground-color: rgb(153, 253, 174);\n}");
    submitButton->setFont(QFont("Ubuntu", 18));
    connect(submitButton, &QPushButton::clicked, dialog, &QDialog::accept);
    submitButtonLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    submitButtonLayout->addWidget(submitButton);
    submitButtonLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    mainLayout->addLayout(groupsListLayout);
    mainLayout->addSpacerItem(new QSpacerItem(50, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));
    mainLayout->addLayout(submitButtonLayout);

    if(dialog->exec() == QDialog::Accepted)
    {

    }
    return groups;
}

void CompetitionManagerWindow::setupGoToNextButtonForNextRound()
{
    ui->pushButton_goToNext->show();
    ui->pushButton_goToNext->setText(tr("Przejdź do ") + QString::number(manager->getActualRound() + 1) + tr(" serii"));
    disableCompetitionManagementButtons();

    QMetaObject::Connection * const connection = new QMetaObject::Connection;
    *connection = connect(ui->pushButton_goToNext, &QPushButton::clicked, this, [this, connection](){
        //Tu będzie ten dialog z edycją grup
        int condition1 = manager->getCompetitionRules()->getRoundsReference().count() > manager->getActualRound();
        int condition2 = false;
        if(condition1)
            condition2 = manager->getCompetitionRules()->getRoundsReference()[manager->getActualRound()].getKO() == true && manager->getCompetitionRules()->getRoundsReference()[manager->getActualRound()].getKoGroupsSelectionType() == CompetitionRules::Manual;

        if(condition1 && condition2)
            dynamic_cast<IndividualCompetitionManager *>(manager)->setupNextRound(getManualKOGroupsFromDialogInputs());
        else if(dynamic_cast<IndividualCompetitionManager *>(manager) != nullptr)
            dynamic_cast<IndividualCompetitionManager *>(manager)->setupNextRound();
        else
            manager->setupNextRound();

        IndividualCompetitionManager * indManager = dynamic_cast<IndividualCompetitionManager *>(manager);
        if(indManager != nullptr)
        {
            if(KOManager != nullptr)
            {
                delete KOManager;
                KOManager = nullptr;
                indManager->setKOManager(nullptr);
                delete KOGroupsResultsModel;
                ui->tableView_KOGroupResults->hide();
                startListModel->setKOGroups(nullptr);
            }
            if(indManager->getCompetitionRules()->getRoundsReference()[manager->getActualRound() - 1].getKO() == true)
            {
                KOManager = new KORoundManager(indManager->getRoundsKOGroupsReference()[manager->getActualRound() - 1], manager->getResults(), &indManager->getCompetitionRules()->getRoundsReference()[manager->getActualRound() - 1], dynamic_cast<IndividualCompetitionManager *>(manager));
                KOManager->updateJumpersSortedByResults();
                KOManager->setLuckyLosersCount(manager->getCompetitionRules()->getRoundsReference()[manager->getActualRound()].getCount() - (KOManager->getGroups()->count() * KOManager->getRoundInfo()->getAdvancingFromKOGroup()));
                KOManager->updateActualGroup(manager->getActualJumper());
                KOManager->setIndManager(dynamic_cast<IndividualCompetitionManager *>(manager));
                indManager->setKOManager(KOManager);
                KOGroupsResultsModel = new KOGroupResultsTableModel(KOManager, &dynamic_cast<IndividualCompetitionManager *>(manager)->getRoundsKOGroupsReference()[0]->first(), this);
                ui->tableView_KOGroupResults->setModel(KOGroupsResultsModel);
                ui->tableView_KOGroupResults->resizeColumnsToContents();
                ui->tableView_KOGroupResults->show();
                startListModel->setKOGroups(KOManager->getGroups());
            }
        }

        if(getType() == CompetitionRules::Team)
            teamResultsTreeModel->setTeams(&dynamic_cast<TeamCompetitionManager *>(manager)->getRoundsTeamsReference()[0]);
        currentInputJumpManipulator = JumpManipulator();
        enableCompetitionManagementButtons();
        ui->pushButton_goToNext->hide();

        QObject::disconnect(*connection);
        delete connection;
    });
}

void CompetitionManagerWindow::showMessageBoxForQualificationsEnd()
{
    QMessageBox * box = new QMessageBox(this);
    box->setStyleSheet("QMessageBox{color: black; background-color: white;}");
    box->setIcon(QMessageBox::Information);
    box->setWindowTitle(tr("Zakończenie kwalifikacji"));
    box->setText(tr("Aby przejść do konkursu, wciśnij przycisk na dole okna konkursu"));
    QPushButton *btnOk = box->addButton("OK", QMessageBox::AcceptRole);
    box->setModal(true);
    box->show();
    accept();
}

void CompetitionManagerWindow::setupGoToNextButtonForQualificationsEnd()
{
    ui->pushButton_goToNext->show();
    ui->pushButton_goToNext->setText(tr("Przejdź do konkursu"));
    disableCompetitionManagementButtons();

    switch(getType())
    {
    case CompetitionRules::Individual:{
        QMetaObject::Connection * const connection = new QMetaObject::Connection;
        *connection = connect(ui->pushButton_goToNext, &QPushButton::clicked, [this, connection](){
            accept();

            QObject::disconnect(*connection);
            delete connection;
        });
    }
    }
}

void CompetitionManagerWindow::showMessageBoxForCompetitionEnd()
{
    QMessageBox * box = new QMessageBox(this);
    box->setStyleSheet("QMessageBox{color: black; background-color: white;}");
    box->setIcon(QMessageBox::Information);
    switch(manager->getCompetitionInfo()->getSerieType()){
    case CompetitionInfo::Competition:{
        box->setWindowTitle(tr("Zakończenie konkursu"));
        box->setText(tr("Aby zakończyć konkurs, wciśnij przycisk na dole okna konkursu"));
        break;
    }
    case CompetitionInfo::Qualifications:{
        box->setWindowTitle(tr("Zakończenie kwalifikacji"));
        box->setText(tr("Aby zakończyć kwalifikacje, wciśnij przycisk na dole okna konkursu"));
        break;
    }
    case CompetitionInfo::TrialRound:{
        box->setWindowTitle(tr("Zakończenie serii próbnej"));
        box->setText(tr("Aby zakończyć serię próbną, wciśnij przycisk na dole okna konkursu"));
        break;
    }
    case CompetitionInfo::Training:{
        box->setWindowTitle(tr("Zakończenie treningu"));
        box->setText(tr("Aby zakończyć trening, wciśnij przycisk na dole okna konkursu"));
        break;
    }
    }
    QPushButton *btnOk = box->addButton("OK", QMessageBox::AcceptRole);
    box->setModal(true);
    box->show();
}

void CompetitionManagerWindow::setupGoToNextButtonForCompetitionEnd()
{
    ui->pushButton_goToNext->show();
    switch(manager->getCompetitionInfo()->getSerieType()){
    case CompetitionInfo::Competition:
        ui->pushButton_goToNext->setText(tr("Zakończ konkurs"));
        break;
    case CompetitionInfo::Qualifications:
        ui->pushButton_goToNext->setText(tr("Zakończ kwalifikacje"));
        break;
    case CompetitionInfo::TrialRound:
        ui->pushButton_goToNext->setText(tr("Zakończ serię próbną"));
        break;
    case CompetitionInfo::Training:
        ui->pushButton_goToNext->setText(tr("Zakończ trening"));
        break;
    }

    disableCompetitionManagementButtons();

    QMetaObject::Connection * const connection = new QMetaObject::Connection;
    *connection = connect(ui->pushButton_goToNext, &QPushButton::clicked, [this, connection](){
        accept();

        QObject::disconnect(*connection);
        delete connection;
    });
}

void CompetitionManagerWindow::autoSimulateRound()
{
    IndividualCompetitionManager * indManager = dynamic_cast<IndividualCompetitionManager *>(manager);
    TeamCompetitionManager * tmManager = dynamic_cast<TeamCompetitionManager *>(manager);
    while(manager->checkRoundEnd() != true && manager->checkCompetitionEnd() != true){
        setupSimulator();
        simulator.simulateJump();
        JumpData jump = simulator.getJumpData();
        jump.setCompetition(manager->getCompetitionInfo());
        if(type == CompetitionRules::Individual){
            indManager->getResults()->addJump(indManager->getActualJumper(), jump);
        }
        else if(type == CompetitionRules::Team){
            tmManager->getResults()->addJump(tmManager->getActualTeam(), jump);
            tmManager->getResults()->getResultOfTeam(tmManager->getActualTeam())->updateTeamJumpersResults();
        }

        int status = StartListCompetitorStatus::Finished;
        if(jump.getDSQ())
            status = StartListCompetitorStatus::Dsq;
        else if(jump.getDNS())
            status = StartListCompetitorStatus::Dns;
        StartListCompetitorStatus::getStatusOfJumper(manager->getActualJumper(), manager->getStartListStatusesReference())->setJumpStatus(status);

        bool ok = false;
        if(jump.getLanding().getType() == Landing::Fall && jump.getDistance() >= 0.95 * jump.getHill()->getHSPoint() && manager->getCompetitionRules()->getHas95HSRule()){
            ok = true;
        }
        StartListCompetitorStatus::getStatusOfJumper(manager->getActualJumper(), manager->getStartListStatusesReference())->setQualifiedBy95HSRule(ok);
        manager->updateLastQualifiedResult();
        manager->updateLeaderResult();
        manager->updateLast10Judges();
        manager->updateToAdvanceLineDistance();
        manager->updateToBeatLineDistance();
        if(KOManager != nullptr)
        {
            KOManager->updateActualGroup(manager->getActualJumper());
            KOManager->updateJumpersSortedByResults();
            KOManager->updateStatuses();
        }
        manager->updateCompetitorsAdvanceStatuses();
        if(getType() == CompetitionRules::Team)
            teamResultsTreeModel->setTeams(&dynamic_cast<TeamCompetitionManager *>(manager)->getRoundsTeamsReference()[0]);

        setActualWinds(windsGenerator.generateWinds());
        if(manager->isAllJumpsAreFinished() == false)
            manager->setActualJumperToNextUnfinished();
        if(type == CompetitionRules::Team && (tmManager->getActualGroup() != tmManager->getCompetitionRules()->getJumpersInTeamCount())){
            if(tmManager->checkGroupEnd()){
                tmManager->setupNextGroup();
            }
        }

    }
    jumperResultsWidget->show();

    emit startListModel->dataChanged(startListModel->index(0), startListModel->index(startListModel->rowCount() - 1));
    ui->listView_startList->scrollTo(startListModel->index(manager->getActualStartListIndex() - 1));
    if(getType() == CompetitionRules::Individual){
        ui->tableView_results->setModel(nullptr);
        ui->tableView_results->setModel(resultsTableModel);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        if(KOManager != nullptr)
        {
            KOManager->updateActualGroup(manager->getActualJumper());
            KOManager->updateJumpersSortedByResults();
            KOManager->updateStatuses();
            KOGroupsResultsModel->setGroup(KOManager->getActualGroup());
            manager->updateLastQualifiedResult();
            updateToAdvanceDistanceLabel();
            ui->tableView_KOGroupResults->setModel(nullptr);
            ui->tableView_KOGroupResults->setModel(KOGroupsResultsModel);
            ui->tableView_KOGroupResults->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->tableView_KOGroupResults->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        }
    }
    else if(getType() == CompetitionRules::Team){
        teamResultsTreeModel->setupTreeItems();
        teamResultsTreeView->setModel(nullptr);
        teamResultsTreeView->setModel(teamResultsTreeModel);
        teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::Stretch);
        teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        teamResultsTreeView->expandToDepth(0);
    }

    manager->updateLast10Judges();
    updateToBeatDistanceLabel();
    updateToAdvanceDistanceLabel();
    updatePointsToTheLeaderLabel();
    ui->label_actualAvgWind->setText(QString::number(WindsCalculator::getAveragedWind(actualWinds, manager->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()) + " m/s");

    manager->updateCompetitorsAdvanceStatuses();

    if(manager->checkCompetitionEnd() == true){
        setupGoToNextButtonForCompetitionEnd();
        showMessageBoxForCompetitionEnd();
    }
    else if(manager->checkRoundEnd() == true){
        setupGoToNextButtonForNextRound();
        showMessageBoxForNextRound();
    }
}

void CompetitionManagerWindow::autoSimulateCompetition()
{
    IndividualCompetitionManager * indManager = dynamic_cast<IndividualCompetitionManager *>(manager);
    TeamCompetitionManager * tmManager = dynamic_cast<TeamCompetitionManager *>(manager);
    while(manager->checkCompetitionEnd() != true){
        setupSimulator();
        simulator.simulateJump();
        JumpData jump = simulator.getJumpData();
        jump.setCompetition(manager->getCompetitionInfo());
        if(type == CompetitionRules::Individual){
            indManager->getResults()->addJump(indManager->getActualJumper(), jump);
        }
        else if(type == CompetitionRules::Team){
            tmManager->getResults()->addJump(tmManager->getActualTeam(), jump);
            tmManager->getResults()->getResultOfTeam(tmManager->getActualTeam())->updateTeamJumpersResults();
        }

        int status = StartListCompetitorStatus::Finished;
        if(jump.getDSQ())
            status = StartListCompetitorStatus::Dsq;
        else if(jump.getDNS())
            status = StartListCompetitorStatus::Dns;
        StartListCompetitorStatus::getStatusOfJumper(manager->getActualJumper(), manager->getStartListStatusesReference())->setJumpStatus(status);

        bool ok = false;
        if(jump.getLanding().getType() == Landing::Fall && jump.getDistance() >= 0.95 * jump.getHill()->getHSPoint() && manager->getCompetitionRules()->getHas95HSRule()){
            ok = true;
        }
        StartListCompetitorStatus::getStatusOfJumper(manager->getActualJumper(), manager->getStartListStatusesReference())->setQualifiedBy95HSRule(ok);
        manager->updateLastQualifiedResult();
        manager->updateLeaderResult();
        manager->updateLast10Judges();
        manager->updateToAdvanceLineDistance();
        manager->updateToBeatLineDistance();
        if(KOManager != nullptr)
        {
            KOManager->updateActualGroup(manager->getActualJumper());
            KOManager->updateJumpersSortedByResults();
            KOManager->updateStatuses();
        }
        manager->updateCompetitorsAdvanceStatuses();

        setActualWinds(windsGenerator.generateWinds());

        if(manager->isAllJumpsAreFinished() == false)
            manager->setActualJumperToNextUnfinished();
        if(manager->checkRoundEnd() && (manager->getActualRound() != manager->getCompetitionRules()->getRounds().count() && manager->getActualRound() != manager->getCompetitionInfo()->getExceptionalRoundsCount())){
            if(type == CompetitionRules::Individual)
            {
                indManager->setupNextRound();
                IndividualCompetitionManager * indManager = dynamic_cast<IndividualCompetitionManager *>(manager);
                if(indManager != nullptr)
                {
                    if(KOManager != nullptr)
                    {
                        delete KOManager;
                        KOManager = nullptr;
                        indManager->setKOManager(nullptr);
                        delete KOGroupsResultsModel;
                        ui->tableView_KOGroupResults->hide();
                        startListModel->setKOGroups(nullptr);
                    }
                    if(indManager->getCompetitionRules()->getRoundsReference()[manager->getActualRound() - 1].getKO() == true)
                    {
                        KOManager = new KORoundManager(indManager->getRoundsKOGroupsReference()[manager->getActualRound() - 1], manager->getResults(), &indManager->getCompetitionRules()->getRoundsReference()[manager->getActualRound() - 1], dynamic_cast<IndividualCompetitionManager *>(manager));
                        KOManager->updateJumpersSortedByResults();
                        KOManager->setLuckyLosersCount(manager->getCompetitionRules()->getRoundsReference()[manager->getActualRound()].getCount() - (KOManager->getGroups()->count() * KOManager->getRoundInfo()->getAdvancingFromKOGroup()));
                        KOManager->updateActualGroup(manager->getActualJumper());
                        KOManager->setIndManager(dynamic_cast<IndividualCompetitionManager *>(manager));
                        indManager->setKOManager(KOManager);
                        KOGroupsResultsModel = new KOGroupResultsTableModel(KOManager, &dynamic_cast<IndividualCompetitionManager *>(manager)->getRoundsKOGroupsReference()[0]->first(), this);
                        ui->tableView_KOGroupResults->setModel(KOGroupsResultsModel);
                        ui->tableView_KOGroupResults->resizeColumnsToContents();
                        ui->tableView_KOGroupResults->show();
                        startListModel->setKOGroups(KOManager->getGroups());
                    }
                }
            }
            else if(type == CompetitionRules::Team){
                tmManager->setupNextRound();
                teamResultsTreeModel->setTeams(&dynamic_cast<TeamCompetitionManager *>(manager)->getRoundsTeamsReference()[0]);
            }
        }
        else if(type == CompetitionRules::Team && (tmManager->getActualGroup() != tmManager->getCompetitionRules()->getJumpersInTeamCount())){
            if(tmManager->checkGroupEnd()){
                tmManager->setupNextGroup();
            }
        }
    }
    jumperResultsWidget->show();

    emit startListModel->dataChanged(startListModel->index(0), startListModel->index(startListModel->rowCount() - 1));
    ui->listView_startList->scrollTo(startListModel->index(manager->getActualStartListIndex() - 1));
    if(getType() == CompetitionRules::Individual){
        ui->tableView_results->setModel(nullptr);
        ui->tableView_results->setModel(resultsTableModel);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        if(KOManager != nullptr)
        {
            KOManager->updateActualGroup(manager->getActualJumper());
            KOManager->updateJumpersSortedByResults();
            KOManager->updateStatuses();
            KOGroupsResultsModel->setGroup(KOManager->getActualGroup());
            manager->updateLastQualifiedResult();
            updateToAdvanceDistanceLabel();
            ui->tableView_KOGroupResults->setModel(nullptr);
            ui->tableView_KOGroupResults->setModel(KOGroupsResultsModel);
            ui->tableView_KOGroupResults->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->tableView_KOGroupResults->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        }
    }
    else if(getType() == CompetitionRules::Team){
        teamResultsTreeModel->setupTreeItems();
        teamResultsTreeView->setModel(nullptr);
        teamResultsTreeView->setModel(teamResultsTreeModel);
        teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::Stretch);
        teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        teamResultsTreeView->expandToDepth(0);
    }//teams sie psuje kiedy jest ostatnia seria

    manager->updateLast10Judges();
    updateToBeatDistanceLabel();
    updateToAdvanceDistanceLabel();
    updatePointsToTheLeaderLabel();
    ui->label_actualAvgWind->setText(QString::number(WindsCalculator::getAveragedWind(actualWinds, manager->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()) + " m/s");

    manager->updateCompetitorsAdvanceStatuses();

    if(manager->checkCompetitionEnd() == true){
        setupGoToNextButtonForCompetitionEnd();
        showMessageBoxForCompetitionEnd();
    }
}

void CompetitionManagerWindow::autoSimulateGroup()
{
    if(getType() == CompetitionRules::Team){
        TeamCompetitionManager * tmManager = dynamic_cast<TeamCompetitionManager *>(manager);
        while(tmManager->checkGroupEnd() != true){
            setupSimulator();
            simulator.simulateJump();
            JumpData jump = simulator.getJumpData();
            jump.setCompetition(manager->getCompetitionInfo());
            tmManager->getResults()->addJump(tmManager->getActualTeam(), jump);
            tmManager->getResults()->getResultOfTeam(tmManager->getActualTeam())->updateTeamJumpersResults();

            int status = StartListCompetitorStatus::Finished;
            if(jump.getDSQ())
                status = StartListCompetitorStatus::Dsq;
            else if(jump.getDNS())
                status = StartListCompetitorStatus::Dns;
            StartListCompetitorStatus::getStatusOfJumper(manager->getActualJumper(), manager->getStartListStatusesReference())->setJumpStatus(status);

            bool ok = false;
            if(jump.getLanding().getType() == Landing::Fall && jump.getDistance() >= 0.95 * jump.getHill()->getHSPoint() && manager->getCompetitionRules()->getHas95HSRule()){
                ok = true;
            }
            StartListCompetitorStatus::getStatusOfJumper(manager->getActualJumper(), manager->getStartListStatusesReference())->setQualifiedBy95HSRule(ok);
            manager->updateLastQualifiedResult();
            manager->updateLeaderResult();
            manager->updateLast10Judges();
            manager->updateToAdvanceLineDistance();
            manager->updateToBeatLineDistance();
            manager->updateCompetitorsAdvanceStatuses();

            setActualWinds(windsGenerator.generateWinds());
            if(manager->isAllJumpsAreFinished() == false)
                manager->setActualJumperToNextUnfinished();
        }
        jumperResultsWidget->show();

        emit startListModel->dataChanged(startListModel->index(0), startListModel->index(startListModel->rowCount() - 1));
        ui->listView_startList->scrollTo(startListModel->index(manager->getActualStartListIndex() - 1));
        teamResultsTreeModel->setupTreeItems();
        teamResultsTreeView->setModel(nullptr);
        teamResultsTreeView->setModel(teamResultsTreeModel);
        teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::Stretch);
        teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        teamResultsTreeView->expandToDepth(0);

        manager->updateLast10Judges();
        updateToBeatDistanceLabel();
        updateToAdvanceDistanceLabel();
        updatePointsToTheLeaderLabel();
        ui->label_actualAvgWind->setText(QString::number(WindsCalculator::getAveragedWind(actualWinds, manager->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()) + " m/s");

        manager->updateCompetitorsAdvanceStatuses();

        if(manager->checkCompetitionEnd() == true){
            setupGoToNextButtonForCompetitionEnd();
            showMessageBoxForCompetitionEnd();
        }
        else if(manager->checkRoundEnd() == true){
            setupGoToNextButtonForNextRound();
            showMessageBoxForNextRound();
        }
        else if(tmManager->checkGroupEnd() == true){
            setupGoToNextButtonForNextGroup();
            showMessageBoxForNextGroup();
        }
    }
}

void CompetitionManagerWindow::autoSimulateJumps()
{
    bool ok;
    int jumpsCount = QInputDialog::getInt(this, tr("Symulacja kilku skoków"), tr("Ilość skoków"), 5, 0, 2147483647, 1, &ok);
    if(ok == true){
        for(int i = 0; i < jumpsCount; i++){
            IndividualCompetitionManager * indManager = dynamic_cast<IndividualCompetitionManager *>(manager);
            TeamCompetitionManager * tmManager = dynamic_cast<TeamCompetitionManager *>(manager);
            setupSimulator();
            simulator.simulateJump();
            JumpData jump = simulator.getJumpData();
            jump.setCompetition(manager->getCompetitionInfo());
            if(type == CompetitionRules::Individual){
                indManager->getResults()->addJump(indManager->getActualJumper(), jump);
            }
            else if(type == CompetitionRules::Team){
                tmManager->getResults()->addJump(tmManager->getActualTeam(), jump);
                tmManager->getResults()->getResultOfTeam(tmManager->getActualTeam())->updateTeamJumpersResults();
            }

            int status = StartListCompetitorStatus::Finished;
            if(jump.getDSQ())
                status = StartListCompetitorStatus::Dsq;
            else if(jump.getDNS())
                status = StartListCompetitorStatus::Dns;
            StartListCompetitorStatus::getStatusOfJumper(manager->getActualJumper(), manager->getStartListStatusesReference())->setJumpStatus(status);

            bool ok = false;
            if(jump.getLanding().getType() == Landing::Fall && jump.getDistance() >= 0.95 * jump.getHill()->getHSPoint() && manager->getCompetitionRules()->getHas95HSRule()){
                ok = true;
            }
            StartListCompetitorStatus::getStatusOfJumper(manager->getActualJumper(), manager->getStartListStatusesReference())->setQualifiedBy95HSRule(ok);
            manager->updateLastQualifiedResult();
            manager->updateLeaderResult();
            manager->updateLast10Judges();
            manager->updateToAdvanceLineDistance();
            manager->updateToBeatLineDistance();
            if(KOManager != nullptr)
            {
                KOManager->updateActualGroup(manager->getActualJumper());
                KOManager->updateJumpersSortedByResults();
                KOManager->updateStatuses();
            }
            manager->updateCompetitorsAdvanceStatuses();

            setActualWinds(windsGenerator.generateWinds());

            if(manager->isAllJumpsAreFinished() == false)
                manager->setActualJumperToNextUnfinished();

            if(manager->checkCompetitionEnd() == true)
                break;
            if(manager->checkRoundEnd() && (manager->getActualRound() != manager->getCompetitionRules()->getRounds().count() && manager->getActualRound() != manager->getCompetitionInfo()->getExceptionalRoundsCount()) && i+1 < jumpsCount){
                if(type == CompetitionRules::Individual)
                {
                    indManager->setupNextRound();
                    IndividualCompetitionManager * indManager = dynamic_cast<IndividualCompetitionManager *>(manager);
                    if(indManager != nullptr)
                    {
                        if(KOManager != nullptr)
                        {
                            delete KOManager;
                            KOManager = nullptr;
                            indManager->setKOManager(nullptr);
                            delete KOGroupsResultsModel;
                            ui->tableView_KOGroupResults->hide();
                            startListModel->setKOGroups(nullptr);
                        }
                        if(indManager->getCompetitionRules()->getRoundsReference()[manager->getActualRound() - 1].getKO() == true)
                        {
                            KOManager = new KORoundManager(indManager->getRoundsKOGroupsReference()[manager->getActualRound() - 1], manager->getResults(), &indManager->getCompetitionRules()->getRoundsReference()[manager->getActualRound() - 1], dynamic_cast<IndividualCompetitionManager *>(manager));
                            KOManager->updateJumpersSortedByResults();
                            KOManager->setLuckyLosersCount(manager->getCompetitionRules()->getRoundsReference()[manager->getActualRound()].getCount() - (KOManager->getGroups()->count() * KOManager->getRoundInfo()->getAdvancingFromKOGroup()));
                            KOManager->updateActualGroup(manager->getActualJumper());
                            KOManager->setIndManager(dynamic_cast<IndividualCompetitionManager *>(manager));
                            indManager->setKOManager(KOManager);
                            KOGroupsResultsModel = new KOGroupResultsTableModel(KOManager, &dynamic_cast<IndividualCompetitionManager *>(manager)->getRoundsKOGroupsReference()[0]->first(), this);
                            ui->tableView_KOGroupResults->setModel(KOGroupsResultsModel);
                            ui->tableView_KOGroupResults->resizeColumnsToContents();
                            ui->tableView_KOGroupResults->show();
                            startListModel->setKOGroups(KOManager->getGroups());
                        }
                    }
                }
                else if(type == CompetitionRules::Team){
                    tmManager->setupNextRound();
                    teamResultsTreeModel->setTeams(&dynamic_cast<TeamCompetitionManager *>(manager)->getRoundsTeamsReference()[0]);
                }
            }
            else if(type == CompetitionRules::Team && (tmManager->getActualGroup() != tmManager->getCompetitionRules()->getJumpersInTeamCount())){
                if(tmManager->checkGroupEnd()){
                    tmManager->setupNextGroup();
                }
            }
        }
        jumperResultsWidget->show();

        emit startListModel->dataChanged(startListModel->index(0), startListModel->index(startListModel->rowCount() - 1));
        ui->listView_startList->scrollTo(startListModel->index(manager->getActualStartListIndex() - 1));
        if(getType() == CompetitionRules::Individual){
            ui->tableView_results->setModel(nullptr);
            ui->tableView_results->setModel(resultsTableModel);
            ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
            if(KOManager != nullptr)
            {
                KOManager->updateActualGroup(manager->getActualJumper());
                KOManager->updateJumpersSortedByResults();
                KOManager->updateStatuses();
                KOGroupsResultsModel->setGroup(KOManager->getActualGroup());
                manager->updateLastQualifiedResult();
                updateToAdvanceDistanceLabel();
                ui->tableView_KOGroupResults->setModel(nullptr);
                ui->tableView_KOGroupResults->setModel(KOGroupsResultsModel);
                ui->tableView_KOGroupResults->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                ui->tableView_KOGroupResults->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
            }
        }
        else if(getType() == CompetitionRules::Team){
            teamResultsTreeModel->setupTreeItems();
            teamResultsTreeView->setModel(nullptr);
            teamResultsTreeView->setModel(teamResultsTreeModel);
            teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::Stretch);
            teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
            teamResultsTreeView->expandToDepth(0);
        }//teams sie psuje kiedy jest ostatnia seria

        manager->updateLast10Judges();
        updateToBeatDistanceLabel();
        updateToAdvanceDistanceLabel();
        updatePointsToTheLeaderLabel();
        ui->label_actualAvgWind->setText(QString::number(WindsCalculator::getAveragedWind(actualWinds, manager->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()) + " m/s");

        manager->updateCompetitorsAdvanceStatuses();

        if(manager->checkCompetitionEnd() == true){
            setupGoToNextButtonForCompetitionEnd();
            showMessageBoxForCompetitionEnd();
        }
    }
}

KORoundManager *CompetitionManagerWindow::getKOManager() const
{
    return KOManager;
}

void CompetitionManagerWindow::setKOManager(KORoundManager *newKOManager)
{
    KOManager = newKOManager;
}

KOGroupResultsTableModel *CompetitionManagerWindow::getKOGroupsResultsModel() const
{
    return KOGroupsResultsModel;
}

void CompetitionManagerWindow::setKOGroupsResultsModel(KOGroupResultsTableModel *newKOGroupsResultsModel)
{
    KOGroupsResultsModel = newKOGroupsResultsModel;
}

InrunSnowGenerator CompetitionManagerWindow::getInrunSnowGenerator() const
{
    return inrunSnowGenerator;
}

void CompetitionManagerWindow::setInrunSnowGenerator(const InrunSnowGenerator &newInrunSnowGenerator)
{
    inrunSnowGenerator = newInrunSnowGenerator;
}

void CompetitionManagerWindow::cancelCompetition()
{
    manager->getResults()->getResultsReference().clear();
    manager->getCompetitionInfo()->setCancelled(true);

    emit startListModel->dataChanged(startListModel->index(manager->getActualStartListIndex() - 1), startListModel->index(manager->getActualStartListIndex() - 1));
    ui->listView_startList->scrollTo(startListModel->index(manager->getActualStartListIndex()));
    if(getType() == CompetitionRules::Individual){
        ui->tableView_results->setModel(nullptr);
        ui->tableView_results->setModel(resultsTableModel);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        if(KOManager != nullptr)
        {
            KOManager->updateActualGroup(manager->getActualJumper());
            KOManager->updateJumpersSortedByResults();
            KOManager->updateStatuses();
            KOGroupsResultsModel->setGroup(KOManager->getActualGroup());
            manager->updateLastQualifiedResult();
            updateToAdvanceDistanceLabel();
            ui->tableView_KOGroupResults->setModel(nullptr);
            ui->tableView_KOGroupResults->setModel(KOGroupsResultsModel);
            ui->tableView_KOGroupResults->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->tableView_KOGroupResults->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        }
    }
    else if(getType() == CompetitionRules::Team){
        teamResultsTreeView->setModel(nullptr);
        teamResultsTreeView->setModel(teamResultsTreeModel);
        teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::Stretch);
        teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        teamResultsTreeView->expandToDepth(0);
    }

    setupGoToNextButtonForCompetitionEnd();
    showMessageBoxForCompetitionEnd();
}

void CompetitionManagerWindow::cancelActualRound()
{
    for(auto & res : manager->getResults()->getResultsReference()){
        if(res.getJumpsReference().count() == manager->getActualRound()){
            res.getJumpsReference().removeLast();
            res.updatePointsSum();
        }
        manager->getResults()->sortInDescendingOrder();

        emit startListModel->dataChanged(startListModel->index(manager->getActualStartListIndex() - 1), startListModel->index(manager->getActualStartListIndex() - 1));
        if(getType() == CompetitionRules::Individual){
            ui->tableView_results->setModel(nullptr);
            ui->tableView_results->setModel(resultsTableModel);
            ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
            if(KOManager != nullptr)
            {
                KOManager->updateActualGroup(manager->getActualJumper());
                KOManager->updateJumpersSortedByResults();
                KOManager->updateStatuses();
                KOGroupsResultsModel->setGroup(KOManager->getActualGroup());
                manager->updateLastQualifiedResult();
                updateToAdvanceDistanceLabel();
                ui->tableView_KOGroupResults->setModel(nullptr);
                ui->tableView_KOGroupResults->setModel(KOGroupsResultsModel);
                ui->tableView_KOGroupResults->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                ui->tableView_KOGroupResults->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
            }
        }
        else if(getType() == CompetitionRules::Team){
            teamResultsTreeView->setModel(nullptr);
            teamResultsTreeView->setModel(teamResultsTreeModel);
            teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::Stretch);
            teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
            teamResultsTreeView->expandToDepth(0);
        }
    }

    setupGoToNextButtonForCompetitionEnd();
    showMessageBoxForCompetitionEnd();
}
//dlaczego jak dodaje trzecii vector z drużynami, to pierwszy się zmienia?
// W roundsTeams jest ta sama wartość ale we wskaźniku do niego jest 0 elementów, czyli być może istnieje dalej w pamięci i ma 0 elementów, ale w roundsTeams pojawił się nowy który ma te 12 ale inny adres.
// Teorytycznie nowe przypisanie wskaźnika do model.teams powinno zadziałać, ale jakie jest rozwiązanie tego problemu w sposób "czysty?"

void CompetitionManagerWindow::setupSimulator()
{
    simulator.setGate(manager->getActualGate());
    simulator.setHasCoachGate(manager->getCoachGateForNextJumper());
    simulator.setCoachGate(manager->getActualCoachGate());
    simulator.setCompetitionStartGate(manager->getRoundsStartingGatesReference()[manager->getActualRound() - 1]);
    simulator.setJumper(manager->getActualJumper());
    simulator.setHill(manager->getCompetitionInfo()->getHill());
    simulator.setManipulator(&this->currentInputJumpManipulator);
    simulator.setCompetitionRules(manager->getCompetitionRules());
    simulator.setDSQBaseProbability(manager->getBaseDSQProbability());
    simulator.setWinds(actualWinds);
    simulator.setInrunSnow(inrunSnowGenerator.generateInrunSnow());
}

void CompetitionManagerWindow::on_pushButton_jump_clicked()
{
    IndividualCompetitionManager * indManager = dynamic_cast<IndividualCompetitionManager *>(manager);
    TeamCompetitionManager * tmManager = dynamic_cast<TeamCompetitionManager *>(manager);
    setupSimulator();
    simulator.simulateJump();
    JumpData jump = simulator.getJumpData();
    jump.setCompetition(manager->getCompetitionInfo());
    if(type == CompetitionRules::Individual){
        indManager->getResults()->addJump(indManager->getActualJumper(), jump);
    }
    else if(type == CompetitionRules::Team){
        tmManager->getResults()->addJump(tmManager->getActualTeam(), jump);
        tmManager->getResults()->getResultOfTeam(tmManager->getActualTeam())->updateTeamJumpersResults();
    }


    int status = StartListCompetitorStatus::Finished;
    if(jump.getDSQ())
        status = StartListCompetitorStatus::Dsq;
    else if(jump.getDNS())
        status = StartListCompetitorStatus::Dns;

    StartListCompetitorStatus::getStatusOfJumper(manager->getActualJumper(), manager->getStartListStatusesReference())->setJumpStatus(status);

    bool ok = false;
    if(jump.getLanding().getType() == Landing::Fall && jump.getDistance() >= 0.95 * jump.getHill()->getHSPoint() && manager->getCompetitionRules()->getHas95HSRule())
        ok = true;
    StartListCompetitorStatus::getStatusOfJumper(manager->getActualJumper(), manager->getStartListStatusesReference())->setQualifiedBy95HSRule(ok);
    manager->updateLastQualifiedResult();
    manager->updateLeaderResult();
    manager->updateLast10Judges();
    updateToAdvanceDistanceLabel();
    updateToBeatDistanceLabel();
    if(KOManager != nullptr)
    {
        KOManager->updateActualGroup(manager->getActualJumper());
        KOManager->updateJumpersSortedByResults();
        KOManager->updateStatuses();
    }
    manager->updateCompetitorsAdvanceStatuses();

    if(jumperResultsWidget->isHidden()) jumperResultsWidget->show();
    if(type == CompetitionRules::Individual){
        jumperResultsWidget->setJumperResult(manager->getResults()->getResultOfIndividualJumper(manager->getActualJumper()));
    }
    else if(type == CompetitionRules::Team){
        jumperResultsWidget->setJumperResult(manager->getResults()->getResultOfTeam(tmManager->getActualTeam())->getTeamJumperResult(manager->getActualJumper()));
        jumperResultsWidget->setPositionShowing(false);
    }
    jumperResultsWidget->fillWidget();

    emit startListModel->dataChanged(startListModel->index(manager->getActualStartListIndex() - 1), startListModel->index(manager->getActualStartListIndex() - 1));
    ui->listView_startList->scrollTo(startListModel->index(manager->getActualStartListIndex() - 1));
    if(getType() == CompetitionRules::Individual){
        ui->tableView_results->setModel(nullptr);
        ui->tableView_results->setModel(resultsTableModel);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        if(KOManager != nullptr)
        {
            KOManager->updateActualGroup(manager->getActualJumper());
            KOManager->updateJumpersSortedByResults();
            KOManager->updateStatuses();
            KOGroupsResultsModel->setGroup(KOManager->getActualGroup());
            manager->updateLastQualifiedResult();
            updateToAdvanceDistanceLabel();
            ui->tableView_KOGroupResults->setModel(nullptr);
            ui->tableView_KOGroupResults->setModel(KOGroupsResultsModel);
            ui->tableView_KOGroupResults->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->tableView_KOGroupResults->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        }
    }
    else if(getType() == CompetitionRules::Team){
        teamResultsTreeModel->setupTreeItems();
        teamResultsTreeView->setModel(nullptr);
        teamResultsTreeView->setModel(teamResultsTreeModel);
        teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::Stretch);
        teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        teamResultsTreeView->expandToDepth(0);
    }

    manager->setCoachGateForNextJumper(false);
    manager->setActualCoachGate(0);
    ui->spinBox_actualGate->setValue(manager->getActualGate());
    qDebug()<<WindsCalculator::getAveragedWind(actualWinds, manager->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()<< "AVG1";
    if(manager->isAllJumpsAreFinished() == false)
        manager->setActualJumperToNextUnfinished();
    setActualWinds(windsGenerator.generateWinds());
    currentInputJumpManipulator = JumpManipulator();
    qDebug()<<WindsCalculator::getAveragedWind(actualWinds, manager->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()<< "AVG2";

    updateAvgWindLabel();
    manager->updateLastQualifiedResult();
    manager->updateLeaderResult();
    manager->updateLast10Judges();
    updateToBeatDistanceLabel();
    updateToAdvanceDistanceLabel();
    updatePointsToTheLeaderLabel();
    qDebug()<<WindsCalculator::getAveragedWind(actualWinds, manager->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()<< "AVG3";

    if(manager->checkCompetitionEnd()){
        setupGoToNextButtonForCompetitionEnd();
        showMessageBoxForCompetitionEnd();
    }
    else if(manager->checkRoundEnd()){
        setupGoToNextButtonForNextRound();
        showMessageBoxForNextRound();
    }
    else if(getType() == CompetitionRules::Team)
    {
        if(tmManager->checkGroupEnd()){
            setupGoToNextButtonForNextGroup();
            showMessageBoxForNextGroup();
        }
    }
}

void CompetitionManagerWindow::on_tableView_results_doubleClicked(const QModelIndex &index)
{
    if(index.column() == 1){
        jumperResultsWidget->setJumperResult(manager->getResults()->getResultByIndex(index.row()));
        jumperResultsWidget->fillWidget();
    }
}


void CompetitionManagerWindow::on_pushButton_generateNewWinds_clicked()
{
    setActualWinds(windsGenerator.generateWinds());
    if(manager->getActualStartListIndex() > 0){
        manager->updateLeaderResult();
        manager->updateLast10Judges();
        updateToBeatDistanceLabel();
        updateToAdvanceDistanceLabel();
    }
    updateAvgWindLabel();
}


void CompetitionManagerWindow::on_pushButton_windsGeneratorSettings_clicked()
{
    QDialog * dialog = new QDialog;
    dialog->setWindowFlags(Qt::Window);
    dialog->setWindowTitle("Edytuj ustawienia generatora wiatru");
    dialog->setStyleSheet("background-color: rgb(225, 225, 225);");
    dialog->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    dialog->setFixedSize(dialog->size());
    dialog->setLayout(new QVBoxLayout(dialog));

    WindsGeneratorSettingsEditorWidget * editor = new WindsGeneratorSettingsEditorWidget;
    editor->setWindGenerationSettings(&manager->getWindGenerationSettingsReference());
    editor->setRemovingSubmitButtons(true);
    editor->setKPoint(manager->getCompetitionInfo()->getHill()->getKPoint());
    editor->setSettingsCount(WindsGenerator::calculateWindsCountByKPoint(editor->getKPoint()));
    editor->fillSettingsInputs();

    dialog->layout()->addWidget(editor);
    connect(editor, &WindsGeneratorSettingsEditorWidget::submitted, dialog, &QDialog::accept);

    if(dialog->exec() == QDialog::Accepted){
        manager->setWindGenerationSettings(editor->getWindsGenerationSettingsFromInputs());
        windsGenerator.setGenerationSettings(manager->getWindGenerationSettingsReference());
        delete editor;
        delete dialog;
    }

}

void CompetitionManagerWindow::on_pushButton_inrunSnowGeneratorSettings_clicked()
{
    QDialog * dialog = new QDialog;
    dialog->setWindowFlags(Qt::Window);
    dialog->setWindowTitle("Edytuj ustawienia generatora wiatru");
    dialog->setStyleSheet("background-color: rgb(225, 225, 225);");
    dialog->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    dialog->setFixedSize(dialog->size());
    dialog->setLayout(new QVBoxLayout(dialog));

    InrunSnowGeneratorSettingsEditorWidget * editor = new InrunSnowGeneratorSettingsEditorWidget;
    editor->setGenerator(&inrunSnowGenerator);
    editor->fillInputs();

    dialog->layout()->addWidget(editor);
    connect(editor, &InrunSnowGeneratorSettingsEditorWidget::submitted, dialog, &QDialog::accept);

    if(dialog->exec() == QDialog::Accepted){
        inrunSnowGenerator.setBaseInrunSnow(editor->getBase());
        inrunSnowGenerator.setInrunSnowDeviation(editor->getDeviation());

        delete editor;
        delete dialog;
    }
}

void CompetitionManagerWindow::on_pushButton_manipulateJump_clicked()
{
    JumpManipulatorConfigWindow * window = new JumpManipulatorConfigWindow;
    window->setWindGenerationSettings(&manager->getWindGenerationSettingsReference());
    window->setKPoint(manager->getCompetitionInfo()->getHill()->getKPoint());
    connect(window, &JumpManipulatorConfigWindow::submitted, window, &JumpManipulatorConfigWindow::accept);
    if(window->exec() == QDialog::Accepted){
        JumpManipulator manipulator = window->getJumpManipulatorFromInputs();
        currentInputJumpManipulator = (manipulator);
        if(manipulator.getExactWinds().size() > 0)
            setActualWinds(manipulator.getExactWinds());
        updateAvgWindLabel();
    }
}


void CompetitionManagerWindow::on_pushButton_coachGate_clicked()
{
    bool ok{};
    int howMany = QInputDialog::getInt(this, "Obniżenie belki na życzenie trenera", "O ile stopni obniżyć belkę?\nAby uzyskać dodatkową rekompensatę, zawodnik musi osiągnąć 95% punktu HS (" + QString::number(roundDoubleToHalf(manager->getCompetitionInfo()->getHill()->getHSPoint() * 0.95)) + " m)", 1, 0, 1000000, 1, &ok);
    if(howMany > 0 && ok){
        manager->setCoachGateForNextJumper(true);
        manager->setActualCoachGate(manager->getActualGate() - howMany);
        manager->updateLast10Judges();
        updateToAdvanceDistanceLabel();
        updateToBeatDistanceLabel();

        ui->spinBox_actualGate->setValue(manager->getActualCoachGate());
        manager->setActualGate(manager->getActualGate() + howMany);
    }
    else if(howMany == 0 && ok)
    {
        manager->setCoachGateForNextJumper(false);
        manager->updateLast10Judges();
        updateToAdvanceDistanceLabel();
        updateToBeatDistanceLabel();

        ui->spinBox_actualGate->setValue(manager->getActualGate());
    }
}

QVector<Wind> CompetitionManagerWindow::getActualWinds() const
{
    return actualWinds;
}

QVector<Wind> & CompetitionManagerWindow::getActualWindsReference()
{
    return actualWinds;
}

void CompetitionManagerWindow::setActualWinds(const QVector<Wind> &newActualWinds)
{
    actualWinds = newActualWinds;
    manager->setActualWinds(&actualWinds);
}
