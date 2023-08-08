#ifndef COMPETITIONRESULTS_H
#define COMPETITIONRESULTS_H
#include "CompetitionSingleResult.h"
#include "../utilities/ClassWithID.h"
#include <QVector>
#include <QObject>
#include <QDebug>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>
#include <QByteArray>

class CompetitionInfo;

class CompetitionResults : public ClassWithID
{
public:
    CompetitionResults();

private:
    CompetitionInfo * competition;
    QVector<CompetitionSingleResult> results;
    QVector<QVector<JumpData *>> constructRoundsJumps(QVector<RoundInfo> * rounds);

public:
    CompetitionResults constructRoundsResults(QVector<RoundInfo> *roundsInfos, QVector<int> rounds);
    void updateRoundsJumps();

    static CompetitionResults getFromJson(QJsonObject obj);
    static QJsonObject getJsonObject(CompetitionResults & results);

public:
    QVector<CompetitionSingleResult> getResults() const;
    QVector<CompetitionSingleResult> & getResultsReference();
    void setResults(const QVector<CompetitionSingleResult> &newResults);

    CompetitionSingleResult * getResultOfIndividualJumper(Jumper * jumper);
    CompetitionSingleResult * getResultOfTeam(Team * team);
    CompetitionSingleResult * getResultByIndex(int index);

    void addJump(Jumper * jumper, JumpData & jump, int jumpNumber = -1);
    void addJump(Team * team, JumpData & jump, int jumpNumber = -1);
    void addJump(CompetitionSingleResult * result, JumpData & jump, int jumpNumber = -1);

    int howManyJumpersJumpedInTeam(Team * team);

    void updatePositions();
    void sortInDescendingOrder();
    void sortInAscendingOrder();
    CompetitionInfo *getCompetition() const;
    void setCompetition(CompetitionInfo *newCompetition);
};

#endif // COMPETITIONRESULTS_H
