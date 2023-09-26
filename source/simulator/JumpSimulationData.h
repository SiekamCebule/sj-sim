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
    int getDSQProbability() const;
    void setDSQProbability(int newDSQProbability);
    double getInrunSnow() const;
    void setInrunSnow(double newInrunSnow);

private:
    double flightRating;
    double takeoffRating;
    int DSQProbability;
    double inrunSnow; //0-10
};

#endif // JUMPSIMULATIONDATA_H
