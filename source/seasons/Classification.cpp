#include "Classification.h"

Classification::Classification(QString name) : name(name)
{
    classificationType = 0;
    punctationType = 0;
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

QMap<int, int> Classification::getPointsForPlaces() const
{
    return pointsForPlaces;
}

QMap<int, int> &Classification::getPointsForPlacesReference()
{
    return pointsForPlaces;
}

void Classification::setPointsForPlaces(const QMap<int, int> &newPointsForPlaces)
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
