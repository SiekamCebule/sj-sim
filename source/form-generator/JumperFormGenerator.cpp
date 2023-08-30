#include "JumperFormGenerator.h"

JumperFormGenerator::JumperFormGenerator()
{

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
}
