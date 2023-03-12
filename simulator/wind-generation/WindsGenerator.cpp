#include "WindsGenerator.h"

#include "../../global/MyRandom.h"
#include "../../utilities/functions.h"

#include <QDebug>
#include <random> //std

WindsGenerator::WindsGenerator(const QVector<WindGenerationSettings> &generationSettings) :
    generationSettings(generationSettings)
{}

int WindsGenerator::getWindsCount() const
{
    return generationSettings.count();
}

QVector<WindGenerationSettings> WindsGenerator::getGenerationSettings() const
{
    return generationSettings;
}

void WindsGenerator::setGenerationSettings(const QVector<WindGenerationSettings> &newGenerationSettings)
{
    generationSettings = newGenerationSettings;
}


QVector<Wind> WindsGenerator::generateWinds()
{
    QVector<Wind> winds;

    for(int i=0; i<getWindsCount(); i++)
    {
        Wind wind;
        WindGenerationSettings * settings = &generationSettings[i];

        double windStrength = settings->getBaseWindStrength();

        double base = MyRandom::randomDouble(-settings->getBaseWindStrength() / 20, settings->getBaseWindStrength() / 20);
        double deviation = settings->getWindStrengthInstability() / 5.75;

        std::random_device rd{};
        std::mt19937 generator{rd()};
        std::normal_distribution<double> distribution(base, deviation);
        double random = 0;
        if(deviation > 0)
            random = distribution(generator);

        short randomType = MyRandom::randomInt(0, 1);
        switch(randomType)
        {
        case 0: windStrength += random; break;
        case 1: windStrength -= random; break;
        default: break;
        }

        qDebug()<<"(random: "<<QString::number(random)<<")";

        short windDirection = Wind::Null;

        //qDebug()<<settings->getWindDirectionInstability()<<" (DIRECTION INSTABILITY)";
        //qDebug()<<Wind::getStringWindDirection(settings->getBaseDirection(), true)<<" (BASE DIRECTION)";

        double backProb = 250 + settings->getLevelOfCharacteristic("back-wind-probability") * 100;
        double backSideProb = 250 + settings->getLevelOfCharacteristic("back-side-wind-probability") * 100;
        double sideProb = 250 + settings->getLevelOfCharacteristic("side-wind-probability") * 100;
        double frontSideProb = 250 + settings->getLevelOfCharacteristic("front-side-wind-probability") * 100;
        double frontProb = 250 + settings->getLevelOfCharacteristic("front-wind-probability") * 100;

        const double baseDirectionChanceMultiplier = 35 - ((settings->getWindDirectionInstability()) * 3.2);

        if(settings->getWindDirectionInstability() == 0)
        {
            QVector<double *> probs;
            probs.push_back(&backProb);
            probs.push_back(&backSideProb);
            probs.push_back(&sideProb);
            probs.push_back(&frontSideProb);
            probs.push_back(&frontProb);

            int i=0;
            for(auto & prob : probs)
            {
                if(i != settings->getBaseDirection())
                    *prob = 0;
                else *prob = 1;
                i++;
            }

        }
        else{
            switch(settings->getBaseDirection())
            {
            case Wind::Back:
                backSideProb *= 2.55;
                sideProb *= 0.55;
                frontSideProb *= 0.07;
                frontProb *= 0.01;

                backProb *= baseDirectionChanceMultiplier;
                break;
            case Wind::BackSide:
                backProb *= 2.4;
                sideProb *= 2.0;
                frontSideProb *= 0.172;
                frontProb *= 0.028;

                backSideProb *= baseDirectionChanceMultiplier;
                break;
            case Wind::Side:
                backProb *= 0.13;
                backSideProb *= 2.5;
                frontSideProb *= 2.5;
                frontProb *= 0.13;

                sideProb *= baseDirectionChanceMultiplier;
                break;
            case Wind::FrontSide:
                backProb *= 0.028;
                backSideProb *= 0.172;
                sideProb *= 2.0;
                frontProb *= 2.4;

                frontSideProb *= baseDirectionChanceMultiplier;
                break;
            case Wind::Front:
                backProb *= 0.01;
                backSideProb *= 0.07;
                sideProb *= 0.55;
                frontSideProb *= 2.55;

                frontProb *= baseDirectionChanceMultiplier;
                break;
            }
        }

        QVector<double> probabilities;
        double drawSum = backProb + backSideProb + sideProb + frontSideProb + frontProb;
        probabilities.push_back(backProb);
        probabilities.push_back(backSideProb);
        probabilities.push_back(sideProb);
        probabilities.push_back(frontSideProb);
        probabilities.push_back(frontProb);
        double drawRandom = MyRandom::randomDouble(0, drawSum);

        double actualSum = 0;
        for(int i=0; i < 5; i++)
        {
            actualSum += probabilities[i];
            if(drawRandom < actualSum){
                windDirection = i + 1; // tak jak w enumie Wind::Direction
                break;
            }
        }

        if(windStrength < 0)
            windStrength = 0;

        wind.setStrength(windStrength);
        wind.setDirection(windDirection);

        winds.push_back(wind);
    }

    /*for(const auto & wind : winds)
    {
        qDebug()<<"Wiatru "<<wind.getStringDirection(false)<<", o prędkości "<<wind.getValue()<<"m/s";
    }*/
    return winds;
}

int WindsGenerator::calculateWindsCountByKPoint(double KPoint)
{
    if(KPoint < 25)
        return 1;
    else if(KPoint < 50)
        return 2;
    else if(KPoint < 75)
        return 3;
    else if(KPoint < 100)
        return 4;
    else if(KPoint < 135)
        return 5;
    else if(KPoint < 160)
        return 6;
    else if(KPoint < 205)
        return 7;
    else
    {
        int i = 8;
        i += (KPoint - 205) / 54;
        return i;
    }
}

QPair<double, double> WindsGenerator::getRangeOfWindSector(int sector, double KPoint)
{
    int windsCount = WindsGenerator::calculateWindsCountByKPoint(KPoint);
    double segmentDistance = (KPoint + (KPoint / windsCount)) / windsCount;
    if(sector == WindsGenerator::calculateWindsCountByKPoint(KPoint))
        return QPair<double, double>(roundDoubleToHalf(((double)windsCount - 1) * segmentDistance), (-1));

    double min = 0, max = 0;
    min = roundDoubleToHalf((sector - 1) * (segmentDistance));
    max = roundDoubleToHalf(sector * segmentDistance);

    return QPair<double, double>(min, max);
}
