#include "NewSeasonConfiguratorWindow.h"
#include "ui_NewSeasonConfiguratorWindow.h"

#include "../../global/GlobalDatabase.h"
#include "../DatabaseEditor/DatabaseItemsListView.h"

NewSeasonConfiguratorWindow::NewSeasonConfiguratorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewSeasonConfiguratorDialog),
    classificationsListViewActualElement(-1)
{
    ui->setupUi(this);
    jumpers = GlobalDatabase::get()->getEditableGlobalJumpers();
    jumpersListView = new DatabaseItemsListView(DatabaseItemsListView::JumperItems, false, this);
    jumpersListView->setJumpers(&jumpers);
    jumpersListView->setupListModel();
    ui->verticalLayout_jumpersList->addWidget(jumpersListView);

    calendarTableModel = new CalendarEditorTableModel(&calendar, &GlobalDatabase::get()->getEditableGlobalHills(), &GlobalDatabase::get()->getEditableCompetitionRules(), this);
    calendarEditor = new CalendarEditorWidget(calendarTableModel, &classifications, this);
    ui->verticalLayout_calendarEditor->addWidget(calendarEditor);

    classificationEditor = new ClassificationEditorWidget();
    classificationEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    classificationEditor->setParent(this);
    ui->verticalLayout_classificationEditor->addWidget(classificationEditor);

    classificationsListView = new DatabaseItemsListView(DatabaseItemsListView::ClassificationItems, true, this);
    classificationsListView->setClassifications(&calendar.getClassificationsReference());
    classificationsListView->setupListModel();
    ui->verticalLayout_classificationsList->addWidget(classificationsListView);
    connect(classificationsListView, &DatabaseItemsListView::listViewDoubleClicked, this, [this](const QModelIndex &index){
        Classification * classification = &classifications[index.row()];
        if(classificationEditor->getClassification() != nullptr)
            classificationEditor->resetInputs();
        classificationEditor->setClassification(classification);
        classificationEditor->fillInputs();
    });
    connect(classificationEditor, &ClassificationEditorWidget::submitted, this, [this](){
        if(classificationsListView->getListView()->selectionModel()->selectedRows().count() > 0)
        {
            int row = classificationsListView->getListView()->selectionModel()->selectedRows().at(0).row();
            classifications[row] = classificationEditor->getClassificationFromInputs();
        }
    });

    connect(classificationsListView, &DatabaseItemsListView::remove, calendarEditor->getCompetitionInfoEditor(), [this](){
        calendar.fixCompetiitonsClassifications();
        calendarTableModel->dataChanged(calendarTableModel->index(0, 0), calendarTableModel->index(calendarTableModel->rowCount() - 1, 6));
    });
}

NewSeasonConfiguratorWindow::~NewSeasonConfiguratorWindow()
{
    delete ui;
}

void NewSeasonConfiguratorWindow::on_pushButton_submit_clicked()
{
    accept();
}

ClassificationEditorWidget *NewSeasonConfiguratorWindow::getClassificationEditor() const
{
    return classificationEditor;
}

void NewSeasonConfiguratorWindow::setClassificationEditor(ClassificationEditorWidget *newClassificationEditor)
{
    classificationEditor = newClassificationEditor;
}

CalendarEditorTableModel *NewSeasonConfiguratorWindow::getCalendarTableModel() const
{
    return calendarTableModel;
}

void NewSeasonConfiguratorWindow::setCalendarTableModel(CalendarEditorTableModel *newCalendarTableModel)
{
    calendarTableModel = newCalendarTableModel;
}

SeasonCalendar NewSeasonConfiguratorWindow::getCalendar() const
{
    return calendar;
}

void NewSeasonConfiguratorWindow::setCalendar(const SeasonCalendar &newCalendar)
{
    calendar = newCalendar;
}

CalendarEditorWidget *NewSeasonConfiguratorWindow::getCalendarEditor() const
{
    return calendarEditor;
}

void NewSeasonConfiguratorWindow::setCalendarEditor(CalendarEditorWidget *newCalendarEditor)
{
    calendarEditor = newCalendarEditor;
}

QVector<Jumper> NewSeasonConfiguratorWindow::getJumpers() const
{
    return jumpers;
}

QVector<Jumper> &NewSeasonConfiguratorWindow::getJumpersReference()
{
    return jumpers;
}

void NewSeasonConfiguratorWindow::setJumpers(const QVector<Jumper> &newJumpers)
{
    jumpers = newJumpers;
}

