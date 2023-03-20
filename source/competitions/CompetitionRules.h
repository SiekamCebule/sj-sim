#ifndef COMPETITIONRULES_H
#define COMPETITIONRULES_H

#include "../utilities/ClassWithID.h"
#include "RoundInfo.h"
#include <QVector>
#include <QString>
#include <QJsonObject>
#include <QByteArray>

class CompetitionRules : ClassWithID
{
public:
    CompetitionRules(const QString & name = "");
    enum CompetitionType{
        Individual,
        Team
    };

private:
    QString name;
    QVector<RoundInfo> rounds;
    bool has95HSRule;
    bool hasWindCompensations;
    bool hasGateCompensations;
    bool hasJudgesPoints;
    bool hasDsq;
    short competitionType;
    int jumpersInTeamCount;

public:
    static QJsonObject getCompetitionRulesJsonObject(const CompetitionRules &competitionRules, bool savePointsAndCompensations, bool saveCompetitionType, bool save95HSRuleAndDsq, bool saveJumpersInTeamCount, bool saveRounds);
    static QVector<CompetitionRules> getCompetitionRulesVectorFromJson(const QByteArray & bytes);

    bool getHas95HSRule() const;
    void setHas95HSRule(bool newHas95HSRule);
    bool getHasWindCompensations() const;
    void setHasWindCompensations(bool newHasWindCompensations);
    bool getHasGateCompensations() const;
    void setHasGateCompensations(bool newHasGateCompensations);
    bool getHasJudgesPoints() const;
    void setHasJudgesPoints(bool newHasJudgesPoints);
    short getCompetitionType() const;
    void setCompetitionType(short newCompetitionType);
    QVector<RoundInfo> getRounds() const;
    void setRounds(const QVector<RoundInfo> &newRounds);
    QVector<RoundInfo> & getEditableRounds();
    QString getName() const;
    void setName(const QString &newName);
    int getJumpersInTeamCount() const;
    void setJumpersInTeamCount(int newJumpersInTeamCount);
    bool getHasDsq() const;
    void setHasDsq(bool newHasDsq);
};

#endif // COMPETITIONRULES_H
