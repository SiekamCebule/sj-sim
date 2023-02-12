#ifndef JUMPSIMULATOR_H
#define JUMPSIMULATOR_H

#include <QList>
#include <QPair>

#include "Jumper.h"
#include "JumpData.h"
#include "ConditionsInfo.h"
#include "Hill.h"

#include <QMap>

class Competition;

class JumpSimulator
{
public:
    JumpSimulator(Jumper *jumper = nullptr, ConditionsInfo *conditionsInfo = nullptr, Hill *hill = nullptr, Competition *competition = nullptr);
private:
    Jumper * jumper;
    JumperSkills * jumperSkills; //automatycznie od jumper
    ConditionsInfo * conditionsInfo;
    Hill * hill;
    Competition * competition;

public:
    //skok
    void simulateJump();
    void resetTemporaryParameters();

    JumpData getJumpData();

    bool jumperCharacteristicsContains(const Characteristic & characteristics);

    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    ConditionsInfo *getConditionsInfo() const;
    void setConditionsInfo(ConditionsInfo *newConditionsInfo);
    Hill *getHill() const;
    void setHill(Hill *newHill);
    Competition *getCompetition() const;
    void setCompetition(Competition *newCompetition);
    double getTakeoffRating() const;
    void setTakeoffRating(double newTakeoffRating);
    double getFlightRating() const;
    void setFlightRating(double newFlightRating);
    JumperSkills *getJumperSkills() const;
    double getDistance() const;
    void setDistance(double newDistance);

    void updateJumperSkills();
private:
    void generateTakeoffRating();
    void generateFlightRating();
    double getMultiplierForFlightStyleEffect();

    void generateDistance();
    void generateWindEffects(); // np. zmiana odległości przez wiatr, albo w przyszłości jakieś nierówności w locie przez taki boczny wiatr.

    //symulacja
    double takeoffRating;
    double flightRating;
    double distance;
};

#endif // JUMPSIMULATOR_H
