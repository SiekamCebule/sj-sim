#ifndef SINGLEJUMPSMANAGER_H
#define SINGLEJUMPSMANAGER_H

#include <QVector>
#include "../simulator/JumpData.h"
#include "../simulator/Hill.h"
#include "../simulator/WindsInfo.h"
#include "../simulator/wind-generation/WindsGenerator.h"
#include "../simulator/wind-generation/WindGenerationSettings.h"
#include "../simulator/JumpSimulator.h"

class SingleJumpsManager
{
public:
    SingleJumpsManager(int gate = 0, int jumpsCount = 0, bool saveResultsToFile = false, const QString &resultsFileName = "", bool changeableWind = false, const QVector<WindGenerationSettings> &windsGeneratorSettings = QVector<WindGenerationSettings>(), const Jumper &jumper = Jumper(), const Hill &hill = Hill());

    void simulate();

    Jumper getJumper() const;
    void setJumper(const Jumper &newJumper);
    Hill getHill() const;
    void setHill(const Hill &newHill);
    int getJumpsCount() const;
    void setJumpsCount(int newJumpsCount);
    QVector<JumpData> getJumps() const;
    void setJumps(const QVector<JumpData> &newJumps);
    QVector<WindGenerationSettings> getWindsGeneratorSettings() const;
    void setWindsGeneratorSettings(const QVector<WindGenerationSettings> &newWindsGeneratorSettings);
    bool getChangeableWind() const;
    void setChangeableWind(bool newChangeableWind);
    bool getSaveResultsToFile() const;
    void setSaveResultsToFile(bool newSaveResultsToFile);
    QString getResultsFileName() const;
    void setResultsFileName(const QString &newResultsFileName);
    int getGate() const;
    void setGate(int newGate);  
    short getWindAverageCalculatingType() const;
    void setWindAverageCalculatingType(short newWindAverageCalculatingType);

private:
    Jumper jumper;
    Hill hill;
    WindsGenerator windsGenerator;
    QVector<WindGenerationSettings> windsGeneratorSettings;
    JumpSimulator jumpSimulator;

    int jumpsCount;
    bool changeableWind;
    bool saveResultsToFile;
    QString resultsFileName;
    short windAverageCalculatingType;
    int gate;

    QVector<JumpData> jumps;
};

#endif // SINGLEJUMPSMANAGER_H
