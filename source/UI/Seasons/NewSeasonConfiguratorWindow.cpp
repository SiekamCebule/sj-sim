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

    calendarTableModel = new CalendarEditorTableModel(&calendar, &GlobalDatabase::get()->getEditableGlobalHills(), &GlobalDatabase::get()->getEditableCompetitionRules(), this);
    calendarEditor = new CalendarEditorWidget(calendarTableModel, this);
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

