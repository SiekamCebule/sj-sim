#include "Hill.h"

Hill::Hill(const QString &name, const QString &country, int KPoint, int HSPoint, double pointsForMeter, double pointsForFrontWind, double pointsForGate, double tableHeight, double baseSpeed, double speedForGate, const QSet<QString> &characteristics) : name(name),
    country(country),
    KPoint(KPoint),
    HSPoint(HSPoint),
    pointsForMeter(pointsForMeter),
    pointsForFrontWind(pointsForFrontWind),
    pointsForGate(pointsForGate),
    tableHeight(tableHeight),
    baseSpeed(baseSpeed),
    speedForGate(speedForGate),
    characteristics(characteristics)
{
    pointsForBackWind = 0;
    recordDistance = 0;
}

double Hill::getTableHeight() const
{
    return tableHeight;
}

void Hill::setTableHeight(double newTableHeight)
{
    tableHeight = newTableHeight;
}

double Hill::getSpeedForGate() const
{
    return speedForGate;
}

void Hill::setSpeedForGate(double newSpeedForGate)
{
    speedForGate = newSpeedForGate;
}

double Hill::getBaseSpeed() const
{
    return baseSpeed;
}

void Hill::setBaseSpeed(double newBaseSpeed)
{
    baseSpeed = newBaseSpeed;
}

void Hill::insertCharacteristic(const QString &text)
{
    characteristics.insert(text);
}

void Hill::removeCharacteristic(const QString &text)
{
    characteristics.remove(text);
}

double Hill::getLandingHillHeight(double distance)
{

}

QString Hill::getName() const
{
    return name;
}

void Hill::setName(const QString &newName)
{
    name = newName;
}

QString Hill::getCountry() const
{
    return country;
}

void Hill::setCountry(const QString &newCountry)
{
    country = newCountry;
}

double Hill::getRecordDistance() const
{
    return recordDistance;
}

void Hill::setRecordDistance(double newRecordDistance)
{
    recordDistance = newRecordDistance;
}

int Hill::getKPoint() const
{
    return KPoint;
}

void Hill::setKPoint(int newKPoint)
{
    KPoint = newKPoint;
}

int Hill::getHSPoint() const
{
    return HSPoint;
}

void Hill::setHSPoint(int newHSPoint)
{
    HSPoint = newHSPoint;
}

double Hill::getPointsForMeter() const
{
    return pointsForMeter;
}

void Hill::setPointsForMeter(double newPointsForMeter)
{
    pointsForMeter = newPointsForMeter;
}

double Hill::getPointsForBackWind() const
{
    return pointsForBackWind;
}

void Hill::setPointsForBackWind(double newPointsForBackWind)
{
    pointsForBackWind = newPointsForBackWind;
}

void Hill::calculatePointsForBackWind()
{
    pointsForBackWind = pointsForFrontWind * 1.21;
}

double Hill::getPointsForGate() const
{
    return pointsForGate;
}

void Hill::setPointsForGate(double newPointsForGate)
{
    pointsForGate = newPointsForGate;
}

QSet<QString> Hill::getCharacteristics() const
{
    return characteristics;
}

void Hill::setCharacteristics(const QSet<QString> &newCharacteristics)
{
    characteristics = newCharacteristics;
}

double Hill::getPointsForFrontWind() const
{
    return pointsForFrontWind;
}

void Hill::setPointsForFrontWind(double newPointsForFrontWind)
{
    pointsForFrontWind = newPointsForFrontWind;
}
