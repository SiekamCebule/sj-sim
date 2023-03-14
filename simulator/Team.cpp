#include "Team.h"

Team::Team(int jumpersCount) : jumpersCount(jumpersCount)
{
    if(getJumpersCount() > 0)
        jumpers.fill(nullptr, getJumpersCount());
}

Team::Team(const QString &countryCode) : countryCode(countryCode)
{
    jumpersCount = 0;
}

QVector<Team> Team::constructTeamsVectorByJumpersList(const QVector<Jumper> &jumpers)
{
    QVector<Team> teams;
    QStringList codes;
    for(const auto & jumper : jumpers)
    {
        if(codes.contains(jumper.getCountryCode().toUpper()) == true)
            continue;
        else{
            codes.append(jumper.getCountryCode().toUpper());
            teams.append(Team(jumper.getCountryCode().toUpper()));
        }
    }

    return teams;
}

QVector<Jumper *> Team::getJumpersFilteredByCountryCode(QVector<Jumper> &jumpers, const QString &countryCode)
{
    QVector<Jumper *> vector;
    for(auto & jumper : jumpers)
    {
        if(jumper.getCountryCode().toUpper() == countryCode.toUpper())
            vector.push_back(&jumper);
    }
    qDebug()<<"size: "<<vector.size();
    return vector;
}

QString Team::getCountryCode() const
{
    return countryCode;
}

void Team::setCountryCode(const QString &newCountryCode)
{
    countryCode = newCountryCode;
}

void Team::setJumper(Jumper *jumper, int index)
{
    if(index < jumpers.count())
        jumpers[index] = jumper;
    else qDebug()<<"too high index! (Team::setJumper)";
}

QVector<Jumper *> Team::getJumpers() const
{
    return jumpers;
}

QVector<Jumper *> &Team::getEditableJumpers()
{
    return jumpers;
}

void Team::setJumpers(const QVector<Jumper *> &newJumpers)
{
    jumpers = newJumpers;
}

int Team::getJumpersCount() const
{
    return jumpersCount;
}

void Team::setJumpersCount(int newJumpersCount)
{
    jumpersCount = newJumpersCount;
}
