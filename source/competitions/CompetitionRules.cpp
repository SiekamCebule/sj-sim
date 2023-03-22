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
    has95HSRule = hasWindCompensations = hasGateCompensations = hasJudgesPoints = hasDsq = false;
    competitionType = jumpersInTeamCount = windAverageCalculatingType = windCompensationDistanceEffect = 0;
}

short CompetitionRules::getWindCompensationDistanceEffect() const
{
    return windCompensationDistanceEffect;
}

void CompetitionRules::setWindCompensationDistanceEffect(short newWindCompensationDistanceEffect)
{
    windCompensationDistanceEffect = newWindCompensationDistanceEffect;
}

short CompetitionRules::getWindAverageCalculatingType() const
{
    return windAverageCalculatingType;
}

void CompetitionRules::setWindAverageCalculatingType(short newWindAverageCalculatingType)
{
    windAverageCalculatingType = newWindAverageCalculatingType;
}

bool CompetitionRules::getHasDsq() const
{
    return hasDsq;
}

void CompetitionRules::setHasDsq(bool newHasDsq)
{
    hasDsq = newHasDsq;
}

int CompetitionRules::getJumpersInTeamCount() const
{
    return jumpersInTeamCount;
}

void CompetitionRules::setJumpersInTeamCount(int newJumpersInTeamCount)
{
    jumpersInTeamCount = newJumpersInTeamCount;
}

QString CompetitionRules::getName() const
{
    return name;
}

void CompetitionRules::setName(const QString &newName)
{
    name = newName;
}

QJsonObject CompetitionRules::getCompetitionRulesJsonObject(const CompetitionRules &competitionRules)
{
    QJsonObject object;
    object.insert("name", competitionRules.getName());
    object.insert("wind-compensations", competitionRules.getHasWindCompensations());
    object.insert("gate-compensations", competitionRules.getHasGateCompensations());
    object.insert("judges-points", competitionRules.getHasJudgesPoints());
    object.insert("jumpers-in-team-count", competitionRules.getJumpersInTeamCount());
    object.insert("95-hs-rule", competitionRules.getHas95HSRule());
    object.insert("dsq", competitionRules.getHasDsq());
    object.insert("competition-type", competitionRules.getCompetitionType());
    object.insert("wind-compensation-distance-effect", competitionRules.getWindCompensationDistanceEffect());
    object.insert("wind-average-calculating-type", competitionRules.getWindAverageCalculatingType());

    QJsonArray roundsArray;
    QVector<RoundInfo> rds = competitionRules.getRounds();
    rds.detach();
    for(QVector<RoundInfo>::iterator it = rds.begin(); it != rds.end(); ++it)
    {
        QJsonObject roundObject;
        roundObject.insert("count", it->getCount());
        roundsArray.push_back(roundObject);
    }
    object.insert("rounds", roundsArray);

    return object;
}

QVector<CompetitionRules> CompetitionRules::getCompetitionRulesVectorFromJson(const QByteArray &bytes)
{
    QVector<CompetitionRules> rulesVector;
    rulesVector.clear();
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
    if(!value.isArray())
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Błąd przy wczytytywaniu zasad konkursów", "Prawdopodobnie w tym pliku nie ma listy z zasadami konkursów\nUpewnij się, że wybrałeś właściwy plik!");
        message.setModal(true);
        message.exec();
        return rulesVector;
    }
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
        rules.setHasDsq(obj.value("dsq").toBool());
        rules.setJumpersInTeamCount(obj.value("jumpers-in-team-count").toInt());
        rules.setCompetitionType(obj.value("competition-type").toInt());
        rules.setWindCompensationDistanceEffect(obj.value("wind-compensation-distance-effect").toInt());
        rules.setWindAverageCalculatingType(obj.value("wind-average-calculating-type").toInt());

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