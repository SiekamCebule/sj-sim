#include "JumpSimulationData.h"

JumpSimulationData::JumpSimulationData() :
ClassWithID()
{
    reset();
}

void JumpSimulationData::reset()
{
    takeoffRating = flightRating = judgesRating = DSQProbability = inrunSnow = landingDifficulty = 0;
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

double JumpSimulationData::getInrunSnow() const
{
    return inrunSnow;
}

void JumpSimulationData::setInrunSnow(double newInrunSnow)
{
    inrunSnow = newInrunSnow;
}

double JumpSimulationData::getLandingDifficulty() const
{
    return landingDifficulty;
}

void JumpSimulationData::setLandingDifficulty(double newLandingDifficulty)
{
    landingDifficulty = newLandingDifficulty;
}
