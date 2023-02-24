#ifndef JUMPER_H
#define JUMPER_H

#include "../utilities/ClassWithID.h"
#include "JumperSkills.h"

#include <QString>
#include <QPixmap>

class Jumper : public ClassWithID
{
public:
    Jumper(const QString &name = "", const QString &surname = "", const QString &countryCode = "", const JumperSkills & skills = JumperSkills());

private:
    QString name, surname;
    QString countryCode;
    QPixmap flagPixmap;

    JumperSkills jumperSkills;
public:
    QString getName() const;
    void setName(const QString &newName);
    QString getSurname() const;
    void setSurname(const QString &newSurname);
    QString getNameAndSurname() const;
    QString getCountryCode() const;
    void setCountryCode(const QString &newCountryCode);
    JumperSkills getJumperSkills() const;
    JumperSkills * getJumperSkillsPointer() const;
    void setJumperSkills(const JumperSkills &newJumperSkills);
    QPixmap getFlagPixmap() const;
    void setFlagPixmap(const QPixmap &newFlagPixmap);
};

#endif // JUMPER_H
