#ifndef HILL_H
#define HILL_H

#include "../utilities/ClassWithID.h"
#include "../utilities/ClassWithCharacteristics.h"
#include "Characteristic.h"

#include <QString>
#include <QSet>

class Hill : public ClassWithID, public ClassWithCharacteristics
{
public:
    Hill(const QString &name = "", const QString &country = "", int KPoint = 0, int HSPoint = 0, double pointsForFrontWind = 0, double pointsForGate = 0, double takeoffEffect = 0, double flightEffect = 0, const QSet<Characteristic> &characteristics = QSet<Characteristic>());

private:
    QString name, country;
    double recordDistance;

    int KPoint;
    int HSPoint; // Jeśli nie ma cechy charakterystycznej, to HSPoint == realHS

    double pointsForMeter;
    double pointsForKPoint;
    double pointsForFrontWind;
    double pointsForBackWind; //obliczane na podstawie wiatru przedniego
    double pointsForGate;

    //symulacja
    double takeoffEffect;
    double flightEffect;
    double realHS; // "prawdziwa wielkość skoczni", tam gdzie robi się niebezpicznie

public:
    void setupPointsForMeter();
    void setupPointsForKPoint();

    double getKAndRealHSDifference();

    double getLandingChanceChangeByHillProfile(double distance, short landingType);
    double getLandingImbalanceChangeByHillProfile(double distance);

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
    QSet<Characteristic> getCharacteristics() const;
    void setCharacteristics(const QSet<Characteristic> &newCharacteristics);
    double getTakeoffEffect() const;
    void setTakeoffEffect(double newTakeoffEffect);
    double getFlightEffect() const;
    void setFlightEffect(double newFlightEffect);
    double getRealHS() const;
    void setRealHS(double newRealHS);
    void setRealHSByCharacteristic();
    double getPointsForKPoint() const;
    void setPointsForKPoint(double newPointsForKPoint);
};

#endif // HILL_H
