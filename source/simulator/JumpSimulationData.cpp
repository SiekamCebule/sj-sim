#include "JumpSimulationData.h"

JumpSimulationData::JumpSimulationData() :
ClassWithID()
{
    reset();
}

void JumpSimulationData::reset()
{
    takeoffRating = flightRating = DSQProbability = inrunSnow = 0;
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

int JumpSimulationData::getDSQProbability() const
{
    return DSQProbability;
}

void JumpSimulationData::setDSQProbability(int newDSQProbability)
{
    DSQProbability = newDSQProbability;
}

double JumpSimulationData::getInrunSnow() const
{
    return inrunSnow;
}

void JumpSimulationData::setInrunSnow(double newInrunSnow)
{
    inrunSnow = newInrunSnow;
}
