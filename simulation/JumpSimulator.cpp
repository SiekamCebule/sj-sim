#include "JumpSimulator.h"

#include <QDebug>
#include <tuple>

JumpSimulator::JumpSimulator(Jumper *jumper, ConditionsInfo *conditionsInfo, Hill *hill, Competition *competition) : jumper(jumper),
    conditionsInfo(conditionsInfo),
    hill(hill),
    competition(competition)
{}

Competition *JumpSimulator::getCompetition() const
{
    return competition;
}

void JumpSimulator::setCompetition(Competition *newCompetition)
{
    competition = newCompetition;
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
}

void JumpSimulator::simulate()
{
    double height = 0; //max 100
    double speed = 0; //max 100
    double distance = 1;

    //TAKEOFF
    setupTakeoffEffect();





    //PĘTLA KAŻDEGO METRA

}

void JumpSimulator::setupTakeoffEffect()
{
    takeoffEffect.duration = 10;

    double takeoffHeightSum = 52;
    double takeoffSpeedSum = 48.5;

    QVector<QPair<double, double>> effectModels; //Jest tam 10 par procentowych dla 10-metrowego efektu skoku (8% ze skoczni K120 = 9.6 = 10)
    effectModels.push_back(QPair<double, double>(13, 7)); //procenty, procenty
    effectModels.push_back(QPair<double, double>(12.4, 7.4)); //2
    effectModels.push_back(QPair<double, double>(12, 8)); //3
    effectModels.push_back(QPair<double, double>(11.4, 8.7)); //4
    effectModels.push_back(QPair<double, double>(10.5, 9.1)); //5

    effectModels.push_back(QPair<double, double>(9.8, 10)); //6
    effectModels.push_back(QPair<double, double>(9, 11)); //7
    effectModels.push_back(QPair<double, double>(8, 12)); //8
    effectModels.push_back(QPair<double, double>(7.1, 13)); //9
    effectModels.push_back(QPair<double, double>(6.8, 13.8)); //10

    if(takeoffEffect.getDuration() == 10)
        takeoffEffect.effects = effectModels;
    else{
        QVector<QPair<double,double>> copyModels = effectModels;
        if(takeoffEffect.getDuration() < 10) //Dla skoczni mniejszych niż około K120 (dokładnie max K119) --> trzeba scalić te modele w ilość równą takeoffEffect.getDuration()
        {
            while(copyModels.length() > takeoffEffect.getDuration())
            {
                double heightToDivideSum = copyModels[copyModels.length() - 1].first;
                double speedToDivideSum = copyModels[copyModels.length() - 1].second;
                copyModels.erase(copyModels.constEnd() - 1);
                double heightToDivideOnce = heightToDivideSum / copyModels.length();
                double speedToDivideOnce = speedToDivideSum / copyModels.length();

                for(auto & model : copyModels)
                {
                    model.first += heightToDivideOnce;
                    model.second += speedToDivideOnce;
                }
            }
            takeoffEffect.effects = copyModels;
        }
        else if(takeoffEffect.getDuration() > 10)//dla skoczni wiekszych niz około K120 (dokładnie od K126) --> trzeba rozszerzy te modele w ilość równą takeoffEffect.getDuration()
        {
            QVector<std::tuple<bool, double, double>> pairsToInsert; // parzysta, wysokosc, predkosc
            for(int i=0; i<takeoffEffect.getDuration() - 10; i++)
            {
                double newHeight = 0;
                double newSpeed = 0;

                if(copyModels.length() % 2 == 0) // jest parzysta
                {
                    newHeight = (copyModels[copyModels.length() / 2 - 1].first + copyModels[copyModels.length() / 2].first) / 2; //Dwie wartosci w połowie
                    newSpeed = (copyModels[copyModels.length() / 2 - 1].second + copyModels[copyModels.length() / 2].second) / 2; //Dwie wartosci w połowie

                    pairsToInsert.push_back(std::tuple<bool, double, double>(true, newHeight, newSpeed));
                }
                else
                {
                    newHeight = (copyModels[copyModels.length() / 2].first - copyModels[copyModels.length() / 2 + 1].first) / 2; //Dwie wartosci w połowie
                    newSpeed = (copyModels[copyModels.length() / 2].second - copyModels[copyModels.length() / 2 + 1].second) / 2; //Dwie wartosci w połowie

                    pairsToInsert.push_back(std::tuple<bool, double, double>(false, newHeight, newSpeed));
                }
                for(auto & model : copyModels)
                {
                    model.first -= newHeight / copyModels.length();
                    model.second -= newSpeed / copyModels.length();
                }
            }
            for(auto & pairs : pairsToInsert)
            {
                if(std::get<0>(pairs) == true)
                    copyModels.insert(copyModels.constBegin() + copyModels.length() / 2 - 1, QPair<double, double>(std::get<1>(pairs), std::get<2>(pairs)));
                else
                    copyModels.insert(copyModels.constBegin() + copyModels.length() / 2, QPair<double, double>(std::get<1>(pairs), std::get<2>(pairs)));
            }
        }
        takeoffEffect.effects = copyModels;
    }
    qDebug()<<takeoffEffect.effects.length();
    for(const auto & model : takeoffEffect.effects)
    {
        qDebug()<<model.first<<", "<<model.second;
    }


    allEffects.push_back(takeoffEffect);
}

JumpSimulator::JumpEffect::JumpEffect()
{
    actualIndex = 0;
    duration = 0;
}

int JumpSimulator::JumpEffect::getDuration() const
{
    return duration;
}
