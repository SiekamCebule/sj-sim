#include "IndividualCompetitionManager.h"

IndividualCompetitionManager::IndividualCompetitionManager() :
    AbstractCompetitionManager()
{
    connect(this, &IndividualCompetitionManager::actualJumperIndexChanged, this, [this](){
        actualJumper = actualRoundJumpers.at(actualJumperIndex);
    });
}

void IndividualCompetitionManager::simulateNext(const JumpManipulator& manipulator)
{
    qDebug()<<"Konkurs czas zacząć!!! BIG VOGGEN ARENAA";
    simulator.setGate(&actualGate);
    qDebug()<<"gate";
    simulator.setJumper(actualJumper);
    qDebug()<<"jumper";
    simulator.setHill(competitionInfo->getHill());
    qDebug()<<"hill";
    simulator.setManipulator(const_cast<JumpManipulator *>(&manipulator));
    qDebug()<<"manipulator";
    simulator.setCompetitionRules(&competitionRules);
    qDebug()<<"cr";

    windsGenerator.setGenerationSettings(actualWindGenerationSettings);
    simulator.setWindsInfo(WindsInfo(windsGenerator.generateWinds()));

    simulator.simulateJump(manipulator.getIsActive());
    qDebug()<<"simulate";
    JumpData jump = simulator.getJumpData();

    qDebug()<<"SKOK: "<<jump;

    //KONIEC
    setActualJumperIndex(getActualJumperIndex() + 1); // zmień skoczka na następnego
}

QVector<Jumper *> IndividualCompetitionManager::getStartingJumpers() const
{
    return startingJumpers;
}

void IndividualCompetitionManager::setStartingJumpers(const QVector<Jumper *> & newStartingJumpers)
{
    startingJumpers = newStartingJumpers;
    actualRoundJumpers = startingJumpers;
}

void IndividualCompetitionManager::setupFirstRoundJumpers(CompetitionResults *qualificationsResults)
{

}

int IndividualCompetitionManager::getActualJumperIndex() const
{
    return actualJumperIndex;
}

void IndividualCompetitionManager::setActualJumperIndex(int newActualJumperIndex)
{
    actualJumperIndex = newActualJumperIndex;
        emit actualJumperIndexChanged();
}
