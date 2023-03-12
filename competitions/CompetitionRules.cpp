#include "CompetitionRules.h"

#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>

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

QVector<CompetitionRules> CompetitionRules::getCompetitionRulesVectorFromJson(const QByteArray &bytes)
{
    QVector<CompetitionRules> rulesVector;
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(bytes, &error);
    if(error.error != QJsonParseError::NoError)
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Błąd przy wczytytywaniu zasad konkursów", "Nie udało się wczytać zawodników z pliku userData/GlobalDatabase/globalCompetitionsRules.json\nTreść błędu: " + error.errorString(), QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return rulesVector;
    }
    QJsonObject object = document.object();
    QJsonValue value = object.value("competitionsRules");
    QJsonArray array = value.toArray();
    for(const auto & val : array)
    {
        QJsonObject obj = val.toObject();
        CompetitionRules rules;
        rules.setName(obj.value("name").toString());
        rules.setHas95HSRule(obj.value("95-hs-rule").toBool());
        rules.setHasWindCompensations(obj.value("wind-compensations").toBool());
        rules.setHasGateCompensations(obj.value("gate-compensations").toBool());
        rules.setHasJudgesPoints(obj.value("judges-points").toBool());
        rules.setCompetitionType(obj.value("competition-type").toInt());

        QJsonArray roundsArray = obj.value("rounds").toArray();
        for(const auto & round : roundsArray)
        {
            RoundInfo roundInfo;
            roundInfo.setCount(round.toObject().value("count").toInt());
            rules.getEditableRounds().push_back(roundInfo);
        }
        rulesVector.push_back(rules);
    }
    return rulesVector;
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
