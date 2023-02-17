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
    hill->setRealHSByCharacteristic();

    generateTakeoffRating();
    generateFlightRating();

    generateDistance();
    generateWindEffects();
    generateLanding();
    generateJudges();
    //qDebug()<<"Uśredniony odczyt wiatru: "<<conditionsInfo->getAveragedWind().getValueToAveragedWind()<<"m/s";
}

void JumpSimulator::generateTakeoffRating()
{
    double ratingMultiplier = 1.00 + 0.1 * hill->getLevelOfCharacteristic("takeoff-technique-effect");
    takeoffRating = jumperSkills->getTakeoffTechnique() * ratingMultiplier;

    ratingMultiplier = 0.5 + 0.1 * hill->getLevelOfCharacteristic("takeoff-power-effect");
    takeoffRating += jumperSkills->getTakeoffPower() * ratingMultiplier;

    ratingMultiplier = 0.7 + 0.1 * hill->getLevelOfCharacteristic("takeoff-form-effect");
    takeoffRating += jumperSkills->getForm() * ratingMultiplier;


    double perfectLevel = 0.5;
    if((hill->getTakeoffEffect() / (hill->getFlightEffect() / 1.00)) > 1)
        perfectLevel += (hill->getTakeoffEffect() / (hill->getFlightEffect() / 0.85));
    else
        perfectLevel += -(hill->getFlightEffect() / (hill->getTakeoffEffect() / 0.85));
    takeoffRating -= std::abs(perfectLevel - jumper->getJumperSkills()->getLevelOfCharacteristic("takeoff-height")) * 1.15;

    double random = 0;
    short randomType = MyRandom::randomInt(1, 20);
    if(randomType <= 3)
        random = MyRandom::reducingChancesRandom(0.1, 8, 0.1, 1.15, 1.126, MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::ResultNumbersType::FromSmallerToLarger);
    else
        random = -(MyRandom::reducingChancesRandom(0.1, 32, 0.1, 1.15, 1.046, MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::ResultNumbersType::FromSmallerToLarger));

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

    double perfectLevel = 0.6;
    if((hill->getTakeoffEffect() / (hill->getFlightEffect() / 1.00)) > 1)
        perfectLevel += (hill->getTakeoffEffect() / (hill->getFlightEffect() / 0.85));
    else
        perfectLevel += -(hill->getFlightEffect() / (hill->getTakeoffEffect() / 0.85));
    flightRating -= std::abs(perfectLevel - jumper->getJumperSkills()->getLevelOfCharacteristic("flight-height"));

    double random = 0;
    short randomType = MyRandom::randomInt(1, 20);
    if(randomType <= 3)
        random = MyRandom::reducingChancesRandom(0.1, 6, 0.1, 1.15, 1.126, MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::ResultNumbersType::FromSmallerToLarger);
    else
        random = -(MyRandom::reducingChancesRandom(0.1, 40, 0.10, 1.15, 1.046, MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::ResultNumbersType::FromSmallerToLarger));

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
            change = wind.getValue() * (segmentDistance / 3.8);
            change *= MyRandom::randomDouble(0.975, 1.025);
            change *= 1.01 - (jumperSkills->getFlightTechnique() / 2200);
            switch(jumperSkills->getFlightStyle())
            {
            case JumperSkills::VStyle: change *= 0.98; break;
            case JumperSkills::ModernVStyle: change *= 0.995; break;
            case JumperSkills::WideVStyle: change *= 1.01; break;
            case JumperSkills::HStyle: change *= 1.025; break;
            }
            change /= 1 + (jumperSkills->getLevelOfCharacteristic("takeoff-height") / 120);
            change /= 1 + (jumperSkills->getLevelOfCharacteristic("flight-height") / 60);
            distance -= change;
        }
        else if(wind.getDirection() == Wind::BackSide)
        {
            change = wind.getValue() * (segmentDistance / 13.8);
            change *= MyRandom::randomDouble(0.875, 1.125);
            change *= 1.02 - (jumperSkills->getFlightTechnique() / 1200);
            switch(jumperSkills->getFlightStyle())
            {
            case JumperSkills::VStyle: change *= 0.995; break;
            case JumperSkills::ModernVStyle: change *= 1.00; break;
            case JumperSkills::WideVStyle: change *= 1.005; break;
            case JumperSkills::HStyle: change *= 1.0112; break;
            }
            change /= 1 + (jumperSkills->getLevelOfCharacteristic("takeoff-height") / 120);
            change /= 1 + (jumperSkills->getLevelOfCharacteristic("flight-height") / 60);
            distance -= change;
        }
        else if(wind.getDirection() == Wind::Side)
        {
            change = MyRandom::randomDouble(-segmentDistance / 23, segmentDistance / 76);
            if(change < 0)
            {
                change *= MyRandom::randomDouble(0.88, 1.12);
                change *= 1.03 - (jumperSkills->getFlightTechnique() / 1050);
                switch(jumperSkills->getFlightStyle())
                {
                case JumperSkills::VStyle: change *= 0.995; break;
                case JumperSkills::ModernVStyle: change *= 1.00; break;
                case JumperSkills::WideVStyle: change *= 1.004; break;
                case JumperSkills::HStyle: change *= 1.01; break;
                }
                change /= 1 + (jumperSkills->getLevelOfCharacteristic("takeoff-height") / 120);
                change /= 1 + (jumperSkills->getLevelOfCharacteristic("flight-height") / 60);
            }
            else if(change > 0)
            {
                change *= MyRandom::randomDouble(0.80, 1.20);
                change *= 0.985 + (jumperSkills->getFlightTechnique() / 1600);
                switch(jumperSkills->getFlightStyle())
                {
                case JumperSkills::VStyle: change *= 0.992; break;
                case JumperSkills::ModernVStyle: change *= 1.008; break;
                case JumperSkills::WideVStyle: change *= 1.011; break;
                case JumperSkills::HStyle: change *= 1.0174; break;
                }
                change *= 1 + (jumperSkills->getLevelOfCharacteristic("takeoff-height") / 120);
                change *= 1 + (jumperSkills->getLevelOfCharacteristic("flight-height") / 60);
            }
            distance += change; //dodać, bo i tak jeśli będzie wiatr w plecy to change będzie ujemne
        }
        else if(wind.getDirection() == Wind::FrontSide)
        {
            change = wind.getValue() * (segmentDistance / 29);
            change *= MyRandom::randomDouble(0.845, 1.155);
            change *= 0.94 + (jumperSkills->getFlightTechnique() / 420);
            switch(jumperSkills->getFlightStyle())
            {
            case JumperSkills::VStyle: change *= 0.97; break;
            case JumperSkills::ModernVStyle: change *= 1.025; break;
            case JumperSkills::WideVStyle: change *= 1.03; break;
            case JumperSkills::HStyle: change *= 1.035; break;
            }
            change *= 1 + (jumperSkills->getLevelOfCharacteristic("takeoff-height") / 120);
            change *= 1 + (jumperSkills->getLevelOfCharacteristic("flight-height") / 60);
            distance += change;
        }
        else if(wind.getDirection() == Wind::Front)
        {
            change = wind.getValue() * (segmentDistance / 9.65);
            change *= MyRandom::randomDouble(0.82, 1.18);
            change *= 0.88 + (jumperSkills->getFlightTechnique() / 197.5);
            switch(jumperSkills->getFlightStyle())
            {
            case JumperSkills::VStyle: change *= 0.95; break;
            case JumperSkills::ModernVStyle: change *= 1.03; break;
            case JumperSkills::WideVStyle: change *= 1.051; break;
            case JumperSkills::HStyle: change *= 1.072; break;
            }
            change *= 1 + (jumperSkills->getLevelOfCharacteristic("takeoff-height") / 120);
            change *= 1 + (jumperSkills->getLevelOfCharacteristic("flight-height") / 60);
            distance += change;
        }
    }
}

void JumpSimulator::generateLanding()
{
    QVector<double> probabilities;
    probabilities.fill(0, 4);
    probabilities[0] = 99.3 - (1 * hill->getLandingChanceChangeByHillProfile(distance, Landing::TelemarkLanding)) - (jumperSkills->getLevelOfCharacteristic("both-legs-landing-tendece") * 7);
    probabilities[1] = 0.68 + (1 * hill->getLandingChanceChangeByHillProfile(distance, Landing::BothLegsLanding)) + (jumperSkills->getLevelOfCharacteristic("both-legs-landing-tendece") * 14);
    probabilities[2] = 0.004 + (1 * hill->getLandingChanceChangeByHillProfile(distance, Landing::SupportLanding));
    probabilities[3] = 0.016 + (1 * hill->getLandingChanceChangeByHillProfile(distance, Landing::Fall));

    //qDebug()<<"Szansa na telemark: "<<probabilities[0];
    //qDebug()<<"Szansa na lądowanie na dwie nogi: "<<probabilities[1];
    //qDebug()<<"Szansa na podpórkę: "<<probabilities[2];
    //qDebug()<<"Szansa na upadek: "<<probabilities[3];

    probabilities[0] *= 1 + (jumperSkills->getLevelOfCharacteristic("landing-skill") / 38);
    probabilities[1] /= 1 + (jumperSkills->getLevelOfCharacteristic("landing-skill") / 38);
    probabilities[2] /= 1 + (jumperSkills->getLevelOfCharacteristic("landing-skill") / 45);
    probabilities[3] /= 1 + (jumperSkills->getLevelOfCharacteristic("landing-skill") / 83);

    double drawSum = 0;
    for(const auto & probability : probabilities)
        drawSum += probability;

    double drawRandom = MyRandom::randomDouble(0, drawSum);
    double actualSum = 0;

    int i=0;
    for(const auto & probability : probabilities)
    {
        actualSum += probability;
        if(drawRandom < actualSum)
        {
            landing.setType(i);
            break;
        }
        i++;
    }

    qDebug()<<"Rodzaj lądowania: "<<landing.getTextLandingType();


    //stabilność lądowania   -     od 1 do 5
    double landingImbalance = MyRandom::reducingChancesRandom(0, 5, 0.05, 1, (0.97 + (jumperSkills->getLandingStyle() / 100) + hill->getLandingImbalanceChangeByHillProfile(distance)), MyRandom::InTurnFromTheHighestChanceNumber, MyRandom::FromSmallerToLarger);
    landing.setImbalance(landingImbalance);
    qDebug()<<"Niestabilność lądowania: "<<landingImbalance<<" / 5";
}

void JumpSimulator::generateJudges()
{
    judgesRating = 18.4;
    judgesRating -= landing.getImbalance() / 1.5;
    judgesRating += ((distance - hill->getKPoint()) / (hill->getKAndRealHSDifference())) / 1.25;
    switch(landing.getType())
    {
    case Landing::TelemarkLanding:
        judgesRating += MyRandom::randomDouble(-0.05, 0.05);
        judgesRating += MyRandom::randomDouble(-(jumperSkills->getLevelOfCharacteristic("both-legs-landing-tendence") / 4.65), 0);
        break;
    case Landing::BothLegsLanding:
        judgesRating -= MyRandom::randomDouble(1.8, 2.1);
        break;
    case Landing::SupportLanding:
        judgesRating -= MyRandom::randomDouble(5, 5.6);
        break;
    case Landing::Fall:
        judgesRating -= MyRandom::randomDouble(7, 7.8);
        break;
    }

    if(judgesRating > 20)
        judgesRating = 20;
    else if(judgesRating < 1)
        judgesRating = 1;

    if(judges.size() != 5)
        judges.fill(0, 5);


    for(auto & jg : judges)
    {
        short randomType = MyRandom::randomInt(0, 1);
        double random = 0;

        jg = judgesRating;

        switch(landing.getType())
        {
        case Landing::TelemarkLanding:
            random = MyRandom::randomDouble(-0.5, 0.5);
            break;
        case Landing::BothLegsLanding:
            random = MyRandom::randomDouble(-0.64, 0.64);
            break;
        case Landing::SupportLanding:
            random = MyRandom::randomDouble(-0.78, 0.78);
            break;
        case Landing::Fall:
            random = MyRandom::randomDouble(-0.72, 0.72);
            break;
        }
        if(randomType == 0)
            jg -= random;
        else if(randomType == 1)
            jg += random;
        else qDebug()<<"BŁĄD randomType przy lądowaniu ";

        jg = double(roundDoubleToHalf(jg));
        if(jg > 20) jg = 20;
        else if(jg < 0.5) jg = 0.5;
    }

    qDebug()<<"Noty sędziowskie: ";
    for(const auto & jg : judges)
        qDebug()<<jg;
}

QVector<double> JumpSimulator::getJudges() const
{
    return judges;
}

void JumpSimulator::setJudges(const QVector<double> &newJudges)
{
    judges = newJudges;
}

Landing JumpSimulator::getLanding() const
{
    return landing;
}

void JumpSimulator::setLanding(const Landing &newLanding)
{
    landing = newLanding;
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
    judgesRating = 0;
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
