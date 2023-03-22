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
    simulator.setGate(&actualGate);
    simulator.setJumper(actualJumper);
    simulator.setHill(competitionInfo->getHill());
    simulator.setManipulator(const_cast<JumpManipulator *>(&manipulator));
    simulator.setCompetitionRules(&competitionRules);

    windsGenerator.setGenerationSettings(actualWindGenerationSettings);
    simulator.setWindsInfo(WindsInfo(windsGenerator.generateWinds()));

    simulator.simulateJump(manipulator.getIsActive());
    JumpData jump = simulator.getJumpData();

    qDebug()<<"SKOK: "<<jump;

    //KONIEC
    setActualJumperIndex(getActualJumperIndex() + 1); // zmień skoczka na następnego
}

QVector<Jumper *> IndividualCompetitionManager::setupFirstRoundJumpers(CompetitionResults *qualificationsResults)
{
    // Na podstawie rankingu kwalifikacji przefiltruj zawodników. Wybierz np. TOP50 podanych wyników, i stwórz z tego QVector ( i go zwróć)
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
    qDebug()<<actualRoundJumpers.count();
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
