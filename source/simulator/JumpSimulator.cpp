#include "JumpSimulator.h"
#include "../utilities/functions.h"
#include "../global/MyRandom.h"
#include "../global/GlobalSimulationSettings.h"

#include <QDebug>
#include <QRandomGenerator>
#include <math.h>

JumpSimulator::JumpSimulator(Jumper *jumper, const QVector<Wind> &winds, Hill *hill) :
    winds(winds),
    hill(hill)
{
    this->jumper = jumper;
    simulationData = nullptr;
    if(jumper != nullptr)
        jumperSkills = this->jumper->getJumperSkillsPointer();
    manipulator = nullptr;
    setHasCoachGate(false);
    setCoachGate(0);

    resetTemporaryParameters();
}

int JumpSimulator::getCoachGate() const
{
    return coachGate;
}

void JumpSimulator::setCoachGate(int newCoachGate)
{
    coachGate = newCoachGate;
}

bool JumpSimulator::getHasCoachGate() const
{
    return hasCoachGate;
}

void JumpSimulator::setHasCoachGate(bool newHasCoachGate)
{
    hasCoachGate = newHasCoachGate;
}

bool JumpSimulator::generateDSQ()
{
    if(jumpData.rules->getHasDsq() == true){
        int probability = 0;
        if(manipulator->getExactDSQProbability() > (-1))
            probability = manipulator->getExactDSQProbability();
        else probability = getDSQBaseProbability() / (1 + (jumperSkills->getLevelOfCharacteristic("dsq-probability") / 7));

        int random = MyRandom::randomInt(0, probability + 1);
        if(random == probability)
            return true;
    }
    return false;
}

void JumpSimulator::simulateJump()
{
    jumpData = JumpData();
    jumpData.setSimulator(this);
    simulationData = &jumpData.simulationData;

    if(manipulator->getExactWinds().size() > 0){
        this->setWinds(manipulator->getExactWinds());
        qDebug()<<"set WINDS";
    }

    resetTemporaryParameters();
    updateJumperSkills();
    hill->setRealHSByCharacteristic();
    setupJumpData();
    if(generateDSQ() == true)
    {
        jumpData.setIsDSQOccured(true);
        return;
    }
    jumpData.setIsDSQOccured(false);

    generateTakeoffRating();
    generateFlightRating();
    generateDistance();
    generateWindEffects();

    jumpData.distance += manipulator->getDistanceBonus();
    if(jumpData.distance < manipulator->getDistanceRange().first)
        jumpData.distance = manipulator->getDistanceRange().first;
    else if(jumpData.distance > manipulator->getDistanceRange().second && manipulator->getDistanceRange().second > (-1))
        jumpData.distance = manipulator->getDistanceRange().second;
    if(jumpData.getDistance() < 0) jumpData.distance = 0;
    generateLanding();
    generateJudges();

    calculateCompensations();
    calculatePoints();
    jumpData.setDistance(roundDoubleToHalf(jumpData.getDistance()));
}

void JumpSimulator::generateTakeoffRating()
{
    double ratingMultiplier = 1.059 + 0.1 * hill->getLevelOfCharacteristic("takeoff-technique-effect");
    simulationData->takeoffRating = jumperSkills->getTakeoffTechnique() * ratingMultiplier;

    simulationData->takeoffRating += ((jumperSkills->getLevelOfCharacteristic("takeoff-power") * 2.75) * (1 + 0.1 * hill->getLevelOfCharacteristic("takeoff-power-effect")));

    ratingMultiplier = 1.141 + 0.1 * hill->getLevelOfCharacteristic("takeoff-form-effect");
    simulationData->takeoffRating += jumperSkills->getForm() * ratingMultiplier;

    simulationData->takeoffRating -= std::abs(Hill::calculateBestTakeoffHeightLevel(hill) - jumper->getJumperSkills().getLevelOfCharacteristic("takeoff-height")) * 1.1;

    double random = JumpSimulator::getRandomForJumpSimulation(JumpSimulator::TakeoffRating, jumper);
    random *= 1 + (0.14 * hill->getLevelOfCharacteristic("takeoff-randomness-effect"));
    simulationData->takeoffRating += random;

    simulationData->takeoffRating += manipulator->getTakeoffRatingBonus();
    if(simulationData->takeoffRating < manipulator->getTakeoffRatingRange().first)
        simulationData->takeoffRating = manipulator->getTakeoffRatingRange().first;
    else if(simulationData->takeoffRating > manipulator->getTakeoffRatingRange().second && manipulator->getTakeoffRatingRange().second > (-1))
        simulationData->takeoffRating = manipulator->getTakeoffRatingRange().second;
}

void JumpSimulator::generateFlightRating()
{
    double ratingMultiplier = 0.915 + 0.12 * hill->getLevelOfCharacteristic("flight-technique-effect");
    simulationData->flightRating = jumperSkills->getFlightTechnique() * ratingMultiplier;

    ratingMultiplier = 1.285 + 0.12 * hill->getLevelOfCharacteristic("flight-form-effect");
    simulationData->flightRating += jumperSkills->getForm() * ratingMultiplier;

    simulationData->flightRating -= std::abs(Hill::calculateBestFlightHeightLevel(hill) - jumper->getJumperSkills().getLevelOfCharacteristic("flight-height") * 2.09);

    double random = JumpSimulator::getRandomForJumpSimulation(JumpSimulator::FlightRating, jumper);
    random *= 1 + (0.14 * hill->getLevelOfCharacteristic("flight-randomness-effect"));
    simulationData->flightRating += random;

    if(simulationData->flightRating < 0.1)
        simulationData->flightRating = 0.1;

    simulationData->flightRating *= getMultiplierForFlightStyleEffect();

    simulationData->flightRating += manipulator->getFlightRatingBonus();
    if(simulationData->flightRating < manipulator->getFlightRatingRange().first)
        simulationData->flightRating = manipulator->getFlightRatingRange().first;
    else if(simulationData->flightRating > manipulator->getFlightRatingRange().second && manipulator->getFlightRatingRange().second > (-1))
        simulationData->flightRating = manipulator->getFlightRatingRange().second;
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
    //qDebug()<<jumpData.distance<<"s";
    int tempGate = gate;
    if(hasCoachGate) tempGate = coachGate;

    jumpData.distance += simulationData->takeoffRating * hill->getTakeoffEffect();
    //qDebug()<<jumpData.distance<<"s";
    jumpData.distance += simulationData->flightRating * hill->getFlightEffect();
    //qDebug()<<jumpData.distance<<"b";
    jumpData.distance += tempGate * (hill->getPointsForGate() / hill->getPointsForMeter());
    //qDebug()<<jumpData.distance<<"t";
    jumpData.distance = roundDoubleToHalf(jumpData.getDistance());
}

void JumpSimulator::generateWindEffects()
{
    double change = 0;
    int i = 0;
    for(const auto & wind : getWinds())
    {
        if(wind.getDirection() == Wind::Back)
        {
            change = wind.getStrength() * (getWindSegmentDistance() / 9);
            change *= MyRandom::normalDistributionRandom(1.035, 0.10);
            change *= 1.01 - (jumperSkills->getFlightTechnique() / 2200);
            switch(jumperSkills->getFlightStyle())
            {
            case JumperSkills::VStyle: change *= 0.97; break;
            case JumperSkills::ModernVStyle: change *= 0.985; break;
            case JumperSkills::WideVStyle: change *= 0.995; break;
            case JumperSkills::HStyle: change *= 1.0; break;
            }
            change /= 1 + (jumperSkills->getLevelOfCharacteristic("takeoff-height") / 57);
            change /= 1 + (jumperSkills->getLevelOfCharacteristic("flight-height") / 32.5);
        }
        else if(wind.getDirection() == Wind::BackSide)
        {
            change = wind.getStrength() * (getWindSegmentDistance() / 18);
            change *= MyRandom::normalDistributionRandom(1.035, 0.13);
            change *= 1.02 - (jumperSkills->getFlightTechnique() / 1200);
            switch(jumperSkills->getFlightStyle())
            {
            case JumperSkills::VStyle: change *= 0.99; break;
            case JumperSkills::ModernVStyle: change *= 1.00; break;
            case JumperSkills::WideVStyle: change *= 1.00; break;
            case JumperSkills::HStyle: change *= 1.0; break;
            }
            change /= 1 + (jumperSkills->getLevelOfCharacteristic("takeoff-height") / 80);
            change /= 1 + (jumperSkills->getLevelOfCharacteristic("flight-height") / 56);
        }
        else if(wind.getDirection() == Wind::Side)
        {
            change = MyRandom::randomInt(1, 14);
            if(change <= 2)
            {
                change = wind.getStrength() * (getWindSegmentDistance() / 50);
                change *= MyRandom::normalDistributionRandom(1.035, 0.15);
                change *= 1.03 - (jumperSkills->getFlightTechnique() / 109);
                switch(jumperSkills->getFlightStyle())
                {
                case JumperSkills::VStyle: change *= 0.995; break;
                case JumperSkills::ModernVStyle: change *= 1.00; break;
                case JumperSkills::WideVStyle: change *= 1.00; break;
                case JumperSkills::HStyle: change *= 1.005; break;
                }
                change /= 1 + (jumperSkills->getLevelOfCharacteristic("takeoff-height") / 100);
                change /= 1 + (jumperSkills->getLevelOfCharacteristic("flight-height") / 68);
            }
            else if(change > 0)
            {
                change = wind.getStrength() * (getWindSegmentDistance() / 90);
                change *= MyRandom::normalDistributionRandom(1.035, 0.16);
                change *= 0.985 + (jumperSkills->getFlightTechnique() / 550);
                switch(jumperSkills->getFlightStyle())
                {
                case JumperSkills::VStyle: change *= 0.992; break;
                case JumperSkills::ModernVStyle: change *= 1.008; break;
                case JumperSkills::WideVStyle: change *= 1.011; break;
                case JumperSkills::HStyle: change *= 1.0174; break;
                }
                change *= 1 - (jumperSkills->getLevelOfCharacteristic("takeoff-height") / 90);
                change *= 1 - (jumperSkills->getLevelOfCharacteristic("flight-height") / 60);
            }
        }
        else if(wind.getDirection() == Wind::FrontSide)
        {
            change = wind.getStrength() * (getWindSegmentDistance() / 32);
            change *= MyRandom::normalDistributionRandom(1.058, 0.14);
            change *= 0.94 + (jumperSkills->getFlightTechnique() / 400);
            switch(jumperSkills->getFlightStyle())
            {
            case JumperSkills::VStyle: change *= 0.955; break;
            case JumperSkills::ModernVStyle: change *= 1.0075; break;
            case JumperSkills::WideVStyle: change *= 1.03; break;
            case JumperSkills::HStyle: change *= 1.07; break;
            }
            change *= 1 - (jumperSkills->getLevelOfCharacteristic("takeoff-height") / 85);
            change *= 1 - (jumperSkills->getLevelOfCharacteristic("flight-height") / 60);
        }
        else if(wind.getDirection() == Wind::Front)
        {
            change = wind.getStrength() * (getWindSegmentDistance() / 16);
            change *= MyRandom::normalDistributionRandom(1.06, 0.11);
            change *= 0.88 + (jumperSkills->getFlightTechnique() / 180);
            switch(jumperSkills->getFlightStyle())
            {
            case JumperSkills::VStyle: change *= 0.87; break;
            case JumperSkills::ModernVStyle: change *= 1.985; break;
            case JumperSkills::WideVStyle: change *= 1.05; break;
            case JumperSkills::HStyle: change *= 1.12; break;
            }
            change *= 1 + (jumperSkills->getLevelOfCharacteristic("takeoff-height") / 63);
            change *= 1 + (jumperSkills->getLevelOfCharacteristic("flight-height") / 35);
        }
        if(i != getWinds().count()){
            qDebug()<<"distance: "<<jumpData.getDistance()<<",  (i + 1) * getWindSegmentDistance(): "<<(i + 1) * getWindSegmentDistance();
            if(jumpData.getDistance() < (i + 1) * getWindSegmentDistance())
            {
                //qDebug()<<" double(jumpData.getDistance() - (getWindSegmentDistance() * i): "<<jumpData.getDistance()<<"  -  "<<getWindSegmentDistance() * i;
                //double percent = double(jumpData.getDistance() - (getWindSegmentDistance() * i)) / ((i + 1) * getWindSegmentDistance());
                double percent = 1;
                if(i > 0)
                    percent = ((jumpData.getDistance()) - (double(getWindSegmentDistance() * (i + 0)))) / getWindSegmentDistance();
                //percent = (double(getWindSegmentDistance() * (i + 1)) - (jumpData.getDistance())) / getWindSegmentDistance();
                //percent = jumpData.getDistance() / double(getWindSegmentDistance() * (i + 1));// / ((i) * getWindSegmentDistance());
                if(percent > 1) percent = 1;
                else if(percent < 0) percent = 0;
                qDebug()<<"percent: "<<percent<<";";
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
        //qDebug()<<jumpData.getDistance()<<", "<<(i + 1) * getWindSegmentDistance();

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

    if(manipulator->getExactLandingType() > (-1))
        jumpData.landing.setType(manipulator->getExactLandingType());

    double landingImbalance = MyRandom::lognormalDistributionRandom((0.5 - (jumperSkills->getLandingStyle() / 30)), (0.585 + hill->getLandingImbalanceChangeByHillProfile(jumpData.distance) - jumperSkills->getLandingStyle() / 80));

    jumpData.landing.setImbalance(landingImbalance + manipulator->getLandingInstabilityBonus());
    if(jumpData.landing.getImbalance() < manipulator->getLandingInstabilityRange().first)
        jumpData.landing.setImbalance(manipulator->getLandingInstabilityRange().first);
    else if(jumpData.landing.getImbalance() > manipulator->getLandingInstabilityRange().second && manipulator->getLandingInstabilityRange().second > (-1))
        jumpData.landing.setImbalance(manipulator->getLandingInstabilityRange().second);

}

void JumpSimulator::generateJudges()
{
    if(jumpData.rules->getHasJudgesPoints() == true){
        //double bothLegsLevel = jumperSkills->getLevelOfCharacteristic("both-legs-landing-tendence");
        simulationData->judgesRating = 18.5;
        simulationData->judgesRating -= jumpData.landing.getImbalance() / 1.5;
        simulationData->judgesRating += ((jumpData.distance - hill->getKPoint()) / (hill->getKAndRealHSDifference())) / 1.28;
        switch(jumpData.landing.getType())
        {
        case Landing::TelemarkLanding:
            simulationData->judgesRating += MyRandom::randomDouble(-0.05, 0.05);
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

        simulationData->judgesRating += manipulator->getJudgesRatingBonus();
        if(simulationData->judgesRating < manipulator->getJudgesRatingRange().first)
            simulationData->judgesRating = manipulator->getJudgesRatingRange().first;
        else if(simulationData->judgesRating > manipulator->getJudgesRatingRange().second && manipulator->getJudgesRatingRange().second > (-1))
            simulationData->judgesRating = manipulator->getJudgesRatingRange().second;

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
    else{
        simulationData->setJudgesRating(0);
        jumpData.judges.fill(0, 5);
    }
    int i=0;
    for(auto & jg : jumpData.judges){
        if(manipulator->getExactJudges().at(i) > (-1)){
            jg = manipulator->getExactJudges().at(i);
        }
        i++;
    }
}

void JumpSimulator::calculateCompensations()
{
    jumpData.setWindCompensation(0);
    if(jumpData.rules->getHasWindCompensations() == true){
        QVector<Wind> tempWinds = getWinds();
        switch(competitionRules->getWindCompensationDistanceEffect()) //
        {
        case WindCompensationDistanceEffect::Original:
            if(jumpData.getDistance() <= (0.75 * hill->getKPoint())){
                int howManyToRemove = winds.count() - std::round((0.75 * hill->getKPoint()) / getWindSegmentDistance());
                for(int i=0; i<howManyToRemove; i++)
                    winds.remove(winds.count() - 1);
            }
            break;
        case WindCompensationDistanceEffect::Full:
            for(int i=0; i<tempWinds.count(); i++)
            {
                if(jumpData.getDistance() < (i + 1) * getWindSegmentDistance())
                {
                    double percent = double(jumpData.getDistance() - (getWindSegmentDistance() * i)) / ((i + 1) * getWindSegmentDistance());
                    if(percent > 1) percent = 1;
                    else if(percent < 0) percent = 0;
                    winds[i] = Wind(winds[i].getDirection(), winds[i].getStrength() * percent);
                }
            }
        }
        tempWinds = winds;

        Wind avgWind = WindsCalculator::getAveragedWind(tempWinds, competitionRules->getWindAverageCalculatingType());

        double avgConvertedStrength = avgWind.getStrengthToAveragedWind();
        avgConvertedStrength += manipulator->getAveragedWindBonus();
        if(manipulator->getAveragedWindRangeEnabled() == true){
            if(avgConvertedStrength < manipulator->getAveragedWindRange().first)
                avgConvertedStrength = (manipulator->getAveragedWindRange().first);
            else if(avgConvertedStrength > manipulator->getAveragedWindRange().second && manipulator->getAveragedWindRange().second > (-1))
                avgConvertedStrength = (manipulator->getAveragedWindRange().second);
        }

        jumpData.setAveragedWind(avgConvertedStrength);

        avgWind.setStrength(abs(avgConvertedStrength));
        if(avgConvertedStrength > 0) avgWind.setDirection(Wind::Front);
        else if(avgConvertedStrength < 0) avgWind.setDirection(Wind::Back);
        double windCompensation = WindsCalculator::getWindCompensation(avgWind, hill);
        jumpData.setWindCompensation(windCompensation);
    }

    jumpData.setGateCompensation(0);
    jumpData.setBeats95HSPercents(false);
    if(jumpData.rules->getHasGateCompensations() == true){
        jumpData.setGateCompensation(WindsCalculator::getGateCompensation(competitionStartGate, gate, hill));
        double coachGateCompensation = 0;
        if(hasCoachGate == true){
            if(JumpSimulator::isJumperBeat95HSPercents(jumpData.distance, hill->getHSPoint())){
                jumpData.setBeats95HSPercents(true);
                coachGateCompensation = WindsCalculator::getGateCompensation(gate, coachGate, hill);
            }
        }
        jumpData.setGateCompensation(jumpData.getGateCompensation() + coachGateCompensation);
        jumpData.setGateCompensation(roundDoubleToOnePlace(jumpData.getGateCompensation()));
    }

    jumpData.setTotalCompensation(jumpData.getWindCompensation() + jumpData.getGateCompensation());
    jumpData.setTotalCompensation(roundDoubleToOnePlace(jumpData.getTotalCompensation()));
}

void JumpSimulator::calculatePoints()
{
    jumpData.points = hill->getPointsForKPoint();
    jumpData.points += (jumpData.getDistance() - hill->getKPoint()) * hill->getPointsForMeter();
    jumpData.points += jumpData.getGateCompensation();
    jumpData.points += jumpData.getWindCompensation();

    jumpData.judgesPoints = 0;
    if(jumpData.rules->getHasJudgesPoints()){
        double min = 20, max = 0;
        for(const auto jg : jumpData.getJudges())
        {
            if(jg > max) max = jg;
            else if(jg < min) min = jg;
            jumpData.judgesPoints += jg;
        }
        jumpData.judgesPoints -= min;
        jumpData.judgesPoints -= max;
    }

    jumpData.points += jumpData.getJudgesPoints();

    jumpData.points = roundDoubleToOnePlace(jumpData.getPoints());
}

void JumpSimulator::setupJumpData()
{
    jumpData.reset();
    jumpData.jumper = getJumper();
    jumpData.hill = getHill();
    jumpData.simulator = this;
    jumpData.winds = winds;
    //sredni wiatr jest zapisywany w jumpDacie w funkcji calculateCompensations().
    jumpData.setGate(this->gate);
    jumpData.rules = getCompetitionRules();
    jumpData.hasCoachGate = this->hasCoachGate;
    jumpData.coachGate = this->coachGate;
    if(hasCoachGate == true) jumpData.setGate(this->coachGate);
}

double JumpSimulator::getWindSegmentDistance()
{
    return (hill->getKPoint() + (hill->getKPoint() / getWinds().count())) / getWinds().count();
}

void JumpSimulator::setJumpData(const JumpData &newJumpData)
{
    jumpData = newJumpData;
}

void JumpSimulator::resetTemporaryParameters()
{
    if(simulationData != nullptr){
        simulationData->reset();
    }
    jumpData.reset();
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

int JumpSimulator::getGate() const
{
    return gate;
}

void JumpSimulator::setGate(int newGate)
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

double JumpSimulator::getDSQBaseProbability() const
{
    return DSQBaseProbability;
}

void JumpSimulator::setDSQBaseProbability(double newDSQBaseProbability)
{
    DSQBaseProbability = newDSQBaseProbability;
}

double JumpSimulator::getRandomForJumpSimulation(short parameter, Jumper *jumper)
{
    GlobalSimulationSettings * s = GlobalSimulationSettings::get();
    double multi = s->getSimulationRandomMultiplier();
    switch(parameter)
    {
    case JumpSimulator::TakeoffRating:
    {
        double base = 1.80;
        double deviation = 0.64;
        deviation -= (jumper->getJumperSkills().getLevelOfCharacteristic("takeoff-height") / 50);
        deviation -= (jumper->getJumperSkills().getJumpsEquality() / 32);
        double random = MyRandom::lognormalDistributionRandom(base, deviation);

        if(jumper->getJumperSkills().getLevelOfCharacteristic("takeoff-height") < 0) //niski lot
            random += MyRandom::lognormalDistributionRandom(0.48, 0.31 - (jumper->getJumperSkills().getLevelOfCharacteristic("takeoff-height") / 20.75));

        random *= multi;
        if(random < 0) random = 0;
        random = (-random);
        return random;
    }
    case JumpSimulator::FlightRating:
    {
        double base = 1.65;
        double deviation = 0.685;
        deviation -= (jumper->getJumperSkills().getLevelOfCharacteristic("flight-height") / 40);
        deviation -= (jumper->getJumperSkills().getJumpsEquality() / 25);
        double random = MyRandom::lognormalDistributionRandom(base, deviation);

        if(jumper->getJumperSkills().getLevelOfCharacteristic("flight-height") < 0) //niski lot
            random += MyRandom::lognormalDistributionRandom(0.445, 0.33 - (jumper->getJumperSkills().getLevelOfCharacteristic("flight-height") / 20.75));

        random *= multi;
        if(random < 0) random = 0;
        random = (-random);
        return random;
    }
    }

    return 0;
}

bool JumpSimulator::isJumperBeat95HSPercents(double distance, double HSPoint)
{
    if(distance >= 0.95 * HSPoint)
        return true;
    else
        return false;
}

CompetitionRules *JumpSimulator::getCompetitionRules() const
{
    return competitionRules;
}

void JumpSimulator::setCompetitionRules(CompetitionRules *newCompetitionRules)
{
    competitionRules = newCompetitionRules;
}

JumpManipulator *JumpSimulator::getManipulator() const
{
    return manipulator;
}

void JumpSimulator::setManipulator(JumpManipulator *newManipulator)
{
    manipulator = newManipulator;
}
QVector<Wind> JumpSimulator::getWinds() const
{
    return winds;
}

void JumpSimulator::setWinds(const QVector<Wind> &newWinds)
{
    winds = newWinds;
}

int JumpSimulator::getCompetitionStartGate() const
{
    return competitionStartGate;
}

void JumpSimulator::setCompetitionStartGate(int newCompetitionStartGate)
{
    competitionStartGate = newCompetitionStartGate;
}
