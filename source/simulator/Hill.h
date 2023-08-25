#ifndef HILL_H
#define HILL_H

#include "../utilities/ClassWithID.h"
#include "../utilities/ClassWithCharacteristics.h"
#include "Characteristic.h"

#include <QString>
#include <QSet>
#include <QPixmap>
#include <QJsonObject>
#include <QByteArray>

class Hill : public ClassWithID, public ClassWithCharacteristics
{
public:
    Hill(const QString &name = "", const QString &countryCode = "", double KPoint = 0, double HSPoint = 0, double pointsForMeter = 0, double pointsForKPoint = 0, double pointsForFrontWind = 0, double pointsForBackWind = 0, double pointsForGate = 0, double takeoffEffect = 0, double flightEffect = 0, double realHS = 0, bool autoPointsForKPoint = false, bool autoPointsForMeter = false, bool autoPointsForBackWind = false);

    enum HillType{
        Small,
        Average,
        Normal,
        Large,
        Big,
        Flying
    };

private:
    QString name;
    QString countryCode;

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

    bool autoPointsForKPoint;
    bool autoPointsForMeter;
    bool autoPointsForBackWind;

public:
    static QJsonObject getJsonObject(const Hill &hill);
    static Hill getFromJson(QJsonObject obj);
    static QVector<Hill> getVectorFromJson(const QByteArray & bytes);
    static void setupHillsFlagPixmaps(QVector<Hill> & hills);

    static double calculatePointsForMeter(double KPoint);
    static double calculatePointsForKPoint(double KPoint);
    static double calculatePointsForBackWindBy50PercentsOfFrontWind(double pointsForFrontWind);
    static double calculateBestTakeoffHeightLevel(Hill * hill);
    static double calculateBestFlightHeightLevel(Hill * hill);

    int getHillType();
    double getKAndRealHSDifference();

    double getLandingChanceChangeByHillProfile(double distance, short landingType);
    double getLandingImbalanceChangeByHillProfile(double distance);

    void updateCountryFlagPixmap(const QString &countryCode);

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
    bool getAutoPointsForKPoint() const;
    void setAutoPointsForKPoint(bool newAutoPointsForKPoint);
    bool getAutoPointsForMeter() const;
    void setAutoPointsForMeter(bool newAutoPointsForMeter);
    bool getAutoPointsForBackWind() const;
    void setAutoPointsForBackWind(bool newAutoPointsForBackWind);
};

#endif // HILL_H
