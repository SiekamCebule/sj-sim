#include "IndividualCompetitionSingleResult.h"
#include <QString>

IndividualCompetitionSingleResult::IndividualCompetitionSingleResult()
{
    pointsSum = position = 0;
    jumper = nullptr;
}

IndividualCompetitionSingleResult::IndividualCompetitionSingleResult(const JumpData &jump)
{
    pointsSum = position = 0;
    jumps.push_back(jump);
    setJumper(jump.getJumper());
}

bool IndividualCompetitionSingleResult::operator >(const IndividualCompetitionSingleResult &r) const
{
    if(getPointsSum() != r.getPointsSum())
        return getPointsSum() > r.getPointsSum();
    else{
        double firstDistanceSum = 0;
        double secondDistanceSum = 0;
        for(auto & j : jumps){
            firstDistanceSum += j.getDistance();
        }
        for(auto & j : r.getJumps()){
            secondDistanceSum += j.getDistance();
        }
        if(firstDistanceSum != secondDistanceSum)
            return firstDistanceSum > secondDistanceSum;
        else{
            if(QString(jumper->getNameAndSurname() + jumper->getCountryCode()).toStdString() != QString(r.getJumper()->getNameAndSurname() + r.getJumper()->getCountryCode()).toStdString()){
                return QString(jumper->getNameAndSurname() + jumper->getCountryCode()).toStdString() > QString(r.getJumper()->getNameAndSurname() + r.getJumper()->getCountryCode()).toStdString();
            }
            else
                return jumper->getID() > r.getJumper()->getID();
        }
    }
    return false;
}

bool IndividualCompetitionSingleResult::operator <(const IndividualCompetitionSingleResult &r) const
{
    if(getPointsSum() != r.getPointsSum())
        return getPointsSum() < r.getPointsSum();
    else{
        double firstDistanceSum = 0;
        double secondDistanceSum = 0;
        for(auto & j : jumps){
            firstDistanceSum += j.getDistance();
        }
        for(auto & j : r.getJumps()){
            secondDistanceSum += j.getDistance();
        }
        if(firstDistanceSum != secondDistanceSum)
            return firstDistanceSum < secondDistanceSum;
        else{
            if(QString(jumper->getNameAndSurname() + jumper->getCountryCode()).toStdString() != QString(r.getJumper()->getNameAndSurname() + r.getJumper()->getCountryCode()).toStdString()){
                return QString(jumper->getNameAndSurname() + jumper->getCountryCode()).toStdString() < QString(r.getJumper()->getNameAndSurname() + r.getJumper()->getCountryCode()).toStdString();
            }
            else
                return jumper->getID() < r.getJumper()->getID();
        }
    }
    return false;
}

void IndividualCompetitionSingleResult::updatePointsSum()
{
    pointsSum = 0;
    for(auto & res : jumps){
        pointsSum += res.getPoints();
    }
}

int IndividualCompetitionSingleResult::getPosition() const
{
    return position;
}

void IndividualCompetitionSingleResult::setPosition(int newPosition)
{
    position = newPosition;
}

Jumper *IndividualCompetitionSingleResult::getJumper() const
{
    return jumper;
}

void IndividualCompetitionSingleResult::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}

QVector<JumpData> IndividualCompetitionSingleResult::getJumps() const
{
    return jumps;
}

void IndividualCompetitionSingleResult::setJumps(const QVector<JumpData> &newJumps)
{
    jumps = newJumps;
}

QVector<JumpData> &IndividualCompetitionSingleResult::getEditableJumps()
{
    return jumps;
}
