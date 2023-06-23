#include "Classification.h"

Classification::Classification()
{

}

bool Classification::getSaveStageResults() const
{
    return saveStageResults;
}

void Classification::setSaveStageResults(bool newSaveStageResults)
{
    saveStageResults = newSaveStageResults;
}

short Classification::getPointsType() const
{
    return pointsType;
}

void Classification::setPointsType(short newPointsType)
{
    pointsType = newPointsType;
}

short Classification::getClassificationType() const
{
    return classificationType;
}

void Classification::setClassificationType(short newClassificationType)
{
    classificationType = newClassificationType;
}
