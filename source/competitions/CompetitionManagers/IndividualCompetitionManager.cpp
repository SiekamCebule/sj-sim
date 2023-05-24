#include "IndividualCompetitionManager.h"
#include "../../utilities/functions.h"

IndividualCompetitionManager::IndividualCompetitionManager(short type, int startingGate) : AbstractCompetitionManager(type, startingGate)
{
    setCoachGateForNextJumper(false);
    setActualCoachGate(0);
    connect(this, &IndividualCompetitionManager::actualStartListIndexChanged, this, [this](){
        if(roundsJumpers.count() > 0)
            actualJumper = roundsJumpers[actualRound - 1].at(actualStartListIndex);
    });
    leaderResult = nullptr;
    lastQualifiedResult = nullptr;
}

void IndividualCompetitionManager::updateToBeatLineDistance()
{
    results->sortInDescendingOrder();
    Hill * hill = competitionInfo->getHill();
    double leaderPoints = leaderResult->getPointsSum();
    qDebug()<<leaderPoints<<" --> (leader)";

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
        qDebug()<<"leader: "<<leaderPoints<<", actual: "<<actualJumperPoints;
        qDebug()<<"TOBEAT: HH: "<<toBeatLineDistance;
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

void IndividualCompetitionManager::updateCompetitorsAdvanceStatuses()
{
    for(auto & status : startListStatuses)
    {
        if(actualRound == competitionRules->getRounds().count() || getActualRoundJumpersReference().count() <= competitionRules->getRounds().at(actualRound).getCount() || status.getQualifiedBy95HSRule())
            status.setAdvanceStatus(StartListCompetitorStatus::SureAdvanced);
        else if(lastQualifiedResult == nullptr || status.getJumpStatus() == StartListCompetitorStatus::Unfinished)
            status.setAdvanceStatus(StartListCompetitorStatus::Waiting);
        else{
            if(results->getResultOfIndividualJumper(status.getJumper())->getPosition() <= lastQualifiedResult->getPosition())
                status.setAdvanceStatus(StartListCompetitorStatus::SureAdvanced);
            else{
                int lastWaiting = competitionRules->getRounds().at(actualRound).getCount();
                results->sortInDescendingOrder();
                QVector<CompetitionSingleResult> * res = &results->getResultsReference();
                int lastPosition = 0;
                qDebug()<<"ptr";
                qDebug()<<res->count();
                for(int i = res->count() - 1; i>=0; i--){
                    qDebug()<<res->at(i).getPosition()<<", "<<competitionRules->getRounds().at(actualRound).getCount();
                    if(res->at(i).getPosition() < competitionRules->getRounds().at(actualRound).getCount()){
                        qDebug()<<"ZMNK";
                        if(res->at(i).getPosition() == lastPosition) //mamy egzekwo
                        {
                            qDebug()<<"egzekwo";
                            int exaequos = 0;
                            for(int i = res->count() - 1; i>=0; i--){
                                if(res->at(i).getPosition() == lastPosition){
                                    exaequos++;
                                    lastPosition = res->at(i).getPosition();
                                }
                                else
                                    break;
                            }
                            //if(exaequos >= (competitionRules->getRounds().at(actualRound - 1).getCount() - getActualRoundJumpersReference().count()))
                            if(exaequos >= (competitionRules->getRounds().at(actualRound - 1).getCount() - lastPosition))
                                lastWaiting += exaequos;
                        }
                    }
                }
                if(results->getResultOfIndividualJumper(status.getJumper())->getPosition() <= lastWaiting)
                    status.setAdvanceStatus(StartListCompetitorStatus::Waiting);
                else
                    status.setAdvanceStatus(StartListCompetitorStatus::SureDroppedOut);
            }
        }
    }
}

QVector<Jumper *> IndividualCompetitionManager::getFilteredJumpersForNextRound()
{
    QVector<Jumper *> jumpers;
    if(competitionRules->getRounds().at(actualRound - 1).getSortStartList() == true){
        results->sortInAscendingOrder();
        for(auto & res : results->getResultsReference()){
            if(StartListCompetitorStatus::getStatusOfJumper(res.getJumper(), startListStatuses)->getAdvanceStatus() == StartListCompetitorStatus::SureAdvanced)
                jumpers.push_back(res.getJumper());
        }
        results->sortInDescendingOrder();
    }
    else{
        for(auto & status : startListStatuses){
            if(status.getAdvanceStatus() == StartListCompetitorStatus::SureAdvanced)
                jumpers.push_back(status.getJumper());
        }
    }
    return jumpers;
}

//lastWaiting czyli ostatni zawodnik który ma szansę na awans. Ostatnia waiting pozycja to teorytycznie count dla roundy ale co jak będzie ex aequo?

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
            if(results->getResultsReference().at(i).getPosition() == oldPosition){ //mamy egzekwo
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
    qDebug()<<"Nowa runda";
    actualRound++; //Przechodzi do następnej rundy
    qDebug()<<results->getResultsReference().count();
    qDebug()<<this->competitionRules->getName();
    roundsJumpers.push_back(getFilteredJumpersForNextRound());
    qDebug()<<"a";
    setActualStartListIndex(0);
    qDebug()<<"a";
    roundsStartingGates.push_back(actualGate);
    qDebug()<<"a";
    setupStartListStatusesForActualRound();
}

void IndividualCompetitionManager::setupStartListStatusesForActualRound()
{
    startListStatuses.clear();
    for(auto & jumper : getActualRoundJumpersReference()){
        qDebug()<<"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB: "<<jumper->getNameAndSurname();
        startListStatuses.push_back(StartListCompetitorStatus(jumper));
    }

    qDebug()<<"startListStatuses :"<<startListStatuses.count();
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
