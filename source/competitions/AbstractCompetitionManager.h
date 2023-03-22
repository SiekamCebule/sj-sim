#ifndef ABSTRACTCOMPETITIONMANAGER_H
#define ABSTRACTCOMPETITIONMANAGER_H

#include <QObject>
#include "../simulator/JumpSimulator.h"
#include "../simulator/JumpManipulator.h"
#include "CompetitionRules.h"
#include "CompetitionInfo.h"
#include "CompetitionResults.h"
#include "../simulator/wind-generation/WindGenerationSettings.h"
#include "../simulator/wind-generation/WindsGenerator.h"

class AbstractCompetitionManager : public QObject
{
    Q_OBJECT
public:
    AbstractCompetitionManager(short type = CompetitionRules::Individual);

    virtual void simulateNext(const JumpManipulator& manipulator = JumpManipulator()) {}

protected:
    short type;

    JumpSimulator simulator;
    WindsGenerator windsGenerator;
    QVector<WindGenerationSettings> actualWindGenerationSettings;

    CompetitionRules competitionRules;
    CompetitionResults results;
    CompetitionInfo * competitionInfo;

    int actualGate;

public:
    CompetitionInfo *getCompetitionInfo() const;
    void setCompetitionInfo(CompetitionInfo *newCompetitionInfo);
    CompetitionRules getCompetitionRules() const;
    void setCompetitionRules(const CompetitionRules &newCompetitionRules);
    QVector<WindGenerationSettings> getActualWindGenerationSettings() const;
    void setActualWindGenerationSettings(const QVector<WindGenerationSettings> &newActualWindGenerationSettings);
    short getType() const;
    void setType(short newType);
    int getActualGate() const;
    void setActualGate(int newActualGate);
};

#endif // ABSTRACTCOMPETITIONMANAGER_H
