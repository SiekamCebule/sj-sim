#ifndef TEAM_H
#define TEAM_H

#include <QVector>
#include <QString>
#include "Jumper.h"

class Team
{
public:
    Team(int jumpersCount = 0);
    Team(const QString & countryCode);

    static QVector<Team> constructTeamsVectorByJumpersList(const QVector<Jumper> & jumpers, int minCount);
    static QVector<Jumper *> getJumpersFilteredByCountryCode(QVector<Jumper> &jumpers, const QString & countryCode);

private:
    QString countryCode;
    QVector<Jumper *> jumpers;
    int jumpersCount;

public:
    void setJumper(Jumper * jumper, int index);
    QVector<Jumper *> getJumpers() const;
    QVector<Jumper*> & getJumpersReference();
    void setJumpers(const QVector<Jumper *> &newJumpers);
    int getJumpersCount() const;
    void setJumpersCount(int newJumpersCount);
    QString getCountryCode() const;
    void setCountryCode(const QString &newCountryCode);
};

#endif // TEAM_H
