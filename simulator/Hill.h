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
    Hill(const QString &name = "", const QString &countryCode = "", double KPoint = 0, double HSPoint = 0, double pointsForFrontWind = 0, double pointsForGate = 0, double takeoffEffect = 0, double flightEffect = 0, const QSet<Characteristic> &characteristics = QSet<Characteristic>());

private:
    QString name, countryCode;

    double KPoint;
    double HSPoint; // Jeśli nie ma cechy charakterystycznej, to HSPoint == realHS

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
    static double calculatePointsForMeter(double KPoint);
    static double calculatePointsForKPoint(double KPoint);
    static double calculatePointsForBackWindBy21PercentsOfFrontWind(double pointsForFrontWind);

    double getKAndRealHSDifference();

    double getLandingChanceChangeByHillProfile(double distance, short landingType);
    double getLandingImbalanceChangeByHillProfile(double distance);

    QString getName() const;
    void setName(const QString &newName);
    QString getCountryCode() const;
    void setCountryCode(const QString &newCountryCode);
    double getKPoint() const;
    void setKPoint(double newKPoint);
    double getHSPoint() const;
    void setHSPoint(double newHSPoint);
    double getPointsForMeter() const;
    void setPointsForMeter(double newPointsForMeter);
    double getPointsForFrontWind() const;
    void setPointsForFrontWind(double newPointsForFrontWind);
    double getPointsForBackWind() const;
    void setPointsForBackWind(double newPointsForBackWind);
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
