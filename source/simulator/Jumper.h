#ifndef JUMPER_H
#define JUMPER_H

#include "../utilities/ClassWithID.h"
#include "JumperSkills.h"

#include <QString>
#include <QPixmap>
#include <QObject>
#include <QJsonObject>

class Jumper : public ClassWithID
{
public:
    Jumper(const QString &name = "", const QString &surname = "", const QString &countryCode = "", const JumperSkills & skills = JumperSkills());
    ~Jumper();

private:
    QString name, surname;
    QString countryCode;
    QPixmap flagPixmap;

    JumperSkills jumperSkills;
public:
    static QJsonObject getJumperJsonObject(const Jumper &jumper, bool saveSkills, bool saveCharacteristics);
    static QVector<Jumper> getJumpersVectorFromJson(const QByteArray & bytes);
    static void setupJumpersFlagPixmaps(QVector<Jumper> & jumpers);

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

    void updateCountryFlagPixmap();
};

#endif // JUMPER_H
