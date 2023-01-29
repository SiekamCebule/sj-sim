#ifndef CHARACTERISTIC_H
#define CHARACTERISTIC_H

#include <QObject>
#include <QString>
#include <QHash>
#include <qhashfunctions.h>


class Characteristic
{
public:
    explicit Characteristic(short level = 0, const QString &type = "");
    Characteristic(const QString &type = "");
    operator QString() const {return type;}
private:

    short level; // -3, -2, -1, 1, 2, 3
    QString type;

public:
    short getLevel() const;
    void setLevel(short newLevel);
    QString getType() const;
    void setType(const QString &newType);
};

inline bool operator==(const Characteristic & char1, const Characteristic char2)
{
    return char1.getType() == char2.getType();
}

inline size_t qHash(const Characteristic & k, size_t seed) noexcept
{
    return(k.getType(), seed);
    /*QtPrivate::QHashCombine hash;
    seed = hash(seed, k.getType());
    return seed;*/
}

#endif // CHARACTERISTIC_H
