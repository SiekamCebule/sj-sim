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

JumpMistake JumpMistake::generateJumpMistake(JumpSimulator * const simulator, short mistakeType)
{
    JumperSkills * const jumperSkills = simulator->getJumper()->getJumperSkills();
    QVector<double> mistakeProbabilities;

    JumpMistake mistake;
    if(mistakeType == TakeoffMistake)
    {
        mistake.setGeneralType(TakeoffMistake);
        mistakeProbabilities.fill(0, 5);
        mistakeProbabilities[0] = 600 - (double(jumperSkills->getTakeoffTechnique()) * 4.49) - (double(jumperSkills->getForm()) * 8.3) + randomInt(-3, 3); //too early takeoff
        mistakeProbabilities[1] = 1175 - (double(jumperSkills->getTakeoffTechnique()) * 4.90) - (double(jumperSkills->getForm()) * 13.6) + randomInt(-3, 3); //too late takeoff
        mistakeProbabilities[2] = 1510 - (double(jumperSkills->getTakeoffTechnique()) * 9.10) - (double(jumperSkills->getForm()) * 18) + randomInt(-3, 3); //too passive takeoff
        mistakeProbabilities[3] = 650 - (double(jumperSkills->getTakeoffTechnique()) * 4.60) - (double(jumperSkills->getForm()) * 8) + randomInt(-3, 3); //too aggressive takeoff
        //qDebug()<<mistakeProbabilities[0]<<", "<<mistakeProbabilities[1];
        mistakeProbabilities[4] = 588; // Jeśli wylosuje 2, nie ma błędu.
        double sum = 0;
        for(const auto & mis : mistakeProbabilities)
            sum += mis;
        double random = randomDouble(1, sum);
        for(int i=0; i<mistakeProbabilities.size(); i++)
        {
            double sum2 = 0;
            for(int b=0; b<=i; b++){
                sum2 += mistakeProbabilities[b];
            }
            if(random < sum2){
                if(i == 0 || i == 1){
                    mistake.setIndirectType(TakeoffMistakeType::TimingMistake);
                    mistake.setExactType(i);
                }
                else if(i == 2 || i == 3){
                    mistake.setIndirectType(TakeoffMistakeType::AggressivenessMistake);
                    mistake.setExactType(i-2);
                }
                break;
            }
        }
        if(mistake.getIndirectType() == TakeoffMistakeType::TimingMistake || mistake.getIndirectType() == TakeoffMistakeType::AggressivenessMistake){ // nie jest bez błędu
            mistake.setIsOccurred(true);
            mistake.setDistance(0);
            mistake.setHarmfulness(generateJumpMistakeHarmfulness(simulator, mistake.getGeneralType(), mistake.getIndirectType(), mistake.getExactType()));
        }
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
                harmProbabilities = {1, 2.5, 6.3, 14, 35, 76, 165, 390, 805, 1900};
                break;
            case TakeoffTimingMistakeType::TooLateTakeoff:
                harmProbabilities = {1, 2.5, 4.8, 12, 28.5, 58, 120, 290, 560, 1180};
                break;
            }
            break;
        case TakeoffMistakeType::AggressivenessMistake:
            switch(exactType)
            {
            case TakeoffAggressivenessMistakeType::TooPassiveTakeoff:
                harmProbabilities = {1, 2.4, 5.5, 13, 33, 70, 150, 400, 850, 2100};
                break;
            case TakeoffAggressivenessMistakeType::TooAgressiveTakeoff:
                harmProbabilities = {1, 2.45, 5.53, 13, 33, 69, 150, 380, 780, 1850};
                break;
            }
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
                    return (double(hill->getTableHeight()) / 50 + randomDouble(-0.007, 0.007, 4)) * double(mistake->getHarmfulness());
                case TakeoffTimingMistakeType::TooLateTakeoff:
                    return (double(hill->getTableHeight()) / 30 + randomDouble(-0.007, 0.007, 4)) * double(mistake->getHarmfulness());
                }
                break;
            case TakeoffMistakeType::AggressivenessMistake:
                switch(mistake->getExactType())
                {
                case TakeoffAggressivenessMistakeType::TooPassiveTakeoff:
                    return 0;
                case TakeoffAggressivenessMistakeType::TooAgressiveTakeoff:
                    return (double(hill->getTableHeight()) / 26 + randomDouble(-0.009, 0.009, 4)) * double(mistake->getHarmfulness());
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
                return (double(hill->getBaseSpeed()) / 270 + randomDouble(-0.085, 0.085, 4)) * double(mistake->getHarmfulness());
            case TakeoffTimingMistakeType::TooLateTakeoff:
                return (double(hill->getBaseSpeed()) / 460 + randomDouble(-0.10, 0.10)) * double(mistake->getHarmfulness());
            }
            break;
        case TakeoffMistakeType::AggressivenessMistake:
            switch(mistake->getExactType())
            {
            case TakeoffAggressivenessMistakeType::TooPassiveTakeoff:
                return (double(hill->getBaseSpeed()) / 200 + randomDouble(-0.08, 0.08, 4)) * double(mistake->getHarmfulness());;
            case TakeoffAggressivenessMistakeType::TooAgressiveTakeoff:
                return (double(hill->getTableHeight()) / 500 + randomDouble(-0.05, 0.05, 4)) * double(mistake->getHarmfulness());
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
                    return 0.4 * double(mistake->getHarmfulness());
                case TakeoffTimingMistakeType::TooLateTakeoff:
                    return 0.18 * double(mistake->getHarmfulness());
                }
                break;
            case TakeoffMistakeType::AggressivenessMistake:
                switch(mistake->getExactType())
                {
                case TakeoffAggressivenessMistakeType::TooPassiveTakeoff:
                    return 1 * double(mistake->getHarmfulness());
                case TakeoffAggressivenessMistakeType::TooAgressiveTakeoff:
                    return 0.215 * double(mistake->getHarmfulness());
                }
                break;
            default: return 0;
            }
            break;
        }
    }

    return 0;
}

double JumpMistake::getDistance() const
{
    return distance;
}

void JumpMistake::setDistance(double newDistance)
{
    distance = newDistance;
}
