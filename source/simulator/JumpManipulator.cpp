#include "JumpManipulator.h"

JumpManipulator::JumpManipulator()
{
    distanceRange = landingInstabilityRange = takeoffRatingRange = flightRatingRange = judgesRatingRange = averagedWindRange = QPair<double, double>(0, -1);
    distanceBonus = landingInstabilityBonus = takeoffRatingBonus = flightRatingBonus = judgesRatingBonus = averagedWindBonus = 0;
    averagedWindRangeEnabled = false;
    exactLandingType = exactDSQProbability = -1;
    exactJudges.fill(-1, 5);
}

bool JumpManipulator::getAveragedWindRangeEnabled() const
{
    return averagedWindRangeEnabled;
}

void JumpManipulator::setAveragedWindRangeEnabled(bool newAveragedWindRangeEnabled)
{
    averagedWindRangeEnabled = newAveragedWindRangeEnabled;
}

int JumpManipulator::getExactDSQProbability() const
{
    return exactDSQProbability;
}

void JumpManipulator::setExactDSQProbability(int newExactDSQProbability)
{
    exactDSQProbability = newExactDSQProbability;
}

QVector<Wind> JumpManipulator::getExactWinds() const
{
    return exactWinds;
}

void JumpManipulator::setExactWinds(const QVector<Wind> &newExactWinds)
{
    exactWinds = newExactWinds;
}

QVector<double> JumpManipulator::getExactJudges() const
{
    return exactJudges;
}

void JumpManipulator::setExactJudges(const QVector<double> &newExactJudges)
{
    exactJudges = newExactJudges;
}

int JumpManipulator::getExactLandingType() const
{
    return exactLandingType;
}

void JumpManipulator::setExactLandingType(int newExactLandingType)
{
    exactLandingType = newExactLandingType;
}

double JumpManipulator::getAveragedWindBonus() const
{
    return averagedWindBonus;
}

void JumpManipulator::setAveragedWindBonus(double newAveragedWindBonus)
{
    averagedWindBonus = newAveragedWindBonus;
}

double JumpManipulator::getJudgesRatingBonus() const
{
    return judgesRatingBonus;
}

void JumpManipulator::setJudgesRatingBonus(double newJudgesRatingBonus)
{
    judgesRatingBonus = newJudgesRatingBonus;
}

double JumpManipulator::getFlightRatingBonus() const
{
    return flightRatingBonus;
}

void JumpManipulator::setFlightRatingBonus(double newFlightRatingBonus)
{
    flightRatingBonus = newFlightRatingBonus;
}

double JumpManipulator::getTakeoffRatingBonus() const
{
    return takeoffRatingBonus;
}

void JumpManipulator::setTakeoffRatingBonus(double newTakeoffRatingBonus)
{
    takeoffRatingBonus = newTakeoffRatingBonus;
}

double JumpManipulator::getLandingInstabilityBonus() const
{
    return landingInstabilityBonus;
}

void JumpManipulator::setLandingInstabilityBonus(double newLandingInstabilityBonus)
{
    landingInstabilityBonus = newLandingInstabilityBonus;
}

double JumpManipulator::getDistanceBonus() const
{
    return distanceBonus;
}

void JumpManipulator::setDistanceBonus(double newDistanceBonus)
{
    distanceBonus = newDistanceBonus;
}

QPair<double, double> JumpManipulator::getAveragedWindRange() const
{
    return averagedWindRange;
}

void JumpManipulator::setAveragedWindRange(const QPair<double, double> &newAveragedWindRange)
{
    averagedWindRange = newAveragedWindRange;
}

QPair<double, double> JumpManipulator::getJudgesRatingRange() const
{
    return judgesRatingRange;
}

void JumpManipulator::setJudgesRatingRange(const QPair<double, double> &newJudgesRatingRange)
{
    judgesRatingRange = newJudgesRatingRange;
}

QPair<double, double> JumpManipulator::getFlightRatingRange() const
{
    return flightRatingRange;
}

void JumpManipulator::setFlightRatingRange(const QPair<double, double> &newFlightRatingRange)
{
    flightRatingRange = newFlightRatingRange;
}

QPair<double, double> JumpManipulator::getTakeoffRatingRange() const
{
    return takeoffRatingRange;
}

void JumpManipulator::setTakeoffRatingRange(const QPair<double, double> &newTakeoffRatingRange)
{
    takeoffRatingRange = newTakeoffRatingRange;
}

QPair<double, double> JumpManipulator::getLandingInstabilityRange() const
{
    return landingInstabilityRange;
}

void JumpManipulator::setLandingInstabilityRange(const QPair<double, double> &newLandingInstabilityRange)
{
    landingInstabilityRange = newLandingInstabilityRange;
}

QPair<double, double> JumpManipulator::getDistanceRange() const
{
    return distanceRange;
}

void JumpManipulator::setDistanceRange(const QPair<double, double> &newDistanceRange)
{
    distanceRange = newDistanceRange;
}
