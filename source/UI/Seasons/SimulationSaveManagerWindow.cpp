#include "SimulationSaveManagerWindow.h"
#include "ui_SimulationSaveManagerWindow.h"
#include "../../global/GlobalAppSettings.h"
#include "../../utilities/functions.h"
#include <QMessageBox>
#include <QTimer>
#include <QPushButton>

SimulationSaveManagerWindow::SimulationSaveManagerWindow(SimulationSave *save, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulationSaveManagerWindow),
    simulationSave(save)
{
    ui->setupUi(this);
    ui->label_saveName->setText(simulationSave->getName() + ": ");
    ui->label_seasonNumber->setText(tr("Sezon ") + QString::number(simulationSave->getActualSeason()->getSeasonNumber()));

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

    jumpersListView = new DatabaseItemsListView(DatabaseItemsListView::JumperItems, true, false, this);
    jumpersListView->setJumpers(&simulationSave->getJumpersReference());
    jumpersListView->setupListModel();
    ui->verticalLayout_jumpersList->addWidget(jumpersListView);

    jumperEditor = new JumperEditorWidget();
    jumperEditor->hide();
    ui->verticalLayout_jumperEditor->addWidget(jumperEditor);
    connect(jumpersListView, &DatabaseItemsListView::listViewDoubleClicked, this, [this](const QModelIndex & index){
        jumperEditor->show();
        jumperEditor->setJumper(&simulationSave->getJumpersReference()[index.row()]);
        jumperEditor->fillJumperInputs();
    });
    connect(jumperEditor, &JumperEditorWidget::submitted, this, [this](){
        int idx = jumpersListView->getLastDoubleClickedIndex();
        simulationSave->getJumpersReference()[idx] = jumperEditor->getJumperFromWidgetInput();
        emit jumpersListView->getListModel()->dataChanged(jumpersListView->getListModel()->index(idx), jumpersListView->getListModel()->index(idx));
    });

    //-----//

    hillsListView = new DatabaseItemsListView(DatabaseItemsListView::HillItems, true, false, this);
    hillsListView->setHills(&simulationSave->getHillsReference());
    hillsListView->setupListModel();
    ui->verticalLayout_hillsList->addWidget(hillsListView);

    hillEditor = new HillEditorWidget();
    hillEditor->hide();
    ui->verticalLayout_hillEditor->addWidget(hillEditor);
    connect(hillsListView, &DatabaseItemsListView::listViewDoubleClicked, this, [this](const QModelIndex & index){
        hillEditor->show();
        hillEditor->setHill(&simulationSave->getHillsReference()[index.row()]);
        hillEditor->fillHillInputs();
    });
    connect(hillEditor, &HillEditorWidget::submitted, this, [this](){
        int idx = hillsListView->getLastDoubleClickedIndex();
        simulationSave->getHillsReference()[idx] = hillEditor->getHillFromWidgetInput();
        emit hillsListView->getListModel()->dataChanged(hillsListView->getListModel()->index(idx), hillsListView->getListModel()->index(idx));
    });
    connect(hillsListView, &DatabaseItemsListView::insert, this, [this](){
        simulationSave->getActualSeason()->getCalendarReference().fixCompetitionsHills(&simulationSave->getHillsReference());
    });

    //-----//

    rulesListView = new DatabaseItemsListView(DatabaseItemsListView::CompetitionRulesItems, true, true, this);
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

    classificationsListView = new DatabaseItemsListView(DatabaseItemsListView::ClassificationItems, true, this);
    classificationsListView->setClassifications(&simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference());
    classificationsListView->setupListModel();
    classificationsListView->selectOnlyFirstRow();
    ui->verticalLayout_classificationsList->addWidget(classificationsListView);
    connect(classificationsListView, &DatabaseItemsListView::listViewDoubleClicked, this, [this](const QModelIndex &index){
        Classification * classification = &simulationSave->getActualSeason()->getCalendarReference().getClassificationsReference()[index.row()];
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
    connect(classificationsListView, &DatabaseItemsListView::remove, this, [this](){
        simulationSave->getActualSeason()->getCalendarReference().fixCompetitionsClassifications();
        calendarTableModel->dataChanged(calendarTableModel->index(0, 0), calendarTableModel->index(calendarTableModel->rowCount() - 1, 6));
    });
}

/* Jak rozwiązać problem z edycją klasyfikacji podczas trwania sezonu?
 * 1. Co można robić z klasyfikacjami w trakcie sezonu?
 *      - Edytować klasyfikacje które nie zostały jeszcze wykorzystane
 *      - Dodawać nowe
 *      - Usuwać nowe (z usunięciem ze wszystkich konkursów i usunięciu historii tej klasyfikacji)
 * 2. Jak wykryć które klasyfikacje nie zostały jeszcze wykorzystane?
 *      - W pętli posprawdzać wszystkie rozegrane konkursy:
 *          - Jeżeli w którymś była dana klasyfikacja, ZABLOKUJ możliwość edycji
 *          - W przeciwnym wypadku, UMOŻLIW edycję
 *      - Po zatwierdzeniu edycji klasyfikacji sprawdzamy czy ta edycja jest umożliwiona. Jeżeli nie, pokaż QMessageBox::information z informacją że ta klasyfikacja była już używana.
*/

SimulationSaveManagerWindow::~SimulationSaveManagerWindow()
{
    delete ui;
}

void SimulationSaveManagerWindow::showJumperAndHillsEditingHelp()
{
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

SimulationSave *SimulationSaveManagerWindow::getSimulationSave() const
{
    return simulationSave;
}
