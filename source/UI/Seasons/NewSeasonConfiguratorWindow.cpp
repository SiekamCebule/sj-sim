#include "NewSeasonConfiguratorWindow.h"
#include "ui_NewSeasonConfiguratorWindow.h"

#include "../../global/GlobalDatabase.h"
#include "../DatabaseEditor/DatabaseItemsListView.h"

NewSeasonConfiguratorWindow::NewSeasonConfiguratorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewSeasonConfiguratorDialog)
{
    ui->setupUi(this);
    jumpers = GlobalDatabase::get()->getEditableGlobalJumpers();
    jumpersListView = new DatabaseItemsListView(DatabaseItemsListView::JumperItems, false, this);
    jumpersListView->setJumpers(&jumpers);
    jumpersListView->setupListModel();
    ui->verticalLayout_jumpersList->addWidget(jumpersListView);

    calendarTreeModel = new CalendarEditorTreeModel(&calendar, this);
    calendarEditor = new CalendarEditorWidget(calendarTreeModel, this);
    ui->verticalLayout_calendarEditor->addWidget(calendarEditor);
}

NewSeasonConfiguratorWindow::~NewSeasonConfiguratorWindow()
{
    delete ui;
}

void NewSeasonConfiguratorWindow::on_pushButton_submit_clicked()
{
    accept();
}

SeasonCalendar NewSeasonConfiguratorWindow::getCalendar() const
{
    return calendar;
}

void NewSeasonConfiguratorWindow::setCalendar(const SeasonCalendar &newCalendar)
{
    calendar = newCalendar;
}

CalendarEditorTreeModel *NewSeasonConfiguratorWindow::getCalendarTreeModel() const
{
    return calendarTreeModel;
}

void NewSeasonConfiguratorWindow::setCalendarTreeModel(CalendarEditorTreeModel *newCalendarTreeModel)
{
    calendarTreeModel = newCalendarTreeModel;
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

