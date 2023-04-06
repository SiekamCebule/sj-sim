#include "CompetitionConfigWindow.h"
#include "ui_CompetitionConfigWindow.h"

#include "../EditorWidgets/HillEditorWidget.h"
#include "../EditorWidgets/WindsGeneratorSettingsEditorWidget.h"
#include "../EditorWidgets/CompetitionRulesEditorWidget.h"
#include "StartList/CompetitionStartListDisplayWidget.h"
#include "../../global/GlobalDatabase.h"
#include "../../global/GlobalSimulationSettings.h"
#include "../../global/CountryFlagsManager.h"
#include "../../competitions/IndividualCompetitions/IndividualCompetitionManager.h"
#include "../../competitions/IndividualCompetitions/IndividualCompetitionResults.h"
#include "CompetitionManagerWindow.h"

#include <QSizePolicy>
#include <QStringListModel>
#include <QStringList>
#include <QCompleter>
#include <QFileDialog>
#include <QFile>
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

    startListDisplayWidget = new CompetitionStartListDisplayWidget(this);
    startListDisplayWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    if(getType() == SingleCompetition){
        startListParentWidget = new QWidget();
        startListParentLayout = new QVBoxLayout();

        QHBoxLayout * buttonLayout = new QHBoxLayout();
        pushButton_loadJumpersList = new QPushButton("Wczytaj zawodników z innego pliku");
        pushButton_loadJumpersList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        pushButton_loadJumpersList->setStyleSheet("QPushButton{color: black; background-color: rgb(245, 172, 166); border: 1px solid rgb(75, 75, 75); border-radius: 9px; margin: 2px; padding: 4px;} QPushButton:hover{background-color: rgb(255, 200, 195)}");
        //pushButton_loadJumpersList->setFixedSize(QSize(270, 40));
        pushButton_loadJumpersList->setFont(QFont("Quicksand [Pfed", 11, 1, false));

        buttonLayout->addSpacerItem(new QSpacerItem(50, 50, QSizePolicy::Expanding, QSizePolicy::Expanding));
        buttonLayout->addWidget(pushButton_loadJumpersList);
        buttonLayout->addSpacerItem(new QSpacerItem(50, 50, QSizePolicy::Expanding, QSizePolicy::Expanding));

        startListParentLayout->addLayout(buttonLayout);
        startListParentLayout->addSpacerItem(new QSpacerItem(50, 50, QSizePolicy::Expanding, QSizePolicy::Expanding));
        startListParentLayout->addWidget(startListDisplayWidget);

        startListParentWidget->setLayout(startListParentLayout);
        ui->verticalLayout_startList->addWidget(startListParentWidget);

        connect(pushButton_loadJumpersList, &QPushButton::clicked, this, [this](){
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

                competitionJumpers = jumpers;
                startListDisplayWidget->setJumpers(&competitionJumpers);
                startListDisplayWidget->setTeams(&competitionTeams);
                startListDisplayWidget->fillCompetitiorsActivity();
                startListDisplayWidget->setupTeamsJumpersVectors();
                startListDisplayWidget->fillItemsLayout();
            }
        });
    }
    else{
        ui->verticalLayout_startList->addWidget(startListDisplayWidget);
    }
    ui->verticalLayout_startList->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));


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
        competitionTeams = Team::constructTeamsVectorByJumpersList(competitionJumpers);
        startListDisplayWidget->setCompetitiorsType(competitionRulesEditor->getCompetitionTypeFromInput());
        startListDisplayWidget->setJumpers(&competitionJumpers);
        startListDisplayWidget->setTeams(&competitionTeams);
        startListDisplayWidget->fillCompetitiorsActivity();
        startListDisplayWidget->setupTeamsJumpersVectors();
        startListDisplayWidget->fillItemsLayout();
    }

    connect(hillEditor, &HillEditorWidget::KPointInputChanged, this, [this](){
        windsGeneratorSettingsEditor->setKPoint(hillEditor->getKPointFromInput());
        windsGeneratorSettingsEditor->setWindGenerationSettings(new QVector<WindGenerationSettings>());
        windsGeneratorSettingsEditor->fillWindGenerationSettingsByDefault();
        windsGeneratorSettingsEditor->fillSettingsInputs();
    });
    connect(competitionRulesEditor, &CompetitionRulesEditorWidget::competitionTypeChanged, this, [this](){
        startListDisplayWidget->setCompetitiorsType(competitionRulesEditor->getCompetitionTypeFromInput());
        startListDisplayWidget->fillItemsLayout();
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

CompetitionStartListDisplayWidget *CompetitionConfigWindow::getStartListDisplayWidget() const
{
    return startListDisplayWidget;
}

void CompetitionConfigWindow::setStartListDisplayWidget(CompetitionStartListDisplayWidget *newStartListDisplayWidget)
{
    startListDisplayWidget = newStartListDisplayWidget;
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
            qualsInfo.setResults(new IndividualCompetitionResults());
            qualsInfo.setSerieType(CompetitionInfo::Qualifications);
            qualsInfo.setExceptionalRoundsCount(1);

            IndividualCompetitionManager * competitionManager = new IndividualCompetitionManager(CompetitionRules::Individual, ui->spinBox_startGate->value());

            IndividualCompetitionResults * qualificationsResults = nullptr;
            IndividualCompetitionManager * qualificationsManager = nullptr;
            if(checkBox_singleCompetitionQualifications->isChecked() == true){
                qualificationsResults = new IndividualCompetitionResults();
                qualificationsManager = new IndividualCompetitionManager(CompetitionRules::Individual, ui->spinBox_startGate->value());
                qualificationsManager->setStartingJumpers(startListDisplayWidget->getIndividualCompetitionJumpers());
                qualificationsManager->setCompetitionInfo(&qualsInfo);
                qualificationsManager->getEditableCompetitionInfo()->setSerieType(CompetitionInfo::Qualifications);
                qualificationsManager->setCompetitionRules(qualsInfo.getRulesPointer());
                qualificationsManager->setResults(qualificationsResults);
                qualificationsManager->setActualWindGenerationSettings(windsGeneratorSettingsEditor->getWindsGenerationSettingsFromInputs());
                qualificationsManager->setActualGate(ui->spinBox_startGate->value());
                qualificationsManager->setActualRound(1);
                qualificationsManager->setActualRoundJumpers(qualificationsManager->getStartingJumpers());
                qualificationsManager->setActualJumperIndex(0);
                qualificationsManager->fillCompletedJumpsToStartOfRound();
                qualificationsManager->setDSQBaseProbability(ui->spinBox_dsqProbability->value());
                QVector<RoundInfo> qualsRounds = {
                    RoundInfo(0, false),
                    RoundInfo(qualificationsManager->getCompetitionRules()->getRounds().at(0).getCount(), qualificationsManager->getCompetitionRules()->getRounds().at(0).getSortStartList())
                };
                qualificationsManager->getEditableCompetitionRules()->setRounds(qualsRounds);

                CompetitionManagerWindow * qualsWindow = new CompetitionManagerWindow(qualificationsManager, this);
                connect(qualsWindow->getManager(), &AbstractCompetitionManager::competitionEnd, qualsWindow, [qualsWindow](){
                    qualsWindow->showMessageBoxFoQualificationsEnd();
                });
                if(qualsWindow->exec() == QDialog::Accepted)
                {

                }
            }

            CompetitionInfo info;
            info.setHill(new Hill(hillEditor->getHillFromWidgetInput()));
            info.setRules(competitionRulesEditor->getCompetitionRulesFromWidgetInputs());
            info.setResults(new IndividualCompetitionResults());
            info.setSerieType(CompetitionInfo::Competition);
            if(qualificationsResults != nullptr){
                competitionManager->setStartingJumpers(IndividualCompetitionManager::getFilteredJumpersVector(qualificationsManager->getActualRoundJumpersPointer(), qualificationsResults, qualificationsManager->getCompetitionRules(), 2, qualificationsManager->getQualifiedBy95HSRule())); //2 zamiast 1, ponieważ wcześniej ustawiliśmy dla kwalifikacji vector rounds o wielkości 2, gdzie właściwa ilość skoczków jest jako druga a nie pierwsza
                competitionManager->setRoundStartingGate(qualificationsManager->getActualGate());
            }
            else
                competitionManager->setStartingJumpers(startListDisplayWidget->getIndividualCompetitionJumpers());
            competitionManager->setCompetitionInfo(&info);
            competitionManager->setCompetitionRules(info.getRulesPointer());
            competitionManager->setResults(info.getResults());
            competitionManager->setActualWindGenerationSettings(windsGeneratorSettingsEditor->getWindsGenerationSettingsFromInputs());
            competitionManager->setActualGate(ui->spinBox_startGate->value());
            competitionManager->setActualRound(1);
            competitionManager->setActualRoundJumpers(competitionManager->getStartingJumpers());
            competitionManager->setActualJumperIndex(0);
            competitionManager->fillCompletedJumpsToStartOfRound();
            competitionManager->setDSQBaseProbability(ui->spinBox_dsqProbability->value());
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
