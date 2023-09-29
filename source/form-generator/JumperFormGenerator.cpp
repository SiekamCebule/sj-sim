#include "JumperFormGenerator.h"
#include "../global/MyRandom.h"

JumperFormGenerator::JumperFormGenerator()
{

}

void JumperFormGenerator::generateJumperFormTendence()
{
    double oldTendence = tendence->getTendence();
    double newTendence = 0;

    double base = 0;

    double deviation = 2.80 + ((settings.getTendenceVariability() - 5) / 1.785714285714286); //1.666666666666667);

    double random = 0;
    if(settings.getTendenceVariability() > 0)
        random = (MyRandom::normalDistributionRandom(base, deviation));
    else
        random = 0;

    double divider = 1 + (abs(oldTendence + random - 0) / 2.5);
    qDebug()<<"divider: "<<divider;
    if(oldTendence + (random / divider) > 0 && (random > 0))
        random /= divider;
    else if(oldTendence + (random / divider) < 0 && (random < 0))
        random /= divider;

    newTendence = oldTendence + random;

    double muliplier = settings.getTendenceAlignmentMultiplier();
    if(muliplier > 1)
        if(newTendence > 0)
            newTendence /= muliplier;
        else
            newTendence /= muliplier;
    else
        if(newTendence > 0)
            newTendence /= muliplier;
        else
            newTendence /= muliplier;

    newTendence += settings.getTendenceBonus();
    if(newTendence > settings.getMaxTendence())
        newTendence = settings.getMaxTendence();
    else if(newTendence < settings.getMinTendence())
        newTendence = settings.getMinTendence();

    tendence->setTendence(newTendence);
    qDebug()<<jumper->getNameAndSurname()<<": "<<oldTendence<<" --> "<<newTendence<<" (Tendencja)";
}

void JumperFormGenerator::generateJumperForm()
{
    double oldForm = jumper->getJumperSkillsPointer()->getForm();
    double formChange = 0;

    formChange += tendence->getTendence() * 4;
    formChange *= 1 + ((settings.getFormVariability() - 5) / 5);

    double distanceFromAverage = abs(oldForm - 50);
    double divider = 1 + (distanceFromAverage / 3.4);
    if(((oldForm + (formChange / divider)) > 50 && (formChange / divider) > 0) || ((oldForm + (formChange / divider)) < 50 && (formChange / divider) < 0))
        formChange /= divider;
    /*else
        formChange *= (divider / 1);*/

    double newForm = oldForm + formChange;
    newForm += settings.getFormBonus();
    if(newForm > settings.getMaxForm())
        newForm = settings.getMaxForm();
    else if(newForm < settings.getMinForm())
        newForm = settings.getMinForm();

    jumper->getJumperSkillsPointer()->setForm(newForm);
    qDebug()<<jumper->getNameAndSurname()<<": "<<oldForm<<" --> "<<newForm;//<<" (Forma na bazie tendencji " + QString::number(tendence->getTendence()) + ")";
}

Jumper *JumperFormGenerator::getJumper() const
{
    return jumper;
}

JumperFormTendence *JumperFormGenerator::getTendence() const
{
    return tendence;
}

void JumperFormGenerator::setTendence(JumperFormTendence *newTendence)
{
    tendence = newTendence;
    jumper = tendence->getJumper();
}

JumperFormGeneratorSettings &JumperFormGenerator::getSettingsReference()
{
    return settings;
}

void JumperFormGenerator::setSettings(const JumperFormGeneratorSettings &newSettings)
{
    settings = newSettings;
    jumper = settings.getJumper();
}
