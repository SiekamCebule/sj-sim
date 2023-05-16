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

    void updateToBeatLineDistance();
    void updateToAdvanceLineDistance();
    void updateActualCompetitorPointsToTheLeader();
    void updateLastQualifiedResult();

private:
    QVector<QVector<Jumper *>> roundsJumpers;

public:
    static QVector<Jumper *> getFilteredJumpersVector(QVector<Jumper *> *previousRoundJumpers, CompetitionResults * results, CompetitionRules * rules, int round, const QSet<uint> &fallQualified);

public:
    QVector<Jumper *> getFirstRoundJumpers() {return roundsJumpers[0];}
    void setFirstRoundJumpers(QVector<Jumper *> jumpers);
    QVector<QVector<Jumper *> > getRoundsJumpers() const;
    QVector<QVector<Jumper *> > & getRoundsJumpersReference();
    void setRoundsJumpers(const QVector<QVector<Jumper *> > &newRoundsJumpers);
};

#endif // INDIVIDUALCOMPETITIONMANAGER_H
