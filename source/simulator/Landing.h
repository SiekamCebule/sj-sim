#ifndef LANDING_H
#define LANDING_H

#include <QString>

class Landing
{
public:
    Landing(short type = 0, double imbalance = 0);
private:
    short type;
    double rating; // od 0 do 5, gdzie 0 oznacza idealne lądowanie a 5 brzydkie lądowanie.

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

    static QString getShortLandingTypeTextInfo(short type);
    static QString getStyleSheetForLandingTypeTextInfo(short type);
    double getRating() const;
    void setRating(double newRating);
};

#endif // LANDING_H
