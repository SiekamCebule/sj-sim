#ifndef JUMPERSKILLS_H
#define JUMPERSKILLS_H

#include "../utilities/ClassWithID.h"
#include "Characteristic.h"

class Jumper;

#include <QSet>

class JumperSkills : public ClassWithID
{
public:
    JumperSkills(short takeoffPower = 0, short takeoffTechnique = 0, short flightStyle = 0, short flightTechnique = 0, short form = 0, short landingStyle = 0, const QSet<Characteristic> &characteristics = QSet<Characteristic>(), Jumper * jumper = nullptr);

private:
    short takeoffPower; // 1-50
    short takeoffTechnique; // 1-50
    short flightStyle; // 0-klasyczny, 1-Styl V, 2-Nowoczesne V, 3-Szerokie V, 4-Styl H
    short flightTechnique; // 1-50
    short form; // 1-50
    short landingStyle; // 1-15
    QSet<Characteristic> characteristics;

    Jumper * jumper;
    ulong jumperID;

public:
    void insertCharacteristic(const Characteristic & characteristic);
    void insertCharacteristic(short level, const QString & type);
    void removeCharacteristic(const Characteristic & characteristic);
    void removeCharacteristic(const QString & type);

    short getLevelOfCharacteristic(const QString & characteristicType);

    void setJumperID(ulong ID);
    short getTakeoffPower() const;
    void setTakeoffPower(short newTakeoffPower);
    short getTakeoffTechnique() const;
    void setTakeoffTechnique(short newTakeoffTechnique);
    short getFlightStyle() const;
    void setFlightStyle(short newFlightStyle);
    short getFlightTechnique() const;
    void setFlightTechnique(short newFlightTechnique);
    short getLandingStyle() const;
    void setLandingStyle(short newLandingStyle);
    QSet<Characteristic> getCharacteristics() const;
    void setCharacteristics(const QSet<Characteristic> &newCharacteristics);
    short getForm() const;
    void setForm(short newForm);
};

#endif // JUMPERSKILLS_H
