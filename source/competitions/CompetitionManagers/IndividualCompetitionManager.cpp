#include "IndividualCompetitionManager.h"
#include "../../utilities/functions.h"

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
    results->updatePositions();
    for(auto & status : startListStatuses)
    {
        //qDebug()<<status.getJumper()->getNameAndSurname()<<" --> "<<(actualRound == competitionRules->getRounds().count())<<", "<<(getActualRoundJumpersReference().count() <= competitionRules->getRounds().at(actualRound).getCount())<<", "<<(status.getQualifiedBy95HSRule())<<" (s : t)";
        if(actualRound == competitionRules->getRounds().count())
            status.setAdvanceStatus(StartListCompetitorStatus::Waiting);
        else if(getActualRoundJumpersReference().count() <= competitionRules->getRounds().at(actualRound).getCount() || status.getQualifiedBy95HSRule()){
            qDebug()<<"SSS    "<<(getActualRoundJumpersReference().count() <= competitionRules->getRounds().at(actualRound).getCount())<<",,,  "<<(status.getQualifiedBy95HSRule());
            status.setAdvanceStatus(StartListCompetitorStatus::SureAdvanced);
        }

        else if(lastQualifiedResult == nullptr || status.getJumpStatus() == StartListCompetitorStatus::Unfinished)
            status.setAdvanceStatus(StartListCompetitorStatus::Waiting);
        else{
            if(results->getResultOfIndividualJumper(status.getJumper())->getPosition() <= lastQualifiedResult->getPosition()){
                qDebug()<<"LQPOSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSLLLLLLLLLLL: "<<lastQualifiedResult->getPosition();
                status.setAdvanceStatus(StartListCompetitorStatus::SureAdvanced);
            }
            else{
                int lastWaiting = competitionRules->getRounds().at(actualRound).getCount();
                results->sortInDescendingOrder();
                QVector<CompetitionSingleResult> * res = &results->getResultsReference();
                int lastPosition = 0;
                for(int i = res->count() - 1; i>=0; i--){
                    if(res->at(i).getPosition() < competitionRules->getRounds().at(actualRound).getCount()){
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

QVector<StartListCompetitorStatus> IndividualCompetitionManager::getStartListStatuses() const
{
    return startListStatuses;
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
