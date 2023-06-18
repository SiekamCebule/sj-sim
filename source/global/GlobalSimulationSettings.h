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
    int baseDsqProbability;

public:
    GlobalSimulationSettings(GlobalSimulationSettings &) = delete;
    static GlobalSimulationSettings * get();

    bool writeToFile();
    bool loadFromFile();

    int getBaseDsqProbability() const;
    void setBaseDsqProbability(int newBaseDsqProbability);
    void updateSimulationRandomMultiplier();
    int getMaxSkills() const;
    void setMaxSkills(int newMaxSkills);
    double getSimulationRandomMultiplier() const;
    void setSimulationRandomMultiplier(double newSimulationRandomMultiplier);
};

#endif // GLOBALSIMULATIONSETTINGS_H
