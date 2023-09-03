#ifndef TEAM_H
#define TEAM_H

#include <QVector>
#include <QString>
#include <QJsonObject>
#include "Jumper.h"
#include "../utilities/ClassWithID.h"
#include "../global/DatabaseObjectsManager.h"

class Team : public ClassWithID
{
public:
    Team(const QString & countryCode = "");

    static QVector<Team> constructTeamsVectorByJumpersList(const QVector<Jumper> & jumpers, int minCount);
    static QVector<Team> constructTeamsVectorByJumpersList(const QVector<Jumper *> & jumpers, int minCount);
    static QVector<Jumper *> getJumpersFilteredByCountryCode(QVector<Jumper> &jumpers, const QString & countryCode);
    static void cutTeamJumpers(Team * team, int jumpersCount);
    static Team * getTeamByCountryCode(QVector<Team> * teams, QString countryCode);

    static QJsonObject getJsonObject(Team & team);
    static Team getFromJson(QJsonObject json, DatabaseObjectsManager *objectsManager);

private:
    QString countryCode;
    QVector<Jumper *> jumpers;

public:
    void setJumper(Jumper * jumper, int index);
    QVector<Jumper *> getJumpers() const;
    QVector<Jumper*> & getJumpersReference();
    void setJumpers(const QVector<Jumper *> &newJumpers);
    int getJumpersCount() const;
    QString getCountryCode() const;
    void setCountryCode(const QString &newCountryCode);
};

#endif // TEAM_H
