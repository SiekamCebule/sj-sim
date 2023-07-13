#include "CompetitionInfo.h"

#include "../simulator/Hill.h"
#include "../global/SeasonDatabaseObjectsManager.h"
#include "CompetitionRules.h"
#include "CompetitionResults.h"

#include <QDate>
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
extern SeasonDatabaseObjectsManager seasonObjectsManager;

CompetitionInfo::CompetitionInfo(Hill *hill) : hill(hill)
{
    exceptionalRoundsCount = (-1);
    cancelled = false;
    results = nullptr;
    serieType = Competition;
    trialRound = nullptr;
}

CompetitionInfo::~CompetitionInfo()
{
    if(results != nullptr){
        delete results;
    }
    /*for(auto & t : trainings)
        if(t != nullptr)
            delete t;
    if(trialRound != nullptr)
        delete trialRound;*/
}

QJsonObject CompetitionInfo::getJsonObject(CompetitionInfo &competition)
{
    QJsonObject object;
    object.insert("id", QString::number(competition.getID()));
    object.insert("hill-id", QString::number(competition.getHill()->getID()));
    object.insert("rules", CompetitionRules::getCompetitionRulesJsonObject(competition.getRules()));
    object.insert("serie-type", competition.getSerieType());
    object.insert("exceptional-rounds-count", competition.getExceptionalRoundsCount());
    object.insert("cancelled", competition.getCancelled());

    object.insert("trial-round-id", QString::number(competition.getTrialRound()->getID()));
    QJsonArray trainingsArray;
    for(auto & t : qAsConst(competition.getTrainingsReference())){
        trainingsArray.push_back(QString::number(t->getID()));
    }
    object.insert("training-ids", trainingsArray);

    QJsonArray classificationsArray;
    for(auto & c : qAsConst(competition.getClassificationsReference())){
        classificationsArray.push_back(QString::number(c->getID()));
    }
    object.insert("classifications-ids", classificationsArray);

    return object;
}

CompetitionInfo CompetitionInfo::getFromJson(const QJsonObject &json)
{
    CompetitionInfo comp;
    comp.setID(json.value("id").toString().toULong());
    comp.setHill(static_cast<Hill *>(seasonObjectsManager.getObjectByID(json.value("hill-id").toString().toULong())));
    comp.setRules(CompetitionRules::getFromJson(json.value("rules").toObject()));
    comp.setSerieType(json.value("serie-type").toInt());
    comp.setExceptionalRoundsCount(json.value("exceptional-rounds-count").toInt());
    comp.setCancelled(json.value("cancelled").toBool());
    comp.setTrialRound(static_cast<CompetitionInfo *>(seasonObjectsManager.getObjectByID(json.value("trial-round-id").toString().toULong())));

    QJsonArray trainingsArray = json.value("training-ids").toArray();
    for(auto val : trainingsArray){
        comp.getTrainingsReference().push_back(static_cast<CompetitionInfo *>(seasonObjectsManager.getObjectByID(val.toString().toULong())));
    }

    QJsonArray classificationsArray = json.value("classifications-ids").toArray();
    for(auto val : classificationsArray){
        comp.getClassificationsReference().push_back(static_cast<Classification *>(seasonObjectsManager.getObjectByID(val.toString().toULong())));
    }

    return comp;
}

QVector<Classification *> CompetitionInfo::getClassifications() const
{
    return classifications;
}

QVector<Classification *> &CompetitionInfo::getClassificationsReference()
{
    return classifications;
}

void CompetitionInfo::setClassifications(const QVector<Classification *> &newClassifications)
{
    classifications = newClassifications;
}

QVector<CompetitionInfo *> CompetitionInfo::getTrainings() const
{
    return trainings;
}

QVector<CompetitionInfo *> &CompetitionInfo::getTrainingsReference()
{
    return trainings;
}

void CompetitionInfo::setTrainings(const QVector<CompetitionInfo *> &newTrainings)
{
    trainings = newTrainings;
}

CompetitionInfo *CompetitionInfo::getTrialRound() const
{
    return trialRound;
}

void CompetitionInfo::setTrialRound(CompetitionInfo *newTrialRound)
{
    trialRound = newTrialRound;
}

bool CompetitionInfo::getCancelled() const
{
    return cancelled;
}

void CompetitionInfo::setCancelled(bool newCancelled)
{
    cancelled = newCancelled;
}

int CompetitionInfo::getExceptionalRoundsCount() const
{
    return exceptionalRoundsCount;
}

void CompetitionInfo::setExceptionalRoundsCount(int newExceptionalRoundsCount)
{
    exceptionalRoundsCount = newExceptionalRoundsCount;
}

short CompetitionInfo::getSerieType() const
{
    return serieType;
}

void CompetitionInfo::setSerieType(short newSerieType)
{
    serieType = newSerieType;
}

CompetitionResults *CompetitionInfo::getResults() const
{
    return results;
}

void CompetitionInfo::setResults(CompetitionResults *newResults)
{
    results = newResults;
}

Hill *CompetitionInfo::getHill() const
{
    return hill;
}

void CompetitionInfo::setHill(Hill *newHill)
{
    hill = newHill;
}

CompetitionRules CompetitionInfo::getRules() const
{
    return rules;
}

void CompetitionInfo::setRules(const CompetitionRules &newRules)
{
    rules = newRules;
}
