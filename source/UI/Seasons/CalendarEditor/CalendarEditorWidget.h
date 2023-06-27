#ifndef CALENDAREDITORWIDGET_H
#define CALENDAREDITORWIDGET_H

#include <QWidget>
#include <QTableView>
#include "CalendarEditorTableModel.h"
#include "../CompetitionInfoEditorWidget.h"

namespace Ui {
class CalendarEditorWidget;
}

class CalendarEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarEditorWidget(CalendarEditorTableModel *model, QWidget *parent = nullptr);
    ~CalendarEditorWidget();

signals:
    void needToUpdateModel();

private:
    Ui::CalendarEditorWidget *ui;

    CalendarEditorTableModel * calendarModel;
    SeasonCalendar * calendar;
    QAction * action_add;
    QAction * action_remove;
    QAction * action_up;
    QAction * action_down;
    Hill defaultHill;

    CompetitionInfoEditorWidget * competitionInfoEditor;
    CompetitionInfo * actualCompetition;
    int actualCompetitionIndex;
    ulong actualCompetitionID;
    void updateActualCompetitionByID();

    void debugCalendar();

private slots:
    void addActionTriggered();
    void removeActionTriggered();
    void upActionTriggered();
    void downActionTriggered();
    void on_tableView_doubleClicked(const QModelIndex &index);

public:
    CalendarEditorTableModel *getModel() const;
    void setModel(CalendarEditorTableModel *newModel);
    QTableView * getTableView();
};

#endif // CALENDAREDITORWIDGET_H
