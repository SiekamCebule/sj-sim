#include "JumpSimulationData.h"

JumpSimulationData::JumpSimulationData()
{

}

void JumpSimulationData::reset()
{
    flightRating = flightRating = judgesRating = DSQProbability = 0;
}

double JumpSimulationData::getTakeoffRating() const
{
    return flightRating;
}

void JumpSimulationData::setTakeoffRating(double newTakeoffRating)
{
    flightRating = newTakeoffRating;
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
