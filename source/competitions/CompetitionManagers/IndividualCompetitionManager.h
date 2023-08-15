#ifndef INDIVIDUALCOMPETITIONMANAGER_H
#define INDIVIDUALCOMPETITIONMANAGER_H

#include <QVector>
#include <QObject>

#include "../AbstractCompetitionManager.h"
#include "../../simulator/Jumper.h"
#include "../CompetitionSingleResult.h"
#include "../CompetitionResults.h"
#include "../CompetitionInfo.h"
#include "KORoundManager.h"

class IndividualCompetitionManager : public AbstractCompetitionManager
{
    Q_OBJECT
public:
    IndividualCompetitionManager();

    void setupNextRound();
    void setupStartListStatusesForActualRound();
    void updateCompetitorsAdvanceStatuses();

    QVector<Jumper *> getFilteredJumpersForNextRound(bool checkKO = false);
    static QVector<Jumper *> getFilteredJumpersAfterQualifications(CompetitionInfo *competition, QVector<Jumper *> &jumpers);
    static QVector<Jumper *> getFilteredJumpersByClassification(CompetitionInfo *competition, Classification * classification, QVector<Jumper *> & jumpers);

    QVector<KOGroup> getFilteredGroupsForNextRound();

    static void setStartListOrderByClassification(QVector<Jumper *> &jumpers, Classification * classification);
    static void setStartListOrderByCompetitionResults(QVector<Jumper *> &jumpers, CompetitionInfo * competition);
    static void setStartListOrderByDefault(QVector<Jumper *> * jumpersList, QVector<Jumper *> & startList);

private:
    QVector<QVector<Jumper *>> roundsJumpers;
    QVector<QVector<KOGroup>> roundsKOGroups;
    KORoundManager * KOManager;

public:
    QVector<Jumper *> & getFirstRoundJumpersReference() {return roundsJumpers[0];}
    QVector<Jumper *> & getActualRoundJumpersReference() {return roundsJumpers[actualRound - 1];}
    void setFirstRoundJumpers(QVector<Jumper *> jumpers);
    QVector<QVector<Jumper *> > getRoundsJumpers() const;
    QVector<QVector<Jumper *> > & getRoundsJumpersReference();
    void setRoundsJumpers(const QVector<QVector<Jumper *> > &newRoundsJumpers);
    QVector<StartListCompetitorStatus> getStartListStatuses() const;
    QVector<QVector<KOGroup> > & getRoundsKOGroupsReference();
    QVector<QVector<KOGroup> > getRoundsKOGroups() const;
    void setRoundsKOGroups(const QVector<QVector<KOGroup> > &newRoundsKOGroups);
    QVector<KOGroup> & getFirstRoundKOGroupsReference() {return roundsKOGroups[0];}
    QVector<KOGroup> & getActualRoundKOGroupsReference() {return roundsKOGroups[actualRound - 1];}
    KORoundManager *getKOManager() const;
    void setKOManager(KORoundManager *newKOManager);
};

#endif // INDIVIDUALCOMPETITIONMANAGER_H
