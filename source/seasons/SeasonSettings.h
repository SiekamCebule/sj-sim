#ifndef SEASONSETTINGS_H
#define SEASONSETTINGS_H


class SeasonSettings
{
public:
    SeasonSettings();

private:
    short resultsSaveLevel; //  0-żadne,   1-treningi,   2-dodatkowo serie próbne   3-wszystko

public:
    enum ResultsSaveLevel{
        Anything,
        Trainings,
        TrialRounds,
        Everything
    };

    short getResultsSaveLevel() const;
    void setResultsSaveLevel(short newResultsSaveLevel);
};

#endif // SEASONSETTINGS_H
