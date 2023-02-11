#include "JumpSimulator.h"
#include "../utilities/functions.h"
#include "JumpMistake.h"

#include <QDebug>
#include <QRandomGenerator>
#include <math.h>

JumpSimulator::JumpSimulator(Jumper *jumper, ConditionsInfo *conditionsInfo, Hill *hill, Competition *competition) : jumper(jumper),
    conditionsInfo(conditionsInfo),
    hill(hill),
    competition(competition)
{
    if(jumper != nullptr)
        jumperSkills = jumper->getJumperSkills();

    takeoffRating = 0;
    flightRating = 0;
}

JumperSkills *JumpSimulator::getJumperSkills() const
{
    return jumperSkills;
}

void JumpSimulator::updateJumperSkills()
{
    if(jumper != nullptr)
        jumperSkills = jumper->getJumperSkills();
}

void JumpSimulator::simulateJump()
{
    updateJumperSkills();
    generateTakeoffRating();
    //generateFlightRating();
    //qDebug()<<"Skok zakończył się na "<<(std::floor((distance * 2) + 0.5) / 2)<<" metrze.";
}

void JumpSimulator::generateTakeoffRating()
{
    double ratingMultiplier = 0.98 + 0.1 * hill->getLevelOfCharacteristic("takeoff-technique-effect");
    takeoffRating = jumperSkills->getTakeoffTechnique() * ratingMultiplier;

    ratingMultiplier = 0.5 + 0.1 * hill->getLevelOfCharacteristic("takeoff-power-effect");
    takeoffRating += jumperSkills->getTakeoffPower() * ratingMultiplier;

    ratingMultiplier = 0.7 + 0.1 * hill->getLevelOfCharacteristic("takeoff-form-effect");
    takeoffRating += jumperSkills->getForm() * ratingMultiplier;

    double randomness = 3.25 + 0.75 * hill->getLevelOfCharacteristic("takeoff-randomness-effect");;
    takeoffRating += randomDouble(-randomness, randomness);

    if(takeoffRating < 0.1)
        takeoffRating = 0.1;

    qDebug()<<"Ocena wyjścia z progu: "<<takeoffRating;
}

void JumpSimulator::generateFlightRating()
{
    double ratingMultiplier;

    flightRating = (jumperSkills->getFlightTechnique() * 0.60);
    flightRating += (jumperSkills->getForm() * 0.65);
    /// WPŁYW STYLU LOTU ----> DO ZROBIENIA

    /// WPŁYW CECH CHARAKTERYSTYCZNYCH ----> DO ZROBIENIA



    flightRating += randomDouble(-3.75, 3.75);
}

double JumpSimulator::getFlightRating() const
{
    return flightRating;
}

void JumpSimulator::setFlightRating(double newFlightRating)
{
    flightRating = newFlightRating;
}

double JumpSimulator::getTakeoffRating() const
{
    return takeoffRating;
}

void JumpSimulator::setTakeoffRating(double newTakeoffRating)
{
    takeoffRating = newTakeoffRating;
}

void JumpSimulator::resetTemporaryParameters()
{
    takeoffRating = 0;
    flightRating = 0;
}

Competition *JumpSimulator::getCompetition() const
{
    return competition;
}

void JumpSimulator::setCompetition(Competition *newCompetition)
{
    competition = newCompetition;
}

bool JumpSimulator::jumperCharacteristicsContains(const Characteristic & characteristics)
{
    return jumper->getJumperSkills()->getCharacteristics().contains(characteristics);
}

JumpData JumpSimulator::getJumpData()
{
    JumpData jumpData;
    return jumpData;
}

Hill *JumpSimulator::getHill() const
{
    return hill;
}

void JumpSimulator::setHill(Hill *newHill)
{
    hill = newHill;
}

ConditionsInfo *JumpSimulator::getConditionsInfo() const
{
    return conditionsInfo;
}

void JumpSimulator::setConditionsInfo(ConditionsInfo *newConditionsInfo)
{
    conditionsInfo = newConditionsInfo;
}

Jumper *JumpSimulator::getJumper() const
{
    return jumper;
}

void JumpSimulator::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
    updateJumperSkills();
}
