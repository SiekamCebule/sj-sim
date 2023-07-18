#include "Classification.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QJsonValue>

Classification::Classification(QString name) : name(name),
    ClassWithID()
{
    classificationType = 0;
    punctationType = 0;
}

Classification Classification::getFromJson(QJsonObject obj)
{
    Classification classification;
    classification.setID(obj.value("id").toString().toULong());
    classification.setName(obj.value("name").toString());
    classification.setClassificationType(obj.value("classification-type").toInt());
    classification.setPunctationType(obj.value("punctation-type").toInt());
    QJsonArray array = obj.value("points-for-places").toArray();
    int i=1;
    for(auto val : array){
        classification.getPointsForPlacesReference().insert(i, val.toInt());
        i++;
    }
    return classification;
}

QJsonObject Classification::getJsonObject(Classification classification)
{
    QJsonObject obj;
    obj.insert("id", QString::number(classification.getID()));
    obj.insert("name", classification.getName());
    obj.insert("classification-type", classification.getClassificationType());
    obj.insert("punctation-type", classification.getPunctationType());
    QJsonArray pointsForPlacesArray;
    for(int i=1; i<=classification.getPointsForPlacesReference().count(); i++){
        pointsForPlacesArray.push_back(classification.getPointsForPlacesReference().value(i));
    }
    obj.insert("points-for-places", pointsForPlacesArray);
    return obj;
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
