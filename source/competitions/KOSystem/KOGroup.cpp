#include "KOGroup.h"
#include "../CompetitionInfo.h"
#include "../../global/SeasonDatabaseObjectsManager.h"
extern SeasonDatabaseObjectsManager seasonObjectsManager;

KOGroup::KOGroup()
{

}

QJsonObject KOGroup::getJsonObject(KOGroup &group)
{
    QJsonObject object;
    object.insert("id", QString::number(group.getID()));
    object.insert("number", group.getNumber());
    object.insert("competition-id", QString::number(group.getCompetition()->getID()));
    if(group.getCompetitionType() == CompetitionRules::Individual){
        QJsonArray jumpersArray;
        for(auto & jumper : group.getJumpersReference())
            jumpersArray.push_back(QString::number(jumper->getID()));
        object.insert("jumpers-ids", jumpersArray);
    }
    else{
        QJsonArray teamsArray;
        for(auto & team : group.getTeamsReference())
            teamsArray.push_back(QString::number(team->getID()));
        object.insert("teams-ids", teamsArray);
    }

    return object;
}

KOGroup KOGroup::getFromJson(const QJsonObject &json)
{
    KOGroup group;
    group.setID(json.value("id").toString().toULong());
    group.setNumber(json.value("number").toInt());
    group.setCompetition(static_cast<CompetitionInfo*>(seasonObjectsManager.getObjectByID(json.value("competition-id").toString().toULong())));
    group.setCompetitionType(group.getCompetition()->getRulesPointer()->getCompetitionType());
    if(group.getCompetitionType() == CompetitionRules::Individual)
    {
        QJsonArray jumpersArray = json.value("jumpers-ids").toArray();
        for(auto val : jumpersArray)
            group.getJumpersReference().push_back(static_cast<Jumper *>(seasonObjectsManager.getObjectByID(val.toString().toULong())));
    }
    else
    {
        QJsonArray teamsArray = json.value("teams-ids").toArray();
        for(auto val : teamsArray)
            group.getTeamsReference().push_back(static_cast<Team *>(seasonObjectsManager.getObjectByID(val.toString().toULong())));
    }

    return group;
}

void KOGroup::setCompetitionType(int newCompetitionType)
{
    competitionType = newCompetitionType;
}

void KOGroup::setTeams(const QVector<Team *> &newTeams)
{
    teams = newTeams;
}

QVector<Jumper *> KOGroup::getJumpers() const
{
    return jumpers;
}

QVector<Jumper *> &KOGroup::getJumpersReference()
{
    return jumpers;
}

void KOGroup::setJumpers(const QVector<Jumper *> &newJumpers)
{
    jumpers = newJumpers;
}

QVector<Team *> &KOGroup::getTeamsReference()
{
    return teams;
}

int KOGroup::getNumber() const
{
    return number;
}

void KOGroup::setNumber(int newNumber)
{
    number = newNumber;
}

int KOGroup::getCompetitionType() const
{
    return competitionType;
}

CompetitionInfo *KOGroup::getCompetition() const
{
    return competition;
}

void KOGroup::setCompetition(CompetitionInfo *newCompetition)
{
    competition = newCompetition;
    competitionType = competition->getRulesPointer()->getCompetitionType();
}
