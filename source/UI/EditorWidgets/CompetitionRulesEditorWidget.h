#ifndef COMPETITIONRULESEDITORWIDGET_H
#define COMPETITIONRULESEDITORWIDGET_H

#include <QWidget>
#include "../../competitions/CompetitionRules.h"

namespace Ui {
class CompetitionRulesEditorWidget;
}

class CompetitionRulesEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompetitionRulesEditorWidget(QWidget *parent = nullptr);
    ~CompetitionRulesEditorWidget();

    void resetInputs();
    void fillCompetitionRulesInputs();
    void fillRoundsInputs(bool setup = false);

    QString getNameFromInput();
    QVector<RoundInfo> getRoundsFromInput();
    bool getHas95HSRuleFromInput();
    bool getHasWindCompensations();
    bool getHasGateCompensations();
    bool getHasJudgesPoints();
    bool getHasDsq();
    int getJumpersCountInTeam();
    short getCompetitionTypeFromInput();

    CompetitionRules getCompetitionRulesFromWidgetInputs();

signals:
    void jumpersCountInTeamChanged();
    void competitionTypeChanged();
    void changed();

private:
    Ui::CompetitionRulesEditorWidget *ui;

    CompetitionRules * competitionRules;
    void setupConnectsForWidgetChange();
    void removeConnectsForWidgetChange();

public:
    CompetitionRules *getCompetitionRules() const;
    void setCompetitionRules(CompetitionRules *newCompetitionRules);
};

#endif // COMPETITIONRULESEDITORWIDGET_H
