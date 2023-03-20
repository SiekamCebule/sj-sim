#ifndef GLOBALSIMULATIONSETTINGS_H
#define GLOBALSIMULATIONSETTINGS_H


class GlobalSimulationSettings
{
private:
    GlobalSimulationSettings();
    ~GlobalSimulationSettings();
    static GlobalSimulationSettings * m_globalSimulationSettings;

private:
    //takeoff rating random
    //characteristics bonuses
    double takeoffRatingTakeoffHeightAbsValue;
    //base random
    double takeoffRatingBaseRandomBaseValue;
    double takeoffRatingBaseRandomDeviationBaseValue;
    double takeoffRatingBaseRandomDeviationSubstractCharacteristicDivider;
    //log random
    double takeoffRatingLogRandomBaseValue;
    double takeoffRatingLogRandomDeviationBaseValue;
    double takeoffRatingLogRandomDeviationJumpsEqualityDivider;

    //flight rating random
    //characteristics bonuses
    double flightRatingFlightHeightAbsValue;
    //base random
    double flightRatingBaseRandomBaseValue;
    double flightRatingBaseRandomDeviationBaseValue;
    double flightRatingBaseRandomDeviationSubstractCharacteristicDivider;
    //log random
    double flightRatingLogRandomBaseValue;
    double flightRatingLogRandomDeviationBaseValue;
    double flightRatingLogRandomDeviationJumpsEqualityDivider;

public:
    GlobalSimulationSettings(GlobalSimulationSettings &) = delete;
    static GlobalSimulationSettings * get();

    bool writeToFile();
    bool loadFromFile();

    double getTakeoffRatingTakeoffHeightAbsValue() const;
    void setTakeoffRatingTakeoffHeightAbsValue(double newTakeoffRatingTakeoffHeightAbsValue);
    double getTakeoffRatingBaseRandomBaseValue() const;
    void setTakeoffRatingBaseRandomBaseValue(double newTakeoffRatingBaseRandomBaseValue);
    double getTakeoffRatingBaseRandomDeviationBaseValue() const;
    void setTakeoffRatingBaseRandomDeviationBaseValue(double newTakeoffRatingBaseRandomDeviationBaseValue);
    double getTakeoffRatingBaseRandomDeviationSubstractCharacteristicDivider() const;
    void setTakeoffRatingBaseRandomDeviationSubstractCharacteristicDivider(double newTakeoffRatingBaseRandomDeviationSubstractCharacteristicDivider);
    double getTakeoffRatingLogRandomBaseValue() const;
    void setTakeoffRatingLogRandomBaseValue(double newTakeoffRatingLogRandomBaseValue);
    double getTakeoffRatingLogRandomDeviationBaseValue() const;
    void setTakeoffRatingLogRandomDeviationBaseValue(double newTakeoffRatingLogRandomDeviationBaseValue);
    double getTakeoffRatingLogRandomDeviationJumpsEqualityDivider() const;
    void setTakeoffRatingLogRandomDeviationJumpsEqualityDivider(double newTakeoffRatingLogRandomDeviationJumpsEqualityDivider);

    double getFlightRatingBaseRandomBaseValue() const;
    void setFlightRatingBaseRandomBaseValue(double newFlightRatingBaseRandomBaseValue);
    double getFlightRatingBaseRandomDeviationBaseValue() const;
    void setFlightRatingBaseRandomDeviationBaseValue(double newFlightRatingBaseRandomDeviationBaseValue);
    double getFlightRatingBaseRandomDeviationSubstractCharacteristicDivider() const;
    void setFlightRatingBaseRandomDeviationSubstractCharacteristicDivider(double newFlightRatingBaseRandomDeviationSubstractCharacteristicDivider);
    double getFlightRatingLogRandomBaseValue() const;
    void setFlightRatingLogRandomBaseValue(double newFlightRatingLogRandomBaseValue);
    double getFlightRatingLogRandomDeviationBaseValue() const;
    void setFlightRatingLogRandomDeviationBaseValue(double newFlightRatingLogRandomDeviationBaseValue);
    double getFlightRatingLogRandomDeviationJumpsEqualityDivider() const;
    void setFlightRatingLogRandomDeviationJumpsEqualityDivider(double newFlightRatingLogRandomDeviationJumpsEqualityDivider);
    double getFlightRatingFlightHeightAbsValue() const;
    void setFlightRatingFlightHeightAbsValue(double newFlightRatingFlightHeightAbsValue);
};

#endif // GLOBALSIMULATIONSETTINGS_H
