#ifndef NEWSEASONCONFIGURATORWINDOW_H
#define NEWSEASONCONFIGURATORWINDOW_H

#include <QDialog>
#include <QAction>
#include "../DatabaseEditor/ListModels/JumpersListModel.h"
#include "CalendarEditor/CalendarEditorWidget.h"

class DatabaseItemsListView;

namespace Ui {
class NewSeasonConfiguratorDialog;
}

class NewSeasonConfiguratorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NewSeasonConfiguratorWindow(QWidget *parent = nullptr);
    ~NewSeasonConfiguratorWindow();

private slots:
    void on_pushButton_submit_clicked();

private:
    Ui::NewSeasonConfiguratorDialog *ui;

    QVector<Jumper> jumpers;
    DatabaseItemsListView * jumpersListView;

    SeasonCalendar calendar;
    CalendarEditorWidget * calendarEditor;
    CalendarEditorTableModel * calendarTableModel;

public:
    QVector<Jumper> getJumpers() const;
    QVector<Jumper>& getJumpersReference();
    void setJumpers(const QVector<Jumper> &newJumpers);
    CalendarEditorWidget *getCalendarEditor() const;
    void setCalendarEditor(CalendarEditorWidget *newCalendarEditor);
    SeasonCalendar getCalendar() const;
    void setCalendar(const SeasonCalendar &newCalendar);
    CalendarEditorTableModel *getCalendarTableModel() const;
    void setCalendarTableModel(CalendarEditorTableModel *newCalendarTableModel);
};

#endif // NEWSEASONCONFIGURATORWINDOW_H
