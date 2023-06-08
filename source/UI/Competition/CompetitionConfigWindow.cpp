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
#include "../../competitions/CompetitionResults.h"
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

CompetitionConfigWindow::CompetitionConfigWindow(short type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SingleCompetitionConfigWindow),
    type(type)
{
    ui->setupUi(this);
    ui->spinBox_dsqProbability->setValue(GlobalSimulationSettings::get()->getBaseDsqProbability());
    delete ui->page_2;
    ui->toolBox->removeItem(1);
    setWindowFlags(Qt::Window);
    setupHillToolBoxItem();

    windsGeneratorSettingsEditor = new WindsGeneratorSettingsEditorWidget(this);
    windsGeneratorSettingsEditor->setRemovingSubmitButtons(true);
    windsGeneratorSettingsEditor->removeSubmitButton();
    competitionRulesEditor = new CompetitionRulesEditorWidget(this);
    competitionRulesEditor->removeSubmitButton();
    competitionRulesEditor->setCompetitionRules(new CompetitionRules(tr("Zasady")));
    competitionRulesEditor->setParent(this);

    if(getType() == SingleCompetition){
        //Wypełnić
        /*connect(pushButton_loadJumpersList, &QPushButton::clicked, this, [this](){
            QUrl fileUrl = QFileDialog::getOpenFileUrl(this, tr("Wybierz plik z zawodnikami"), QUrl(), "JSON (*.json)");

            QFile file(fileUrl.path());
            if(!(file.open(QIODevice::ReadOnly | QIODevice::Text)))
            {
                QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zawodnikami", "Nie udało się otworzyć wybranego pliku\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
                message.setModal(true);
                message.exec();
            }
            QVector<Jumper> jumpers = Jumper::getJumpersVectorFromJson(file.readAll());
            file.close();
            if(jumpers.isEmpty() == false){

                //Wypełnić kodem
            }
        });*/
    }

    ui->toolBox->raise();
    ui->label_title->raise();
    ui->toolBox->addItem(windsGeneratorSettingsEditor, "Ustawienia generatora wiatru");
    setupCompetitionRulesToolBoxItem();

    if(getType() == SeasonCompetition)
    {
        setWindowTitle("Konfiguracja konkursu");
        delete ui->toolBox->widget(0); //index 0
        ui->toolBox->removeItem(0);
        delete ui->toolBox->widget(1); //index 2
        ui->toolBox->removeItem(1);
    }
    else if(getType() == SingleCompetition)
    {
        checkBox_singleCompetitionQualifications = new QCheckBox("Przeprowadzenie kwalifikacji", this);
        checkBox_singleCompetitionQualifications->setStyleSheet("color: rgb(30, 30, 30);\nQCheckBox::indicator{\nwidth: 40px;\nheight: 30px;\n}");
        checkBox_singleCompetitionQualifications->setFont(QFont("Quicksand Medium", 12, 650));
        ui->formLayout_competitionOptions->addWidget(checkBox_singleCompetitionQualifications);

        setWindowTitle("Konfiguracja pojedynczego konkursu");
        competitionJumpers = GlobalDatabase::get()->getGlobalJumpers();
        competitionTeams = Team::constructTeamsVectorByJumpersList(competitionJumpers, competitionRulesEditor->getJumpersCountInTeam());
    }
    connect(competitionRulesEditor, &CompetitionRulesEditorWidget::jumpersCountInTeamChanged, this, [this](){
        competitionTeams = Team::constructTeamsVectorByJumpersList(competitionJumpers, competitionRulesEditor->getJumpersCountInTeam());
        delete teamsSquadsModel;
        teamsSquadsModel = new TeamsSquadsTreeModel(&competitionTeams, competitionRulesEditor->getJumpersCountInTeam());
        teamsSquadsModel->updateItemsByTeamsVector();
        teamsTreeView->setModel(teamsSquadsModel);
        teamsTreeView->expandToDepth(0);
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
    jumpersListView = new DatabaseItemsListView(DatabaseItemsListView::JumperItems, false, this);
    jumpersListView->getListView()->setSelectionMode(QAbstractItemView::ExtendedSelection);
    jumpersListView->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    jumpersListView->setJumpers(&competitionJumpers);
    jumpersListView->setupListModel();
    ui->verticalLayout_startList->addWidget(jumpersListView);

    teamsTreeView = new QTreeView(this);
    teamsTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    teamsTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    teamsTreeView->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    teamsTreeView->setHidden(true);
    teamsSquadsModel = new TeamsSquadsTreeModel(&competitionTeams, competitionRulesEditor->getJumpersCountInTeam());
    teamsSquadsModel->updateItemsByTeamsVector();
    teamsTreeView->setModel(teamsSquadsModel);
    ui->verticalLayout_startList->addWidget(teamsTreeView);

    connect(teamsTreeView, &QTreeView::doubleClicked, this, [this](const QModelIndex & index){
        TreeItem * item = static_cast<TreeItem *>(index.internalPointer());
        int teamIndex = 0;
        if(item->getParentItem() == teamsSquadsModel->getRootItem()){
            teamIndex = item->row();
        }
        else{
            teamIndex = item->getParentItem()->row();
        }

        TeamEditorWidget * editor = new TeamEditorWidget(&competitionTeams[teamIndex], competitionRulesEditor->getJumpersCountInTeam());
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
            teamsSquadsModel = new TeamsSquadsTreeModel(&competitionTeams, competitionRulesEditor->getJumpersCountInTeam());
            teamsSquadsModel->updateItemsByTeamsVector();
            teamsTreeView->setModel(teamsSquadsModel);
            teamsTreeView->expandToDepth(0);
        }
    });

    ui->verticalLayout_startList->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Maximum));


    emit competitionRulesEditor->competitionTypeChanged();
}

CompetitionConfigWindow::~CompetitionConfigWindow()
{
    delete ui;
}

int CompetitionConfigWindow::getJumpersCountInTeam() const
{
    return competitionRulesEditor->getJumpersCountInTeam();
}

void CompetitionConfigWindow::setupHillToolBoxItem()
{
    ui->comboBox_existingHill->clear();
    ui->comboBox_existingHill->addItem("BRAK");
    for(const auto & hill : GlobalDatabase::get()->getGlobalHills())
    {
        ui->comboBox_existingHill->addItem(QIcon(QPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(hill.getCountryCode().toLower())))) ,hill.getName() + " HS" + QString::number(hill.getHSPoint()));
    }

    hillEditor = new HillEditorWidget(this);
    hillEditor->removeSubmitButton();
    ui->verticalLayout_hillEditorWidget->addWidget(hillEditor);

    connect(ui->comboBox_existingHill, &QComboBox::currentIndexChanged, this, [this](){
        int selected = ui->comboBox_existingHill->currentIndex();
        if(selected > 0){
            hillEditor->setHill(&GlobalDatabase::get()->getGlobalHills()[selected - 1]);
            hillEditor->fillHillInputs();
            emit hillEditor->KPointInputChanged(hillEditor->getHill()->getKPoint());
        }
        else hillEditor->resetHillInputs();
    });
}

void CompetitionConfigWindow::setupCompetitionRulesToolBoxItem()
{
    existingCompetitionRulesComboBox = new QComboBox;
    QStringList rulesStringList;
    rulesStringList.push_back("BRAK");
    for(const auto & rule : GlobalDatabase::get()->getGlobalCompetitionsRules())
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

    existingCompetitionRulesLabel = new QLabel("Istniejące zasady konkursu", competitionRulesParentWidget);
    existingCompetitionRulesLabel->setStyleSheet("color: rgb(20, 20, 20);");
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    font.setFamily("Quicksand [PfEd]");
    existingCompetitionRulesLabel->setFont(font);
    existingRulesLabelAndComboBoxLayout->addWidget(existingCompetitionRulesLabel);
    existingRulesLabelAndComboBoxLayout->addWidget(existingCompetitionRulesComboBox);

    ui->toolBox->addItem(competitionRulesParentWidget, "Zasady konkursu");

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

QTreeView *CompetitionConfigWindow::getTeamsTreeView() const
{
    return teamsTreeView;
}

void CompetitionConfigWindow::setTeamsTreeView(QTreeView *newTeamsTreeView)
{
    teamsTreeView = newTeamsTreeView;
}

TeamsSquadsTreeModel *CompetitionConfigWindow::getTeamsSquadsModel() const
{
    return teamsSquadsModel;
}

void CompetitionConfigWindow::setTeamsSquadsModel(TeamsSquadsTreeModel *newTeamsSquadsModel)
{
    teamsSquadsModel = newTeamsSquadsModel;
}

void CompetitionConfigWindow::setCompetitionJumpers(const QVector<Jumper> &newCompetitionJumpers)
{
    competitionJumpers = newCompetitionJumpers;
}

QVector<Team> CompetitionConfigWindow::getCompetitionTeams() const
{
    return competitionTeams;
}

void CompetitionConfigWindow::setCompetitionTeams(const QVector<Team> &newCompetitionTeams)
{
    competitionTeams = newCompetitionTeams;
}

QVector<Jumper> CompetitionConfigWindow::getCompetitionJumpers() const
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

WindsGeneratorSettingsEditorWidget *CompetitionConfigWindow::getWindGeneratorSettingsEditor() const
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
    switch(getType())
    {
    case SingleCompetition:{
        switch(competitionRulesEditor->getCompetitionTypeFromInput()){
        case CompetitionRules::Individual:
        {
            CompetitionInfo qualsInfo;
            qualsInfo.setHill(new Hill(hillEditor->getHillFromWidgetInput()));
            qualsInfo.setRules(competitionRulesEditor->getCompetitionRulesFromWidgetInputs());
            qualsInfo.setResults(new CompetitionResults());
            qualsInfo.setSerieType(CompetitionInfo::Qualifications);
            qualsInfo.setExceptionalRoundsCount(1);

            IndividualCompetitionManager * competitionManager = new IndividualCompetitionManager(CompetitionRules::Individual);

            CompetitionResults * qualificationsResults = nullptr;
            IndividualCompetitionManager * qualificationsManager = nullptr;
            if(checkBox_singleCompetitionQualifications->isChecked() == true){
                qualificationsResults = new CompetitionResults();
                qualificationsManager = new IndividualCompetitionManager(CompetitionRules::Individual);
                qualificationsManager->getRoundsStartingGatesReference().push_back(ui->spinBox_startGate->value());
                QVector<Jumper *> jumpers;
                for(auto & j : competitionJumpers)
                    jumpers.push_back(&j);
                qualificationsManager->getRoundsJumpersReference().append(jumpers);
                qualificationsManager->setCompetitionInfo(&qualsInfo);
                qualificationsManager->getCompetitionInfo()->setSerieType(CompetitionInfo::Qualifications);
                qualificationsManager->setCompetitionRules(qualsInfo.getRulesPointer());
                qualificationsManager->setResults(qualificationsResults);
                qualificationsManager->setActualGate(ui->spinBox_startGate->value());
                qualificationsManager->getRoundsStartingGatesReference().push_back(qualificationsManager->getActualGate());
                qualificationsManager->setActualRound(1);
                qualificationsManager->setActualStartListIndex(0);
                qualificationsManager->setBaseDSQProbability(ui->spinBox_dsqProbability->value());
                qualificationsManager->setupStartListStatusesForActualRound();
                qualificationsManager->setWindGenerationSettings(windsGeneratorSettingsEditor->getWindsGenerationSettingsFromInputs());
                QVector<RoundInfo> qualsRounds = {
                    RoundInfo(0, false),
                    RoundInfo(qualificationsManager->getCompetitionRules()->getRounds().at(0).getCount(), qualificationsManager->getCompetitionRules()->getRounds().at(0).getSortStartList())
                };
                qualificationsManager->getCompetitionRules()->setRounds(qualsRounds);

                CompetitionManagerWindow * qualsWindow = new CompetitionManagerWindow(qualificationsManager, this);
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
            info.setResults(new CompetitionResults());
            info.setSerieType(CompetitionInfo::Competition);
            qDebug()<<"Anoo";

            //competitionManager->setStartingJumpers(startListDisplayWidget->getIndividualCompetitionJumpers());
            //Wypełnić

            competitionManager->setCompetitionInfo(&info);
            competitionManager->setCompetitionRules(info.getRulesPointer());
            competitionManager->setResults(info.getResults());
            competitionManager->setActualGate(ui->spinBox_startGate->value());
            competitionManager->setActualRound(1);
            competitionManager->setBaseDSQProbability(ui->spinBox_dsqProbability->value());
            competitionManager->setWindGenerationSettings(windsGeneratorSettingsEditor->getWindsGenerationSettingsFromInputs());

            if(qualificationsResults != nullptr){
                if(qualsInfo.getCancelled() == true)
                    return;
                if(qualificationsResults->getResultsReference().count() > 0){
                    competitionManager->getRoundsJumpersReference().push_back(qualificationsManager->getFilteredJumpersForNextRound());// właściwa ilość skoczków jest jako druga a nie pierwsza
                    qDebug()<<"fryty: "<<competitionManager->getRoundsJumpersReference().first().count();

                    competitionManager->setActualGate(qualificationsManager->getActualGate());
                    competitionManager->setWindGenerationSettings(qualificationsManager->getWindGenerationSettingsReference());
                }
            }
            else{
                QVector<Jumper *> jumpers;
                for(auto & j : competitionJumpers) jumpers.push_back(&j);
                competitionManager->getRoundsJumpersReference().append(jumpers);
            }
            competitionManager->setupStartListStatusesForActualRound();
            qDebug()<<":DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
            qDebug()<<competitionManager->getRoundsJumpersReference().first().count();
            competitionManager->setActualStartListIndex(0);
            qDebug()<<"ffffffffffffffffffffffffffffff bobre :   "<<competitionManager->getActualGate();
            competitionManager->getRoundsStartingGatesReference().push_back(competitionManager->getActualGate());
            qDebug()<<"countL: "<<competitionManager->getRoundsStartingGatesReference().count();

            CompetitionManagerWindow * window = new CompetitionManagerWindow(competitionManager, this);
            if(window->exec() == QDialog::Accepted)
            {
                this->setModal(true);
            }

            delete info.getHill();
            delete competitionManager;
            if(qualificationsManager != nullptr)
                delete qualificationsManager;
            if(qualificationsResults != nullptr)
                delete qualificationsResults;
        }
            break;
        }
    }
        break;
    }

}
