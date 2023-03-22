#include "JumpSimulator.h"
#include "../utilities/functions.h"
#include "../global/MyRandom.h"
#include "../global/GlobalSimulationSettings.h"

#include <QDebug>
#include <QRandomGenerator>
#include <math.h>

JumpSimulator::JumpSimulator(Jumper *jumper, const WindsInfo & windsInfo, Hill *hill) : jumper(jumper),
    windsInfo(windsInfo),
    hill(hill)
{
    simulationData = nullptr;
    if(jumper != nullptr)
        jumperSkills = jumper->getJumperSkillsPointer();
    manipulator = nullptr;

    resetTemporaryParameters();
}

bool JumpSimulator::generateDSQ()
{
    if(jumpData.rules->getHasDsq() == true){
        int probability = getDSQBaseProbability() / (1 + (jumperSkills->getLevelOfCharacteristic("dsq-probability") / 7));
        qDebug()<<probability;
        int random = MyRandom::randomInt(0, probability + 1);
        if(random == probability)
            return true;
    }
    return false;
}

void JumpSimulator::simulateJump(bool manipulate)
{
    qDebug()<<"jumper: "<<jumper;
    jumpData = JumpData();
    jumpData.setSimulator(this);
    simulationData = &jumpData.simulationData;
    qDebug()<<"dz";

    resetTemporaryParameters();
    updateJumperSkills();
    qDebug()<<"dz";
    hill->setRealHSByCharacteristic();
    setupJumpData();
    qDebug()<<"dz";
    if(generateDSQ() == true)
    {
        jumpData.setIsDSQOccured(true);
        return;
    }
    jumpData.setIsDSQOccured(false);

    qDebug()<<"kz";
    generateTakeoffRating();
    qDebug()<<"dz";
    generateFlightRating();
    generateDistance();
    generateWindEffects();
    qDebug()<<"dz";
    if(jumpData.getDistance() < 0) jumpData.distance = 0;
    generateLanding();
    generateJudges();
    qDebug()<<"dz";

    calculateCompensations();
    qDebug()<<"bb";
    calculatePoints();
    qDebug()<<"bb";
    jumpData.setDistance(roundDoubleToHalf(jumpData.getDistance()));
    qDebug()<<"bb";
}

void JumpSimulator::generateTakeoffRating()
{
    double ratingMultiplier = 0.825  + 0.1 * hill->getLevelOfCharacteristic("takeoff-technique-effect");
    simulationData->takeoffRating = jumperSkills->getTakeoffTechnique() * ratingMultiplier;

    ratingMultiplier = 0.55 + 0.1 * hill->getLevelOfCharacteristic("takeoff-power-effect");
    simulationData->takeoffRating += jumperSkills->getTakeoffPower() * ratingMultiplier;

    ratingMultiplier = 0.825 + 0.1 * hill->getLevelOfCharacteristic("takeoff-form-effect");
    simulationData->takeoffRating += jumperSkills->getForm() * ratingMultiplier;

    simulationData->takeoffRating -= std::abs(Hill::calculateBestTakeoffHeightLevel(hill) - jumper->getJumperSkills().getLevelOfCharacteristic("takeoff-height")) * 3;

    double random = JumpSimulator::getRandomForJumpSimulation(JumpSimulator::TakeoffRating, jumper);
    random *= 1 + (0.14 * hill->getLevelOfCharacteristic("takeoff-randomness-effect"));
    simulationData->takeoffRating += random;

    if(simulationData->takeoffRating < 0.1)
        simulationData->takeoffRating = 0.1;
}

void JumpSimulator::generateFlightRating()
{
    double ratingMultiplier = 0.85  + 0.12 * hill->getLevelOfCharacteristic("flight-technique-effect");
    simulationData->flightRating = jumperSkills->getFlightTechnique() * ratingMultiplier;

    ratingMultiplier = 1.35 + 0.12 * hill->getLevelOfCharacteristic("flight-form-effect");
    simulationData->flightRating += jumperSkills->getForm() * ratingMultiplier;

    simulationData->flightRating -= std::abs(Hill::calculateBestFlightHeightLevel(hill) - jumper->getJumperSkills().getLevelOfCharacteristic("flight-height") * 3);

    double random = JumpSimulator::getRandomForJumpSimulation(JumpSimulator::FlightRating, jumper);
    random *= 1 + (0.14 * hill->getLevelOfCharacteristic("flight-randomness-effect"));
    simulationData->flightRating += random;

    if(simulationData->flightRating < 0.1)
        simulationData->flightRating = 0.1;

    simulationData->flightRating *= getMultiplierForFlightStyleEffect();
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
            change = wind.getStrength() * (getWindSegmentDistance() / 3.8);
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
            change = wind.getStrength() * (getWindSegmentDistance() / 13.8);
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
            change = wind.getStrength() * (getWindSegmentDistance() / 29);
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
            change = wind.getStrength() * (getWindSegmentDistance() / 9.65);
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
    if(jumpData.rules->getHasJudgesPoints() == true){
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
    else{
        simulationData->setJudgesRating(0);
        jumpData.judges.fill(0, 5);
    }
}

void JumpSimulator::calculateCompensations()
{
    qDebug()<<jumpData.rules->getHasDsq();
    jumpData.setWindCompensation(0);
    if(jumpData.rules->getHasWindCompensations() == true){
        WindsInfo tempWindsInfo = windsInfo;
        QVector<Wind> winds = tempWindsInfo.getWinds();
        qDebug()<<"s";
        qDebug()<<competitionRules->getWindCompensationDistanceEffect();
        switch(competitionRules->getWindCompensationDistanceEffect()) //
        {
        case WindCompensationDistanceEffect::Original:
            qDebug()<<jumpData.getDistance();
            qDebug()<<", "<<hill->getKPoint();
            qDebug()<<winds.count()<<", "<<hill->getKPoint();
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
                    winds[i] = Wind(winds[i].getDirection(), winds[i].getStrength() * percent);
                }
            }
        }
        tempWindsInfo.setWinds(winds);

        Wind avgWind = tempWindsInfo.getAveragedWind(competitionRules->getWindAverageCalculatingType());
        jumpData.setAveragedWind(avgWind.getStrengthToAveragedWind());
        if(avgWind.getDirection() == Wind::Back)
            jumpData.setWindCompensation(avgWind.getStrength() * hill->getPointsForBackWind());
        else if(avgWind.getDirection() == Wind::Front)
            jumpData.setWindCompensation(-(avgWind.getStrength() * hill->getPointsForFrontWind()));
        jumpData.setWindCompensation(roundDoubleToOnePlace(jumpData.getWindCompensation()));
    }
    qDebug()<<"elo";

    jumpData.setGateCompensation(0);
    if(jumpData.rules->getHasGateCompensations() == true){
        jumpData.setGateCompensation(0); /// na razie nie ma belki startowej
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
    jumpData.windsInfo = windsInfo;
    //sredni wiatr jest zapisywany w jumpDacie w funkcji calculateCompensations().
    jumpData.setGate(*getGate());
    jumpData.rules = getCompetitionRules();
}

double JumpSimulator::getWindSegmentDistance()
{
    return (hill->getKPoint() + (hill->getKPoint() / windsInfo.getWinds().count())) / windsInfo.getWinds().count();
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
    double multi = s->getSimulationMultiplier();
    switch(parameter)
    {
    case JumpSimulator::TakeoffRating:
    {
        int randomType = MyRandom::randomInt(1, 1000);
        if(randomType < 86)
            randomType = 1;
        else randomType = 0;
        switch(randomType)
        {
        case 0:{
            double deviation = 0.56;
            deviation -= (jumper->getJumperSkills().getLevelOfCharacteristic("takeoff-height") / 55);
            double random = MyRandom::lognormalDistributionRandom(1.7, deviation);

            random += MyRandom::lognormalDistributionRandom(0.92, (0.62) - (jumper->getJumperSkills().getJumpsEquality() / 38));

            random *= multi;
            if(random < 0) random = 0;
            random = (-random);
            return random;
        }
        case 1:
            double deviation = 0.54;
            deviation -= (jumper->getJumperSkills().getLevelOfCharacteristic("takeoff-height") / 70);
            double random = MyRandom::lognormalDistributionRandom(0.65, deviation);

            random += MyRandom::lognormalDistributionRandom(0.3, (0.49) - (jumper->getJumperSkills().getJumpsEquality() / 130));

            random *= multi;
            if(random < 0) random = 0;
            return random;
        }
        break;
    }
    case JumpSimulator::FlightRating:
    {
        int randomType = MyRandom::randomInt(1, 1000);
        if(randomType < 68)
            randomType = 1;
        else randomType = 0;
        switch(randomType)
        {
        case 0:{
            double deviation = 0.58;
            deviation -= (jumper->getJumperSkills().getLevelOfCharacteristic("flight-height") / 45);
            double random = MyRandom::lognormalDistributionRandom(1.7, deviation);

            random += MyRandom::lognormalDistributionRandom(0.94, (0.63) - (jumper->getJumperSkills().getJumpsEquality() / 25.75));

            random *= multi;
            if(random < 0) random = 0;
            random = (-random);
            return random;
        }
        case 1:
            double deviation = 0.52;
            deviation -= (jumper->getJumperSkills().getLevelOfCharacteristic("flight-height") / 66);
            double random = MyRandom::lognormalDistributionRandom(0.65, deviation);

            random += MyRandom::lognormalDistributionRandom(0.27, (0.45) - (jumper->getJumperSkills().getJumpsEquality() / 178));

            random *= multi;
            if(random < 0) random = 0;
            return random;
        }
        break;
    }
    }
    return 0;
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