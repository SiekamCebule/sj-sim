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
    AbstractCompetitionManager(short type = CompetitionRules::Individual, int startingGate = 0);

    virtual void simulateNext() {}
    virtual bool checkRoundEnd() {return false;}
    virtual bool checkCompetitionEnd() {return false;}

    virtual void setupNextRound() {}
    virtual void fillCompletedJumpsToStartOfRound() {}

    virtual int getCompetitiorsCountForActualRound() {return 0;}
    virtual bool getSortStartListForActualRound() {return false;}

    virtual void updateToBeatDistance() {}
    virtual void updateToAdvanceDistance() {}
    virtual void updateLeaderResult() {}
    virtual void updateLastQualifiedResult() {}
    virtual void updateActualJumperPointsToTheLeader() {}

    void generateActualWinds();

signals:
    void roundEnd();
    void competitionEnd();

    void actualJumperIndexChanged();

protected:
    short type;

    JumpSimulator simulator;
    WindsGenerator windsGenerator;
    QVector<WindGenerationSettings> actualWindGenerationSettings;

    CompetitionRules * competitionRules;
    AbstractCompetitionResults * results;
    CompetitionInfo * competitionInfo;

    int roundStartingGate;
    int actualGate;
    int actualRound; //liczone od 1.
    double toBeatDistance;
    double toAdvanceDistance;

    bool isCoachGate;
    int actualCoachGate;

    QVector<Wind> actualWinds;

    Jumper * actualJumper;
    int actualJumperIndex;
    double actualJumperPointsToTheLeader;
    JumpManipulator actualJumpManipulator;

    QVector<bool> completedJumps;
    QSet<int> hasDNS;
    QSet<int> hasDSQ;

    int DSQBaseProbability;

    bool roundShouldBeEnded;
    bool competiitonShouldBeEnded;
    bool lastJump;

public:
    CompetitionInfo *getCompetitionInfo() const;
    CompetitionInfo *getEditableCompetitionInfo();
    void setCompetitionInfo(CompetitionInfo *newCompetitionInfo);
    QVector<WindGenerationSettings> getActualWindGenerationSettings() const;
    QVector<WindGenerationSettings> * getActualWindGenerationSettingsPointer();
    void setActualWindGenerationSettings(const QVector<WindGenerationSettings> &newActualWindGenerationSettings);
    short getType() const;
    void setType(short newType);
    int getActualGate() const;
    void setActualGate(int newActualGate);
    int getActualRound() const;
    void setActualRound(int newActualRound);
    AbstractCompetitionResults getQualificationsResults() const;
    QVector<bool> * getCompletedJumpsPointer();
    QSet<int> *getHasDSQPointer();
    QSet<int> * getHasDNSPointer();
    CompetitionRules *getCompetitionRules() const;
    CompetitionRules *getEditableCompetitionRules();
    void setCompetitionRules(CompetitionRules *newCompetitionRules);
    AbstractCompetitionResults *getResults() const;
    void setResults(AbstractCompetitionResults *newResults);
    int getDSQBaseProbability() const;
    void setDSQBaseProbability(int newDSQBaseProbability);
    bool getRoundShouldBeEnded() const;
    void setRoundShouldBeEnded(bool newRoundShouldBeEnded);
    bool getCompetiitonShouldBeEnded() const;
    void setCompetiitonShouldBeEnded(bool newCompetiitonShouldBeEnded);
    bool getLastJump() const;
    void setLastJump(bool newLastJump);
    double getToBeatDistance() const;
    void setToBeatDistance(double newToBeatDistance);
    int getRoundStartingGate() const;
    void setRoundStartingGate(int newRoundStartingGate);
    QVector<Wind> getNextJumperWinds() const;
    void setNextJumperWinds(const QVector<Wind> &newNextJumperWinds);
    QVector<Wind> getActualWinds() const;
    void setActualWinds(const QVector<Wind> &newActualWinds);
    Jumper *getActualJumper() const;
    void setActualJumper(Jumper *newActualJumper);
    int getActualJumperIndex() const;
    void setActualJumperIndex(int newActualJumperIndex);
    double getToAdvanceDistance() const;
    void setToAdvanceDistance(double newToAdvanceDistance);
    double getActualJumperPointsToTheLeader() const;
    void setActualJumperPointsToTheLeader(double newActualJumperPointsToTheLeader);
    JumpManipulator getActualJumpManipulator() const;
    void setActualJumpManipulator(const JumpManipulator &newActualJumpManipulator);
    int getActualCoachGate() const;
    void setActualCoachGate(int newActualCoachGate);
    bool getIsCoachGate() const;
    void setIsCoachGate(bool newIsCoachGate);
};

#endif // ABSTRACTCOMPETITIONMANAGER_H
