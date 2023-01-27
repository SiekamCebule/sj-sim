#ifndef JUMPMISTAKE_H
#define JUMPMISTAKE_H

#include  "../utilities/ClassWithID.h"
#include <QPair>

class JumpSimulator;

class JumpMistake : ClassWithID
{
public:
    JumpMistake(short type = 0, short harmuflness = 0, bool isOccurred = false, double distance = 0);
private:
    short type;
    short harmfulness; // 1-10
    bool isOccurred;
    double distance;

public:
    static JumpMistake generateJumpMistake(JumpSimulator * const simulator, short mistakeType);


    enum MistakeType{
        TakeoffMistake,
        FlightMistake,
        LandMistake,
        OutrunMistake
    };
    enum TakeoffMistakeType{
        TooEarlyTakeoff,
        TooLateTakeoff,
        TooPassiveTakeoff,
        TooAggressiveTakeoff
    };
    enum FlightMistakeType{
        FlightPassivity,
        FlightAggressiveness,
        FlightInstability
    };

    short getType() const;
    void setType(short newType);
    double getDistance() const;
    void setDistance(double newDistance);
    short getHarmfulness() const;
    void setHarmfulness(short newHarmfulness);
    bool getIsOccurred() const;
    void setIsOccurred(bool newIsOccurred);
};

#endif // JUMPMISTAKE_H
