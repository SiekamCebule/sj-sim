#ifndef ABSTRACTCOMPETITIONMANAGER_H
#define ABSTRACTCOMPETITIONMANAGER_H

#include <QObject>
#include "../simulator/JumpSimulator.h"
#include "../simulator/JumpManipulator.h"
#include "CompetitionRules.h"
#include "CompetitionInfo.h"
#include "AbstractCompetitionResults.h"
#include "../simulator/wind-generation/WindGenerationSettings.h"
#include "../simulator/wind-generation/WindsGenerator.h"

class AbstractCompetitionManager : public QObject
{
    Q_OBJECT
public:
    AbstractCompetitionManager(short type = CompetitionRules::Individual);

    void initActualRound();
    virtual void simulateNext() {}
    virtual bool checkRoundEnd() {return false;}
    virtual bool checkCompetitionEnd() {return false;}

    virtual void setupNextRound() {}
    virtual void fillCompletedJumpsToStartOfRound() {}

    virtual int getCompetitiorsCountForActualRound() {return 0;}
    virtual bool getSortStartListForActualRound() {return false;}

signals:
    void roundEnd();
    void competitionEnd();

protected:
    short type;

    JumpSimulator simulator;
    WindsGenerator windsGenerator;
    QVector<WindGenerationSettings> actualWindGenerationSettings;

    CompetitionRules * competitionRules;
    AbstractCompetitionResults * results;
    CompetitionInfo * competitionInfo;

    int actualGate;
    int actualRound; //liczone od 1.   0 - kwalifikacje (poj. konkurs)
    bool runQualificationsForSingleCompetition;
    AbstractCompetitionResults qualificationsResults;

    QVector<bool> completedJumps;
    QSet<int> hasDNS;
    QSet<int> hasDSQ;

    int DSQBaseProbability;

public:
    CompetitionInfo *getCompetitionInfo() const;
    void setCompetitionInfo(CompetitionInfo *newCompetitionInfo);
    QVector<WindGenerationSettings> getActualWindGenerationSettings() const;
    void setActualWindGenerationSettings(const QVector<WindGenerationSettings> &newActualWindGenerationSettings);
    short getType() const;
    void setType(short newType);
    int getActualGate() const;
    void setActualGate(int newActualGate);
    int getActualRound() const;
    void setActualRound(int newActualRound);
    bool getRunQualificationsForSingleCompetition() const;
    void setRunQualificationsForSingleCompetition(bool newRunQualificationsForSingleCompetition);
    AbstractCompetitionResults getQualificationsResults() const;
    QVector<bool> * getCompletedJumpsPointer();
    QSet<int> *getHasDSQPointer();
    QSet<int> * getHasDNSPointer();
    CompetitionRules *getCompetitionRules() const;
    void setCompetitionRules(CompetitionRules *newCompetitionRules);
    AbstractCompetitionResults *getResults() const;
    void setResults(AbstractCompetitionResults *newResults);
    int getDSQBaseProbability() const;
    void setDSQBaseProbability(int newDSQBaseProbability);
};

#endif // ABSTRACTCOMPETITIONMANAGER_H
