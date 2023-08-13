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
        int type = comp->getRulesPointer()->getCompetitionType();
        for(auto & classification : comp->getClassificationsReference()){
            if(classification == nullptr)
                MyFunctions::removeFromVector(comp->getClassificationsReference(), classification);
            if(MyFunctions::vectorContainsByID(classifications, classification) == false || type != classification->getClassificationType() && classification->getPunctationType() == Classification::PointsForPlaces){
                MyFunctions::removeFromVectorByID(comp->getClassificationsReference(), classification->getID());
            }
        }
    }
}

void SeasonCalendar::fixCompetitionsHills(QVector<Hill *> *hillsList)
{
    for(auto & comp : competitions){
        ulong hillID = comp->getHill()->getID();
        bool contains = false;
        Hill * hillWhichContains = nullptr;
        for(auto & hill : qAsConst(*hillsList)){
            if(hill->getID() == hillID){
                contains = true;
                hillWhichContains = hill;
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

void SeasonCalendar::fixAdvancementCompetitions()
{
    for(auto & comp : competitions)
    {
        if(comp->getAdvancementCompetition() != nullptr)
        {
            if(MyFunctions::vectorContains(competitions, comp->getAdvancementCompetition()) == false
                || comp->getAdvancementCompetition()->getRulesPointer()->getCompetitionType() != comp->getRulesPointer()->getCompetitionType())
            {
                comp->setAdvancementCompetition(nullptr);
            }
        }
    }
}

void SeasonCalendar::fixAdvancementClassifications()
{
    for(auto & comp : competitions)
    {
        if(comp->getAdvancementClassification() != nullptr)
        {
            if(MyFunctions::vectorContains(classifications, comp->getAdvancementClassification()) == false
                || comp->getAdvancementClassification()->getClassificationType() != comp->getRulesPointer()->getCompetitionType())
            {
                comp->setAdvancementClassification(nullptr);
            }
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
        calendar.getClassificationsReference().push_back(Classification::getFromJson(val.toObject()));
    }
    seasonObjectsManager.fill(&calendar.getClassificationsReference());

    QJsonArray competitionsArray = json.value("competitions").toArray();
    for(auto val : competitionsArray){
        CompetitionInfo * c = new CompetitionInfo(CompetitionInfo::getFromJson(val.toObject()));
        calendar.getCompetitionsReference().push_back(c);
        seasonObjectsManager.addObject(c);
    }
    calendar.updateCompetitionsQualifyingCompetitions();

    for(auto val : classificationsArray){
        Classification * classification = static_cast<Classification *>(seasonObjectsManager.getObjectByID(val.toObject().value("id").toString().toULong()));
        for(auto & singleResult : classification->getResultsReference())
        {
            singleResult->setClassification(classification);
            QJsonArray singleResultsArray = val.toObject().value("results").toArray();
            for(auto jsonRes : singleResultsArray)
            {
                if(jsonRes.toObject().value("id").toString().toULong() == singleResult->getID())
                {
                    QJsonArray compsIds = jsonRes.toObject().value("competitions-results-ids").toArray();
                    for(auto id : compsIds)
                        singleResult->getCompetitionsResultsReference().push_back(static_cast<CompetitionResults *>(seasonObjectsManager.getObjectByID(id.toString().toULong())));
                }
            }
            singleResult->updateSingleResults();
        }
        classification->sortInDescendingOrder();
    }

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

int SeasonCalendar::getCompetitionMainIndex(QVector<CompetitionInfo *> &competitions, CompetitionInfo *competition)
{
    int index = 0;
    for(auto & comp : competitions)
    {
        if(comp == competition)
            return index;
        if(comp->getSerieType() == CompetitionInfo::Competition || comp->getSerieType() == CompetitionInfo::Qualifications)
            index++;
    }
    return index;
}

CompetitionInfo *SeasonCalendar::getMainCompetitionByIndex(QVector<CompetitionInfo *> &competitions, int index)
{
    int i=0;
    for(auto & comp : competitions)
    {
        if(comp->getSerieType() == CompetitionInfo::Competition || comp->getSerieType() == CompetitionInfo::Qualifications){
            if(i == index)
                return comp;
            i++;
        }
    }
    return nullptr;
}

QVector<Classification *> SeasonCalendar::getClassifications() const
{
    return classifications;
}

QVector<Classification *> &SeasonCalendar::getClassificationsReference()
{
    return classifications;
}

void SeasonCalendar::setClassifications(const QVector<Classification *> &newClassifications)
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
