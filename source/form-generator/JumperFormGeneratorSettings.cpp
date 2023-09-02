#include "JumperFormGeneratorSettings.h"

JumperFormGeneratorSettings::JumperFormGeneratorSettings()
{
    reset();
}

void JumperFormGeneratorSettings::reset()
{
    tendenceVariability = tendenceBonus = tendenceAlignmentMultiplier = formVariability = formBonus = 0;
    minTendence = -10;
    minForm = 0;
    maxTendence = 10;
    maxForm = 100;
    jumper = nullptr;
}

QVector<JumperFormGeneratorSettings> JumperFormGeneratorSettings::constructJumperFormGeneratorsSettingsVector(QVector<Jumper *> &jumpers)
{
    QVector<JumperFormGeneratorSettings> toReturn;
    for(auto & jumper : jumpers)
    {
        JumperFormGeneratorSettings settings;
        settings.setJumper(jumper);
        toReturn.push_back(settings);
    }
    return toReturn;
}

double JumperFormGeneratorSettings::getTendenceAlignmentMultiplier() const
{
    return tendenceAlignmentMultiplier;
}

void JumperFormGeneratorSettings::setTendenceAlignmentMultiplier(double newTendenceAlignmentMultiplier)
{
    tendenceAlignmentMultiplier = newTendenceAlignmentMultiplier;
}

Jumper *JumperFormGeneratorSettings::getJumper() const
{
    return jumper;
}

void JumperFormGeneratorSettings::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}

double JumperFormGeneratorSettings::getTendenceVariability() const
{
    return tendenceVariability;
}

void JumperFormGeneratorSettings::setTendenceVariability(double newTendenceVariability)
{
    tendenceVariability = newTendenceVariability;
}

double JumperFormGeneratorSettings::getTendenceBonus() const
{
    return tendenceBonus;
}

void JumperFormGeneratorSettings::setTendenceBonus(double newTendenceBonus)
{
    tendenceBonus = newTendenceBonus;
}

double JumperFormGeneratorSettings::getMinTendence() const
{
    return minTendence;
}

void JumperFormGeneratorSettings::setMinTendence(double newMinTendence)
{
    minTendence = newMinTendence;
}

double JumperFormGeneratorSettings::getMaxTendence() const
{
    return maxTendence;
}

void JumperFormGeneratorSettings::setMaxTendence(double newMaxTendence)
{
    maxTendence = newMaxTendence;
}

double JumperFormGeneratorSettings::getFormVariability() const
{
    return formVariability;
}

void JumperFormGeneratorSettings::setFormVariability(double newFormVariability)
{
    formVariability = newFormVariability;
}

double JumperFormGeneratorSettings::getFormBonus() const
{
    return formBonus;
}

void JumperFormGeneratorSettings::setFormBonus(double newFormBonus)
{
    formBonus = newFormBonus;
}

double JumperFormGeneratorSettings::getMinForm() const
{
    return minForm;
}

void JumperFormGeneratorSettings::setMinForm(double newMinForm)
{
    minForm = newMinForm;
}

double JumperFormGeneratorSettings::getMaxForm() const
{
    return maxForm;
}

void JumperFormGeneratorSettings::setMaxForm(double newMaxForm)
{
    maxForm = newMaxForm;
}
