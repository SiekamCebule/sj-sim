#ifndef GLOBALSIMULATIONSETTINGS_H
#define GLOBALSIMULATIONSETTINGS_H


class GlobalSimulationSettings
{
private:
    GlobalSimulationSettings();
    ~GlobalSimulationSettings();
    static GlobalSimulationSettings * m_globalSimulationSettings;

private:
    double simulationMultiplier;
    int baseDsqProbability;

public:
    GlobalSimulationSettings(GlobalSimulationSettings &) = delete;
    static GlobalSimulationSettings * get();

    bool writeToFile();
    bool loadFromFile();

    double getSimulationMultiplier() const;
    void setSimulationMultiplier(double newSimulationMultiplier);
    int getBaseDsqProbability() const;
    void setBaseDsqProbability(int newBaseDsqProbability);
};

#endif // GLOBALSIMULATIONSETTINGS_H
