#include "IndividualCompetitionManager.h"
#include "../../utilities/functions.h"
#include <QVector>

IndividualCompetitionManager::IndividualCompetitionManager() : AbstractCompetitionManager(CompetitionRules::Individual)
{
    connect(this, &IndividualCompetitionManager::actualStartListIndexChanged, this, [this](){
        if(roundsJumpers.count() > 0){
            actualJumper = roundsJumpers[actualRound - 1].at(actualStartListIndex);
            emit actualJumperChanged();
        }
    });
}

void IndividualCompetitionManager::updateCompetitorsAdvanceStatuses()
{
    int limit = 0;
    if(competitionRules->getRounds().count() > actualRound)
        limit = competitionRules->getRounds().at(actualRound).getCount();
    if(altQualifiersLimit > 0 && actualRound == competitionRules->getRounds().count())
        limit = altQualifiersLimit;

    results->updatePositions();
    for(auto & status : startListStatuses)
    {
        if(KOManager != nullptr){
            if(KOManager->getStatusesReference().value(status.getJumper()) == KORoundManager::Winner)
                status.setAdvanceStatus(StartListCompetitorStatus::SureAdvanced);
            else if(KOManager->getLuckyLoserReference().contains(status.getJumper()))
            {
                bool allFinished = true;
                for(auto & status : startListStatuses)
                    if(status.getJumpStatus() == StartListCompetitorStatus::Unfinished)
                        allFinished = false;
                if(allFinished)
                    status.setAdvanceStatus(StartListCompetitorStatus::SureAdvanced);
                else
                    status.setAdvanceStatus(StartListCompetitorStatus::Waiting);
            }
            else
                status.setAdvanceStatus(StartListCompetitorStatus::SureDroppedOut);
        }
        else if(actualRound == competitionRules->getRounds().count() && (altQualifiersLimit == 0 && actualRound != competitionRules->getRounds().count())){
            status.setAdvanceStatus(StartListCompetitorStatus::Waiting);
        }
        else if(getActualRoundJumpersReference().count() <= limit || status.getQualifiedBy95HSRule()){
            status.setAdvanceStatus(StartListCompetitorStatus::SureAdvanced);
        }
        else if(lastQualifiedResult == nullptr || status.getJumpStatus() == StartListCompetitorStatus::Unfinished)
            status.setAdvanceStatus(StartListCompetitorStatus::Waiting);
        else{
            if(results->getResultOfIndividualJumper(status.getJumper())->getPosition() <= lastQualifiedResult->getPosition()){
                status.setAdvanceStatus(StartListCompetitorStatus::SureAdvanced);
            }
            else{
                int lastWaiting = limit;
                results->sortInDescendingOrder();
                QVector<CompetitionSingleResult> * res = &results->getResultsReference();
                int lastPosition = 0;
                for(int i = res->count() - 1; i>=0; i--){
                    if(res->at(i).getPosition() < limit){
                        if(res->at(i).getPosition() == lastPosition) //mamy egzekwo
                        {
                            int exaequos = 0;
                            for(int i = res->count() - 1; i>=0; i--){
                                if(res->at(i).getPosition() == lastPosition){
                                    exaequos++;
                                    lastPosition = res->at(i).getPosition();
                                }
                                else
                                    break;
                            }
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
    int i = 1;
    for(auto & status : startListStatuses){
        qDebug()<<i<<"> "<<status.getJumper()->getNameAndSurname()<<" --> "<<status.getAdvanceStatus();
        i++;
    }

    if(competitionRules->getRounds().at(actualRound - 1).getSortStartList() == true){
        results->sortInAscendingOrder();
        QVector<CompetitionSingleResult *> roundResults;
        for(auto & res : results->getResultsReference()){
            if(StartListCompetitorStatus::getStatusOfJumper(res.getJumper(), startListStatuses) != nullptr)
                roundResults.push_back(&res);
        }

        for(auto & res : roundResults){
            qDebug()<<res->getJumper()->getNameAndSurname()<<": "<<StartListCompetitorStatus::getStatusOfJumper(res->getJumper(), startListStatuses);
            if(StartListCompetitorStatus::getStatusOfJumper(res->getJumper(), startListStatuses)->getAdvanceStatus() == StartListCompetitorStatus::SureAdvanced)
                jumpers.push_back(res->getJumper());
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

QVector<Jumper *> IndividualCompetitionManager::getFilteredJumpersAfterQualifications(CompetitionInfo *competition, QVector<Jumper *> & jumpers)
{
    CompetitionResults * results = &competition->getAdvancementCompetition()->getResultsReference();
    int lastQualifiedPosition = competition->getRulesPointer()->getRoundsReference()[0].getCount();

    bool exaequo = true;
    for(auto & res : results->getResultsReference()){ // Sprawdzamy, czy wystąpiło ex aequo.
        if(res.getPosition() == lastQualifiedPosition){
            exaequo = false;
            break;
        }
    }
    if(exaequo == true) // Jeżeli wystąpiło ex aequo
    {
        for(auto & res : results->getResultsReference()){
            if(res.getPosition() > lastQualifiedPosition){
                lastQualifiedPosition = res.getPosition();
                break;
            }
        }
    }

    QVector<Jumper *> toReturn;
    for(auto & jumper : jumpers)
    {
        if(results->getResultOfIndividualJumper(jumper) != nullptr)
            if(results->getResultOfIndividualJumper(jumper)->getPosition() <= lastQualifiedPosition)
                toReturn.push_back(jumper);
    }
    return toReturn;
}

QVector<Jumper *> IndividualCompetitionManager::getFilteredJumpersByClassification(CompetitionInfo *competition, Classification *classification, QVector<Jumper *> &jumpers)
{
    int lastQualifiedPosition = competition->getRulesPointer()->getRoundsReference()[0].getCount();

    bool exaequo = true;
    for(auto & res : classification->getResultsReference()){ // Sprawdzamy, czy wystąpiło ex aequo.
        if(res->getPosition() == lastQualifiedPosition){
            exaequo = false;
            break;
        }
    }
    if(exaequo == true)
    {
        for(auto & res : classification->getResultsReference()){
            if(res->getPosition() > lastQualifiedPosition){
                lastQualifiedPosition = res->getPosition();
                break;
            }
        }
    }

    QVector<Jumper *> toReturn;
    for(auto & jumper : jumpers)
    {
        if(classification->getResultOfIndividualJumper(jumper) != nullptr)
            if(classification->getResultOfIndividualJumper(jumper)->getPosition() <= lastQualifiedPosition)
                toReturn.push_back(jumper);
    }
    return toReturn;
}

void IndividualCompetitionManager::setStartListOrderByClassification(QVector<Jumper *> & jumpers, Classification *classification)
{
    if(classification->getClassificationType() == Classification::Individual)
    {
        QVector<Jumper *> tempJumpers;
        for(auto & result : classification->getResultsReference())
        {
            if(jumpers.contains(result->getJumper()) && result->getPointsSum() != 0)
                tempJumpers.push_back(result->getJumper());
        }

        QVector<Jumper *> others;
        std::reverse(jumpers.begin(), jumpers.end());
        for(auto & jumper : jumpers)
        {
            if(tempJumpers.contains(jumper) == false)
                others.push_back(jumper);
        }
        std::random_shuffle(others.begin(), others.end());

        tempJumpers.append(others);

        std::reverse(tempJumpers.begin(), tempJumpers.end());
        jumpers = tempJumpers;
    }
}

void IndividualCompetitionManager::setStartListOrderByCompetitionResults(QVector<Jumper *> & jumpers, CompetitionInfo *competition)
{
    if(competition->getRulesPointer()->getCompetitionType() == CompetitionRules::Individual)
    {
        QVector<Jumper *> tempJumpers;
        for(auto & result : competition->getResultsReference().getResultsReference())
        {
            if(jumpers.contains(result.getJumper()))
                tempJumpers.push_back(result.getJumper());
        }
        std::reverse(jumpers.begin(), jumpers.end());
        for(auto & jumper : jumpers)
        {
            if(tempJumpers.contains(jumper) == false)
                tempJumpers.push_back(jumper);
        }

        std::reverse(tempJumpers.begin(), tempJumpers.end());
        jumpers = tempJumpers;
    }
}

void IndividualCompetitionManager::setStartListOrderByDefault(QVector<Jumper *> *jumpersList, QVector<Jumper *> &startList)
{
    QVector<Jumper *> tempJumpers;
    for(auto & jumper : *jumpersList)
    {
        if(startList.contains(jumper))
            tempJumpers.push_back(jumper);
    }
    startList = tempJumpers;
}

KORoundManager *IndividualCompetitionManager::getKOManager() const
{
    return KOManager;
}

void IndividualCompetitionManager::setKOManager(KORoundManager *newKOManager)
{
    KOManager = newKOManager;
}

QVector<QVector<KOGroup> > IndividualCompetitionManager::getRoundsKOGroups() const
{
    return roundsKOGroups;
}

void IndividualCompetitionManager::setRoundsKOGroups(const QVector<QVector<KOGroup> > &newRoundsKOGroups)
{
    roundsKOGroups = newRoundsKOGroups;
}

QVector<StartListCompetitorStatus> IndividualCompetitionManager::getStartListStatuses() const
{
    return startListStatuses;
}

QVector<QVector<KOGroup> > &IndividualCompetitionManager::getRoundsKOGroupsReference()
{
    return roundsKOGroups;
}

void IndividualCompetitionManager::setupNextRound()
{
    actualRound++; //Przechodzi do następnej rundy
    roundsJumpers.push_back(getFilteredJumpersForNextRound());
    setActualStartListIndex(0);
    roundsStartingGates.push_back(actualGate);
    setupStartListStatusesForActualRound();
}

void IndividualCompetitionManager::setupStartListStatusesForActualRound()
{
    startListStatuses.clear();
    for(auto & jumper : getActualRoundJumpersReference()){
        startListStatuses.push_back(StartListCompetitorStatus(jumper));
    }
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
