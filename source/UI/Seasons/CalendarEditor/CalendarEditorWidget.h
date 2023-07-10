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
    explicit CalendarEditorWidget(CalendarEditorTableModel *model, QVector<Classification> * classificationsList, QWidget *parent = nullptr);
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
    QAction * action_edit;
    Hill defaultHill;

    QVector<Classification> * classificationsList;

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
    void editActionTriggered();
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    void execMultipleHillEditDialog(QSet<int> * rows, int column);
    void execMultipleTrainingsEditDialog(QSet<int> * rows, int column);
    void execMultipleCompetitionRulesEditDialog(QSet<int> * rows, int column);
    void execMultipleTrialRoundsEditDialog(QSet<int> * rows, int column);
    void execMultipleClassificationsEditDialog(QSet<int> * rows, int column);
    void multipleEditCompetitionTypes(QSet<int> * rows, int column);
    void multipleEditSerieTypes(QSet<int> * rows, int column);

public:
    CalendarEditorTableModel *getModel() const;
    void setModel(CalendarEditorTableModel *newModel);
    QTableView * getTableView();
    QVector<Classification> *getClassificationsList() const;
    void setClassificationsList(QVector<Classification> *newClassificationsList);
    CompetitionInfoEditorWidget *getCompetitionInfoEditor() const;
};

#endif // CALENDAREDITORWIDGET_H
