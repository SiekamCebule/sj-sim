#ifndef JUMPSIMULATOR_H
#define JUMPSIMULATOR_H

#include <QList>
#include <QPair>

#include "Jumper.h"
#include "JumpData.h"
#include "ConditionsInfo.h"
#include "Hill.h"

class Competition;

class JumpSimulator
{
public:
    JumpSimulator(Jumper *jumper = nullptr, ConditionsInfo *conditionsInfo = nullptr, Hill *hill = nullptr, Competition *competition = nullptr);
private:
    Jumper * jumper;
    ConditionsInfo * conditionsInfo;
    Hill * hill;
    Competition * competition;

public:
    //skok
    void simulateJump();

    JumpData getJumpData();

    bool jumperCharacteristicsContains(QString characteristics);

    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    ConditionsInfo *getConditionsInfo() const;
    void setConditionsInfo(ConditionsInfo *newConditionsInfo);
    Hill *getHill() const;
    void setHill(Hill *newHill);
    Competition *getCompetition() const;
    void setCompetition(Competition *newCompetition);

    double getDistance() const;
    double getSpeed() const;
    double getHeight() const;
    short getAerodynamicPosition() const;

private:
    //skok
    void simulateInrun();
    void simulateTakeoff();
    void setAerodynamicPositionAfterTakeoff();
    void simulateFlight();

    // parametry symulacji
    double takeoffDuration;

    double distance; // w metrach
    double speed; // w km/h
    double height; // w metrach
    short aerodynamicPosition; // od 1 do 50

    double takeoffMistakeHeightEffect;
    double takeoffMistakeSpeedEffect;
    short takeoffMistakeAerodynamicPositionEffect;

    double heightAboveLandingHill; // Wysokość nad zeskokiem. Np zmniejsza się nad bulą, spowrotem zwiększa się aż do punktu K gdzie powoli się zmniejsza.
    bool isLanding;
};

#endif // JUMPSIMULATOR_H
