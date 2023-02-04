#include "JumpMistake.h"
#include "JumperSkills.h"
#include "JumpSimulator.h"

#include "../utilities/functions.h"

#include <QVector>
#include <QDebug>

JumpMistake::JumpMistake(short generalType, short exactType, short harmfulness, bool isOccurred, double distance) : generalType(generalType),
    exactType(exactType),
    harmfulness(harmfulness),
    isOccurred(isOccurred),
    distance(distance)
{

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
        switch(getExactType())
        {
        case TakeoffMistakeType::TooEarlyTakeoff:
            return "Za wczesne wybicie";
        case TakeoffMistakeType::TooLateTakeoff:
            return "Za późne wybicie";
        }
        break;
    case FlightMistake:
        switch(getExactType())
        {
        case FlightMistakeType::FlightAggressiveness:
            return "Nadmiernie agresywny lot";
        case FlightMistakeType::FlightPassivity:
            return "Nadmiernie pasywny lot";
        case FlightMistakeType::FlightInstability:
            return "Niestabilność w locie";
        }
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
        mistakeProbabilities.fill(0, 3);
        mistakeProbabilities[0] = 600 - (double(jumperSkills->getTakeoffTechnique()) * 4.49) - (double(jumperSkills->getForm()) * 8.3) + randomInt(-2, 2); //too early takeoff
        mistakeProbabilities[1] = 1175 - (double(jumperSkills->getTakeoffTechnique()) * 4.90) - (double(jumperSkills->getForm()) * 13.6) + randomInt(-2, 2); //too late takeoff
        //qDebug()<<mistakeProbabilities[0]<<", "<<mistakeProbabilities[1];
        mistakeProbabilities[2] = 230; // Jeśli wylosuje 2, nie ma błędu.
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
                mistake.setExactType(i);
                break;
            }
        }
        if(mistake.getExactType() != mistakeProbabilities.size() - 1){ // nie jest ostatnim elementem
            mistake.setIsOccurred(true);
            mistake.setDistance(0);
            mistake.setHarmfulness(generateJumpMistakeHarmfulness(simulator, mistake.getGeneralType(), mistake.getExactType()));
        }
    }
    return mistake;
}

short JumpMistake::generateJumpMistakeHarmfulness(JumpSimulator * const simulator, short generalType, short exactType)
{
    QVector<double> harmProbabilities(10);
    switch(generalType)
    {
    case TakeoffMistake:
        switch(exactType)
        {
        case TakeoffMistakeType::TooEarlyTakeoff:
            harmProbabilities = {1, 2.5, 6.3, 14, 35, 76, 165, 390, 805, 1900};
            break;
        case TakeoffMistakeType::TooLateTakeoff:
            harmProbabilities = {1, 2.5, 4.8, 12, 28.5, 58, 120, 290, 560, 1180};
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
            switch(mistake->getExactType()){
            case TakeoffMistakeType::TooEarlyTakeoff:
                return (double(hill->getTableHeight()) / 50 + randomDouble(-0.007, 0.007, 4)) * double(mistake->getHarmfulness());
            case TakeoffMistakeType::TooLateTakeoff:
                return (double(hill->getTableHeight()) / 30 + randomDouble(-0.007, 0.007, 4)) * double(mistake->getHarmfulness());
            default: return 0;
            }
        }
        break;
    case Speed:
        switch(mistake->getGeneralType())
        {
        case TakeoffMistake:
            switch(mistake->getExactType())
            {
            case TakeoffMistakeType::TooEarlyTakeoff:
                return (double(hill->getBaseSpeed()) / 270 + randomDouble(-0.085, 0.085, 4)) * double(mistake->getHarmfulness());
            case TakeoffMistakeType::TooLateTakeoff:
                return (double(hill->getBaseSpeed()) / 460 + randomDouble(-0.10, 0.10)) * double(mistake->getHarmfulness());
            default: return 0;
            }
        }
        break;
    case AerodynamicPosition:
        switch(mistake->getGeneralType())
        {
        case TakeoffMistake:
            switch(mistake->getExactType())
            {
            case TakeoffMistakeType::TooEarlyTakeoff:
                return 0.4 * mistake->getHarmfulness();
            case TakeoffMistakeType::TooLateTakeoff:
                return 0.18 * mistake->getHarmfulness();
            default: return 0;
            }
        }
        break;
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
