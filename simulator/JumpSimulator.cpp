#include "JumpSimulator.h"
#include "../utilities/functions.h"
#include "../global/MyRandom.h"
#include "JumpMistake.h"

#include <QDebug>
#include <QRandomGenerator>
#include <math.h>

JumpSimulator::JumpSimulator(Jumper *jumper, const WindsInfo & windsInfo, Hill *hill) : jumper(jumper),
    windsInfo(windsInfo),
    hill(hill)
{
    if(jumper != nullptr)
        jumperSkills = jumper->getJumperSkillsPointer();

    resetTemporaryParameters();
}

WindsInfo JumpSimulator::getWindsInfo() const
{
    return windsInfo;
}

void JumpSimulator::setWindsInfo(const WindsInfo &newWindsInfo)
{
    windsInfo = newWindsInfo;
}

int *JumpSimulator::getGate() const
{
    return gate;
}

void JumpSimulator::setGate(int *newGate)
{
    gate = newGate;
}

JumperSkills *JumpSimulator::getJumperSkills() const
{
    return jumperSkills;
}

void JumpSimulator::updateJumperSkills()
{
    if(jumper != nullptr)
        jumperSkills = jumper->getJumperSkillsPointer();
}

void JumpSimulator::simulateJump()
{
    jumpData = JumpData();
    jumpData.setSimulator(this);
    simulationData = &jumpData.simulationData;

    resetTemporaryParameters();
    updateJumperSkills();
    hill->setRealHSByCharacteristic();
    setupJumpData();

    generateTakeoffRating();
    generateFlightRating();
    generateDistance();
    generateWindEffects();
    if(jumpData.getDistance() < 0) jumpData.distance = 0;
    generateLanding();
    generateJudges();

    calculateCompensations();
    calculatePoints();
    jumpData.setDistance(roundDoubleToHalf(jumpData.getDistance()));
}

void JumpSimulator::generateTakeoffRating()
{
    double ratingMultiplier = 1.00 + 0.1 * hill->getLevelOfCharacteristic("takeoff-technique-effect");
    simulationData->takeoffRating = jumperSkills->getTakeoffTechnique() * ratingMultiplier;

    ratingMultiplier = 0.5 + 0.1 * hill->getLevelOfCharacteristic("takeoff-power-effect");
    simulationData->takeoffRating += jumperSkills->getTakeoffPower() * ratingMultiplier;

    ratingMultiplier = 0.7 + 0.1 * hill->getLevelOfCharacteristic("takeoff-form-effect");
    simulationData->takeoffRating += jumperSkills->getForm() * ratingMultiplier;

    simulationData->takeoffRating -= std::abs(Hill::calculateBestTakeoffHeightLevel(hill) - jumper->getJumperSkills().getLevelOfCharacteristic("takeoff-height")) * 1;

    double takeoffHeightLevel = jumper->getJumperSkills().getLevelOfCharacteristic("takeoff-height");
    double random = 0;
    random = -(MyRandom::reducingChancesRandom(-6.5 + (takeoffHeightLevel * 2.55), 60, 0.5, 1, 1.112 + (takeoffHeightLevel / 120), MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::ResultNumbersType::FromSmallerToLarger));
    random *= 1 + (0.14 * hill->getLevelOfCharacteristic("takeoff-randomness-effect"));
    //qDebug()<<"Takeoff Random: "<<random;
    simulationData->takeoffRating += random;

    if(simulationData->takeoffRating < 0.1)
        simulationData->takeoffRating = 0.1;

    //qDebug()<<"Ocena wyjścia z progu: "<<takeoffRating;
}

void JumpSimulator::generateFlightRating()
{
    double ratingMultiplier = 1.015 + 0.12 * hill->getLevelOfCharacteristic("flight-technique-effect");
    simulationData->flightRating = jumperSkills->getFlightTechnique() * ratingMultiplier;

    ratingMultiplier = 1.185 + 0.12 * hill->getLevelOfCharacteristic("flight-form-effect");
    simulationData->flightRating += jumperSkills->getForm() * ratingMultiplier;

    simulationData->flightRating -= std::abs(Hill::calculateBestFlightHeightLevel(hill) - jumper->getJumperSkills().getLevelOfCharacteristic("flight-height") * 3);

    double flightHeightLevel = jumper->getJumperSkills().getLevelOfCharacteristic("flight-height");
    double random = 0;
    random = -(MyRandom::reducingChancesRandom(-6 + (flightHeightLevel * 0.85), 60, 0.5, 1, 1.112 + (flightHeightLevel / 360), MyRandom::DrawType::InTurnFromTheHighestChanceNumber, MyRandom::ResultNumbersType::FromSmallerToLarger));
    random *= 1 + (0.14 * hill->getLevelOfCharacteristic("flight-randomness-effect"));
    //qDebug()<<"Flight Random: "<<random;
    simulationData->flightRating += random;

    if(simulationData->flightRating < 0.1)
        simulationData->flightRating = 0.1;

    //qDebug()<<"Mnożnik za styl lotu: "<<getMultiplierForFlightStyleEffect();
    simulationData->flightRating *= getMultiplierForFlightStyleEffect();

    //qDebug()<<"Ocena lotu: "<<flightRating;
}

double JumpSimulator::getMultiplierForFlightStyleEffect()
{
    switch(jumperSkills->getFlightStyle())
    {
    case JumperSkills::FlightStyle::VStyle:
        return 1.024 - ((hill->getFlightEffect() / hill->getTakeoffEffect()) / 41);
    case JumperSkills::FlightStyle::ModernVStyle:
        return 0.995 + ((hill->getFlightEffect() / hill->getTakeoffEffect()) / 56);
    case JumperSkills::FlightStyle::WideVStyle:
        return 0.98 + ((hill->getFlightEffect() / hill->getTakeoffEffect()) / 36);
    case JumperSkills::FlightStyle::HStyle:
        return 0.960 + ((hill->getFlightEffect() / hill->getTakeoffEffect()) / 27.5);;
    }
    return 1.00;
}

void JumpSimulator::generateDistance()
{
    jumpData.distance += simulationData->takeoffRating * hill->getTakeoffEffect();
    jumpData.distance += simulationData->flightRating * hill->getFlightEffect();
    jumpData.distance += *getGate() * (hill->getPointsForGate() / hill->getPointsForMeter());
    jumpData.distance = roundDoubleToHalf(jumpData.getDistance());
}

void JumpSimulator::generateWindEffects()
{
    double change = 0;
    int i = 0;
    for(const auto & wind : windsInfo.getWinds())
    {
        //podczas liczenia w pętli change dla zmian odleglosci przez wiatr, należy dodać warunek "Jeżeli odległość < początek następnego segmentu wiatru" to "Przelicz procent (odległości od początku obecnego segmentu do początku kolejnego) z (całego zakresu między segmentami)". Odjąć otrzymaną wartość przez 1, i przez ten wynik pomnożyć wpływ wiatru
        if(wind.getDirection() == Wind::Back)
        {
            change = wind.getValue() * (getWindSegmentDistance() / 3.8);
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
        }
        else if(wind.getDirection() == Wind::BackSide)
        {
            change = wind.getValue() * (getWindSegmentDistance() / 13.8);
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
        }
        else if(wind.getDirection() == Wind::Side)
        {
            change = MyRandom::randomDouble(-getWindSegmentDistance() / 23, getWindSegmentDistance() / 76);
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
        }
        else if(wind.getDirection() == Wind::FrontSide)
        {
            change = wind.getValue() * (getWindSegmentDistance() / 29);
            change *= MyRandom::randomDouble(0.845, 1.155);
            change *= 0.94 + (jumperSkills->getFlightTechnique() / 420);
            switch(jumperSkills->getFlightStyle())
            {
            case JumperSkills::VStyle: change *= 0.97; break;
            case JumperSkills::ModernVStyle: change *= 1.025; break;
            case JumperSkills::WideVStyle: change *= 1.03; break;
            case JumperSkills::HStyle: change *= 1.035; break;
            }
            change *= 1 + (jumperSkills->getLevelOfCharacteristic("takeoff-height") / 110);
            change *= 1 + (jumperSkills->getLevelOfCharacteristic("flight-height") / 48);
        }
        else if(wind.getDirection() == Wind::Front)
        {
            change = wind.getValue() * (getWindSegmentDistance() / 9.65);
            change *= MyRandom::randomDouble(0.82, 1.18);
            change *= 0.88 + (jumperSkills->getFlightTechnique() / 197.5);
            switch(jumperSkills->getFlightStyle())
            {
            case JumperSkills::VStyle: change *= 0.95; break;
            case JumperSkills::ModernVStyle: change *= 1.03; break;
            case JumperSkills::WideVStyle: change *= 1.051; break;
            case JumperSkills::HStyle: change *= 1.072; break;
            }
            change *= 1 + (jumperSkills->getLevelOfCharacteristic("takeoff-height") / 110);
            change *= 1 + (jumperSkills->getLevelOfCharacteristic("flight-height") / 48);
        }
        if(i != windsInfo.getWinds().count()){
            if(jumpData.getDistance() < (i + 1) * getWindSegmentDistance())
            {
                double percent = double(jumpData.getDistance() - (getWindSegmentDistance() * i)) / ((i + 1) * getWindSegmentDistance());
                if(percent > 1) percent = 1;
                else if(percent < 0) percent = 0;
                change *= percent;
            }
        }
        switch(wind.getDirection())
        {
        case Wind::Back:
            jumpData.distance -= change; break;
        case Wind::BackSide:
            jumpData.distance -= change; break;
        case Wind::Side:
            jumpData.distance += change; break;
        case Wind::FrontSide:
            jumpData.distance += change; break;
        case Wind::Front:
            jumpData.distance += change; break;
        }

        i++;
    }
}

void JumpSimulator::generateLanding()
{
    QVector<double> probabilities;
    probabilities.fill(0, 4);
    probabilities[0] = 99.3 - (1 * hill->getLandingChanceChangeByHillProfile(jumpData.distance, Landing::TelemarkLanding)) - (jumperSkills->getLevelOfCharacteristic("both-legs-landing-tendece") * 7);
    probabilities[1] = 0.68 + (1 * hill->getLandingChanceChangeByHillProfile(jumpData.distance, Landing::BothLegsLanding)) + (jumperSkills->getLevelOfCharacteristic("both-legs-landing-tendece") * 14);
    probabilities[2] = 0.004 + (1 * hill->getLandingChanceChangeByHillProfile(jumpData.distance, Landing::SupportLanding));
    probabilities[3] = 0.016 + (1 * hill->getLandingChanceChangeByHillProfile(jumpData.distance, Landing::Fall));

    //qDebug()<<"Szansa na telemark: "<<probabilities[0];
    //qDebug()<<"Szansa na lądowanie na dwie nogi: "<<probabilities[1];
    //qDebug()<<"Szansa na podpórkę: "<<probabilities[2];
    //qDebug()<<"Szansa na upadek: "<<probabilities[3];

    probabilities[0] *= 1 + (jumperSkills->getLevelOfCharacteristic("landing-skill") / 25);
    probabilities[1] /= 1 + (jumperSkills->getLevelOfCharacteristic("landing-skill") / 80);
    probabilities[1] *= 1 + (jumperSkills->getLevelOfCharacteristic("both-legs-landing-tendence") / 3.2);
    probabilities[2] /= 1 + (jumperSkills->getLevelOfCharacteristic("landing-skill") / 35);
    probabilities[3] /= 1 + (jumperSkills->getLevelOfCharacteristic("landing-skill") / 25);

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
            jumpData.landing.setType(i);
            break;
        }
        i++;
    }


    //stabilność lądowania   -     od 1 do 5
    double landingImbalance = MyRandom::reducingChancesRandom(0, 5, 0.05, 1, (0.97 + (jumperSkills->getLandingStyle() / 100) + hill->getLandingImbalanceChangeByHillProfile(jumpData.distance)), MyRandom::InTurnFromTheHighestChanceNumber, MyRandom::FromSmallerToLarger);
    jumpData.landing.setImbalance(landingImbalance);
}

void JumpSimulator::generateJudges()
{
    double bothLegsLevel = jumperSkills->getLevelOfCharacteristic("both-legs-landing-tendence");

    simulationData->judgesRating = 18.4;
    simulationData->judgesRating -= jumpData.landing.getImbalance() / 1.5;
    simulationData->judgesRating += ((jumpData.distance - hill->getKPoint()) / (hill->getKAndRealHSDifference())) / 1.25;
    switch(jumpData.landing.getType())
    {
    case Landing::TelemarkLanding:
        simulationData->judgesRating += MyRandom::randomDouble(-0.05, 0.05);
        if(bothLegsLevel < 0)
            simulationData->judgesRating += MyRandom::randomDouble(-bothLegsLevel, 0);
        else if(bothLegsLevel > 0)
            simulationData->judgesRating += MyRandom::randomDouble(0, bothLegsLevel);

        break;
    case Landing::BothLegsLanding:
        simulationData->judgesRating -= MyRandom::randomDouble(1.8, 2.1);
        break;
    case Landing::SupportLanding:
        simulationData->judgesRating -= MyRandom::randomDouble(5, 5.6);
        break;
    case Landing::Fall:
        simulationData->judgesRating -= MyRandom::randomDouble(7, 7.8);
        break;
    }

    if(simulationData->judgesRating > 20)
        simulationData->judgesRating = 20;
    else if(simulationData->judgesRating < 1)
        simulationData->judgesRating = 1;

    if(jumpData.judges.size() != 5)
        jumpData.judges.fill(0, 5);

    for(auto & jg : jumpData.judges)
    {
        short randomType = MyRandom::randomInt(0, 1);
        double random = 0;

        jg = simulationData->judgesRating;

        switch(jumpData.landing.getType())
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
}

void JumpSimulator::calculateCompensations()
{
    WindsInfo tempWindsInfo = windsInfo;
    QVector<Wind> winds = tempWindsInfo.getWinds();
    switch(getWindCompensationDistanceEffect()) //
    {
    case WindCompensationDistanceEffect::Original:
        if(jumpData.getDistance() <= (0.75 * hill->getKPoint())){
            int howManyToRemove = winds.count() - std::round((0.75 * hill->getKPoint()) / getWindSegmentDistance());
            for(int i=0; i<howManyToRemove; i++)
                winds.remove(winds.count() - 1);
        }
        break;
    case WindCompensationDistanceEffect::Full:
        for(int i=0; i<tempWindsInfo.getWinds().count(); i++)
        {
            if(jumpData.getDistance() < (i + 1) * getWindSegmentDistance())
            {
                double percent = double(jumpData.getDistance() - (getWindSegmentDistance() * i)) / ((i + 1) * getWindSegmentDistance());
                if(percent > 1) percent = 1;
                else if(percent < 0) percent = 0;
                winds[i] = Wind(winds[i].getDirection(), winds[i].getValue() * percent);
                qDebug()<<"VALUE ZA SEGMENT: "<<winds[i].getValue()<<", original it has been "<<winds[i].getValue() / percent;
            }
        }
    }
    tempWindsInfo.setWinds(winds);

    Wind avgWind = tempWindsInfo.getAveragedWind(windAverageCalculatingType);
    jumpData.setAveragedWind(avgWind.getValueToAveragedWind());
    if(avgWind.getDirection() == Wind::Back)
        jumpData.setWindCompensation(avgWind.getValue() * hill->getPointsForBackWind());
    else if(avgWind.getDirection() == Wind::Front)
        jumpData.setWindCompensation(-(avgWind.getValue() * hill->getPointsForFrontWind()));
    jumpData.setWindCompensation(roundDoubleToOnePlace(jumpData.getWindCompensation()));

    jumpData.setGateCompensation(0); /// na razie nie ma belki startowej
    jumpData.setGateCompensation(roundDoubleToOnePlace(jumpData.getGateCompensation()));

    jumpData.setTotalCompensation(jumpData.getWindCompensation() + jumpData.getGateCompensation());
    jumpData.setTotalCompensation(roundDoubleToOnePlace(jumpData.getTotalCompensation()));
}

void JumpSimulator::calculatePoints()
{
    jumpData.points = hill->getPointsForKPoint();
    jumpData.points += (jumpData.getDistance() - hill->getKPoint()) * hill->getPointsForMeter();
    jumpData.points += jumpData.getGateCompensation();
    jumpData.points += jumpData.getWindCompensation();

    double min = 20, max = 0;
    for(const auto jg : jumpData.getJudges())
    {
        if(jg > max) max = jg;
        else if(jg < min) min = jg;
        jumpData.judgesPoints += jg;
    }
    jumpData.judgesPoints -= min;
    jumpData.judgesPoints -= max;

    jumpData.points += jumpData.getJudgesPoints();

    jumpData.points = roundDoubleToOnePlace(jumpData.getPoints());
}

void JumpSimulator::setupJumpData()
{
    jumpData.jumper = getJumper();
    jumpData.hill = getHill();
    jumpData.simulator = this;
    jumpData.windsInfo = windsInfo;
    //sredni wiatr jest zapisywany w jumpDacie w funkcji calculateCompensations().
    jumpData.setGate(*getGate());
}

double JumpSimulator::getWindSegmentDistance()
{
    return (hill->getKPoint() + (hill->getKPoint() / windsInfo.getWinds().count())) / windsInfo.getWinds().count();
}

short JumpSimulator::getWindCompensationDistanceEffect() const
{
    return windCompensationDistanceEffect;
}

void JumpSimulator::setWindCompensationDistanceEffect(short newWindCompensationDistanceEffect)
{
    windCompensationDistanceEffect = newWindCompensationDistanceEffect;
}

short JumpSimulator::getWindAverageCalculatingType() const
{
    return windAverageCalculatingType;
}

void JumpSimulator::setWindAverageCalculatingType(short newWindAverageCalculatingType)
{
    windAverageCalculatingType = newWindAverageCalculatingType;
}

void JumpSimulator::setJumpData(const JumpData &newJumpData)
{
    jumpData = newJumpData;
}

void JumpSimulator::resetTemporaryParameters()
{
    simulationData->reset();
    jumpData.reset();
}

bool JumpSimulator::jumperCharacteristicsContains(const Characteristic & characteristics)
{
    return jumper->getJumperSkills().getCharacteristics().contains(characteristics);
}

JumpData JumpSimulator::getJumpData()
{
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

Jumper *JumpSimulator::getJumper() const
{
    return jumper;
}

void JumpSimulator::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
    updateJumperSkills();
}
