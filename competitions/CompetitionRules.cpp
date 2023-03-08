#include "CompetitionRules.h"

#include <QJsonArray>

CompetitionRules::CompetitionRules(const QString & name) : name(name)
{
    has95HSRule = hasWindCompensations = hasGateCompensations = hasJudgesPoints = false;
    competitionType = 0;
}

QString CompetitionRules::getName() const
{
    return name;
}

void CompetitionRules::setName(const QString &newName)
{
    name = newName;
}

QJsonObject CompetitionRules::getCompetitionRulesJsonObject(CompetitionRules *competitionRules, bool savePointsAnd95Rule, bool saveCompetitionType, bool saveRounds)
{
    QJsonObject object;
    object.insert("name", competitionRules->getName());
    if(savePointsAnd95Rule == true){
        object.insert("95-hs-rule", competitionRules->getHas95HSRule());
        object.insert("wind-compensations", competitionRules->getHasWindCompensations());
        object.insert("gate-compensations", competitionRules->getHasGateCompensations());
        object.insert("judges-points", competitionRules->getHasJudgesPoints());
    }
    if(saveCompetitionType == true){
        object.insert("competition-type", competitionRules->getCompetitionType());
    }
    if(saveRounds == true){
        QJsonArray roundsArray;
        for(const auto & round : competitionRules->getRounds())
        {
            QJsonObject roundObject;
            roundObject.insert("count", round.getCount());
            roundsArray.push_back(roundObject);
        }
        object.insert("rounds", roundsArray);
    }

    return object;
}

QVector<RoundInfo> CompetitionRules::getRounds() const
{
    return rounds;
}

void CompetitionRules::setRounds(const QVector<RoundInfo> &newRounds)
{
    rounds = newRounds;
}

QVector<RoundInfo> &CompetitionRules::getEditableRounds()
{
    return rounds;
}

short CompetitionRules::getCompetitionType() const
{
    return competitionType;
}

void CompetitionRules::setCompetitionType(short newCompetitionType)
{
    competitionType = newCompetitionType;
}

bool CompetitionRules::getHasJudgesPoints() const
{
    return hasJudgesPoints;
}

void CompetitionRules::setHasJudgesPoints(bool newHasJudgesPoints)
{
    hasJudgesPoints = newHasJudgesPoints;
}

bool CompetitionRules::getHasGateCompensations() const
{
    return hasGateCompensations;
}

void CompetitionRules::setHasGateCompensations(bool newHasGateCompensations)
{
    hasGateCompensations = newHasGateCompensations;
}

bool CompetitionRules::getHasWindCompensations() const
{
    return hasWindCompensations;
}

void CompetitionRules::setHasWindCompensations(bool newHasWindCompensations)
{
    hasWindCompensations = newHasWindCompensations;
}

bool CompetitionRules::getHas95HSRule() const
{
    return has95HSRule;
}

void CompetitionRules::setHas95HSRule(bool newHas95HSRule)
{
    has95HSRule = newHas95HSRule;
}
