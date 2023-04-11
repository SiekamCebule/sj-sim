#ifndef INDIVIDUALCOMPETITIONMANAGER_H
#define INDIVIDUALCOMPETITIONMANAGER_H

#include <QVector>
#include <QObject>

#include "../AbstractCompetitionManager.h"
#include "../../simulator/Jumper.h"

class IndividualCompetitionResults;
class IndividualCompetitionSingleResult;

class IndividualCompetitionManager : public AbstractCompetitionManager
{
    Q_OBJECT
public:
    IndividualCompetitionManager(short type = CompetitionRules::Individual, int startingGate = 0);

    void simulateNext();
    bool checkRoundEnd();
    bool checkCompetitionEnd();

    void setupNextRound();
    void fillCompletedJumpsToStartOfRound();
    void updateToBeatDistance();
    void updateToAdvanceDistance();
    void updateLeaderResult();
    void updateLastQualifiedResult();
    void updateActualJumperPointsToTheLeader();

    int getCompetitiorsCountForActualRound();
    bool getSortStartListForActualRound();

private:
    QVector<Jumper *> startingJumpers;
    QVector<Jumper *> actualRoundJumpers;
    QSet<uint> resultsWithFallIds;
    QSet<uint> qualifiedBy95HSRule;

    IndividualCompetitionSingleResult * leaderResult;
    IndividualCompetitionSingleResult * lastQualifiedResult;

public:
    static QVector<Jumper *> getFilteredJumpersVector(QVector<Jumper *> *previousRoundJumpers, IndividualCompetitionResults * results, CompetitionRules * rules, int round, const QSet<uint> &fallQualified);

public:
    QVector<Jumper *> getStartingJumpers() const;
    QVector<Jumper *> * getStartingJumpersPointer();
    void setStartingJumpers(const QVector<Jumper *> &newStartingJumpers);
    void setStartingJumpers(const QVector<Jumper> * newStartingJumpers);
    QVector<Jumper *> *getActualRoundJumpersPointer() const;
    void setActualRoundJumpers(const QVector<Jumper *> &newActualRoundJumpers);
    void incrementActualJumperIndex();
    void decrementActualJumperIndex();
    QVector<Jumper *> getActualRoundJumpers() const;
    IndividualCompetitionSingleResult *getLeaderResult() const;
    void setLeaderResult(IndividualCompetitionSingleResult *newLeaderResult);
    QVector<Wind> getNextJumperWinds() const;
    void setNextJumperWinds(const QVector<Wind> &newNextJumperWinds);
    IndividualCompetitionSingleResult *getLastQualifiedResult() const;
    void setLastQualifiedResult(IndividualCompetitionSingleResult *newLastQualifiedResult);
    QSet<uint> getResultsWithFallIds() const;
    void setResultsWithFallIds(const QSet<uint> &newResultsWithFallIds);
    QSet<uint> getQualifiedBy95HSRule() const;
    void setQualifiedBy95HSRule(const QSet<uint> &newQualifiedBy95HSRule);
};

#endif // INDIVIDUALCOMPETITIONMANAGER_H
