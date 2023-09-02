#ifndef JUMPERFORMGENERATOR_H
#define JUMPERFORMGENERATOR_H

#include "JumperFormGeneratorSettings.h"
#include "JumperFormTendence.h"

class JumperFormGenerator
{
public:
    JumperFormGenerator();

    void generateJumperFormTendence();
    void generateJumperForm();

private:
    JumperFormGeneratorSettings settings;
    JumperFormTendence * tendence;
    Jumper * jumper;
public:
    JumperFormGeneratorSettings & getSettingsReference();
    void setSettings(const JumperFormGeneratorSettings &newSettings);
    JumperFormTendence *getTendence() const;
    void setTendence(JumperFormTendence *newTendence);
    Jumper *getJumper() const;
};

#endif // JUMPERFORMGENERATOR_H
