#include "AbstractCompetitionManager.h"

#include <QDebug>

AbstractCompetitionManager::AbstractCompetitionManager(short type, int startingGate) : type(type), roundStartingGate(startingGate)
{
    actualGate = actualRound = 0;
    results = nullptr;
    competitionRules = nullptr;
    competitionInfo = nullptr;
    toBeatDistance = toAdvanceDistance = 0;
    isCoachGate = false;
    actualCoachGate = 0;
    actualJumper = nullptr;
    actualStartListIndex = actualJumperPointsToTheLeader = 0;
    DSQBaseProbability = 0;
    roundShouldBeEnded = competiitonShouldBeEnded = lastJump = false;
}

void AbstractCompetitionManager::updateToBeatDistance()
{
    results->sortInDescendingOrder();
    Hill * hill = competitionInfo->getHill();
    double leaderPoints = leaderResult->getPointsSum();

    double windCompensation = WindsCalculator::getWindCompensation(WindsCalculator::getAveragedWind(actualWinds, competitionRules->getWindAverageCalculatingType()).getStrengthToAveragedWind(), competitionInfo->getHill());
    int tempGate = actualGate;
    if(coachGateForNextJumper == true) tempGate = actualCoachGate;
    double gateCompensation = WindsCalculator::getGateCompensation(roundsStartingGates.at(actualRound - 1), tempGate, competitionInfo->getHill());
    toBeatDistance = 0;
    if(actualRound == 1){
        toBeatDistance = leaderPoints;
    }
    else{
        double actualJumperPoints = results->getResultOfIndividualJumper(actualJumper)->getPointsSum();
        toBeatDistance = leaderPoints - actualJumperPoints;
    }
    toBeatDistance -= hill->getPointsForKPoint();
    if(competitionRules->getHasJudgesPoints())
    {
        toBeatDistance -= 54;
    }
    if(competitionRules->getHasGateCompensations())
        toBeatDistance -= gateCompensation;
    if(competitionRules->getHasWindCompensations())
        toBeatDistance -= windCompensation;

    toBeatDistance /= hill->getPointsForMeter();
    toBeatDistance += hill->getKPoint();
    toBeatDistance = std::round(toBeatDistance * 2) / 2;
}

void AbstractCompetitionManager::updateToAdvanceDistance()
{
    results->sortInDescendingOrder();
    Hill * hill = competitionInfo->getHill();

    int shouldBeQualified = 0;
    if(actualRound < competitionRules->getRounds().count())
        shouldBeQualified = competitionRules->getRounds().at(actualRound).getCount();
    toAdvanceDistance = 0;
    if(actualRoundJumpers.count() - shouldBeQualified - (actualStartListIndex) > 0 || actualRound == competitionRules->getRounds().count() || actualRoundJumpers.count() <= shouldBeQualified || lastQualifiedResult == nullptr)
    {
        toAdvanceDistance = -1;
    }
    else{
        double lastQualifiedPoints = lastQualifiedResult->getPointsSum();
        if(actualRound == 1){
            toAdvanceDistance = lastQualifiedPoints;
        }
        else{
            double actualJumperPoints = indResults->getResultsOfJumper(actualJumper)->getPointsSum();
            toAdvanceDistance = lastQualifiedPoints - actualJumperPoints;
        }
        double windCompensation = WindsCalculator::getWindCompensation(WindsCalculator::getAveragedWind(actualWinds, competitionRules->getWindAverageCalculatingType()).getStrengthToAveragedWind(), competitionInfo->getHill());
        int tempGate = actualGate;
        if(isCoachGate == true) tempGate = getActualCoachGate();
        double gateCompensation = WindsCalculator::getGateCompensation(getRoundStartingGate(), tempGate, competitionInfo->getHill());

        toAdvanceDistance -= hill->getPointsForKPoint();
        if(competitionRules->getHasJudgesPoints())
        {
            toAdvanceDistance -= 54;
        }
        if(competitionRules->getHasGateCompensations())
            toAdvanceDistance -= gateCompensation;
        if(competitionRules->getHasWindCompensations())
            toAdvanceDistance -= windCompensation;

        toAdvanceDistance /= hill->getPointsForMeter();
        toAdvanceDistance += hill->getKPoint();
        toAdvanceDistance = std::round(toAdvanceDistance * 2) / 2;
    }
}

void AbstractCompetitionManager::updateLeaderResult()
{
    results->sortInDescendingOrder();
    leaderResult = results->getResultByIndex(0);
}

void AbstractCompetitionManager::updateLastQualifiedResult()
{
    results->sortInDescendingOrder();

    int lastQualifiedPosition = 0;

    int shouldBeQualified = 0;
    if(actualRound < competitionRules->getRounds().count())
        shouldBeQualified = competitionRules->getRounds().at(actualRound).getCount();
    if(actualRoundJumpers.count() - shouldBeQualified - (actualStartListIndex) > 0 || actualRound == competitionRules->getRounds().count() || actualRoundJumpers.count() <= shouldBeQualified){
        lastQualifiedPosition = -1;
    }
    else{
        bool isExAequoOccured = false;
        int exAequoFirstPositionAfterShouldBeQualified = (-1);
        int i=0;
        int oldPosition = 0;
        while(!(i == indResults->getJumpersResults().count() - 1)){
            if(indResults->getJumpersResults().at(i).getPosition() == oldPosition){
                if(indResults->getJumpersResults().at(i).getPosition() > shouldBeQualified)
                    exAequoFirstPositionAfterShouldBeQualified = indResults->getJumpersResults().at(i).getPosition();
                isExAequoOccured = true;
                break;
            }
            oldPosition = indResults->getJumpersResults().at(i).getPosition();
            i++;
        }
        // Jeżeli egzekwo jest powyzej shouldBeQualified
        if(isExAequoOccured == false || (shouldBeQualified - actualStartListIndex > 0) || exAequoFirstPositionAfterShouldBeQualified == (-1)){
            lastQualifiedPosition = abs(actualRoundJumpers.count() - shouldBeQualified - (actualStartListIndex+1));
        }
        else{
            qDebug()<<shouldBeQualified<<", "<<indResults->getEditableJumpersResults().count();
            for(int i=shouldBeQualified - 1; i<indResults->getEditableJumpersResults().count(); i++){
                if(i+1 == indResults->getJumpersResults().at(i).getPosition())
                {
                    lastQualifiedPosition = i+1;
                }
            }
        }
        qDebug()<<"lq: "<<lastQualifiedPosition;
    }

    if(lastQualifiedPosition > 0 && lastQualifiedPosition <= indResults->getJumpersResults().count() && actualRound < competitionRules->getRounds().count()){
        lastQualifiedResult = indResults->getPointerOfExactJumperResults(lastQualifiedPosition - 1);
    }
    else{
        //qDebug()<<"hhh: "<<", "<<bool(lastQualifiedPosition > 0)<<","<<bool(lastQualifiedPosition < indResults->getJumpersResults().count())<<","<<bool(actualRound < competitionRules->getRounds().count());
        lastQualifiedResult = nullptr;
    }
}

short AbstractCompetitionManager::getType() const
{
    return type;
}

void AbstractCompetitionManager::setType(short newType)
{
    type = newType;
}

CompetitionInfo *AbstractCompetitionManager::getCompetitionInfo() const
{
    return competitionInfo;
}

void AbstractCompetitionManager::setCompetitionInfo(CompetitionInfo *newCompetitionInfo)
{
    competitionInfo = newCompetitionInfo;
}

CompetitionRules *AbstractCompetitionManager::getCompetitionRules() const
{
    return competitionRules;
}

void AbstractCompetitionManager::setCompetitionRules(CompetitionRules *newCompetitionRules)
{
    competitionRules = newCompetitionRules;
}

CompetitionResults *AbstractCompetitionManager::getResults() const
{
    return results;
}

void AbstractCompetitionManager::setResults(CompetitionResults *newResults)
{
    results = newResults;
}

QVector<int> AbstractCompetitionManager::getRoundsStartingGates() const
{
    return roundsStartingGates;
}

void AbstractCompetitionManager::setRoundsStartingGates(const QVector<int> &newRoundsStartingGates)
{
    roundsStartingGates = newRoundsStartingGates;
}

int AbstractCompetitionManager::getActualGate() const
{
    return actualGate;
}

void AbstractCompetitionManager::setActualGate(int newActualGate)
{
    actualGate = newActualGate;
}

int AbstractCompetitionManager::getActualRound() const
{
    return actualRound;
}

void AbstractCompetitionManager::setActualRound(int newActualRound)
{
    actualRound = newActualRound;
}

double AbstractCompetitionManager::getToBeatLineDistance() const
{
    return toBeatLineDistance;
}

void AbstractCompetitionManager::setToBeatLineDistance(double newToBeatLineDistance)
{
    toBeatLineDistance = newToBeatLineDistance;
}

double AbstractCompetitionManager::getToAdvanceLineDistance() const
{
    return toAdvanceLineDistance;
}

void AbstractCompetitionManager::setToAdvanceLineDistance(double newToAdvanceLineDistance)
{
    toAdvanceLineDistance = newToAdvanceLineDistance;
}

bool AbstractCompetitionManager::getCoachGateForNextJumper() const
{
    return coachGateForNextJumper;
}

void AbstractCompetitionManager::setCoachGateForNextJumper(bool newCoachGateForNextJumper)
{
    coachGateForNextJumper = newCoachGateForNextJumper;
}

int AbstractCompetitionManager::getActualCoachGate() const
{
    return actualCoachGate;
}

void AbstractCompetitionManager::setActualCoachGate(int newActualCoachGate)
{
    actualCoachGate = newActualCoachGate;
}

int AbstractCompetitionManager::getActualStartListIndex() const
{
    return actualStartListIndex;
}

void AbstractCompetitionManager::setActualStartListIndex(int newActualStartListIndex)
{
    actualStartListIndex = newActualStartListIndex;
}

double AbstractCompetitionManager::getActualCompetitorPointsToTheLeader() const
{
    return actualCompetitorPointsToTheLeader;
}

void AbstractCompetitionManager::setActualCompetitorPointsToTheLeader(double newActualCompetitorPointsToTheLeader)
{
    actualCompetitorPointsToTheLeader = newActualCompetitorPointsToTheLeader;
}

JumpManipulator AbstractCompetitionManager::getActualJumpManipulator() const
{
    return actualJumpManipulator;
}

void AbstractCompetitionManager::setActualJumpManipulator(const JumpManipulator &newActualJumpManipulator)
{
    actualJumpManipulator = newActualJumpManipulator;
}

QVector<StartListCompetitorStatus> AbstractCompetitionManager::getStartListStatus() const
{
    return startListStatus;
}

void AbstractCompetitionManager::setStartListStatus(const QVector<StartListCompetitorStatus> &newStartListStatus)
{
    startListStatus = newStartListStatus;
}

bool AbstractCompetitionManager::getRoundShouldBeEnded() const
{
    return roundShouldBeEnded;
}

void AbstractCompetitionManager::setRoundShouldBeEnded(bool newRoundShouldBeEnded)
{
    roundShouldBeEnded = newRoundShouldBeEnded;
}

bool AbstractCompetitionManager::getCompetiitonShouldBeEnded() const
{
    return competiitonShouldBeEnded;
}

void AbstractCompetitionManager::setCompetiitonShouldBeEnded(bool newCompetiitonShouldBeEnded)
{
    competiitonShouldBeEnded = newCompetiitonShouldBeEnded;
}

bool AbstractCompetitionManager::getLastJump() const
{
    return lastJump;
}

void AbstractCompetitionManager::setLastJump(bool newLastJump)
{
    lastJump = newLastJump;
}

CompetitionSingleResult *AbstractCompetitionManager::getLeaderResult() const
{
    return leaderResult;
}

void AbstractCompetitionManager::setLeaderResult(CompetitionSingleResult *newLeaderResult)
{
    leaderResult = newLeaderResult;
}

CompetitionSingleResult *AbstractCompetitionManager::getLastQualifiedResult() const
{
    return lastQualifiedResult;
}

void AbstractCompetitionManager::setLastQualifiedResult(CompetitionSingleResult *newLastQualifiedResult)
{
    lastQualifiedResult = newLastQualifiedResult;
}

CompetitionInfo *AbstractCompetitionManager::getCompetitionInfo() const
{
    return competitionInfo;
}

void AbstractCompetitionManager::setCompetitionInfo(CompetitionInfo *newCompetitionInfo)
{
    competitionInfo = newCompetitionInfo;
}

short AbstractCompetitionManager::getType() const
{
    return type;
}

void AbstractCompetitionManager::setType(short newType)
{
    type = newType;
}

void AbstractCompetitionManager::generateActualWinds()
{
    windsGenerator.setGenerationSettings(actualWindGenerationSettings);
    actualWinds = windsGenerator.generateWinds();
}

bool AbstractCompetitionManager::getIsCoachGate() const
{
    return isCoachGate;
}

void AbstractCompetitionManager::setIsCoachGate(bool newIsCoachGate)
{
    isCoachGate = newIsCoachGate;
}

int AbstractCompetitionManager::getActualCoachGate() const
{
    return actualCoachGate;
}

void AbstractCompetitionManager::setActualCoachGate(int newActualCoachGate)
{
    actualCoachGate = newActualCoachGate;
}

JumpManipulator AbstractCompetitionManager::getActualJumpManipulator() const
{
    return actualJumpManipulator;
}

void AbstractCompetitionManager::setActualJumpManipulator(const JumpManipulator &newActualJumpManipulator)
{
    actualJumpManipulator = newActualJumpManipulator;
}

double AbstractCompetitionManager::getActualJumperPointsToTheLeader() const
{
    return actualJumperPointsToTheLeader;
}

void AbstractCompetitionManager::setActualJumperPointsToTheLeader(double newActualJumperPointsToTheLeader)
{
    actualJumperPointsToTheLeader = newActualJumperPointsToTheLeader;
}

double AbstractCompetitionManager::getToAdvanceDistance() const
{
    return toAdvanceDistance;
}

void AbstractCompetitionManager::setToAdvanceDistance(double newToAdvanceDistance)
{
    toAdvanceDistance = newToAdvanceDistance;
}

int AbstractCompetitionManager::getActualJumperIndex() const
{
    return actualStartListIndex;
}

void AbstractCompetitionManager::setActualJumperIndex(int newActualJumperIndex)
{
    actualStartListIndex = newActualJumperIndex;
    emit actualJumperIndexChanged();
}

Jumper *AbstractCompetitionManager::getActualJumper() const
{
    return actualJumper;
}

void AbstractCompetitionManager::setActualJumper(Jumper *newActualJumper)
{
    actualJumper = newActualJumper;
}

QVector<Wind> AbstractCompetitionManager::getActualWinds() const
{
    return actualWinds;
}

void AbstractCompetitionManager::setActualWinds(const QVector<Wind> &newActualWinds)
{
    actualWinds = newActualWinds;
}

int AbstractCompetitionManager::getRoundStartingGate() const
{
    return roundStartingGate;
}

void AbstractCompetitionManager::setRoundStartingGate(int newRoundStartingGate)
{
    roundStartingGate = newRoundStartingGate;
}

double AbstractCompetitionManager::getToBeatDistance() const
{
    return toBeatDistance;
}

void AbstractCompetitionManager::setToBeatDistance(double newToBeatDistance)
{
    toBeatDistance = newToBeatDistance;
}

bool AbstractCompetitionManager::getLastJump() const
{
    return lastJump;
}

void AbstractCompetitionManager::setLastJump(bool newLastJump)
{
    lastJump = newLastJump;
}

bool AbstractCompetitionManager::getCompetiitonShouldBeEnded() const
{
    return competiitonShouldBeEnded;
}

void AbstractCompetitionManager::setCompetiitonShouldBeEnded(bool newCompetiitonShouldBeEnded)
{
    competiitonShouldBeEnded = newCompetiitonShouldBeEnded;
}

bool AbstractCompetitionManager::getRoundShouldBeEnded() const
{
    return roundShouldBeEnded;
}

void AbstractCompetitionManager::setRoundShouldBeEnded(bool newRoundShouldBeEnded)
{
    roundShouldBeEnded = newRoundShouldBeEnded;
}

int AbstractCompetitionManager::getDSQBaseProbability() const
{
    return DSQBaseProbability;
}

void AbstractCompetitionManager::setDSQBaseProbability(int newDSQBaseProbability)
{
    DSQBaseProbability = newDSQBaseProbability;
}

CompetitionResults *AbstractCompetitionManager::getResults() const
{
    return results;
}

void AbstractCompetitionManager::setResults(CompetitionResults *newResults)
{
    results = newResults;
}

CompetitionRules *AbstractCompetitionManager::getCompetitionRules() const
{
    return competitionRules;
}

CompetitionRules *AbstractCompetitionManager::getEditableCompetitionRules()
{
    return competitionRules;
}

void AbstractCompetitionManager::setCompetitionRules(CompetitionRules *newCompetitionRules)
{
    competitionRules = newCompetitionRules;
}

QVector<bool> * AbstractCompetitionManager::getCompletedJumpsPointer()
{
    return &completedJumps;
}

QSet<int> * AbstractCompetitionManager::getHasDSQPointer()
{
    return &hasDSQ;
}

QSet<int> * AbstractCompetitionManager::getHasDNSPointer()
{
    return &hasDNS;
}

int AbstractCompetitionManager::getActualRound() const
{
    return actualRound;
}

void AbstractCompetitionManager::setActualRound(int newActualRound)
{
    actualRound = newActualRound;
}

int AbstractCompetitionManager::getActualGate() const
{
    return actualGate;
}

void AbstractCompetitionManager::setActualGate(int newActualGate)
{
    actualGate = newActualGate;
}

short AbstractCompetitionManager::getType() const
{
    return type;
}

void AbstractCompetitionManager::setType(short newType)
{
    type = newType;
}

QVector<WindGenerationSettings> AbstractCompetitionManager::getActualWindGenerationSettings() const
{
    return actualWindGenerationSettings;
}

QVector<WindGenerationSettings> * AbstractCompetitionManager::getActualWindGenerationSettingsPointer()
{
    return &actualWindGenerationSettings;
}

void AbstractCompetitionManager::setActualWindGenerationSettings(const QVector<WindGenerationSettings> &newActualWindGenerationSettings)
{
    actualWindGenerationSettings = newActualWindGenerationSettings;
}

CompetitionInfo *AbstractCompetitionManager::getCompetitionInfo() const
{
    return competitionInfo;
}

CompetitionInfo *AbstractCompetitionManager::getEditableCompetitionInfo()
{
    return competitionInfo;
}

void AbstractCompetitionManager::setCompetitionInfo(CompetitionInfo *newCompetitionInfo)
{
    competitionInfo = newCompetitionInfo;
}
