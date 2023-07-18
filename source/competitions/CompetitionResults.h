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
    CompetitionResults(CompetitionInfo * competitionInfo = nullptr);

private:
    CompetitionInfo * competitionInfo;
    QVector<CompetitionSingleResult> results;
    QVector<QVector<JumpData *>> constructRoundsJumps();

public:
    CompetitionResults constructRoundsResults(QVector<int> rounds);
    void updateRoundsJumps();

    static CompetitionResults getFromJson(QJsonObject obj);
    static QJsonObject getJsonObject(CompetitionResults & results);

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

    int howManyJumpersJumpedInTeam(Team * team);

    void updatePositions();
    void sortInDescendingOrder();
    void sortInAscendingOrder();
};

#endif // COMPETITIONRESULTS_H
