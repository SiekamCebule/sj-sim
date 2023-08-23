#include "Classification.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QJsonValue>
#include "../seasons/Season.h"
#include "../global/SeasonDatabaseObjectsManager.h"
#include "../global/MyRandom.h"
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

QMap<int, double> Classification::getAltPointsForPlaces() const
{
    return altPointsForPlaces;
}

QMap<int, double> &Classification::getAltPointsForPlacesReference()
{
    return altPointsForPlaces;
}

void Classification::setAltPointsForPlaces(const QMap<int, double> &newAltPointsForPlaces)
{
    altPointsForPlaces = newAltPointsForPlaces;
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
    QJsonArray altArray = obj.value("alt-points-for-places").toArray();
    int ii=1;
    for(auto val : altArray){
        classification->getAltPointsForPlacesReference().insert(ii, val.toDouble());
        ii++;
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

    QJsonArray altPointsForPlacesArray;
    for(int i=1; i<=classification->getAltPointsForPlacesReference().count(); i++){
        altPointsForPlacesArray.push_back(classification->getAltPointsForPlacesReference().value(i));
    }
    obj.insert("alt-points-for-places", altPointsForPlacesArray);

    QJsonArray resultsArray;
    for(auto result : classification->getResultsReference())
    {
        resultsArray.push_back(ClassificationSingleResult::getJsonObject(*result));
    }
    obj.insert("results", resultsArray);

    return obj;
}

QHash<QString, QHash<CompetitionInfo *, int> > Classification::constructTeamsArchiveResults(Season *season)
{

}

QHash<Jumper *, QHash<CompetitionInfo *, int>> Classification::constructJumpersArchiveResults(Season *season)
{
    QHash<Jumper *, QHash<CompetitionInfo *, int>> results;
    QHash<Jumper *, double> tempResultsToSort;

    for(auto & seasonCompetition : season->getCalendarReference().getCompetitionsReference())
    {
        if(seasonCompetition->getPlayed() == true)
        {
            if(seasonCompetition->getClassificationsReference().contains(this) == true)
            {
                QSet<Jumper *> remainingJumpers;
                for(auto & singleResult : seasonCompetition->getResultsReference().getResultsReference())
                {
                    tempResultsToSort[singleResult.getJumper()] = tempResultsToSort.value(singleResult.getJumper()) + singleResult.getPointsSum();
                    remainingJumpers.insert(singleResult.getJumper());
                }
                QHash<Jumper *, int> poss = CompetitionResults::getResultsWithPositionsForClassificationArchiveResults(tempResultsToSort);
                for(auto & singleResult : seasonCompetition->getResultsReference().getResultsReference())
                {
                    QHash<CompetitionInfo *, int> resultsMap;
                    if(results.contains(singleResult.getJumper()))
                    {
                        resultsMap = results.value(singleResult.getJumper());
                    }
                    else
                    {
                        resultsMap = QHash<CompetitionInfo *, int>();
                    }
                    resultsMap.insert(seasonCompetition, poss.value(singleResult.getJumper()));

                    results[singleResult.getJumper()] = resultsMap;
                    if(remainingJumpers.contains(singleResult.getJumper()))
                        remainingJumpers.remove(singleResult.getJumper());
                }
                for(auto & remainingJumper : remainingJumpers)
                {
                    QHash<CompetitionInfo *, int> resultsMap;
                    if(results.contains(remainingJumper))
                    {
                        resultsMap = results.value(remainingJumper);
                    }
                    else
                    {
                        resultsMap = QHash<CompetitionInfo *, int>();
                    }
                    resultsMap.insert(seasonCompetition, poss.value(remainingJumper));
                    results[remainingJumper] = resultsMap;
                }
            }
        }
        else break;
    }

    return results;
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
