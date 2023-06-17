#ifndef JUMPSIMULATIONDATA_H
#define JUMPSIMULATIONDATA_H

#include "../utilities/ClassWithID.h"

class JumpSimulationData : ClassWithID
{
    friend class JumpSimulator;
public:
    JumpSimulationData();

    void reset();

    double getTakeoffRating() const;
    void setTakeoffRating(double newTakeoffRating);
    double getFlightRating() const;
    void setFlightRating(double newFlightRating);
    double getJudgesRating() const;
    void setJudgesRating(double newJudgesRating);
    int getDSQProbability() const;
    void setDSQProbability(int newDSQProbability);
    double getInrunSnow() const;
    void setInrunSnow(double newInrunSnow);
    double getLandingDifficulty() const;
    void setLandingDifficulty(double newLandingDifficulty);

private:
    double flightRating;
    double takeoffRating;
    double judgesRating;

    int DSQProbability;

    double inrunSnow; //0-10
    double landingDifficulty; //0-10
};

#endif // JUMPSIMULATIONDATA_H
