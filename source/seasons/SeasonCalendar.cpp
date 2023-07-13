#include "SeasonCalendar.h"
#include "../utilities/functions.h"
#include "../global/SeasonDatabaseObjectsManager.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

extern SeasonDatabaseObjectsManager seasonObjectsManager;

SeasonCalendar::SeasonCalendar()
{

}

SeasonCalendar::~SeasonCalendar()
{
    for(auto & comp : competitions)
        delete comp;
}

void SeasonCalendar::fixCompetiitonsClassifications()
{
    for(auto & comp : competitions){
        for(auto & classification : comp->getClassificationsReference()){
            if(MyFunctions::vectorContains(classifications, classification) == false){
                //Trzeba usunąć classification z classificationsList ponieważ wcześniej classification zostało usunięte
                MyFunctions::removeFromVector(comp->getClassificationsReference(), classification);
            }
        }
    }
}

void SeasonCalendar::loadCalendarFromJson(QJsonObject &json)
{
    classifications.clear();
    competitions.clear();

    QJsonArray classificationsArray = json.value("classifications").toArray();
    for(auto val : classificationsArray){
        Classification c = Classification::getFromJson(val.toObject());
        classifications.push_back(c);
        seasonObjectsManager.addObject(&c);
    }

    QJsonArray competitionsArray = json.value("competitions").toArray();
    for(auto val : competitionsArray){
        CompetitionInfo * c = new CompetitionInfo(CompetitionInfo::getFromJson(val.toObject()));
        competitions.push_back(c);
        seasonObjectsManager.addObject(c);
    }
}

QVector<Classification> SeasonCalendar::getClassifications() const
{
    return classifications;
}

QVector<Classification> &SeasonCalendar::getClassificationsReference()
{
    return classifications;
}

void SeasonCalendar::setClassifications(const QVector<Classification> &newClassifications)
{
    classifications = newClassifications;
}

void SeasonCalendar::setCompetitions(const QVector<CompetitionInfo *> &newCompetitions)
{
    competitions = newCompetitions;
}

QVector<CompetitionInfo *> SeasonCalendar::getCompetitions() const
{
    return competitions;
}

QVector<CompetitionInfo *> &SeasonCalendar::getCompetitionsReference()
{
    return competitions;
}
