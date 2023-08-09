#include "Classification.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QJsonValue>

#include "../global/SeasonDatabaseObjectsManager.h"
extern SeasonDatabaseObjectsManager seasonObjectsManager;

Classification::Classification(QString name) : name(name),
    ClassWithID()
{
    classificationType = 0;
    punctationType = 0;
}

Classification::~Classification()
{

}

void Classification::updatePositions()
{
    double previousResultPoints = 0;
    int actualPosition = 1;
    int add = 1;
    int i=0;
    for(auto & result : results){
        if(i==0){
            result->setPosition(1);
            previousResultPoints = result->getPointsSum();
            i++;
            continue;
        }

        if(previousResultPoints == result->getPointsSum())
        {
            result->setPosition(actualPosition);
            add += 1;
        }
        else{
            actualPosition += add;
            add = 1;
            result->setPosition(actualPosition);
        }

        previousResultPoints = result->getPointsSum();
        i++;
    }
}

void Classification::sortInDescendingOrder()
{
    std::stable_sort(results.begin(), results.end(), comparePointers);
    updatePositions();
}

void Classification::sortInAscendingOrder()
{
    //std::sort(results.begin(), results.end(), std::less<ClassificationSingleResult *>());
    updatePositions();
}


ClassificationSingleResult *Classification::getResultOfIndividualJumper(Jumper *jumper)
{
    for(auto & result : results)
    {
        if(result->getJumper() == jumper){
            return result;
        }
    }
    return nullptr;
}

ClassificationSingleResult *Classification::getResultOfTeam(QString teamCode)
{
    for(auto & result : results)
    {
        if(result->getTeamCode() == teamCode){
            return result;
        }
    }
    return nullptr;
}

ClassificationSingleResult *Classification::getResultByIndex(int index)
{
    return const_cast<ClassificationSingleResult *>(results[index]);
}

QVector<Classification *> Classification::getSpecificTypeClassifications(QVector<Classification *> classifications, int type)
{
    QVector<Classification *> toReturn;
    for(auto & classification : classifications)
    {
        if(classification->getClassificationType() == type)
            toReturn.push_back(classification);
    }
    return toReturn;
}

Classification * Classification::getFromJson(QJsonObject obj)
{
    Classification * classification = new Classification();
    classification->setID(obj.value("id").toString().toULong());
    classification->setName(obj.value("name").toString());
    classification->setClassificationType(obj.value("classification-type").toInt());
    classification->setPunctationType(obj.value("punctation-type").toInt());
    QJsonArray array = obj.value("points-for-places").toArray();
    int i=1;
    for(auto val : array){
        classification->getPointsForPlacesReference().insert(i, val.toDouble());
        i++;
    }

    QJsonArray resultsArray = obj.value("results").toArray();
    for(auto val : resultsArray)
    {
        classification->getResultsReference().push_back(new ClassificationSingleResult(ClassificationSingleResult::getFromJson(val.toObject())));
        classification->getResultsReference().last()->setClassification(classification);
    }
    seasonObjectsManager.fill(&classification->getResultsReference());

    return classification;
}

QJsonObject Classification::getJsonObject(Classification * classification)
{
    QJsonObject obj;
    obj.insert("id", QString::number(classification->getID()));
    obj.insert("name", classification->getName());
    obj.insert("classification-type", classification->getClassificationType());
    obj.insert("punctation-type", classification->getPunctationType());
    QJsonArray pointsForPlacesArray;
    for(int i=1; i<=classification->getPointsForPlacesReference().count(); i++){
        pointsForPlacesArray.push_back(classification->getPointsForPlacesReference().value(i));
    }
    obj.insert("points-for-places", pointsForPlacesArray);

    QJsonArray resultsArray;
    for(auto result : classification->getResultsReference())
    {
        resultsArray.push_back(ClassificationSingleResult::getJsonObject(*result));
    }
    obj.insert("results", resultsArray);

    return obj;
}

QVector<ClassificationSingleResult *> Classification::getResults() const
{
    return results;
}

QVector<ClassificationSingleResult *> &Classification::getResultsReference()
{
    return results;
}

void Classification::setResults(const QVector<ClassificationSingleResult *> &newResults)
{
    results = newResults;
}

short Classification::getPunctationType() const
{
    return punctationType;
}

void Classification::setPunctationType(short newPunctationType)
{
    punctationType = newPunctationType;
}

QString Classification::getName() const
{
    return name;
}

void Classification::setName(const QString &newName)
{
    name = newName;
}

QMap<int, double> Classification::getPointsForPlaces() const
{
    return pointsForPlaces;
}

QMap<int, double> &Classification::getPointsForPlacesReference()
{
    return pointsForPlaces;
}

void Classification::setPointsForPlaces(const QMap<int, double> &newPointsForPlaces)
{
    pointsForPlaces = newPointsForPlaces;
}

short Classification::getClassificationType() const
{
    return classificationType;
}

void Classification::setClassificationType(short newClassificationType)
{
    classificationType = newClassificationType;
}
