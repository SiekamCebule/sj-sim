#include "Team.h"

Team::Team(const QString &countryCode) : countryCode(countryCode)
{
}

QVector<Team> Team::constructTeamsVectorByJumpersList(const QVector<Jumper> &jumpers, int minCount)
{
    qDebug()<<"minCount: "<<minCount;
    QVector<Team> teams;
    QStringList codes;
    for(const auto & jumper : jumpers)
    {
        if(codes.contains(jumper.getCountryCode().toUpper()) == true){
            for(auto & team : teams){
                if(team.getCountryCode() == jumper.getCountryCode())
                    team.getJumpersReference().push_back(const_cast<Jumper *>(&jumper));
            }
        }
        else{
            codes.append(jumper.getCountryCode().toUpper());
            Team team;
            team.getJumpersReference().push_back(const_cast<Jumper *>(&jumper));
            team.setCountryCode(jumper.getCountryCode());
            teams.append(team);
        }
    }
    while(true){
        bool next = false;
        int i=0;
        for(auto & team : teams){
            if(team.getJumpersReference().count() < minCount){
                teams.remove(i);
                next = true;
                break;
            }
            i++;
        }
        if(next == false)
            break;
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

void Team::cutTeamJumpers(Team *team, int jumpersCount)
{
    qDebug()<<team->getJumpersCount()<<", "<<jumpersCount;
    if(team->getJumpersCount() > jumpersCount){
        qDebug()<<team->getJumpersReference().count();
        team->getJumpersReference().erase(team->getJumpersReference().begin() + (jumpersCount), team->getJumpersReference().end());
        qDebug()<<team->getJumpersReference().count();
    }
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

QVector<Jumper *> &Team::getJumpersReference()
{
    return jumpers;
}

void Team::setJumpers(const QVector<Jumper *> &newJumpers)
{
    jumpers = newJumpers;
}

int Team::getJumpersCount() const
{
    return jumpers.count();
}
