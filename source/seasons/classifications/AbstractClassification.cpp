#include "AbstractClassification.h"

AbstractClassification::AbstractClassification()
{

}

bool AbstractClassification::getSaveStageResults() const
{
    return saveStageResults;
}

void AbstractClassification::setSaveStageResults(bool newSaveStageResults)
{
    saveStageResults = newSaveStageResults;
}

short AbstractClassification::getPointsType() const
{
    return pointsType;
}

void AbstractClassification::setPointsType(short newPointsType)
{
    pointsType = newPointsType;
}

short AbstractClassification::getClassificationType() const
{
    return classificationType;
}

void AbstractClassification::setClassificationType(short newClassificationType)
{
    classificationType = newClassificationType;
}
