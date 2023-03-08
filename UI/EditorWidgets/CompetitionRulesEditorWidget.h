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

    void fillCompetitionRulesInputs();
    void fillRoundsInputs();

    QString getNameFromInput();
    QVector<RoundInfo> getRoundsFromInput();
    bool getHas95HSRuleFromInput();
    bool getHasWindCompensations();
    bool getHasGateCompensations();
    bool getHasJudgesPoints();
    short getCompetitionTypeFromInput();

private:
    Ui::CompetitionRulesEditorWidget *ui;

    CompetitionRules * competitionRules;

public:
    CompetitionRules *getCompetitionRules() const;
    void setCompetitionRules(CompetitionRules *newCompetitionRules);
};

#endif // COMPETITIONRULESEDITORWIDGET_H
