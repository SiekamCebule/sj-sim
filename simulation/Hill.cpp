#include "Hill.h"


Hill::Hill(const QString &name, const QString &country, int KPoint, int HSPoint, double pointsForFrontWind, double pointsForGate, double tableHeight, double baseSpeed, double speedForGate, const QSet<Characteristic> &characteristics) : name(name),
    country(country),
    KPoint(KPoint),
    HSPoint(HSPoint),
    pointsForFrontWind(pointsForFrontWind),
    pointsForGate(pointsForGate),
    tableHeight(tableHeight),
    baseSpeed(baseSpeed),
    speedForGate(speedForGate),
    characteristics(characteristics)
{

}

void Hill::insertCharacteristic(const Characteristic &characteristic)
{
    characteristics.insert(characteristic);
}

void Hill::insertCharacteristic(short level, const QString &type)
{
    characteristics.insert(Characteristic(level, type));
}

void Hill::removeCharacteristic(Characteristic &characteristic)
{
    characteristics.remove(characteristic);
}

void Hill::removeCharacteristic(const QString &type)
{
    characteristics.remove(Characteristic(0, type));
}

short Hill::getLevelOfCharacteristic(const QString &characteristicType)
{
    for(QSet<Characteristic>::iterator it = characteristics.begin(); it != characteristics.end(); it++)
    {
        if(it->getType() == characteristicType)
            return it->getLevel();
    }
}

void Hill::setPointsForMeter(double newPointsForMeter)
{
    pointsForMeter = newPointsForMeter;
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

double Hill::getRelativeHeightSubstractFromHillProfile(double distance)
{
    double knollLength = 0.34 * KPoint; // długość buli
}

void Hill::setupPointsForMeter()
{
    if(KPoint <= 24)
        pointsForMeter = 4.8;
    else if(KPoint <= 29)
         pointsForMeter = 4.4;
    else if(KPoint <= 34)
         pointsForMeter = 4.0;
    else if(KPoint <= 39)
         pointsForMeter = 3.6;
    else if(KPoint <= 49)
         pointsForMeter = 3.2;
    else if(KPoint <= 59)
         pointsForMeter = 2.8;
    else if(KPoint <= 69)
         pointsForMeter = 2.4;
    else if(KPoint <= 79)
         pointsForMeter = 2.2;
    else if(KPoint <= 99)
         pointsForMeter = 2.0;
    else if(KPoint <= 169)
         pointsForMeter = 1.8;
    else if(KPoint >= 170)
         pointsForMeter = 1.2;
    else pointsForMeter = 0;
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

QSet<Characteristic> Hill::getCharacteristics() const
{
    return characteristics;
}

void Hill::setCharacteristics(const QSet<Characteristic> &newCharacteristics)
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
