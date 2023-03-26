#include "IndividualCompetitionManager.h"
#include "IndividualCompetitionResults.h"

IndividualCompetitionManager::IndividualCompetitionManager() :
    AbstractCompetitionManager()
{
    connect(this, &IndividualCompetitionManager::actualJumperIndexChanged, this, [this](){
        actualJumper = actualRoundJumpers.at(actualJumperIndex);
    });
    //connect(this, &IndividualCompetitionManager::roundEnd, this, &IndividualCompetitionManager::setupNextRound);
}

void IndividualCompetitionManager::simulateNext(const JumpManipulator& manipulator)
{
    simulator.setGate(&actualGate);
    simulator.setJumper(actualJumper);
    simulator.setHill(competitionInfo->getHill());
    simulator.setManipulator(const_cast<JumpManipulator *>(&manipulator));
    simulator.setCompetitionRules(competitionRules);
    simulator.setDSQBaseProbability(this->getDSQBaseProbability());

    windsGenerator.setGenerationSettings(actualWindGenerationSettings);
    simulator.setWinds(windsGenerator.generateWinds());

    simulator.simulateJump();
    JumpData jump = simulator.getJumpData();
    if(jump.getIsDSQOccured() == true)
        hasDSQ.insert(actualJumperIndex);

    qDebug()<<"SKOK: "<<jump;

    completedJumps[actualJumperIndex] = true;


    //Uzupełnianie vectora z wynikami
    bool jumperExists = false;
    IndividualCompetitionResults * indResults = dynamic_cast<IndividualCompetitionResults *>(this->results);
    /*for(auto & result : indResults->getJumpersResults())
    {
        if(result.getJumper() == actualJumper){
            jumperExists = true;
            result.getEditableJumps().push_back(jump);
            result.updatePointsSum();
            break;
        }
    }*/
    //qDebug()<<"Dziala";
    IndividualCompetitionSingleResult * res = indResults->getResultsOfJumper(actualJumper);
    //qDebug()<<"Dziala";
    if(res != nullptr){
        res->getEditableJumps().push_back(jump);
        res->updatePointsSum();
        jumperExists = true;
    }
    if(jumperExists == false){
        indResults->getEditableJumpersResults().push_back(IndividualCompetitionSingleResult(jump));
        indResults->getResultsOfJumper(jump.getJumper())->updatePointsSum();
    }
    //qDebug()<<"Dziala";
    std::sort(indResults->getEditableJumpersResults().begin(), indResults->getEditableJumpersResults().end(), std::greater<IndividualCompetitionSingleResult>());
    indResults->updatePositions();

    if(checkRoundEnd() == true){
        lastJump = true;
        if(checkCompetitionEnd() == false){
            roundShouldBeEnded = true;
            competiitonShouldBeEnded = false;
        }
        else{
            competiitonShouldBeEnded = true;
            roundShouldBeEnded = false;
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

    return false;
    //Sprawdź czy każdy element vectoru completed ma wartosc true. Jeśli DSQ lub DNS, zalicz jako true
}

bool IndividualCompetitionManager::checkCompetitionEnd()
{
    if(actualRound == competitionRules->getRounds().count())
        return true;
    else
        return false;

    return false;
}

void IndividualCompetitionManager::setupNextRound()
{
    qDebug()<<"Nowa runda";
    actualRound++; //Przechodzi do następnej rundy
    setupJumpersForActualRound(); //filtruje zawodników względem wyników z poprzedniej rundy
    setActualJumperIndex(0);
    completedJumps.fill(false, actualRoundJumpers.count());
    hasDNS.clear();
    hasDSQ.clear();
}

void IndividualCompetitionManager::fillCompletedJumpsToStartOfRound()
{
    completedJumps.fill(false, actualRoundJumpers.count());
}

int IndividualCompetitionManager::getCompetitiorsCountForActualRound()
{
    return competitionRules->getRounds().at(actualRound - 1).getCount();
}

bool IndividualCompetitionManager::getSortStartListForActualRound()
{
    return competitionRules->getRounds().at(actualRound - 1).getSortStartList();
}

QVector<Jumper *> IndividualCompetitionManager::getActualRoundJumpers() const
{
    return actualRoundJumpers;
}

Jumper *IndividualCompetitionManager::getActualJumper() const
{
    return actualJumper;
}

void IndividualCompetitionManager::setupJumpersForActualRound()
{
    if(actualRound == 0)
    {
        if(getRunQualificationsForSingleCompetition() == true)
            actualRoundJumpers = startingJumpers;
    }
    else if(actualRound == 1 && getRunQualificationsForSingleCompetition() == false){
        actualRoundJumpers = startingJumpers;
    }
    else if(actualRound > 0){
        if(actualRoundJumpers.count() > getCompetitiorsCountForActualRound()){
            IndividualCompetitionResults * indResults = dynamic_cast<IndividualCompetitionResults *>(this->results);
            QVector<Jumper *> newJumpers;
            if(getSortStartListForActualRound() == true){
                qDebug()<<"Z sortowaniem!";
                int max = getCompetitiorsCountForActualRound();

                QVector<IndividualCompetitionSingleResult> results = indResults->getJumpersResults();
                results.erase(results.begin() + max, results.end());
                std::sort(results.begin(), results.end());
                for(auto & res : results){
                    newJumpers.push_back(res.getJumper());
                    if(newJumpers.count() == max)
                        break;
                }
            }
            else{
                for(auto & jum : actualRoundJumpers){
                    int i = 0;
                    int max = getCompetitiorsCountForActualRound();
                    for(auto & res : indResults->getJumpersResults()){
                        if(res.getJumper() == jum){
                            newJumpers.push_back(jum);
                            break;
                        }
                        i++;
                        if(i == max) break;
                    }
                }
            }
            actualRoundJumpers = newJumpers;
        }
        qDebug()<<"Proszę państwa, aż "<<actualRoundJumpers.count()<<" zawodników na nową rundę!";
    }
}

int IndividualCompetitionManager::getActualJumperIndex() const
{
    return actualJumperIndex;
}

QVector<Jumper *> * IndividualCompetitionManager::getActualRoundJumpersPointer() const
{
    return const_cast<QVector<Jumper *> *>(&actualRoundJumpers);
}

void IndividualCompetitionManager::setActualRoundJumpers(const QVector<Jumper *> &newActualRoundJumpers)
{
    actualRoundJumpers = newActualRoundJumpers;
}

void IndividualCompetitionManager::setActualJumperIndex(int newActualJumperIndex)
{
    actualJumperIndex = newActualJumperIndex;
    emit actualJumperIndexChanged();
}

void IndividualCompetitionManager::inncrementActualJumperIndex()
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

void IndividualCompetitionManager::setStartingJumpers(const QVector<Jumper *> & newStartingJumpers)
{
    startingJumpers = newStartingJumpers;
}
