#ifndef LANDING_H
#define LANDING_H

#include <QString>

class Landing
{
public:
    Landing(short type = 0, double imbalance = 0);
private:
    short type;
    double imbalance; // od 0 do 5, gdzie 0 oznacza idealnie stabilne lądowanie.

public:
    enum Type{
        TelemarkLanding,
        BothLegsLanding,
        SupportLanding,
        Fall
    };

    QString getTextLandingType() const;
    short getType() const;
    void setType(short newType);
    double getImbalance() const;
    void setImbalance(double newImbalance);
};

#endif // LANDING_H
