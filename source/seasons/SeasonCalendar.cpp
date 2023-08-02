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
}

void SeasonCalendar::fixCompetitionsClassifications()
{
    for(auto & comp : competitions){
        for(auto & classification : comp->getClassificationsReference()){
            if(MyFunctions::vectorContainsByID(classifications, classification) == false){
                //Trzeba usunąć classification z classificationsList ponieważ wcześniej classification zostało usunięte
                MyFunctions::removeFromVectorByID(comp->getClassificationsReference(), classification->getID());
            }
        }
    }
}

void SeasonCalendar::fixCompetitionsHills(QVector<Hill> *hillsList)
{
    for(auto & comp : competitions){
        ulong hillID = comp->getHill()->getID();
        bool contains = false;
        Hill * hillWhichContains = nullptr;
        for(auto & hill : qAsConst(*hillsList)){
            if(hill.getID() == hillID){
                contains = true;
                hillWhichContains = const_cast<Hill*>(&hill);
                break;
            }
        }
        if(contains){
            comp->setHill(hillWhichContains);
        }
        else{
            comp->setHill(new Hill("Hill"));
        }
    }
}

void SeasonCalendar::updateCompetitionsQualifyingCompetitions()
{
    for(auto & comp : competitions)
    {
        comp->updateQualifyingCompetitions(this);
    }
}

SeasonCalendar SeasonCalendar::getFromJson(QJsonObject json)
{
    SeasonCalendar calendar;

    QJsonArray classificationsArray = json.value("classifications").toArray();
    for(auto val : classificationsArray){
        Classification c = Classification::getFromJson(val.toObject());
        calendar.getClassificationsReference().push_back(c);
    }
    seasonObjectsManager.fill(&calendar.getClassificationsReference());

    QJsonArray competitionsArray = json.value("competitions").toArray();
    for(auto val : competitionsArray){
        CompetitionInfo * c = new CompetitionInfo(CompetitionInfo::getFromJson(val.toObject()));
        calendar.getCompetitionsReference().push_back(c);
        seasonObjectsManager.addObject(c);
    }
    seasonObjectsManager.fill(&calendar.getCompetitionsReference());

    calendar.updateCompetitionsQualifyingCompetitions();

    return calendar;
}

QJsonObject SeasonCalendar::getJsonObject(SeasonCalendar &calendar)
{
    QJsonObject object;

    QJsonArray classificationsArray;
    for(auto & cls : qAsConst(calendar.getClassificationsReference()))
        classificationsArray.push_back(Classification::getJsonObject(cls));
    object.insert("classifications", classificationsArray);

    QJsonArray competitionsArray;
    for(auto & cmp : qAsConst(calendar.getCompetitionsReference()))
        competitionsArray.push_back(CompetitionInfo::getJsonObject(*cmp));
    object.insert("competitions", competitionsArray);

    return object;
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
