#include "WindsGenerator.h"

#include "../../global/MyRandom.h"

#include <QDebug>

WindsGenerator::WindsGenerator(int windsCount, const QVector<WindGenerationSettings> &generationSettings) : windsCount(windsCount),
    generationSettings(generationSettings)
{}


QVector<Wind> WindsGenerator::generateWinds()
{
    QVector<Wind> winds;

    for(int i=0; i<windsCount; i++)
    {
        Wind wind;
        WindGenerationSettings * settings = &generationSettings[i];

        double windValue = settings->getBaseWindStrength();
        double random = MyRandom::reducingChancesRandom(0, settings->getBaseWindStrength() * 1.3, settings->getBaseWindStrength() / 45, 1, (1.33 - settings->getWindStrengthInstability() / 39.5), MyRandom::InTurnFromTheHighestChanceNumber, MyRandom::FromSmallerToLarger);
        short randomType = MyRandom::randomInt(0, 1);
        switch(randomType)
        {
        case 0: windValue += random; break;
        case 1: windValue -= random; break;
        default: break;
        }

        short windDirection = Wind::Null;

        double backProb = 250 + settings->getLevelOfCharacteristic("back-wind-probability") * 100;
        double backSideProb = 250 + settings->getLevelOfCharacteristic("back-side-wind-probability") * 100;
        double sideProb = 250 + settings->getLevelOfCharacteristic("side-wind-probability") * 100;
        double frontSideProb = 250 + settings->getLevelOfCharacteristic("front-side-wind-probability") * 100;
        double frontProb = 250 + settings->getLevelOfCharacteristic("front-wind-probability") * 100;

        const double baseDirectionChanceMultiplier = 133 - (settings->getWindDirectionInstability() * 13);

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

        wind.setValue(windValue);
        wind.setDirection(windDirection);

        winds.push_back(wind);
    }

    /*for(const auto & wind : winds)
    {
        qDebug()<<"Wiatru "<<wind.getStringDirection(false)<<", o prędkości "<<wind.getValue()<<"m/s";
    }*/
    return winds;
}
