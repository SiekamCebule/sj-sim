#ifndef INDIVIDUALCOMPETITIONMANAGER_H
#define INDIVIDUALCOMPETITIONMANAGER_H

#include <QVector>
#include <QObject>

#include "../AbstractCompetitionManager.h"
#include "../../simulator/Jumper.h"
#include "../CompetitionSingleResult.h"
#include "../CompetitionResults.h"

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

    void updateActualJumperPointsToTheLeader();

    int getCompetitiorsCountForActualRound();
    bool getSortStartListForActualRound();

private:
    QVector<QVector<Jumper *>> roundsStartingJumpers;

public:
    static QVector<Jumper *> getFilteredJumpersVector(QVector<Jumper *> *previousRoundJumpers, CompetitionResults * results, CompetitionRules * rules, int round, const QSet<uint> &fallQualified);

public:
    QVector<Jumper *> getFirstRoundStartingJumpers() {return roundsStartingJumpers[0];}
    void setFirstRoundStartingJumpers(QVector<Jumper *> jumpers);
    QVector<QVector<Jumper *> > getRoundsStartingJumpers() const;
    QVector<QVector<Jumper *> > & getRoundsStartingJumpersReference();
    void setRoundsStartingJumpers(const QVector<QVector<Jumper *> > &newRoundsStartingJumpers);
};

#endif // INDIVIDUALCOMPETITIONMANAGER_H
