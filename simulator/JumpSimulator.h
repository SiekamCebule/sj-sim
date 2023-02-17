#ifndef JUMPSIMULATOR_H
#define JUMPSIMULATOR_H

#include <QVector>
#include <QPair>

#include "Jumper.h"
#include "JumpData.h"
#include "ConditionsInfo.h"
#include "Hill.h"
#include "Landing.h"

#include <QMap>

class Competition;

class JumpSimulator
{
public:
    JumpSimulator(Jumper *jumper = nullptr, const ConditionsInfo & conditionsInfo = ConditionsInfo(), Hill *hill = nullptr);
private:
    Jumper * jumper;
    JumperSkills * jumperSkills; //automatycznie od jumper
    ConditionsInfo conditionsInfo;
    Hill * hill;

public:
    //skok
    void simulateJump();
    void resetTemporaryParameters();

    bool jumperCharacteristicsContains(const Characteristic & characteristics);

    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    Hill *getHill() const;
    void setHill(Hill *newHill);
    double getTakeoffRating() const;
    void setTakeoffRating(double newTakeoffRating);
    double getFlightRating() const;
    void setFlightRating(double newFlightRating);
    JumperSkills *getJumperSkills() const;

    void updateJumperSkills();

    QVector<double> getJudges() const;
    void setJudges(const QVector<double> &newJudges);

    JumpData getJumpData();
    void setJumpData(const JumpData &newJumpData);

    ConditionsInfo  & getConditionsInfo();
    void setConditionsInfo(const ConditionsInfo &newConditionsInfo);

private:
    void generateTakeoffRating();
    void generateFlightRating();
    double getMultiplierForFlightStyleEffect();

    void generateDistance();
    void generateWindEffects(); // np. zmiana odległości przez wiatr, albo w przyszłości jakieś nierówności w locie przez taki boczny wiatr.
    void generateLanding();
    void generateJudges();
    void calculateCompensations();
    void calculatePoints();

    //symulacja
    double takeoffRating;
    double flightRating;
    double judgesRating;

    JumpData jumpData;
    void setupJumpData();

};

#endif // JUMPSIMULATOR_H
