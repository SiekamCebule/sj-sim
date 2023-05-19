#include "IndividualCompetitionManager.h"
#include "../../utilities/functions.h"

IndividualCompetitionManager::IndividualCompetitionManager(short type, int startingGate) : AbstractCompetitionManager(type, startingGate)
{
    setCoachGateForNextJumper(false);
    setActualCoachGate(0);
    connect(this, &IndividualCompetitionManager::actualJumperIndexChanged, this, [this](){
        actualJumper = roundsJumpers[0].at(actualStartListIndex);
    });
    leaderResult = nullptr;
    lastQualifiedResult = nullptr;
}

void IndividualCompetitionManager::updateToBeatLineDistance()
{
    results->sortInDescendingOrder();
    Hill * hill = competitionInfo->getHill();
    double leaderPoints = leaderResult->getPointsSum();

    double windCompensation = WindsCalculator::getWindCompensation(WindsCalculator::getAveragedWind(*actualWinds, competitionRules->getWindAverageCalculatingType()).getStrengthToAveragedWind(), competitionInfo->getHill());
    int tempGate = actualGate;
    if(coachGateForNextJumper == true) tempGate = actualCoachGate;
    double gateCompensation = WindsCalculator::getGateCompensation(roundsStartingGates.at(actualRound - 1), tempGate, competitionInfo->getHill());
    toBeatLineDistance = 0;
    if(actualRound == 1){
        toBeatLineDistance = leaderPoints;
    }
    else{
        double actualJumperPoints = results->getResultOfIndividualJumper(actualJumper)->getPointsSum();
        toBeatLineDistance = leaderPoints - actualJumperPoints;
    }
    toBeatLineDistance -= hill->getPointsForKPoint();
    if(competitionRules->getHasJudgesPoints())
    {
        toBeatLineDistance -= 54;
    }
    if(competitionRules->getHasGateCompensations())
        toBeatLineDistance -= gateCompensation;
    if(competitionRules->getHasWindCompensations())
        toBeatLineDistance -= windCompensation;

    toBeatLineDistance /= hill->getPointsForMeter();
    toBeatLineDistance += hill->getKPoint();
    toBeatLineDistance = std::round(toBeatLineDistance * 2) / 2;
}

void IndividualCompetitionManager::updateToAdvanceLineDistance()
{
    results->sortInDescendingOrder();
    Hill * hill = competitionInfo->getHill();

    int shouldBeQualified = 0;
    if(actualRound < competitionRules->getRounds().count())
        shouldBeQualified = competitionRules->getRounds().at(actualRound).getCount();
    toAdvanceLineDistance = 0;
    if(roundsJumpers[actualRound - 1].count() - shouldBeQualified - (actualStartListIndex) > 0 || actualRound == competitionRules->getRounds().count() || roundsJumpers[actualRound - 1].count() <= shouldBeQualified || lastQualifiedResult == nullptr)
    {
        toAdvanceLineDistance = -1;
    }
    else{
        double lastQualifiedPoints = lastQualifiedResult->getPointsSum();
        if(actualRound == 1){
            toAdvanceLineDistance = lastQualifiedPoints;
        }
        else{
            double actualJumperPoints = results->getResultOfIndividualJumper(actualJumper)->getPointsSum();
            toAdvanceLineDistance = lastQualifiedPoints - actualJumperPoints;
        }
        double windCompensation = WindsCalculator::getWindCompensation(WindsCalculator::getAveragedWind(*actualWinds, competitionRules->getWindAverageCalculatingType()).getStrengthToAveragedWind(), competitionInfo->getHill());
        int tempGate = actualGate;
        if(coachGateForNextJumper == true) tempGate = getActualCoachGate();
        double gateCompensation = WindsCalculator::getGateCompensation(roundsStartingGates[actualRound - 1], tempGate, competitionInfo->getHill());

        toAdvanceLineDistance -= hill->getPointsForKPoint();
        if(competitionRules->getHasJudgesPoints())
        {
            toAdvanceLineDistance -= 54;
        }
        if(competitionRules->getHasGateCompensations())
            toAdvanceLineDistance -= gateCompensation;
        if(competitionRules->getHasWindCompensations())
            toAdvanceLineDistance -= windCompensation;

        toAdvanceLineDistance /= hill->getPointsForMeter();
        toAdvanceLineDistance += hill->getKPoint();
        toAdvanceLineDistance = std::round(toAdvanceLineDistance * 2) / 2;
    }
}

void IndividualCompetitionManager::updateActualCompetitorPointsToTheLeader()
{
    if(actualRound == 1)
        actualCompetitorPointsToTheLeader = (-1);
    else{
        double actualJumperPoints = results->getResultOfIndividualJumper(actualJumper)->getPointsSum();
        double actualLeaderPointsWithoutActualRound = leaderResult->getPointsSum() - leaderResult->getJumps().at(leaderResult->getJumps().count() - 1).getPoints();
        actualCompetitorPointsToTheLeader = roundDoubleToOnePlace(actualJumperPoints - actualLeaderPointsWithoutActualRound);
    }
}

void IndividualCompetitionManager::updateLastQualifiedResult()
{
    results->sortInDescendingOrder();

    int lastQualifiedPosition = 0;

    int shouldBeQualified = 0;
    if(actualRound < competitionRules->getRounds().count())
        shouldBeQualified = competitionRules->getRounds().at(actualRound).getCount();
    if(roundsJumpers[actualRound - 1].count() - shouldBeQualified - (actualStartListIndex) > 0 || actualRound == competitionRules->getRounds().count() || roundsJumpers[actualRound - 1].count() <= shouldBeQualified){
        lastQualifiedPosition = -1;
    }
    else{
        bool isExAequoOccured = false;
        int exAequoFirstPositionAfterShouldBeQualified = (-1);
        int i=0;
        int oldPosition = 0;
        while(!(i == results->getResultsReference().count() - 1)){
            if(results->getResultsReference().at(i).getPosition() == oldPosition){
                if(results->getResultsReference().at(i).getPosition() > shouldBeQualified)
                    exAequoFirstPositionAfterShouldBeQualified = results->getResultsReference().at(i).getPosition();
                isExAequoOccured = true;
                break;
            }
            oldPosition = results->getResultsReference().at(i).getPosition();
            i++;
        }
        // Jeżeli egzekwo jest powyzej shouldBeQualified
        if(isExAequoOccured == false || (shouldBeQualified - actualStartListIndex > 0) || exAequoFirstPositionAfterShouldBeQualified == (-1)){
            lastQualifiedPosition = abs(roundsJumpers[actualRound - 1].count() - shouldBeQualified - (actualStartListIndex+1));
        }
        else{
            for(int i=shouldBeQualified - 1; i<results->getResultsReference().count(); i++){
                if(i+1 == results->getResultsReference().at(i).getPosition())
                {
                    lastQualifiedPosition = i+1;
                }
            }
        }
        qDebug()<<"lq: "<<lastQualifiedPosition;
    }

    if(lastQualifiedPosition > 0 && lastQualifiedPosition <= results->getResultsReference().count() && actualRound < competitionRules->getRounds().count()){
        lastQualifiedResult = results->getResultByIndex(lastQualifiedPosition - 1);
    }
    else{
        //qDebug()<<"hhh: "<<", "<<bool(lastQualifiedPosition > 0)<<","<<bool(lastQualifiedPosition < indResults->getJumpersResults().count())<<","<<bool(actualRound < competitionRules->getRounds().count());
        lastQualifiedResult = nullptr;
    }
}

void IndividualCompetitionManager::setActualJumperToNextUnfinished()
{
    setActualStartListIndex(getFirstUnfinishedStartListStatus());
    setActualJumper(startListStatuses[getFirstUnfinishedStartListStatus()].getJumper());
}

void IndividualCompetitionManager::setupNextRound()
{

}

QVector<QVector<Jumper *> > IndividualCompetitionManager::getRoundsJumpers() const
{
    return roundsJumpers;
}

QVector<QVector<Jumper *> > & IndividualCompetitionManager::getRoundsJumpersReference()
{
    return roundsJumpers;
}

void IndividualCompetitionManager::setRoundsJumpers(const QVector<QVector<Jumper *> > &newRoundsJumpers)
{
    roundsJumpers = newRoundsJumpers;
}

void IndividualCompetitionManager::setFirstRoundJumpers(QVector<Jumper *> jumpers)
{
    if(roundsJumpers.count() == 0)
        roundsJumpers.append(jumpers);
    else
        roundsJumpers[0] = jumpers;
}

QVector<Jumper *> IndividualCompetitionManager::getFilteredJumpersVector(QVector<Jumper*> * previousRoundJumpers, CompetitionResults *results, CompetitionRules *rules, int round, const QVector<StartListCompetitorStatus> & startListStatuses)
{
    QSet<ulong> fallQualified;
    for(auto & status : startListStatuses){
        if(status.getQualifiedBy95HSRule())
            fallQualified.insert(status.getJumper()->getID());
    }
    QVector<Jumper *> jumpers;

    if(rules->getRounds().count() < round)
    {
        qDebug()<<"Za mało rund! przerywam...";
        return jumpers;
    }
    int maxJumpersCount = rules->getRounds().at(round - 1).getCount();
    bool sortStartList = rules->getRounds().at(round - 1).getSortStartList();

    results->sortInDescendingOrder();

    if(previousRoundJumpers->count() > maxJumpersCount){
        // Sprawdzanie, czy nie wystąpiło ex aequo (Dokładnie to czy jest zawodnik z pozycją równą round.count()
        int i=0;
        bool isLastQualifyPositionOccured = false;
        int exaequoAt = 0;
        while(!(results->getResultsReference().at(i).getPosition() > maxJumpersCount)){
            if(results->getResultsReference().at(i).getPosition() == maxJumpersCount){
                isLastQualifyPositionOccured = true;
                break;
            }
            i++;
        }
        i = maxJumpersCount;
        if(isLastQualifyPositionOccured == false) // Jeśli wystąpiło ex aequo
        {
            //Trzeba znaleźć następną pozycję od maxCount na której jest zawodnik (to nasze egzekwo)
            for(int i=maxJumpersCount - 1; i<results->getResultsReference().count(); i++){
                if(i+1 == results->getResultsReference().at(i).getPosition()) //znalezlismy egzekwo
                {
                    exaequoAt = i+1;
                }
            }
        }

        for(auto & jum : *previousRoundJumpers){
            qDebug()<<fallQualified.size()<<"grg";
            qDebug()<<jum->getNameAndSurname()<<": "<<fallQualified.contains(results->getResultOfIndividualJumper(jum)->getID());
            if(results->getResultOfIndividualJumper(jum)->getPosition() <= maxJumpersCount || fallQualified.contains(results->getResultOfIndividualJumper(jum)->getID())){
                jumpers.push_back(jum);
            }
            if(results->getResultOfIndividualJumper(jum)->getPosition() > maxJumpersCount){
                if(isLastQualifyPositionOccured == false){
                    if(results->getResultOfIndividualJumper(jum)->getPosition() == exaequoAt)
                        jumpers.push_back(jum);
                }
            }
        }
    }
    else jumpers = *previousRoundJumpers;
    if(sortStartList == true){
        QVector<CompetitionSingleResult> jumpersResultsAscending = results->getResultsReference();
        std::sort(jumpersResultsAscending.begin(), jumpersResultsAscending.end());
        QVector<Jumper *> newJumpers;
        for(auto & res : jumpersResultsAscending){
            for(auto & jum : jumpers){
                if(jum == res.getJumper()){
                    newJumpers.push_back(jum);
                    break;
                }
            }
        }
        jumpers = newJumpers;
    }

    return jumpers;
}
