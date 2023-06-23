#ifndef COMPETITIONINFOEDITORWIDGET_H
#define COMPETITIONINFOEDITORWIDGET_H

#include <QWidget>
#include "../EditorWidgets/CompetitionRulesEditorWidget.h"
#include "../../competitions/CompetitionInfo.h"
#include "../../global/CountryFlagsManager.h"

namespace Ui {
class CompetitionInfoEditorWidget;
}

class CompetitionInfoEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompetitionInfoEditorWidget(CompetitionInfo * competitionInfo, QVector<Hill> * hillsList, QWidget *parent = nullptr);
    ~CompetitionInfoEditorWidget();

    void fillInputs();
    void resetInputs();
    void setupHillsComboBox();
    void setReadOnly(bool ok);

private:
    Ui::CompetitionInfoEditorWidget *ui;

    CompetitionRulesEditorWidget * rulesEditor;
    CompetitionInfo * competitionInfo;
    QVector<Hill> * hillsList;

public:
    CompetitionRulesEditorWidget *getRulesEditor() const;
    void setRulesEditor(CompetitionRulesEditorWidget *newRulesEditor);
    CompetitionInfo *getCompetitionInfo() const;
    void setCompetitionInfo(CompetitionInfo *newCompetitionInfo);
    QVector<Hill> *getHillsList() const;
    void setHillsList(QVector<Hill> *newHillsList);
};

#endif // COMPETITIONINFOEDITORWIDGET_H
