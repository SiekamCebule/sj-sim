#ifndef JUMPMISTAKE_H
#define JUMPMISTAKE_H

#include  "../utilities/ClassWithID.h"
#include <QPair>

class JumpSimulator;

class JumpMistake : ClassWithID
{
public:
    JumpMistake(short generalType = 0, short indirectType = 0, short exactType = 0, short harmuflness = 0, bool isOccurred = false, double distance = 0);
private:
    short generalType; // np wybicie
    short indirectType; // np timing wybicia
    short exactType; // np za wczesne wybicie
    short harmfulness; // 1-10
    bool isOccurred;
    double distance;

public:
    static JumpMistake generateJumpMistake(JumpSimulator * const simulator, short mistakeType);
    static short generateJumpMistakeHarmfulness(JumpSimulator * const simulator, short generalType, short indirectType, short exactType);
    static double generateJumpMistakeEffect(JumpSimulator * const simulator, JumpMistake * const mistake , short effectType);

    enum MistakeType{
        TakeoffMistake,
        FlightMistake,
        LandMistake,
        OutrunMistake
    };
    enum JumpMistakeEffectType{
        Height,
        Speed,
        AerodynamicPosition
    };
    enum TakeoffMistakeType{
        TimingMistake,
        AggressivenessMistake
    };
    enum TakeoffAggressivenessMistakeType{
        TooPassiveTakeoff,
        TooAgressiveTakeoff,
    };
    enum TakeoffTimingMistakeType{
        TooEarlyTakeoff,
        TooLateTakeoff,
    };


    QString getNote();

    double getDistance() const;
    void setDistance(double newDistance);
    short getHarmfulness() const;
    void setHarmfulness(short newHarmfulness);
    bool getIsOccurred() const;
    void setIsOccurred(bool newIsOccurred);
    short getExactType() const;
    void setExactType(short newExactType);
    short getGeneralType() const;
    void setGeneralType(short newGeneralType);
    short getIndirectType() const;
    void setIndirectType(short newIndirectType);
};

#endif // JUMPMISTAKE_H
