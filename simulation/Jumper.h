#ifndef JUMPER_H
#define JUMPER_H

#include "../utilities/ClassWithID.h"
#include "JumperSkills.h"

#include <QString>


class Jumper : public ClassWithID
{
public:
    Jumper(const QString &name = "", const QString &surname = "", const QString &countryCode = "", JumperSkills * jumperSkills = nullptr, ulong jumperSkillsID = 0);

private:
    QString name, surname;
    QString countryCode;

    JumperSkills * jumperSkills;
    ulong jumperSkillsID;

public:
    QString getName() const;
    void setName(const QString &newName);
    QString getSurname() const;
    void setSurname(const QString &newSurname);
    QString getCountryCode() const;
    void setCountryCode(const QString &newCountryCode);
    JumperSkills *getJumperSkills() const;
    void setJumperSkills(JumperSkills *newJumperSkills);
    void setJumperSkillsID(ulong ID);
    ulong getJumperSkillsID() const;
};

#endif // JUMPER_H
