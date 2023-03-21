#include "JumpSimulationData.h"

JumpSimulationData::JumpSimulationData()
{

}

void JumpSimulationData::reset()
{
    takeoffRating = flightRating = judgesRating = DSQProbability = 0;
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

int JumpSimulationData::getDSQProbability() const
{
    return DSQProbability;
}

void JumpSimulationData::setDSQProbability(int newDSQProbability)
{
    DSQProbability = newDSQProbability;
}
