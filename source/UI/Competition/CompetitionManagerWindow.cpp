#include "CompetitionManagerWindow.h"
#include "ui_CompetitionManagerWindow.h"
#include "../../competitions/IndividualCompetitions/IndividualCompetitionManager.h"
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
        ui->spinBox_actualGate->setValue(manager->getRoundStartingGate());
    }

    ui->pushButton_competitionDetails->hide();
    ui->pushButton_goToNext->hide();

    startListModel = new StartListModel(manager, this);
    startListModel->setType(getType());
    switch(type)
    {
    case CompetitionRules::Individual:
        IndividualCompetitionManager * indManager = dynamic_cast<IndividualCompetitionManager *>(this->manager);
        startListModel->setJumpers(indManager->getActualRoundJumpersPointer());

        ui->label_nameAndSurname->setText(indManager->getActualRoundJumpers().at(0)->getNameAndSurname());
        ui->label_flag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(indManager->getActualJumper()->getCountryCode().toLower())).scaled(ui->label_flag->size()));

        connect(indManager, &IndividualCompetitionManager::actualJumperIndexChanged, this, [this, indManager](){
            ui->label_nameAndSurname->setText(indManager->getActualJumper()->getNameAndSurname());
            ui->label_flag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(indManager->getActualRoundJumpers().at(0)->getCountryCode().toLower())).scaled(ui->label_flag->size()));
        });
        break;
    }
    ui->listView_startList->setModel(startListModel);

    resultsTableModel = new ResultsTableModel(getType(), manager->getResults(), manager, this);
    ui->tableView_results->setModel(resultsTableModel);
    ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    jumperResultsWidget = new JumperCompetitionResultsWidget(this);
    jumperResultsWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    ui->verticalLayout_jumperResult->addWidget(jumperResultsWidget);

    ui->label_toBeatDistance->setText("0m");
    ui->label_toAdvancementDistance->setText("-");

    manager->generateActualWinds();
    updateAvgWindLabel();

    ui->label_pointsBehindLeader->setText("");

    Hill * hill = manager->getCompetitionInfo()->getHill();

    ui->label_hillNameAndHSPoint->setText(hill->getName() + " HS" + QString::number(hill->getHSPoint()));
    ui->label_hillFlag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(hill->getCountryCode().toLower())).scaled(ui->label_hillFlag->size()));

    //jumperResultsWidget->hide();

    connect(ui->spinBox_actualGate, &QSpinBox::valueChanged, this, [this, manager](){
        if(manager->getActualJumperIndex() == 0){
            manager->setRoundStartingGate(ui->spinBox_actualGate->value());
            manager->setActualGate(ui->spinBox_actualGate->value());
        }
        else{
            manager->setActualGate(ui->spinBox_actualGate->value());
            manager->updateToBeatDistance();
            ui->label_toBeatDistance->setText(QString::number(manager->getToBeatDistance()) + "m");
        }
    });
}

CompetitionManagerWindow::~CompetitionManagerWindow()
{
    delete ui;
}

void CompetitionManagerWindow::updateToBeatDistanceLabel()
{
    IndividualCompetitionManager * m = dynamic_cast<IndividualCompetitionManager *>(manager);
    m->updateToBeatDistance();
    ui->label_toBeatDistance->setText(QString::number(m->getToBeatDistance()) + "m");
}

void CompetitionManagerWindow::updateToAdvanceDistanceLabel()
{
    IndividualCompetitionManager * m = dynamic_cast<IndividualCompetitionManager *>(manager);
    m->updateToAdvanceDistance();
    if(m->getToAdvanceDistance() == (-1))
        ui->label_toAdvancementDistance->setText("-");
    else ui->label_toAdvancementDistance->setText(QString::number(m->getToAdvanceDistance()) + "m");
}

void CompetitionManagerWindow::updatePointsToTheLeaderLabel()
{
    IndividualCompetitionManager * m = dynamic_cast<IndividualCompetitionManager *>(manager);
    m->updateActualJumperPointsToTheLeader();
    if(m->getActualJumperPointsToTheLeader() == (-1))
        ui->label_pointsBehindLeader->setText("");
    else ui->label_pointsBehindLeader->setText(" + " + QString::number(m->getActualJumperPointsToTheLeader()) + "pkt");
}

void CompetitionManagerWindow::updateAvgWindLabel()
{
    IndividualCompetitionManager * m = dynamic_cast<IndividualCompetitionManager *>(manager);
    switch(type){
    case CompetitionRules::Individual:{
        ui->label_actualAvgWind->setText(QString::number(WindsCalculator::getAveragedWind(m->getActualWinds(), m->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()) + " m/s");
    }
    }
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

void CompetitionManagerWindow::setupGoToNextButtonForQualificationsEnd()
{
    ui->pushButton_goToNext->show();
    ui->pushButton_goToNext->setText("Przejdź do konkursu");
    disableCompetitionManagementButtons();

    switch(getType())
    {
    case CompetitionRules::Individual:{
        IndividualCompetitionManager * m = dynamic_cast<IndividualCompetitionManager *>(manager);

        QMetaObject::Connection * const connection = new QMetaObject::Connection;
        *connection = connect(ui->pushButton_goToNext, &QPushButton::clicked, [this, m, connection](){
            qDebug()<<"quals end";
            accept();

            QObject::disconnect(*connection);
            delete connection;
        });
    }
    }
}

void CompetitionManagerWindow::setupGoToNextButtonForCompetitionEnd()
{
    ui->pushButton_goToNext->show();
    ui->pushButton_goToNext->setText("Zakończ konkurs");
    disableCompetitionManagementButtons();

    switch(getType())
    {
    case CompetitionRules::Individual:{
        IndividualCompetitionManager * m = dynamic_cast<IndividualCompetitionManager *>(manager);

        QMetaObject::Connection * const connection = new QMetaObject::Connection;
        *connection = connect(ui->pushButton_goToNext, &QPushButton::clicked, [this, m, connection](){
            qDebug()<<"competition end";
            accept();

            QObject::disconnect(*connection);
            delete connection;
        });
    }
    }
}

void CompetitionManagerWindow::on_pushButton_jump_clicked()
{
    switch(getType())
    {
    case CompetitionRules::Individual:{
        IndividualCompetitionManager * m = dynamic_cast<IndividualCompetitionManager *>(manager);
        IndividualCompetitionResults * indRes = dynamic_cast<IndividualCompetitionResults *>(m->getResults());
        m->simulateNext();
        m->updateLeaderResult();
        m->updateLastQualifiedResult();
        m->generateActualWinds();
        updateToBeatDistanceLabel();
        updateToAdvanceDistanceLabel();
        updatePointsToTheLeaderLabel();

        //if(jumperResultsWidget->isHidden()) jumperResultsWidget->show();

        ui->label_actualAvgWind->setText(QString::number(WindsCalculator::getAveragedWind(m->getActualWinds(), m->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()) + " m/s");

        if(m->getLastJump() == true){
            jumperResultsWidget->setJumperResult(indRes->getResultsOfJumper(m->getActualRoundJumpersPointer()->at(m->getActualJumperIndex())));
        }
        else{
            jumperResultsWidget->setJumperResult(indRes->getResultsOfJumper(m->getActualRoundJumpersPointer()->at(m->getActualJumperIndex() - 1)));
        }
        jumperResultsWidget->fillWidget();

        emit startListModel->dataChanged(startListModel->index(m->getActualJumperIndex() - 1), startListModel->index(m->getActualJumperIndex() - 1));
        ui->tableView_results->setModel(nullptr);
        ui->tableView_results->setModel(resultsTableModel);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        manager->setIsCoachGate(false);
        manager->setActualCoachGate(0);

        if(m->getRoundShouldBeEnded()){
            ui->pushButton_goToNext->show();
            ui->pushButton_goToNext->setText("Przejdź do " + QString::number(manager->getActualRound() + 1) + " serii");
            disableCompetitionManagementButtons();

            QMetaObject::Connection * const connection = new QMetaObject::Connection;
            *connection = connect(ui->pushButton_goToNext, &QPushButton::clicked, [this, m, connection](){
                //qDebug()<<"next round";
                m->setupNextRound();
                enableCompetitionManagementButtons();
                ui->pushButton_goToNext->hide();

                QObject::disconnect(*connection);
                delete connection;
            });

            QMessageBox * box = new QMessageBox(this);
            box->setStyleSheet("QMessageBox{color: black; background-color: white;}");
            box->setIcon(QMessageBox::Information);
            box->setWindowTitle("Zakończenie " + QString::number(manager->getActualRound()) + " serii");
            box->setText(tr("Aby przejść do następnej serii, wciśnij przycisk na dole okna konkursu"));
            QPushButton *btnOk = box->addButton("OK", QMessageBox::AcceptRole);
            box->setModal(true);
            box->show();
        }

        manager->setIsCoachGate(false);
        updateToAdvanceDistanceLabel();
        updateToBeatDistanceLabel();

        if(m->getCompetiitonShouldBeEnded() == true){
            QMessageBox * box = new QMessageBox(this);
            box->setStyleSheet("QMessageBox{color: black; background-color: white;}");
            box->setIcon(QMessageBox::Information);
            box->setWindowTitle("Zakończenie konkursu");
            box->setText(tr("Aby zakończyć konkurs, wciśnij przycisk na dole okna konkursu"));
            QPushButton *btnOk = box->addButton("OK", QMessageBox::AcceptRole);
            box->setModal(true);
            box->show();

            setupGoToNextButtonForCompetitionEnd();
        }
        break;
    }
    }
}

void CompetitionManagerWindow::on_tableView_results_doubleClicked(const QModelIndex &index)
{
    IndividualCompetitionManager * m = dynamic_cast<IndividualCompetitionManager *>(manager);
    IndividualCompetitionResults * indRes = dynamic_cast<IndividualCompetitionResults *>(m->getResults());
    if(index.column() == 1){
        jumperResultsWidget->setJumperResult(indRes->getPointerOfExactJumperResults(index.row()));
        jumperResultsWidget->fillWidget();
    }
}


void CompetitionManagerWindow::on_pushButton_generateNewWinds_clicked()
{
    IndividualCompetitionManager * m = dynamic_cast<IndividualCompetitionManager *>(manager);
    m->generateActualWinds();
    if(m->getActualJumperIndex() > 1){
        m->updateLeaderResult();
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
    editor->setWindGenerationSettings(m->getActualWindGenerationSettingsPointer());
    editor->setRemovingSubmitButtons(true);
    editor->setKPoint(m->getCompetitionInfo()->getHill()->getKPoint());
    editor->setSettingsCount(WindsGenerator::calculateWindsCountByKPoint(editor->getKPoint()));
    editor->fillSettingsInputs();

    dialog->layout()->addWidget(editor);
    connect(editor, &WindsGeneratorSettingsEditorWidget::submitted, dialog, &QDialog::accept);

    if(dialog->exec() == QDialog::Accepted){
        m->setActualWindGenerationSettings(editor->getWindsGenerationSettingsFromInputs());
        delete editor;
        delete dialog;
    }

}


void CompetitionManagerWindow::on_pushButton_manipulateJump_clicked()
{
    JumpManipulatorConfigWindow * window = new JumpManipulatorConfigWindow;
    window->setWindGenerationSettings(manager->getActualWindGenerationSettingsPointer());
    window->setKPoint(manager->getCompetitionInfo()->getHill()->getKPoint());
    connect(window, &JumpManipulatorConfigWindow::submitted, window, &JumpManipulatorConfigWindow::accept);
    if(window->exec() == QDialog::Accepted){
        JumpManipulator manipulator = window->getJumpManipulatorFromInputs();
        manager->setActualJumpManipulator(manipulator);
        if(manipulator.getExactWinds().size() > 0)
            manager->setActualWinds(manipulator.getExactWinds());
        updateAvgWindLabel();
    }
}


void CompetitionManagerWindow::on_pushButton_coachGate_clicked()
{
    int howMany = QInputDialog::getInt(this, "Obniżenie belki na życzenie trenera", "O ile stopni obniżyć belkę?\nAby uzyskać dodatkową rekompensatę, zawodnik musi osiągnąć 95% punktu HS", 1, 0);
    if(howMany > 0){
        manager->setIsCoachGate(true);
        manager->setActualCoachGate(manager->getActualGate() - howMany);
        updateToAdvanceDistanceLabel();
        updateToBeatDistanceLabel();

        ui->spinBox_actualGate->setValue(manager->getActualCoachGate());
        manager->setActualGate(manager->getActualGate() + howMany);
    }
}


void CompetitionManagerWindow::on_pushButton_goToNext_clicked()
{

}

