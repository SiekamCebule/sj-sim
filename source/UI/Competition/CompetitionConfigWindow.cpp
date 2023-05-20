#include "CompetitionConfigWindow.h"
#include "ui_CompetitionConfigWindow.h"

#include "../EditorWidgets/HillEditorWidget.h"
#include "../EditorWidgets/WindsGeneratorSettingsEditorWidget.h"
#include "../EditorWidgets/CompetitionRulesEditorWidget.h"
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
        competitionTeams = Team::constructTeamsVectorByJumpersList(competitionJumpers);
    }

    connect(hillEditor, &HillEditorWidget::KPointInputChanged, this, [this](){
        windsGeneratorSettingsEditor->setKPoint(hillEditor->getKPointFromInput());
        windsGeneratorSettingsEditor->setWindGenerationSettings(new QVector<WindGenerationSettings>());
        windsGeneratorSettingsEditor->fillWindGenerationSettingsByDefault();
        windsGeneratorSettingsEditor->fillSettingsInputs();
    });
    connect(competitionRulesEditor, &CompetitionRulesEditorWidget::competitionTypeChanged, this, [this](){
        if(competitionRulesEditor->getCompetitionTypeFromInput() == CompetitionRules::Individual){
            jumpersListView->setHidden(false);
        }
        else if(competitionRulesEditor->getCompetitionTypeFromInput() == CompetitionRules::Team){
            jumpersListView->setHidden(true);
        }
    });
    jumpersListView = new DatabaseItemsListView(DatabaseItemsListView::JumperItems, false, this);
    jumpersListView->getListView()->setSelectionMode(QAbstractItemView::ExtendedSelection);
    jumpersListView->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    jumpersListView->setJumpers(&competitionJumpers);
    jumpersListView->setupListModel();
    ui->verticalLayout_startList->addWidget(jumpersListView);
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

/*void CompetitionConfigWindow::removeFromStartList()
{
    QVector<QModelIndex> rows = ui->listView_startList->selectionModel()->selectedRows();
    if(rows.size() > 0){
        switch(competitionRulesEditor->getCompetitionTypeFromInput()){
        case CompetitionRules::Individual:{
            //Usunąć zawodników z vectora wewnatrz modelu
            int firstRow = ui->listView_startList->selectionModel()->selectedRows().first().row();
            while(ui->listView_startList->selectionModel()->selectedRows().size() > 0)
            {
                int rowToRemove = ui->listView_startList->selectionModel()->selectedRows().first().row();
                individualStartListEditorModel->removeRows(rowToRemove, 1);
                individualStartListEditorModel->getJumpers()->remove(rowToRemove, 1);
            }
            emit individualStartListEditorModel->dataChanged(individualStartListEditorModel->index(firstRow), individualStartListEditorModel->index(individualStartListEditorModel->rowCount() - 1));
        }
        }
    }
}

void CompetitionConfigWindow::moveToTop()
{
    QVector<QModelIndex> rows = ui->listView_startList->selectionModel()->selectedRows();
    if(rows.size() > 0){
        int firstRow = rows.first().row();
        if(firstRow == 0){
            QItemSelectionModel * selectionModel = ui->listView_startList->selectionModel();
            QVector<QModelIndex> rows = selectionModel->selectedRows();
            rows.remove(0);
            selectionModel->clearSelection();
            for(auto & index : rows){
                selectionModel->select(index, QItemSelectionModel::Select);
            }
            ui->listView_startList->setSelectionModel(selectionModel);
            if(ui->listView_startList->selectionModel()->selectedRows().size() > 0)
                firstRow = ui->listView_startList->selectionModel()->selectedRows().first().row();
            else return;
        }
        rows = ui->listView_startList->selectionModel()->selectedRows();
        switch(competitionRulesEditor->getCompetitionTypeFromInput()){
        case CompetitionRules::Individual:{
            //Usunąć zawodników z vectora wewnatrz model
            ui->listView_startList->clearSelection();
            for(auto & index : rows){
                ui->listView_startList->setCurrentIndex(individualStartListEditorModel->index(index.row() - 1));
            }
            std::sort(rows.begin(), rows.end());
            for(auto & index : rows){
                individualStartListEditorModel->getJumpers()->swapItemsAt(index.row(), index.row() - 1);
            }
            emit individualStartListEditorModel->dataChanged(individualStartListEditorModel->index(firstRow), individualStartListEditorModel->index(individualStartListEditorModel->rowCount() - 1));
        }
        }
    }
}

void CompetitionConfigWindow::moveToDown()
{
    int rowCount = 0;
    switch(competitionRulesEditor->getCompetitionTypeFromInput()){
    case CompetitionRules::Individual:
        rowCount = individualStartListEditorModel->rowCount();
        break;
    case CompetitionRules::Team:
        break;
    }

    QVector<QModelIndex> rows = ui->listView_startList->selectionModel()->selectedRows();
    if(rows.size() > 0){
        int lastRow = rows.last().row();
        if(lastRow == rowCount - 1){
            QItemSelectionModel * selectionModel = ui->listView_startList->selectionModel();
            QVector<QModelIndex> rows = selectionModel->selectedRows();
            rows.removeLast();
            selectionModel->clearSelection();
            for(auto & index : rows){
                selectionModel->select(index, QItemSelectionModel::Select);
            }
            ui->listView_startList->setSelectionModel(selectionModel);
            if(ui->listView_startList->selectionModel()->selectedRows().size() > 0)
                lastRow = ui->listView_startList->selectionModel()->selectedRows().last().row();
            else
                return;
        }
        rows = ui->listView_startList->selectionModel()->selectedRows();
        switch(competitionRulesEditor->getCompetitionTypeFromInput()){
        case CompetitionRules::Individual:{
            //Usunąć zawodników z vectora wewnatrz model
            ui->listView_startList->clearSelection();
            for(auto & index : rows){
                ui->listView_startList->setCurrentIndex(individualStartListEditorModel->index(index.row() + 1));
            }
            std::sort(rows.begin(), rows.end());
            for(auto & index : rows){
                individualStartListEditorModel->getJumpers()->swapItemsAt(index.row(), index.row() + 1);
            }
            emit individualStartListEditorModel->dataChanged(individualStartListEditorModel->index(lastRow), individualStartListEditorModel->index(0));
        }
        }
    }
}*/

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
            qDebug()<<"qualsInfo - constructor";
            qualsInfo.setHill(new Hill(hillEditor->getHillFromWidgetInput()));
            qDebug()<<"qualsInfo - setHill";
            qualsInfo.setRules(competitionRulesEditor->getCompetitionRulesFromWidgetInputs());
            qDebug()<<"qualsInfo - setRules";
            qualsInfo.setResults(new CompetitionResults());
            qDebug()<<"qualsInfo - setResults";
            qualsInfo.setSerieType(CompetitionInfo::Qualifications);
            qDebug()<<"qualsInfo - setSerieType";
            qualsInfo.setExceptionalRoundsCount(1);
            qDebug()<<"qualsInfo - setExceptionalRoundsCount";

            IndividualCompetitionManager * competitionManager = new IndividualCompetitionManager(CompetitionRules::Individual, ui->spinBox_startGate->value());
            qDebug()<<"competitionManager - constructor";

            CompetitionResults * qualificationsResults = nullptr;
            qDebug()<<"qualificationsResults";
            IndividualCompetitionManager * qualificationsManager = nullptr;
            qDebug()<<"qualificationsManager";
            if(checkBox_singleCompetitionQualifications->isChecked() == true){
                qualificationsResults = new CompetitionResults();
                qDebug()<<"qualificationsResults";
                qualificationsManager = new IndividualCompetitionManager(CompetitionRules::Individual, ui->spinBox_startGate->value());
                qDebug()<<"qualificationsManager";
                //qualificationsManager->setStartingJumpers(startListDisplayWidget->getIndividualCompetitionJumpers());
                QVector<Jumper *> jumpers;
                for(auto & j : competitionJumpers)
                    jumpers.push_back(&j);
                qDebug()<<"for(auto & j : competitionJumpers)";
                qualificationsManager->getRoundsJumpersReference().append(jumpers);
                qDebug()<<"qualificationsManager->getRoundsJumpersReference().append(jumpers);";
                qualificationsManager->setCompetitionInfo(&qualsInfo);
                qDebug()<<"qualificationsManager->setCompetitionInfo(&qualsInfo);";
                qualificationsManager->getCompetitionInfo()->setSerieType(CompetitionInfo::Qualifications);
                qDebug()<<"qualificationsManager->getCompetitionInfo()->setSerieType(CompetitionInfo::Qualifications);";
                qualificationsManager->setCompetitionRules(qualsInfo.getRulesPointer());
                qDebug()<<"qualificationsManager->setCompetitionRules(qualsInfo.getRulesPointer());";
                qualificationsManager->setResults(qualificationsResults);
                qDebug()<<"qualificationsManager->setResults(qualificationsResults);";
                qualificationsManager->setActualGate(ui->spinBox_startGate->value());
                qDebug()<<"qualificationsManager->setActualGate(ui->spinBox_startGate->value());";
                qualificationsManager->setActualRound(1);
                qDebug()<<"qualificationsManager->setActualRound(1);";
                qualificationsManager->setActualStartListIndex(0);
                qDebug()<<"qualificationsManager->setActualStartListIndex(0);";
                qualificationsManager->setBaseDSQProbability(ui->spinBox_dsqProbability->value());
                qualificationsManager->setupStartListStatusesForActualRound();
                qualificationsManager->setWindGenerationSettings(windsGeneratorSettingsEditor->getWindsGenerationSettingsFromInputs());
                qDebug()<<"qualificationsManager->setBaseDSQProbability(ui->spinBox_dsqProbability->value());";
                QVector<RoundInfo> qualsRounds = {
                    RoundInfo(0, false),
                    RoundInfo(qualificationsManager->getCompetitionRules()->getRounds().at(0).getCount(), qualificationsManager->getCompetitionRules()->getRounds().at(0).getSortStartList())
                };
                qDebug()<<"QVector<RoundInfo> qualsRounds = {";
                qualificationsManager->getCompetitionRules()->setRounds(qualsRounds);
                qDebug()<<"qualificationsManager->getCompetitionRules()->setRounds(qualsRounds);";

                CompetitionManagerWindow * qualsWindow = new CompetitionManagerWindow(qualificationsManager, this);
                qDebug()<<"CompetitionManagerWindow * qualsWindow = new CompetitionManagerWindow(qualificationsManager, this);";
                qDebug()<<"qualsWindow->setWindGenerationSettings(windsGeneratorSettingsEditor->getWindsGenerationSettingsFromInputs());";
                connect(qualsWindow->getManager(), &AbstractCompetitionManager::competitionEnd, qualsWindow, [qualsWindow](){
                    qualsWindow->showMessageBoxForQualificationsEnd();
                });
                qDebug()<<"d";
                if(qualsWindow->exec() == QDialog::Accepted)
                {
                    qDebug()<<"e";
                }
                qDebug()<<"f";
            }

            CompetitionInfo info;
            info.setHill(new Hill(hillEditor->getHillFromWidgetInput()));
            info.setRules(competitionRulesEditor->getCompetitionRulesFromWidgetInputs());
            info.setResults(new CompetitionResults());
            info.setSerieType(CompetitionInfo::Competition);
            if(qualificationsResults != nullptr){
                if(qualificationsResults->getResultsReference().count() > 0){
                    competitionManager->getRoundsJumpersReference().append(IndividualCompetitionManager::getFilteredJumpersVector(&qualificationsManager->getActualRoundJumpersReference(), qualificationsResults, qualificationsManager->getCompetitionRules(), 2, qualificationsManager->getStartListStatuses())); //2 zamiast 1, ponieważ wcześniej ustawiliśmy dla kwalifikacji vector rounds o wielkości 2, gdzie właściwa ilość skoczków jest jako druga a nie pierwsza
                    competitionManager->getRoundsStartingGatesReference().append(qualificationsManager->getActualGate());
                }
            }
            else{
                QVector<Jumper *> jumpers;
                for(auto & j : competitionJumpers) jumpers.push_back(&j);
                competitionManager->getRoundsJumpersReference().append(jumpers);
            }
            //competitionManager->setStartingJumpers(startListDisplayWidget->getIndividualCompetitionJumpers());
            //Wypełnić

            competitionManager->setCompetitionInfo(&info);
            competitionManager->setCompetitionRules(info.getRulesPointer());
            competitionManager->setResults(info.getResults());
            competitionManager->setActualGate(ui->spinBox_startGate->value());
            competitionManager->setActualRound(1);
            competitionManager->setActualStartListIndex(0);
            competitionManager->setBaseDSQProbability(ui->spinBox_dsqProbability->value());
            competitionManager->setupStartListStatusesForActualRound();
            competitionManager->setWindGenerationSettings(windsGeneratorSettingsEditor->getWindsGenerationSettingsFromInputs());
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
