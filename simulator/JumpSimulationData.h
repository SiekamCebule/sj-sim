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

private:
    double takeoffRating;
    double flightRating;
    double judgesRating;
};

#endif // JUMPSIMULATIONDATA_H
