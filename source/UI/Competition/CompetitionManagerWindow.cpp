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

    startListModel = new StartListModel(manager, this);
    startListModel->setType(getType());
    switch(type)
    {
    case CompetitionRules::Individual:
        IndividualCompetitionManager * indManager = dynamic_cast<IndividualCompetitionManager *>(this->manager);
        startListModel->setJumpers(indManager->getActualRoundJumpersPointer());
        connect(indManager, &IndividualCompetitionManager::actualJumperIndexChanged, this, [this, indManager](){
            ui->label_nameAndSurname->setText(indManager->getActualJumper()->getNameAndSurname());
            ui->label_flag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(indManager->getActualJumper()->getCountryCode().toLower())).scaled(ui->label_flag->size()));
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

        ui->label_actualAvgWind->setText(QString::number(WindsCalculator::getAveragedWind(m->getActualWinds(), m->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()) + " m/s");

        if(m->getLastJump() == true){
            qDebug()<<"LAST";
            jumperResultsWidget->setJumperResult(indRes->getResultsOfJumper(m->getActualRoundJumpersPointer()->at(m->getActualJumperIndex())));
        }
        else{
            qDebug()<<"NORMAL";
            jumperResultsWidget->setJumperResult(indRes->getResultsOfJumper(m->getActualRoundJumpersPointer()->at(m->getActualJumperIndex() - 1)));
        }
        jumperResultsWidget->fillWidget();
        qDebug()<<"UZUPEPE";

        emit startListModel->dataChanged(startListModel->index(m->getActualJumperIndex() - 1), startListModel->index(m->getActualJumperIndex() - 1));
        ui->tableView_results->setModel(nullptr);
        ui->tableView_results->setModel(resultsTableModel);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView_results->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        manager->setIsCoachGate(false);
        manager->setActualCoachGate(0);

        if(m->getRoundShouldBeEnded()){
            QMessageBox * box = new QMessageBox(this);
            box->setStyleSheet("QMessageBox{color: black; background-color: white;}");
            box->setIcon(QMessageBox::Information);
            box->setWindowTitle("Zakończenie " + QString::number(manager->getActualRound()) + " serii");
            box->setText(tr("Aby przejść do następnej serii, wciśnij \"OK\"."));
            QPushButton *btnOk = box->addButton("OK", QMessageBox::AcceptRole);
            //box->setAttribute(Qt::WA_DeleteOnClose); // delete pointer after close
            connect(box, &QMessageBox::accepted, this, [this, box, m](){
                qDebug()<<"akcepted message";
                m->setupNextRound();
            });
            box->setModal(true);
            box->show();
        }

        if(m->getCompetiitonShouldBeEnded() == true){
            emit m->competitionEnd();
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
    ui->label_actualAvgWind->setText(QString::number(WindsCalculator::getAveragedWind(m->getActualWinds(), m->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()) + " m/s");
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
        manager->setActualJumpManipulator(window->getJumpManipulatorFromInputs());
        qDebug()<<manager->getActualJumpManipulator().getExactLandingType();
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
    }
}

