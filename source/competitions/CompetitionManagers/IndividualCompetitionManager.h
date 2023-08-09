#ifndef INDIVIDUALCOMPETITIONMANAGER_H
#define INDIVIDUALCOMPETITIONMANAGER_H

#include <QVector>
#include <QObject>

#include "../AbstractCompetitionManager.h"
#include "../../simulator/Jumper.h"
#include "../CompetitionSingleResult.h"
#include "../CompetitionResults.h"
#include "../CompetitionInfo.h"

class IndividualCompetitionManager : public AbstractCompetitionManager
{
    Q_OBJECT
public:
    IndividualCompetitionManager();

    void setupNextRound();
    void setupStartListStatusesForActualRound();
    void updateCompetitorsAdvanceStatuses();

    QVector<Jumper *> getFilteredJumpersForNextRound();
    static QVector<Jumper *> getFilteredJumpersAfterQualifications(CompetitionInfo *competition, QVector<Jumper> &jumpers);
    static void setStartListOrderByClassification(QVector<Jumper *> &jumpers, Classification * classification);
    static void setStartListOrderByCompetitionResults(QVector<Jumper *> &jumpers, CompetitionInfo * competition);

private:
    QVector<QVector<Jumper *>> roundsJumpers;

public:
    QVector<Jumper *> & getFirstRoundJumpersReference() {return roundsJumpers[0];}
    QVector<Jumper *> & getActualRoundJumpersReference() {return roundsJumpers[actualRound - 1];}
    void setFirstRoundJumpers(QVector<Jumper *> jumpers);

    QVector<QVector<Jumper *> > getRoundsJumpers() const;
    QVector<QVector<Jumper *> > & getRoundsJumpersReference();
    void setRoundsJumpers(const QVector<QVector<Jumper *> > &newRoundsJumpers);
    QVector<StartListCompetitorStatus> getStartListStatuses() const;
};

#endif // INDIVIDUALCOMPETITIONMANAGER_H
