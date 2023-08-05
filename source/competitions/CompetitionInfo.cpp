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
#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>
#include <QByteArray>
extern SeasonDatabaseObjectsManager seasonObjectsManager;

CompetitionInfo::CompetitionInfo(Hill *hill) : hill(hill),
    ClassWithID()
{
    exceptionalRoundsCount = (-1);
    cancelled = false;
    played = false;
    serieType = Competition;
    trialRound = nullptr;
    advancementClassification = nullptr;
    advancementCompetition = nullptr;
}

CompetitionInfo::~CompetitionInfo()
{
    /*for(auto & t : trainings)
        if(t != nullptr)
            delete t;
    if(trialRound != nullptr)
        delete trialRound;*/
}

void CompetitionInfo::updateQualifyingCompetitions(SeasonCalendar *calendar)
{
    for(auto & comp : calendar->getCompetitionsReference())
    {
        if(this == comp->getAdvancementCompetition())
        {
            this->getQualifyingCompetitionsReference().push_back(comp);
        }
    }
}

bool CompetitionInfo::saveToFile(QString dir, QString name)
{
    QDir d(dir);
    if(!d.exists())
        d.mkpath(".");
    QFile file(dir + name);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku ", "Nie udało się otworzyć pliku " + dir + name+ "\nUpewnij się, że istnieją odpowiednie foldery lub aplikacja ma odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
       qDebug()<<file.errorString();
 message.setModal(true);
        message.exec();
        return false;
    }
    QJsonDocument document;
    QJsonObject mainObject;
    mainObject.insert("competition", CompetitionInfo::getJsonObject(*this));
    document.setObject(mainObject);

    file.resize(0);
    file.write(document.toJson());
    file.close();
    return true;
}

bool CompetitionInfo::getPlayed() const
{
    return played;
}

void CompetitionInfo::setPlayed(bool newPlayed)
{
    played = newPlayed;
}

QVector<CompetitionInfo *> CompetitionInfo::getQualifyingCompetitions() const
{
    return qualifyingCompetitions;
}

QVector<CompetitionInfo *> &CompetitionInfo::getQualifyingCompetitionsReference()
{
    return qualifyingCompetitions;
}

void CompetitionInfo::setQualifyingCompetitions(const QVector<CompetitionInfo *> &newQualifyingCompetitions)
{
    qualifyingCompetitions = newQualifyingCompetitions;
}

CompetitionInfo *CompetitionInfo::getAdvancementCompetition() const
{
    return advancementCompetition;
}

void CompetitionInfo::setAdvancementCompetition(CompetitionInfo *newAdvancementCompetition)
{
    advancementCompetition = newAdvancementCompetition;
}

Classification *CompetitionInfo::getAdvancementClassification() const
{
    return advancementClassification;
}

void CompetitionInfo::setAdvancementClassification(Classification *newAdvancementClassification)
{
    advancementClassification = newAdvancementClassification;
}

CompetitionResults CompetitionInfo::getResults() const
{
    return results;
}

void CompetitionInfo::setResults(const CompetitionResults &newResults)
{
    results = newResults;
}

CompetitionResults &CompetitionInfo::getResultsReference()
{
    return results;
}

QJsonObject CompetitionInfo::getJsonObject(CompetitionInfo &competition)
{
    QJsonObject object;
    object.insert("id", QString::number(competition.getID()));
    object.insert("hill-id", QString::number(competition.getHill()->getID()));
    object.insert("results", CompetitionResults::getJsonObject(competition.getResultsReference()));
    object.insert("rules", CompetitionRules::getJsonObject(competition.getRules()));
    object.insert("serie-type", competition.getSerieType());
    object.insert("exceptional-rounds-count", competition.getExceptionalRoundsCount());
    object.insert("cancelled", competition.getCancelled());
    object.insert("played", competition.getPlayed());

    if(competition.getTrialRound() != nullptr)
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

    if(competition.getAdvancementClassification() != nullptr)
        object.insert("advancement-classification-id", QString::number(competition.getAdvancementClassification()->getID()));
    if(competition.getAdvancementCompetition() != nullptr)
        object.insert("advancement-competition-id", QString::number(competition.getAdvancementCompetition()->getID()));

    return object;
}

CompetitionInfo CompetitionInfo::getFromJson(const QJsonObject &json)
{
    CompetitionInfo comp;
    comp.setID(json.value("id").toString().toULong());
    comp.setHill(static_cast<Hill *>(seasonObjectsManager.getObjectByID(json.value("hill-id").toString().toULong())));
    comp.setResults(CompetitionResults::getFromJson(json.value("results").toObject()));
    comp.setRules(CompetitionRules::getFromJson(json.value("rules").toObject()));
    comp.setSerieType(json.value("serie-type").toInt());
    comp.setExceptionalRoundsCount(json.value("exceptional-rounds-count").toInt());
    comp.setCancelled(json.value("cancelled").toBool());
    comp.setPlayed(json.value("played").toBool());
    comp.setTrialRound(static_cast<CompetitionInfo *>(seasonObjectsManager.getObjectByID(json.value("trial-round-id").toString().toULong())));

    QJsonArray trainingsArray = json.value("training-ids").toArray();
    for(auto val : trainingsArray){
        comp.getTrainingsReference().push_back(static_cast<CompetitionInfo *>(seasonObjectsManager.getObjectByID(val.toString().toULong())));
    }

    QJsonArray classificationsArray = json.value("classifications-ids").toArray();
    for(auto val : classificationsArray){
        comp.getClassificationsReference().push_back(static_cast<Classification *>(seasonObjectsManager.getObjectByID(val.toString().toULong())));
    }

    SeasonDatabaseObjectsManager * sdom = &seasonObjectsManager;
    comp.setAdvancementClassification(static_cast<Classification *>(seasonObjectsManager.getObjectByID(json.value("advancement-classification-id").toString().toULong())));
    comp.setAdvancementCompetition(static_cast<CompetitionInfo *>(seasonObjectsManager.getObjectByID(json.value("advancement-competition-id").toString().toULong())));

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
