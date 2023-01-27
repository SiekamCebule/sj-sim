#ifndef HILL_H
#define HILL_H

#include "../utilities/ClassWithID.h"

#include <QString>

class Hill : public ClassWithID
{
public:
    Hill(const QString &name = "", const QString &country = "", int KPoint = 0, int HSPoint = 0, double pointsForMeter = 0, double pointsForFrontWind = 0, double pointsForGate = 0, double tableHeight = 0, double baseSpeed = 0, double speedForGate = 0, const QSet<QString> &characteristics = QSet<QString>());

private:
    QString name, country;
    double recordDistance;

    int KPoint, HSPoint;

    double pointsForMeter;
    double pointsForFrontWind;
    double pointsForBackWind; //obliczane na podstawie wiatru przedniego
    double pointsForGate;

    //symulacja
    double tableHeight; //wysokość progu
    double baseSpeed; //bazowa prędkość na najniższej belce
    double speedForGate; //predkosc dodawana/odejmowana za każdą belkę.



    QSet<QString> characteristics;

public:
    void insertCharacteristic(const QString & text);
    void removeCharacteristic(const QString & text);

    QString getName() const;
    void setName(const QString &newName);
    QString getCountry() const;
    void setCountry(const QString &newCountry);
    double getRecordDistance() const;
    void setRecordDistance(double newRecordDistance);
    int getKPoint() const;
    void setKPoint(int newKPoint);
    int getHSPoint() const;
    void setHSPoint(int newHSPoint);
    double getPointsForMeter() const;
    void setPointsForMeter(double newPointsForMeter);
    double getPointsForFrontWind() const;
    void setPointsForFrontWind(double newPointsForFrontWind);
    double getPointsForBackWind() const;
    void setPointsForBackWind(double newPointsForBackWind);
    void calculatePointsForBackWind();
    double getPointsForGate() const;
    void setPointsForGate(double newPointsForGate);
    QSet<QString> getCharacteristics() const;
    void setCharacteristics(const QSet<QString> &newCharacteristics);
    double getBaseSpeed() const;
    void setBaseSpeed(double newBaseSpeed);
    double getSpeedForGate() const;
    void setSpeedForGate(double newSpeedForGate);
    double getTableHeight() const;
    void setTableHeight(double newTableHeight);
};

#endif // HILL_H
