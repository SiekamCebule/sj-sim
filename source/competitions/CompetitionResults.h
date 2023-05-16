#ifndef COMPETITIONRESULTS_H
#define COMPETITIONRESULTS_H
#include "CompetitionSingleResult.h"
#include <QVector>

class CompetitionInfo;

class CompetitionResults
{
public:
    CompetitionResults(CompetitionInfo * competitionInfo = nullptr);
    ~CompetitionResults();

private:
    CompetitionInfo * competitionInfo;
    QVector<CompetitionSingleResult> results;

public:
    CompetitionInfo *getCompetitionInfo() const;
    void setCompetitionInfo(CompetitionInfo *newCompetitionInfo);
    QVector<CompetitionSingleResult> getResults() const;
    QVector<CompetitionSingleResult> & getResultsReference();
    void setResults(const QVector<CompetitionSingleResult> &newResults);

    CompetitionSingleResult * getResultOfIndividualJumper(Jumper * jumper);
    CompetitionSingleResult * getResultOfTeam(Team * team);
    CompetitionSingleResult * getResultByIndex(int index);

    void addJump(Jumper * jumper, JumpData & jump, int jumpNumber = -1);
    void addJump(Team * team, JumpData & jump, int jumpNumber = -1);
    void addJump(CompetitionSingleResult * result, JumpData & jump, int jumpNumber = -1);

    void updatePositions();
    void sortInDescendingOrder();
};

#endif // COMPETITIONRESULTS_H
