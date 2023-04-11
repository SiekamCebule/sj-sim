#include "IndividualCompetitionManager.h"
#include "IndividualCompetitionResults.h"

IndividualCompetitionManager::IndividualCompetitionManager(short type, int startingGate) : AbstractCompetitionManager(type, startingGate)
{
    setIsCoachGate(false);
    setActualCoachGate(0);
    connect(this, &IndividualCompetitionManager::actualJumperIndexChanged, this, [this](){
        actualJumper = actualRoundJumpers.at(actualJumperIndex);
    });
    leaderResult = nullptr;
    lastQualifiedResult = nullptr;
}

void IndividualCompetitionManager::simulateNext()
{
    simulator.setGate(actualGate);
    simulator.setHasCoachGate(isCoachGate);
    simulator.setCoachGate(getActualCoachGate());
    simulator.setCompetitionStartGate(getRoundStartingGate());
    simulator.setJumper(actualJumper);
    simulator.setHill(competitionInfo->getHill());
    simulator.setManipulator(&this->actualJumpManipulator);
    simulator.setCompetitionRules(competitionRules);
    simulator.setDSQBaseProbability(this->getDSQBaseProbability());

    simulator.setWinds(actualWinds);
    simulator.simulateJump();
    JumpData jump = simulator.getJumpData();
    if(jump.getIsDSQOccured() == true)
        hasDSQ.insert(actualJumperIndex);

    qDebug()<<"SKOK: "<<jump;

    completedJumps[actualJumperIndex] = true;

    //Uzupełnianie vectora z wynikami
    bool jumperExists = false;
    IndividualCompetitionResults * indResults = dynamic_cast<IndividualCompetitionResults *>(this->results);
    IndividualCompetitionSingleResult * res = indResults->getResultsOfJumper(actualJumper);
    if(res != nullptr){
        res->getEditableJumps().push_back(jump);
        res->updatePointsSum();
        jumperExists = true;
    }
    if(jumperExists == false){
        indResults->getEditableJumpersResults().push_back(IndividualCompetitionSingleResult(jump));
        indResults->getResultsOfJumper(jump.getJumper())->updatePointsSum();
    }
    if(jump.getLanding().getType() == Landing::Fall && jump.getDistance() >= (competitionInfo->getHill()->getHSPoint() * 0.95) && competitionRules->getHas95HSRule() == true){
        qDebug()<<"UPADŁ POZA 95 HS%!";
        qualifiedBy95HSRule.insert(indResults->getEditableJumpersResults().last().getID());
    }

    indResults->sortJumpersResultsInDescendingOrder();

    actualJumpManipulator = JumpManipulator();
    if(checkRoundEnd() == true){
        if(actualRound == competitionInfo->getExceptionalRoundsCount()){
            qDebug()<<"ANKA";
            roundShouldBeEnded = false;
            competiitonShouldBeEnded = true;
            lastJump = true;
            //emit competitionEnd();
        }
        else{
            lastJump = true;
            if(checkCompetitionEnd() == false){
                roundShouldBeEnded = true;
                competiitonShouldBeEnded = false;
                //emit roundEnd();
            }
            else{
                competiitonShouldBeEnded = true;
                roundShouldBeEnded = false;
            }
        }
    }
    else{
        roundShouldBeEnded = false;
        competiitonShouldBeEnded = false;
        lastJump = false;
        setActualJumperIndex(getActualJumperIndex() + 1);// zmień skoczka na następnego
    }
}

bool IndividualCompetitionManager::checkRoundEnd()
{
    //roboczo:
    if(actualJumperIndex + 1 == actualRoundJumpers.count()){
        return true;
    }
    else
        return false;
    //Sprawdź czy każdy element vectoru completed ma wartosc true. Jeśli DSQ lub DNS, zalicz jako true
}

bool IndividualCompetitionManager::checkCompetitionEnd()
{
    if(actualRound == competitionRules->getRounds().count())
        return true;
    else
        return false;
}

void IndividualCompetitionManager::setupNextRound()
{
    qDebug()<<"Nowa runda";
    actualRound++; //Przechodzi do następnej rundy
    qDebug()<<"a";
    qDebug()<<actualRoundJumpers.count();
    qDebug()<<dynamic_cast<IndividualCompetitionResults *>(this->results)->getJumpersResults().count();
    qDebug()<<this->competitionRules->getName();
    qDebug()<<actualRound;
    qDebug()<<"HASAA: "<<qualifiedBy95HSRule.count();
    actualRoundJumpers = IndividualCompetitionManager::getFilteredJumpersVector(&actualRoundJumpers, dynamic_cast<IndividualCompetitionResults *>(this->results), this->competitionRules, actualRound, qualifiedBy95HSRule);
    qDebug()<<"a";
    setActualJumperIndex(0);
    qDebug()<<"a";
    completedJumps.fill(false, actualRoundJumpers.count());
    qDebug()<<"a";
    hasDNS.clear();
    qDebug()<<"a";
    hasDSQ.clear();
    qDebug()<<"a";
    roundStartingGate = actualGate;
    qDebug()<<"a";
    roundShouldBeEnded = false;
    qDebug()<<"a";
    competiitonShouldBeEnded = false;
    qDebug()<<"a";
    qualifiedBy95HSRule.clear();
}

void IndividualCompetitionManager::fillCompletedJumpsToStartOfRound()
{
    completedJumps.fill(false, actualRoundJumpers.count());
}

void IndividualCompetitionManager::updateToBeatDistance()
{
    IndividualCompetitionResults * indResults = dynamic_cast<IndividualCompetitionResults *>(this->results);
    Hill * hill = competitionInfo->getHill();
    double leaderPoints = leaderResult->getPointsSum();

    double windCompensation = WindsCalculator::getWindCompensation(WindsCalculator::getAveragedWind(actualWinds, competitionRules->getWindAverageCalculatingType()).getStrengthToAveragedWind(), competitionInfo->getHill());
    int tempGate = actualGate;
    if(isCoachGate == true) tempGate = getActualCoachGate();
    double gateCompensation = WindsCalculator::getGateCompensation(getRoundStartingGate(), tempGate, competitionInfo->getHill());
    double toBeat = 0;
    if(actualRound == 1){
        toBeat = leaderPoints;
    }
    else{
        double actualJumperPoints = indResults->getResultsOfJumper(actualJumper)->getPointsSum();
        toBeat = leaderPoints - actualJumperPoints;
    }
    toBeat -= hill->getPointsForKPoint();
    if(competitionRules->getHasJudgesPoints())
    {
        toBeat -= 54;
    }
    if(competitionRules->getHasGateCompensations())
        toBeat -= gateCompensation;
    if(competitionRules->getHasWindCompensations())
        toBeat -= windCompensation;

    toBeat /= hill->getPointsForMeter();
    toBeat += hill->getKPoint();
    toBeat = std::round(toBeat * 2) / 2;

    toBeatDistance = toBeat;
}

void IndividualCompetitionManager::updateToAdvanceDistance()
{
    IndividualCompetitionResults * indResults = dynamic_cast<IndividualCompetitionResults *>(this->results);
    indResults->sortJumpersResultsInDescendingOrder();
    Hill * hill = competitionInfo->getHill();

    int shouldBeQualified = 0;
    if(actualRound < competitionRules->getRounds().count())
        shouldBeQualified = competitionRules->getRounds().at(actualRound).getCount();
    double toAdvance = 0;
    if(actualRoundJumpers.count() - shouldBeQualified - (actualJumperIndex) > 0 || actualRound == competitionRules->getRounds().count() || actualRoundJumpers.count() <= shouldBeQualified || lastQualifiedResult == nullptr)
    {
        //qDebug()<<bool(actualRoundJumpers.count() - shouldBeQualified - (actualJumperIndex) > 0)<<","<<bool(actualRound == competitionRules->getRounds().count())<<", "<<bool(actualRoundJumpers.count() <= shouldBeQualified);
        toAdvance = -1;
    }

    else{
        double lastQualifiedPoints = lastQualifiedResult->getPointsSum();
        if(actualRound == 1){
            toAdvance = lastQualifiedPoints;
        }
        else{
            double actualJumperPoints = indResults->getResultsOfJumper(actualJumper)->getPointsSum();
            toAdvance = lastQualifiedPoints - actualJumperPoints;
        }
        double windCompensation = WindsCalculator::getWindCompensation(WindsCalculator::getAveragedWind(actualWinds, competitionRules->getWindAverageCalculatingType()).getStrengthToAveragedWind(), competitionInfo->getHill());
        int tempGate = actualGate;
        if(isCoachGate == true) tempGate = getActualCoachGate();
        double gateCompensation = WindsCalculator::getGateCompensation(getRoundStartingGate(), tempGate, competitionInfo->getHill());

        toAdvance -= hill->getPointsForKPoint();
        if(competitionRules->getHasJudgesPoints())
        {
            toAdvance -= 54;
        }
        if(competitionRules->getHasGateCompensations())
            toAdvance -= gateCompensation;
        if(competitionRules->getHasWindCompensations())
            toAdvance -= windCompensation;

        toAdvance /= hill->getPointsForMeter();
        toAdvance += hill->getKPoint();
        toAdvance = std::round(toAdvance * 2) / 2;
    }

    toAdvanceDistance = toAdvance;
}

void IndividualCompetitionManager::updateLeaderResult()
{
    dynamic_cast<IndividualCompetitionResults *>(this->results)->sortJumpersResultsInDescendingOrder();
    leaderResult = dynamic_cast<IndividualCompetitionResults *>(this->results)->getPointerOfExactJumperResults(0);
}

void IndividualCompetitionManager::updateLastQualifiedResult()
{
    IndividualCompetitionResults * indResults = dynamic_cast<IndividualCompetitionResults *>(this->results);
    indResults->sortJumpersResultsInDescendingOrder();

    int lastQualifiedPosition = 0;

    int shouldBeQualified = 0;
    if(actualRound < competitionRules->getRounds().count())
        shouldBeQualified = competitionRules->getRounds().at(actualRound).getCount();
    if(actualRoundJumpers.count() - shouldBeQualified - (actualJumperIndex) > 0 || actualRound == competitionRules->getRounds().count() || actualRoundJumpers.count() <= shouldBeQualified){
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
        if(isExAequoOccured == false || (shouldBeQualified - actualJumperIndex > 0) || exAequoFirstPositionAfterShouldBeQualified == (-1)){
            lastQualifiedPosition = abs(actualRoundJumpers.count() - shouldBeQualified - (actualJumperIndex+1));
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

void IndividualCompetitionManager::updateActualJumperPointsToTheLeader()
{
    if(actualRound == 1)
        actualJumperPointsToTheLeader = (-1);
    else{
        IndividualCompetitionResults * indResults = dynamic_cast<IndividualCompetitionResults *>(this->results);
        double actualJumperPoints = indResults->getResultsOfJumper(actualJumper)->getPointsSum();
        double actualLeaderPointsWithoutActualRound = leaderResult->getPointsSum() - leaderResult->getJumps().at(leaderResult->getJumps().count() - 1).getPoints();
        actualJumperPointsToTheLeader = actualJumperPoints - actualLeaderPointsWithoutActualRound;
    }
}

int IndividualCompetitionManager::getCompetitiorsCountForActualRound()
{
    return competitionRules->getRounds().at(actualRound - 1).getCount();
}

bool IndividualCompetitionManager::getSortStartListForActualRound()
{
    return competitionRules->getRounds().at(actualRound - 1).getSortStartList();
}

QSet<uint> IndividualCompetitionManager::getQualifiedBy95HSRule() const
{
    return qualifiedBy95HSRule;
}

void IndividualCompetitionManager::setQualifiedBy95HSRule(const QSet<uint> &newQualifiedBy95HSRule)
{
    qualifiedBy95HSRule = newQualifiedBy95HSRule;
}

QSet<uint> IndividualCompetitionManager::getResultsWithFallIds() const
{
    return resultsWithFallIds;
}

void IndividualCompetitionManager::setResultsWithFallIds(const QSet<uint> &newResultsWithFallIds)
{
    resultsWithFallIds = newResultsWithFallIds;
}

IndividualCompetitionSingleResult *IndividualCompetitionManager::getLastQualifiedResult() const
{
    return lastQualifiedResult;
}

void IndividualCompetitionManager::setLastQualifiedResult(IndividualCompetitionSingleResult *newLastQualifiedResult)
{
    lastQualifiedResult = newLastQualifiedResult;
}

IndividualCompetitionSingleResult *IndividualCompetitionManager::getLeaderResult() const
{
    return leaderResult;
}

void IndividualCompetitionManager::setLeaderResult(IndividualCompetitionSingleResult *newLeaderResult)
{
    leaderResult = newLeaderResult;
}

QVector<Jumper *> IndividualCompetitionManager::getActualRoundJumpers() const
{
    return actualRoundJumpers;
}

QVector<Jumper *> IndividualCompetitionManager::getFilteredJumpersVector(QVector<Jumper*> * previousRoundJumpers, IndividualCompetitionResults *results, CompetitionRules *rules, int round, const QSet<uint> & fallQualified)
{
    QVector<Jumper *> jumpers;

    if(rules->getRounds().count() < round)
    {
        qDebug()<<"Za mało rund! przerywam...";
        return jumpers;
    }
    int maxJumpersCount = rules->getRounds().at(round - 1).getCount();
    bool sortStartList = rules->getRounds().at(round - 1).getSortStartList();

    results->sortJumpersResultsInDescendingOrder();

    if(previousRoundJumpers->count() > maxJumpersCount){
        // Sprawdzanie, czy nie wystąpiło ex aequo (Dokładnie to czy jest zawodnik z pozycją równą round.count()
        int i=0;
        bool isLastQualifyPositionOccured = false;
        int exaequoAt = 0;
        while(!(results->getJumpersResults().at(i).getPosition() > maxJumpersCount)){
            if(results->getJumpersResults().at(i).getPosition() == maxJumpersCount){
                isLastQualifyPositionOccured = true;
                break;
            }
            i++;
        }
        i = maxJumpersCount;
        if(isLastQualifyPositionOccured == false) // Jeśli wystąpiło ex aequo
        {
            //Trzeba znaleźć następną pozycję od maxCount na której jest zawodnik (to nasze egzekwo)
            for(int i=maxJumpersCount - 1; i<results->getJumpersResults().count(); i++){
                if(i+1 == results->getJumpersResults().at(i).getPosition()) //znalezlismy egzekwo
                {
                    exaequoAt = i+1;
                }
            }
        }

        for(auto & jum : *previousRoundJumpers){
            qDebug()<<fallQualified.size()<<"grg";
            qDebug()<<jum->getNameAndSurname()<<": "<<fallQualified.contains(results->getResultsOfJumper(jum)->getID());
            if(results->getResultsOfJumper(jum)->getPosition() <= maxJumpersCount || fallQualified.contains(results->getResultsOfJumper(jum)->getID())){
                jumpers.push_back(jum);
            }
            if(results->getResultsOfJumper(jum)->getPosition() > maxJumpersCount){
                if(isLastQualifyPositionOccured == false){
                    if(results->getResultsOfJumper(jum)->getPosition() == exaequoAt)
                        jumpers.push_back(jum);
                }
            }
        }
    }
    else jumpers = *previousRoundJumpers;
    if(sortStartList == true){
        QVector<IndividualCompetitionSingleResult> jumpersResultsAscending = results->getJumpersResults();
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

QVector<Jumper *> * IndividualCompetitionManager::getActualRoundJumpersPointer() const
{
    return const_cast<QVector<Jumper *> *>(&actualRoundJumpers);
}

void IndividualCompetitionManager::setActualRoundJumpers(const QVector<Jumper *> &newActualRoundJumpers)
{
    actualRoundJumpers = newActualRoundJumpers;
}

void IndividualCompetitionManager::incrementActualJumperIndex()
{
    actualJumperIndex++;
    emit actualJumperIndexChanged();
}

void IndividualCompetitionManager::decrementActualJumperIndex()
{
    actualJumperIndex--;
    emit actualJumperIndexChanged();
}

QVector<Jumper *> IndividualCompetitionManager::getStartingJumpers() const
{
    return startingJumpers;
}

QVector<Jumper *> *IndividualCompetitionManager::getStartingJumpersPointer()
{
    return &startingJumpers;
}

void IndividualCompetitionManager::setStartingJumpers(const QVector<Jumper *> & newStartingJumpers)
{
    startingJumpers = newStartingJumpers;
}

void IndividualCompetitionManager::setStartingJumpers(const QVector<Jumper> * newStartingJumpers)
{
    QVector<Jumper *> vector;
    for(auto & jumper : *newStartingJumpers){
        vector.push_back(const_cast<Jumper*>(&jumper));
    }
    startingJumpers = vector;
}
