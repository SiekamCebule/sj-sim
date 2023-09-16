#ifndef GLOBALSIMULATIONSETTINGS_H
#define GLOBALSIMULATIONSETTINGS_H


class GlobalSimulationSettings
{
private:
    GlobalSimulationSettings();
    ~GlobalSimulationSettings();
    static GlobalSimulationSettings * m_globalSimulationSettings;

private:
    int maxSkills;
    double simulationRandomMultiplier;
    double hillsEffectsMultiplier;
    bool autoAdjustHillEffects;
    int baseDsqProbability;

public:
    GlobalSimulationSettings(GlobalSimulationSettings &) = delete;
    static GlobalSimulationSettings * get();

    bool writeToFile();
    bool loadFromFile();

    int getBaseDsqProbability() const;
    void setBaseDsqProbability(int newBaseDsqProbability);
    int getMaxSkills() const;
    void setMaxSkills(int newMaxSkills);
    double getSimulationRandomMultiplier() const;
    void setSimulationRandomMultiplier(double newSimulationRandomMultiplier);
    bool getAutoAdjustHillEffects() const;
    void setAutoAdjustHillEffects(bool newAutoAdjustHillEffects);
    double getHillsEffectsMultiplier() const;
    void setHillsEffectsMultiplier(double newHillsEffectsMultiplier);
};

#endif // GLOBALSIMULATIONSETTINGS_H
