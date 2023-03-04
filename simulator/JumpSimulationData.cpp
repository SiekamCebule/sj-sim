#include "JumpSimulationData.h"

JumpSimulationData::JumpSimulationData()
{

}

void JumpSimulationData::reset()
{
    takeoffRating = flightRating = judgesRating = 0;
}

double JumpSimulationData::getTakeoffRating() const
{
    return takeoffRating;
}

void JumpSimulationData::setTakeoffRating(double newTakeoffRating)
{
    takeoffRating = newTakeoffRating;
}

double JumpSimulationData::getFlightRating() const
{
    return flightRating;
}

void JumpSimulationData::setFlightRating(double newFlightRating)
{
    flightRating = newFlightRating;
}

double JumpSimulationData::getJudgesRating() const
{
    return judgesRating;
}

void JumpSimulationData::setJudgesRating(double newJudgesRating)
{
    judgesRating = newJudgesRating;
}
