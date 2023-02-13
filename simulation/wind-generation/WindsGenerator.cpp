#include "WindsGenerator.h"

#include "../../utilities/MyRandom.h"

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

        const double baseDirectionChanceMultiplier = 35 / settings->getWindDirectionInstability();

        switch(settings->getBaseDirection())
        {
        case Wind::Back:
            backSideProb *= 2.74;
            sideProb *= 0.97;
            frontSideProb *= 0.22;
            frontProb *= 0.07;

            backProb *= baseDirectionChanceMultiplier;
            break;
        case Wind::BackSide:
            backProb *= 2.00;
            sideProb *= 1.53;
            frontSideProb *= 0.33;
            frontProb *= 0.15;

            backSideProb *= baseDirectionChanceMultiplier;
            break;
        case Wind::Side:
            backProb *= 0.68;
            backSideProb *= 1.32;
            frontSideProb *= 1.32;
            frontProb *= 0.68;

            sideProb *= baseDirectionChanceMultiplier;
            break;
        case Wind::FrontSide:
            backProb *= 0.15;
            backSideProb *= 0.33;
            sideProb *= 1.53;
            frontProb *= 2.00;

            frontSideProb *= baseDirectionChanceMultiplier;
            break;
        case Wind::Front:
            backProb *= 0.07;
            backSideProb *= 0.22;
            sideProb *= 0.97;
            frontSideProb *= 2.74;

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

    for(const auto & wind : winds)
    {
        qDebug()<<"Wiatr "<<wind.getStringDirection(false)<<", o prędkości "<<wind.getValue()<<"m/s";
    }
    return winds;
}
