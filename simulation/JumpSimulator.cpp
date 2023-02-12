#include "JumpSimulator.h"
#include "../utilities/functions.h"
#include "../utilities/MyRandom.h"
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

    resetTemporaryParameters();
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
    resetTemporaryParameters();
    updateJumperSkills();

    generateTakeoffRating();
    generateFlightRating();

    generateDistance();
    generateWindEffects();
    qDebug()<<"Uśredniony odczyt wiatru: "<<conditionsInfo->getAveragedWind().getValueToAveragedWind()<<"m/s";
}

void JumpSimulator::generateTakeoffRating()
{
    double ratingMultiplier = 1.00 + 0.1 * hill->getLevelOfCharacteristic("takeoff-technique-effect");
    takeoffRating = jumperSkills->getTakeoffTechnique() * ratingMultiplier;

    ratingMultiplier = 0.5 + 0.1 * hill->getLevelOfCharacteristic("takeoff-power-effect");
    takeoffRating += jumperSkills->getTakeoffPower() * ratingMultiplier;

    ratingMultiplier = 0.7 + 0.1 * hill->getLevelOfCharacteristic("takeoff-form-effect");
    takeoffRating += jumperSkills->getForm() * ratingMultiplier;

    double random = 0;
    short randomType = MyRandom::randomInt(1, 20);
    if(randomType <= 3)
        random = MyRandom::reducingChancesRandom(0.1, 8, 0.1, 1.15, 1.15, MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::ResultNumbersType::FromSmallerToLarger);
    else
        random = -(MyRandom::reducingChancesRandom(0.1, 32, 0.1, 1.15, 1.045, MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::ResultNumbersType::FromSmallerToLarger));

    random *= 1 + (0.15 * hill->getLevelOfCharacteristic("takeoff-randomness-effect"));
    //qDebug()<<"Takeoff Random: "<<random;
    takeoffRating += random;

    if(takeoffRating < 0.1)
        takeoffRating = 0.1;

    //qDebug()<<"Ocena wyjścia z progu: "<<takeoffRating;
}

void JumpSimulator::generateFlightRating()
{
    double ratingMultiplier = 1.015 + 0.12 * hill->getLevelOfCharacteristic("flight-technique-effect");
    flightRating = jumperSkills->getFlightTechnique() * ratingMultiplier;

    ratingMultiplier = 1.185 + 0.12 * hill->getLevelOfCharacteristic("flight-form-effect");
    flightRating += jumperSkills->getForm() * ratingMultiplier;

    double random = 0;
    short randomType = MyRandom::randomInt(1, 20);
    if(randomType <= 3)
        random = MyRandom::reducingChancesRandom(0.1, 6, 0.1, 1.15, 1.15, MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::ResultNumbersType::FromSmallerToLarger);
    else
        random = -(MyRandom::reducingChancesRandom(0.1, 40, 0.10, 1.15, 1.045, MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::ResultNumbersType::FromSmallerToLarger));

    random *= 1 + (0.15 * hill->getLevelOfCharacteristic("flight-randomness-effect"));
    //qDebug()<<"Flight Random: "<<random;
    flightRating += random;

    if(flightRating < 0.1)
        flightRating = 0.1;

    //qDebug()<<"Mnożnik za styl lotu: "<<getMultiplierForFlightStyleEffect();
    flightRating *= getMultiplierForFlightStyleEffect();

    //qDebug()<<"Ocena lotu: "<<flightRating;
}

double JumpSimulator::getMultiplierForFlightStyleEffect()
{
    switch(jumperSkills->getFlightStyle())
    {
    case JumperSkills::FlightStyle::VStyle:
        return 1.024 - ((hill->getFlightEffect() / hill->getTakeoffEffect()) / 41);
    case JumperSkills::FlightStyle::ModernVStyle:
        return 0.995 + ((hill->getFlightEffect() / hill->getTakeoffEffect()) / 56); //
    case JumperSkills::FlightStyle::WideVStyle:
        return 0.98 + ((hill->getFlightEffect() / hill->getTakeoffEffect()) / 36);
    case JumperSkills::FlightStyle::HStyle:
        return 0.960 + ((hill->getFlightEffect() / hill->getTakeoffEffect()) / 27.5);;
    }
    return 1.00;
}

void JumpSimulator::generateDistance()
{
    distance += takeoffRating * hill->getTakeoffEffect();
    distance += flightRating * hill->getFlightEffect();
    distance += conditionsInfo->getGate() * (hill->getPointsForGate() / hill->getPointsForMeter());
}

void JumpSimulator::generateWindEffects()
{
    double segmentDistance = hill->getKPoint() / conditionsInfo->getWinds().size();
    double change = 0;
    for(const auto & wind : conditionsInfo->getWinds())
    {
        if(wind.getDirection() == Wind::Back)
        {
            change = wind.getValue() * (segmentDistance / 5);
            change *= MyRandom::randomDouble(0.975, 1.025);
            switch(jumperSkills->getFlightStyle())
            {
            case JumperSkills::VStyle: change *= 0.98; break;
            case JumperSkills::ModernVStyle: change *= 0.995; break;
            case JumperSkills::WideVStyle: change *= 1.01; break;
            case JumperSkills::HStyle: change *= 1.025; break;
            }
            distance -= change;
        }
        else if(wind.getDirection() == Wind::BackLeft || wind.getDirection() == Wind::BackRight)
        {
            change = wind.getValue() * (segmentDistance / 13.75);
            change *= MyRandom::randomDouble(0.875, 1.125);
            switch(jumperSkills->getFlightStyle())
            {
            case JumperSkills::VStyle: change *= 0.995; break;
            case JumperSkills::ModernVStyle: change *= 1.00; break;
            case JumperSkills::WideVStyle: change *= 1.005; break;
            case JumperSkills::HStyle: change *= 1.0112; break;
            }
            distance -= change;
        }
        else if(wind.getDirection() == Wind::Left || wind.getDirection() == Wind::Right)
        {
            change = MyRandom::randomDouble(-segmentDistance / 23, segmentDistance / 76);
            if(change < 0)
            {
                change *= MyRandom::randomDouble(0.88, 1.12);
                switch(jumperSkills->getFlightStyle())
                {
                case JumperSkills::VStyle: change *= 0.995; break;
                case JumperSkills::ModernVStyle: change *= 1.00; break;
                case JumperSkills::WideVStyle: change *= 1.004; break;
                case JumperSkills::HStyle: change *= 1.01; break;
                }
            }
            else if(change > 0)
            {
                change *= MyRandom::randomDouble(0.80, 1.20);
                switch(jumperSkills->getFlightStyle())
                {
                case JumperSkills::VStyle: change *= 0.992; break;
                case JumperSkills::ModernVStyle: change *= 1.008; break;
                case JumperSkills::WideVStyle: change *= 1.011; break;
                case JumperSkills::HStyle: change *= 1.0174; break;
                }
            }
            distance += change; //dodać, bo i tak jeśli będzie wiatr w plecy to change będzie ujemne
        }
        else if(wind.getDirection() == Wind::FrontLeft || wind.getDirection() == Wind::FrontRight)
        {
            change = wind.getValue() * (segmentDistance / 20.5);
            change *= MyRandom::randomDouble(0.845, 1.155);
            switch(jumperSkills->getFlightStyle())
            {
            case JumperSkills::VStyle: change *= 0.97; break;
            case JumperSkills::ModernVStyle: change *= 1.025; break;
            case JumperSkills::WideVStyle: change *= 1.03; break;
            case JumperSkills::HStyle: change *= 1.035; break;
            }
            distance += change;
        }
        else if(wind.getDirection() == Wind::Front)
        {
            change = wind.getValue() * (segmentDistance / 8.5);
            change *= MyRandom::randomDouble(0.82, 1.18);
            switch(jumperSkills->getFlightStyle())
            {
            case JumperSkills::VStyle: change *= 0.95; break;
            case JumperSkills::ModernVStyle: change *= 1.03; break;
            case JumperSkills::WideVStyle: change *= 1.051; break;
            case JumperSkills::HStyle: change *= 1.072; break;
            }
            distance += change;
        }
    }
}

double JumpSimulator::getDistance() const
{
    return distance;
}

void JumpSimulator::setDistance(double newDistance)
{
    distance = newDistance;
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
    distance = 0;
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
