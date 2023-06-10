#include "CompetitionManagerWindow.h"
#include "ui_CompetitionManagerWindow.h"
#include "../../competitions/CompetitionManagers/IndividualCompetitionManager.h"
#include "../../competitions/CompetitionManagers/TeamCompetitionManager.h"
#include "../../global/CountryFlagsManager.h"
#include "../EditorWidgets/WindsGeneratorSettingsEditorWidget.h"
#include "../JumpManipulation/JumpManipulatorConfigWindow.h"
#include <QInputDialog>
#include <QMessageBox>

CompetitionManagerWindow::CompetitionManagerWindow(AbstractCompetitionManager *manager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompetitionManagerWindow),
    manager(manager)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    if(manager != nullptr){
        type = manager->getType();
        ui->spinBox_actualGate->setValue(manager->getActualGate());
    }

    ui->pushButton_competitionDetails->hide();
    ui->pushButton_goToNext->hide();

    {
        startListModel = new StartListModel(this->manager, this);
        startListModel->setType(getType());
        startListModel->setStartListStatuses(&this->manager->getStartListStatusesReference());

        ui->label_nameAndSurname->setText(this->manager->getActualJumper()->getNameAndSurname());
        ui->label_flag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(this->manager->getActualJumper()->getCountryCode().toLower())).scaled(ui->label_flag->size()));

        connect(this->manager, &IndividualCompetitionManager::actualStartListIndexChanged, this, [this](){
            ui->label_nameAndSurname->setText(this->manager->getActualJumper()->getNameAndSurname());
            ui->label_flag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(this->manager->getActualJumper()->getCountryCode().toLower())).scaled(ui->label_flag->size()));
        });
    }


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
            qDebug()<<"teamIndex: "<<teamIndex;
            qDebug()<<"jumperIndex: "<<jumperIndex;

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
            manager->updateToBeatLineDistance();
            updateToAdvanceDistanceLabel();
            updateToBeatDistanceLabel();
        }
    });

    toolBar = new QToolBar(tr("Zarządzaj konkursem"), this);
    toolBar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    toolBar->setStyleSheet("QToolBar QToolButton{background-color: rgb(245, 245, 245); border: 2px solid rgb(100, 100, 240); border-radius: 4px; color: rgb(10, 10, 10);} QToolBar:hover QToolButton:hover{background-color: rgb(255, 255, 255);}");
    action_cancelCompetition = new QAction(tr("Odwołaj konkurs"), toolBar);
    action_cancelRound = new QAction(tr("Odwołaj aktualną rundę"), toolBar);
    action_autoSimulateRound = new QAction(tr("Automatycznie przesymuluj serię"), toolBar);
    competitionActions.push_back(action_cancelCompetition);
    competitionActions.push_back(action_cancelRound);
    competitionActions.push_back(action_autoSimulateRound);
    toolBar->addActions(competitionActions);
    layout()->setMenuBar(toolBar);

    connect(action_autoSimulateRound, &QAction::triggered, this, &CompetitionManagerWindow::autoSimulateRound);
    connect(action_cancelCompetition, &QAction::triggered, this, &CompetitionManagerWindow::cancelCompetition);
    connect(action_cancelRound, &QAction::triggered, this, &CompetitionManagerWindow::cancelActualRound);
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
    ui->label_actualAvgWind->setText(QString::number(WindsCalculator::getAveragedWind(actualWinds, manager->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()) + " m/s");
}

void CompetitionManagerWindow::disableCompetitionManagementButtons()
{
    ui->pushButton_coachGate->setDisabled(true);
    ui->pushButton_competitionDetails->setDisabled(true);
    ui->pushButton_generateNewWinds->setDisabled(true);
    ui->pushButton_jump->setDisabled(true);
    ui->pushButton_manipulateJump->setDisabled(true);
    ui->pushButton_windsGeneratorSettings->setDisabled(true);
}

void CompetitionManagerWindow::enableCompetitionManagementButtons()
{
    ui->pushButton_coachGate->setEnabled(true);
    ui->pushButton_competitionDetails->setEnabled(true);
    ui->pushButton_generateNewWinds->setEnabled(true);
    ui->pushButton_jump->setEnabled(true);
    ui->pushButton_manipulateJump->setEnabled(true);
    ui->pushButton_windsGeneratorSettings->setEnabled(true);
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
    qDebug()<<"hhh";
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

void CompetitionManagerWindow::setupGoToNextButtonForNextRound()
{
    qDebug()<<"hoho";
    ui->pushButton_goToNext->show();
    ui->pushButton_goToNext->setText(tr("Przejdź do ") + QString::number(manager->getActualRound() + 1) + tr(" serii"));
    disableCompetitionManagementButtons();

    QMetaObject::Connection * const connection = new QMetaObject::Connection;
    *connection = connect(ui->pushButton_goToNext, &QPushButton::clicked, this, [this, connection](){
        manager->setupNextRound();
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

void CompetitionManagerWindow::autoSimulateRound()
{
    switch(getType())
    {
    case CompetitionRules::Individual:{
        IndividualCompetitionManager * m = dynamic_cast<IndividualCompetitionManager *>(manager);
        while(m->checkRoundEnd() != true && m->checkCompetitionEnd() != true){
            setupSimulator();
            simulator.simulateJump();
            JumpData jump = simulator.getJumpData();
            m->getResults()->addJump(m->getActualJumper(), jump);

            int status = StartListCompetitorStatus::Finished;
            if(jump.getDSQ())
                status = StartListCompetitorStatus::Dsq;
            else if(jump.getDNS())
                status = StartListCompetitorStatus::Dns;
            StartListCompetitorStatus::getStatusOfJumper(m->getActualJumper(), m->getStartListStatusesReference())->setJumpStatus(status);

            bool ok = false;
            if(jump.getLanding().getType() == Landing::Fall && jump.getDistance() >= 0.95 * jump.getHill()->getHSPoint() && m->getCompetitionRules()->getHas95HSRule()){
                ok = true;
            }
            StartListCompetitorStatus::getStatusOfJumper(m->getActualJumper(), m->getStartListStatusesReference())->setQualifiedBy95HSRule(ok);

            m->updateLastQualifiedResult();
            qDebug()<<"lqr: "<<m->getLastQualifiedResult();
            m->updateLeaderResult();
            m->updateCompetitorsAdvanceStatuses();

            setActualWinds(windsGenerator.generateWinds());
            if(m->isAllJumpsAreFinished() == false)
                m->setActualJumperToNextUnfinished();
        }
        jumperResultsWidget->show();

        emit startListModel->dataChanged(startListModel->index(m->getActualStartListIndex() - 1), startListModel->index(m->getActualStartListIndex() - 1));
        ui->listView_startList->scrollTo(startListModel->index(manager->getActualStartListIndex()));

        if(getType() == CompetitionRules::Individual){
            ui->tableView_results->setModel(nullptr);
            ui->tableView_results->setModel(resultsTableModel);
            ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        }
        else if(getType() == CompetitionRules::Team){
            teamResultsTreeView->setModel(nullptr);
            teamResultsTreeView->setModel(teamResultsTreeModel);
            teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::Stretch);
            teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
            teamResultsTreeView->expandToDepth(0);
        }

        updateToBeatDistanceLabel();
        updateToAdvanceDistanceLabel();
        updatePointsToTheLeaderLabel();
        ui->label_actualAvgWind->setText(QString::number(WindsCalculator::getAveragedWind(actualWinds, m->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()) + " m/s");

        m->updateCompetitorsAdvanceStatuses();

        if(m->checkCompetitionEnd() == true){
            setupGoToNextButtonForCompetitionEnd();
            showMessageBoxForCompetitionEnd();
        }
        else if(m->checkRoundEnd() == true){
            setupGoToNextButtonForNextRound();
            showMessageBoxForNextRound();
        }

        break;
    }
    }
}

void CompetitionManagerWindow::cancelCompetition()
{
    switch(getType()){
    case CompetitionRules::Individual:{
        IndividualCompetitionManager * m = dynamic_cast<IndividualCompetitionManager *>(manager);
        m->getCompetitionInfo()->setCancelled(true);

        emit startListModel->dataChanged(startListModel->index(m->getActualStartListIndex() - 1), startListModel->index(m->getActualStartListIndex() - 1));
        ui->listView_startList->scrollTo(startListModel->index(manager->getActualStartListIndex()));
        if(getType() == CompetitionRules::Individual){
            ui->tableView_results->setModel(nullptr);
            ui->tableView_results->setModel(resultsTableModel);
            ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        }
        else if(getType() == CompetitionRules::Team){
            teamResultsTreeView->setModel(nullptr);
            teamResultsTreeView->setModel(teamResultsTreeModel);
            teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::Stretch);
            teamResultsTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
            teamResultsTreeView->expandToDepth(0);
        }
    }
    }

    setupGoToNextButtonForCompetitionEnd();
    showMessageBoxForCompetitionEnd();
}

void CompetitionManagerWindow::cancelActualRound()
{
    switch(getType()){
    for(auto & res : manager->getResults()->getResultsReference()){
        if(res.getJumpsReference().count() == manager->getActualRound()){
            res.getJumpsReference().removeLast();
            res.updatePointsSum();
        }
    }
    manager->getResults()->sortInDescendingOrder();

    emit startListModel->dataChanged(startListModel->index(manager->getActualStartListIndex() - 1), startListModel->index(manager->getActualStartListIndex() - 1));
    if(getType() == CompetitionRules::Individual){
        ui->tableView_results->setModel(nullptr);
        ui->tableView_results->setModel(resultsTableModel);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
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
}

void CompetitionManagerWindow::on_pushButton_jump_clicked()
{
    IndividualCompetitionManager * indManager = dynamic_cast<IndividualCompetitionManager *>(manager);
    TeamCompetitionManager * tmManager = dynamic_cast<TeamCompetitionManager *>(manager);
    setupSimulator();
    simulator.simulateJump();
    JumpData jump = simulator.getJumpData();
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
    manager->updateToAdvanceLineDistance();
    manager->updateToBeatLineDistance();
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
    if(getType() == CompetitionRules::Individual){
        ui->tableView_results->setModel(nullptr);
        ui->tableView_results->setModel(resultsTableModel);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableView_results->scrollTo(resultsTableModel->index(manager->getActualStartListIndex(), 0));
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

    if(manager->isAllJumpsAreFinished() == false)
        manager->setActualJumperToNextUnfinished();
    setActualWinds(windsGenerator.generateWinds());
    currentInputJumpManipulator = JumpManipulator();

    manager->updateLastQualifiedResult();
    manager->updateLeaderResult();
    manager->updateToAdvanceLineDistance();
    manager->updateToBeatLineDistance();

    updateToBeatDistanceLabel();
    updateToAdvanceDistanceLabel();
    updatePointsToTheLeaderLabel();
    ui->label_actualAvgWind->setText(QString::number(WindsCalculator::getAveragedWind(actualWinds, manager->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()) + " m/s");

    if(manager->checkCompetitionEnd()){
        setupGoToNextButtonForCompetitionEnd();
        showMessageBoxForCompetitionEnd();
    }
    else if(manager->checkRoundEnd()){
        setupGoToNextButtonForNextRound();
        showMessageBoxForNextRound();
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

    IndividualCompetitionManager * m = dynamic_cast<IndividualCompetitionManager *>(manager);
    WindsGeneratorSettingsEditorWidget * editor = new WindsGeneratorSettingsEditorWidget;
    editor->setWindGenerationSettings(&manager->getWindGenerationSettingsReference());
    editor->setRemovingSubmitButtons(true);
    editor->setKPoint(m->getCompetitionInfo()->getHill()->getKPoint());
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
    int howMany = QInputDialog::getInt(this, "Obniżenie belki na życzenie trenera", "O ile stopni obniżyć belkę?\nAby uzyskać dodatkową rekompensatę, zawodnik musi osiągnąć 95% punktu HS", 1, 0);
    if(howMany > 0){
        manager->setCoachGateForNextJumper(true);
        manager->setActualCoachGate(manager->getActualGate() - howMany);
        updateToAdvanceDistanceLabel();
        updateToBeatDistanceLabel();

        ui->spinBox_actualGate->setValue(manager->getActualCoachGate());
        manager->setActualGate(manager->getActualGate() + howMany);
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
