#ifndef JUMPERFORMGENERATORSETTINGS_H
#define JUMPERFORMGENERATORSETTINGS_H

#include <QVector>
#include "../simulator/Jumper.h"

class JumperFormGeneratorSettings
{
public:
    JumperFormGeneratorSettings();
    void reset();

    static QVector<JumperFormGeneratorSettings> constructJumperFormGeneratorsSettingsVector(QVector<Jumper *> & jumpers);

private:
    //TENDENCE
    double tendenceVariability;
    double tendenceBonus;
    double minTendence;
    double maxTendence;
    double tendenceAlignmentMultiplier;
    //FORM
    double formVariability;
    double formBonus;
    double minForm;
    double maxForm;

    Jumper * jumper;
public:
    double getTendenceVariability() const;
    void setTendenceVariability(double newTendenceVariability);
    double getTendenceBonus() const;
    void setTendenceBonus(double newTendenceBonus);
    double getMinTendence() const;
    void setMinTendence(double newMinTendence);
    double getMaxTendence() const;
    void setMaxTendence(double newMaxTendence);
    double getFormVariability() const;
    void setFormVariability(double newFormVariability);
    double getFormBonus() const;
    void setFormBonus(double newFormBonus);
    double getMinForm() const;
    void setMinForm(double newMinForm);
    double getMaxForm() const;
    void setMaxForm(double newMaxForm);
    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    double getTendenceAlignmentMultiplier() const;
    void setTendenceAlignmentMultiplier(double newTendenceAlignmentMultiplier);
};

#endif // JUMPERFORMGENERATORSETTINGS_H
