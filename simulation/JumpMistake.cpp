#include "JumpMistake.h"
#include "JumperSkills.h"
#include "JumpSimulator.h"

#include "../utilities/functions.h"

#include <QVector>
#include <QDebug>

JumpMistake::JumpMistake(short generalType, short indirectType, short exactType, short harmfulness, bool isOccurred, double distance) : generalType(generalType),
    indirectType(indirectType),
    exactType(exactType),
    harmfulness(harmfulness),
    isOccurred(isOccurred),
    distance(distance)
{

}

short JumpMistake::getIndirectType() const
{
    return indirectType;
}

void JumpMistake::setIndirectType(short newIndirectType)
{
    indirectType = newIndirectType;
}

short JumpMistake::getGeneralType() const
{
    return generalType;
}

void JumpMistake::setGeneralType(short newGeneralType)
{
    generalType = newGeneralType;
}

short JumpMistake::getExactType() const
{
    return exactType;
}

void JumpMistake::setExactType(short newExactType)
{
    exactType = newExactType;
}

QString JumpMistake::getNote()
{
    switch(getGeneralType())
    {
    case TakeoffMistake:
        switch(getIndirectType())
        {
        case TakeoffMistakeType::TimingMistake:
            switch(getExactType())
            {
            case TakeoffTimingMistakeType::TooEarlyTakeoff:
                return "Za wczesne wybicie";
            case TakeoffTimingMistakeType::TooLateTakeoff:
                return "Za późne wybicie";
            }
            break;
        case TakeoffMistakeType::AggressivenessMistake:
            switch(getExactType())
            {
            case TakeoffAggressivenessMistakeType::TooPassiveTakeoff:
                return "Zbyt pasywne wybicie";
            case TakeoffAggressivenessMistakeType::TooAgressiveTakeoff:
                return "Zbyt agresywne wybicie";
            }
            break;
        }
        break;
    case FlightMistake:
        /*switch(getIndirectType())
        {
        case TakeoffMistakeType::TimingMistake:
            switch(getExactType())
            {
            case TakeoffTimingMistakeType::TooEarlyTakeoff:
                return "Za wczesne wybicie";
            case TakeoffTimingMistakeType::TooLateTakeoff:
                return "Za późne wybicie";
            }
        case TakeoffMistakeType::AgressivnessMistake:
            switch(getExactType())
            {
            case TakeoffAgressivenessMistakeType::TooPassiveTakeoff:
                return "Zbyt pasywne wybicie";
            case TakeoffAgressivenessMistakeType::TooAgressiveTakeoff:
                return "Zbyt agresywne wybicie";
            }
        }*/
        break;
    }
    return "";
}

bool JumpMistake::getIsOccurred() const
{
    return isOccurred;
}

void JumpMistake::setIsOccurred(bool newIsOccurred)
{
    isOccurred = newIsOccurred;
}

short JumpMistake::getHarmfulness() const
{
    return harmfulness;
}

void JumpMistake::setHarmfulness(short newHarmfulness)
{
    harmfulness = newHarmfulness;
}

/*JumpMistake JumpMistake::generateJumpMistake(JumpSimulator * const simulator, short mistakeType)
{
    JumperSkills * const jumperSkills = simulator->getJumper()->getJumperSkills();
    QVector<double> mistakeProbabilities;
    double sum = 0;
    double random;

    JumpMistake mistake;
    if(mistakeType == TakeoffMistake)
    {
        mistake.setGeneralType(TakeoffMistake);
        mistakeProbabilities.fill(0, 5);
        mistakeProbabilities[0] = 500 - (double(jumperSkills->getTakeoffTechnique()) * 4.49) - (double(jumperSkills->getForm()) * 8.3) + randomInt(-6, 6); //too early takeoff
        mistakeProbabilities[1] = 1200 - (double(jumperSkills->getTakeoffTechnique()) * 4.90) - (double(jumperSkills->getForm()) * 13.6) + randomInt(-6, 6); //too late takeoff
        mistakeProbabilities[2] = 1550 - (double(jumperSkills->getTakeoffTechnique()) * 9.10) - (double(jumperSkills->getForm()) * 18) + randomInt(-6, 6); //too passive takeoff
        mistakeProbabilities[3] = 590 - (double(jumperSkills->getTakeoffTechnique()) * 4.60) - (double(jumperSkills->getForm()) * 8) + randomInt(-6, 6); //too aggressive takeoff
        mistakeProbabilities[4] = 850; // Jeśli wylosuje 2, nie ma błędu.
        for(const auto & mis : mistakeProbabilities)
            sum += mis;
    }
    else if(mistakeType == FlightMistake)
    {
        mistake.setGeneralType(FlightMistake);
        mistakeProbabilities.fill(0, 5);
        mistakeProbabilities[0] = 480 - (double(jumperSkills->getFlightTechnique()) * 5.1) - (double(jumperSkills->getForm()) * 4.5) + randomInt(-6, 6); //ustable ski
        //WPŁYW WARUNKÓW
        mistakeProbabilities[1] = 270 - (double(jumperSkills->getFlightTechnique()) * 2.5) - (double(jumperSkills->getForm()) * 2.5) + randomInt(-15, 15); //pulling to the side
        //WPŁYW WARUNKÓW i CECHY CHARAKTERYSTYCZNEJ
        mistakeProbabilities[2] = 850 - (double(jumperSkills->getFlightTechnique()) * 6) - (double(jumperSkills->getForm()) * 5.5) + randomInt(-6, 6); // too passive flight
        mistakeProbabilities[3] = 700 - (double(jumperSkills->getFlightTechnique()) * 4.8) - (double(jumperSkills->getForm()) * 4) + randomInt(-6, 6);
        mistakeProbabilities[4] = 70000;


    }

    for(const auto & prob : mistakeProbabilities)
        sum += prob;


    random = randomDouble(0, sum);
    for(int i=0; i<mistakeProbabilities.size(); i++)
    {
        double sum2 = 0;
        for(int b=0; b<=i; b++){
            sum2 += mistakeProbabilities[b];
        }
        if(random < sum2)
        {
            if(mistake.getGeneralType() == JumpMistake::MistakeType::TakeoffMistake){
                if(i == 0 || i == 1){
                    mistake.setIndirectType(TakeoffMistakeType::TimingMistake);
                    mistake.setExactType(i);
                }
                else if(i == 2 || i == 3){
                    mistake.setIndirectType(TakeoffMistakeType::AggressivenessMistake);
                    mistake.setExactType(i-2);
                }
                else mistake.setIndirectType(2);
            }
            else if(mistake.getGeneralType() == JumpMistake::MistakeType::FlightMistake){
                if(i == 0 || i == 1){
                    mistake.setIndirectType(FlightMistakeType::BalanceMistake);
                    mistake.setExactType(i);
                }
                else if(i == 2 || i == 3){
                    mistake.setIndirectType(FlightMistakeType::PositionMistake);
                    mistake.setExactType(i-2);
                }
                else mistake.setIndirectType(2);
            }
            break;
        }
    }


    if(mistake.getIndirectType() == TakeoffMistakeType::TimingMistake || mistake.getIndirectType() == TakeoffMistakeType::AggressivenessMistake || mistake.getIndirectType() == FlightMistakeType::BalanceMistake || mistake.getIndirectType() == FlightMistakeType::PositionMistake)
    {
        mistake.setIsOccurred(true);
        mistake.setDistance(0);
        mistake.setHarmfulness(generateJumpMistakeHarmfulness(simulator, mistake.getGeneralType(), mistake.getIndirectType(), mistake.getExactType()));
    }

    return mistake;
}

short JumpMistake::generateJumpMistakeHarmfulness(JumpSimulator * const simulator, short generalType, short indirectType, short exactType)
{
    QVector<double> harmProbabilities(10);
    switch(generalType)
    {
    case TakeoffMistake:
        switch(indirectType)
        {
        case TakeoffMistakeType::TimingMistake:
            switch(exactType)
            {
            case TakeoffTimingMistakeType::TooEarlyTakeoff:
                harmProbabilities = {1, 4.9, 16, 51, 140, 310, 604, 1300, 2200, 3500};
                break;
            case TakeoffTimingMistakeType::TooLateTakeoff:
                harmProbabilities = {1, 3.47, 11.2, 33, 102, 245, 527, 1070, 1590, 2000};
                break;
            }
            break;
        case TakeoffMistakeType::AggressivenessMistake:
            switch(exactType)
            {
            case TakeoffAggressivenessMistakeType::TooPassiveTakeoff:
                harmProbabilities = {1, 3.75, 12.6, 37, 105, 250, 530, 1060, 1800, 2900};
                break;
            case TakeoffAggressivenessMistakeType::TooAgressiveTakeoff:
                harmProbabilities = {1, 3.9, 14, 40, 100, 248, 500, 1000, 1540, 1997};
                break;
            }
        }
        break;
    case FlightMistake:
        switch(indirectType)
        {
        case FlightMistakeType::BalanceMistake:
            switch (exactType) {
            case FlightBalanceMistakeType::UnstableSki:
                break;
            case FlightBalanceMistakeType::PullingToTheSide:
                break;
            }
            break;
        case FlightMistakeType::PositionMistake:
            switch (exactType) {
            case FlightPositionMistakeType::TooPassiveFlight:
                break;
            case FlightPositionMistakeType::TooAggressiveFlight:
                break;
            }
            break;
        }
        break;
    }

    for(int i=9; i>(-1); i--)
    {
        double chance = 1 / (harmProbabilities[i]) * 100;
        //qDebug()<<"chance: "<<chance;
        double random = randomDouble(0, 100, 7);
        if(random < chance)
        {
            //qDebug()<<"random: "<<random<<", chance: "<<chance<<", harmProbab: "<<harmProbabilities[i]<<", exactType: "<<exactType<<", generalType: "<<generalType;
            return i + 1;
        }
    }
    return 0;
}

double JumpMistake::generateJumpMistakeEffect(JumpSimulator * const simulator, JumpMistake * const mistake, short effectType)
{
    Hill * hill = simulator->getHill();
    switch(effectType)
    {
    case Height:
        switch(mistake->getGeneralType())
        {
        case TakeoffMistake:
            switch(mistake->getIndirectType()){
            case TakeoffMistakeType::TimingMistake:
                switch(mistake->getExactType())
                {
                case TakeoffTimingMistakeType::TooEarlyTakeoff:
                    return (double(hill->getTableHeight()) / 65 + randomDouble(-0.003, 0.003, 4)) * double(mistake->getHarmfulness());
                case TakeoffTimingMistakeType::TooLateTakeoff:
                    return (double(hill->getTableHeight()) / 15.9 + randomDouble(-0.004, 0.004, 4)) * double(mistake->getHarmfulness());
                }
                break;
            case TakeoffMistakeType::AggressivenessMistake:
                switch(mistake->getExactType())
                {
                case TakeoffAggressivenessMistakeType::TooPassiveTakeoff:
                    return 0;
                case TakeoffAggressivenessMistakeType::TooAgressiveTakeoff:
                    return (double(hill->getTableHeight()) / 12 + randomDouble(-0.0071, 0.0071, 4)) * double(mistake->getHarmfulness());
                }
                break;
            default: return 0;
            }
        }
        break;
    case Speed:
        switch(mistake->getIndirectType()){
        case TakeoffMistakeType::TimingMistake:
            switch(mistake->getExactType())
            {
            case TakeoffTimingMistakeType::TooEarlyTakeoff:
                return (double(simulator->getSpeed()) / 200 + randomDouble(-0.02, 0.02, 4)) * double(mistake->getHarmfulness());
            case TakeoffTimingMistakeType::TooLateTakeoff:
                return (double(simulator->getSpeed()) / 900 + randomDouble(-0.06, 0.06, 4)) * double(mistake->getHarmfulness());
            }
            break;
        case TakeoffMistakeType::AggressivenessMistake:
            switch(mistake->getExactType())
            {
            case TakeoffAggressivenessMistakeType::TooPassiveTakeoff:
                return (double(hill->getBaseSpeed()) / 120 + randomDouble(-0.026, 0.026, 4)) * double(mistake->getHarmfulness());;
            case TakeoffAggressivenessMistakeType::TooAgressiveTakeoff:
                return (double(hill->getTableHeight()) / 450 + randomDouble(-0.09, 0.09, 4)) * double(mistake->getHarmfulness());
            default: break;
            }
            break;
        default: return 0;
        }
        break;
    case AerodynamicPosition:
        switch(mistake->getGeneralType())
        {
        case TakeoffMistake:
            switch(mistake->getIndirectType()){
            case TakeoffMistakeType::TimingMistake:
                switch(mistake->getExactType())
                {
                case TakeoffTimingMistakeType::TooEarlyTakeoff:
                    return 0.58 * double(mistake->getHarmfulness());
                case TakeoffTimingMistakeType::TooLateTakeoff:
                    return 0.278 * double(mistake->getHarmfulness());
                }
                break;
            case TakeoffMistakeType::AggressivenessMistake:
                switch(mistake->getExactType())
                {
                case TakeoffAggressivenessMistakeType::TooPassiveTakeoff:
                    return 0.88 * double(mistake->getHarmfulness());
                case TakeoffAggressivenessMistakeType::TooAgressiveTakeoff:
                    return 0.4875 * double(mistake->getHarmfulness());
                }
                break;
            default: return 0;
            }
            break;
        }
    }

    return 0;
}*/

double JumpMistake::getDistance() const
{
    return distance;
}

void JumpMistake::setDistance(double newDistance)
{
    distance = newDistance;
}
