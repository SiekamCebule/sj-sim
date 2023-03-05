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
    SingleJumpsManager(int gate = 0, int jumpsCount = 0, bool saveResultsToFile = false, const QString &resultsFileName = "", bool changeableWind = false, short resultsFormat = 0);

    void simulate();

    enum ResultsFileFormat{
        Json,
        Csv
    };
    bool saveResultsToFile(short fileFormat = Json);

private:
    Jumper jumper;
    Hill hill;
    WindsGenerator windsGenerator;
    QVector<WindGenerationSettings> windsGeneratorSettings;
    JumpSimulator jumpSimulator;

    int jumpsCount;
    bool changeableWind;
    short resultsFormat;
    QString resultsFileName;
    short windAverageCalculatingType;
    short windCompensationDistanceEffect;
    int gate;

    QVector<JumpData> jumps;

public:
    Jumper getJumper() const;
    void setJumper(const Jumper &newJumper);
    Hill getHill() const;
    void setHill(const Hill &newHill);
    int getJumpsCount() const;
    void setJumpsCount(int newJumpsCount);
    QVector<JumpData> getJumps() const;
    QVector<JumpData> & getEditableJumps();
    void setJumps(const QVector<JumpData> &newJumps);
    QVector<WindGenerationSettings> getWindsGeneratorSettings() const;
    void setWindsGeneratorSettings(const QVector<WindGenerationSettings> &newWindsGeneratorSettings);
    bool getChangeableWind() const;
    void setChangeableWind(bool newChangeableWind);
    QString getResultsFileName() const;
    void setResultsFileName(const QString &newResultsFileName);
    int getGate() const;
    void setGate(int newGate);
    short getWindAverageCalculatingType() const;
    void setWindAverageCalculatingType(short newWindAverageCalculatingType);
    short getWindCompensationDistanceEffect() const;
    void setWindCompensationDistanceEffect(short newWindCompensationDistanceEffect);
    short getResultsFormat() const;
    void setResultsFormat(short newResultsFormat);
};

#endif // SINGLEJUMPSMANAGER_H
