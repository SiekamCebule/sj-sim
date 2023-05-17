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

    void checkRoundEnd();
    void checkCompetitionEnd();

    void updateToBeatLineDistance();
    void updateToAdvanceLineDistance();
    void updateActualCompetitorPointsToTheLeader();
    void updateLastQualifiedResult();

private:
    QVector<QVector<Jumper *>> roundsJumpers;

public:
    QVector<Jumper *> & getFirstRoundJumpersReference() {return roundsJumpers[0];}
    QVector<Jumper *> & getActualRoundJumpersReference() {return roundsJumpers[actualRound];}
    void setFirstRoundJumpers(QVector<Jumper *> jumpers);

    QVector<QVector<Jumper *> > getRoundsJumpers() const;
    QVector<QVector<Jumper *> > & getRoundsJumpersReference();
    void setRoundsJumpers(const QVector<QVector<Jumper *> > &newRoundsJumpers);
};

#endif // INDIVIDUALCOMPETITIONMANAGER_H
