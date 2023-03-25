#ifndef ABSTRACTCOMPETITIONSINGLERESULT_H
#define ABSTRACTCOMPETITIONSINGLERESULT_H

class CompetitionInfo;

class AbstractCompetitionSingleResult
{
public:
    AbstractCompetitionSingleResult();
    virtual ~AbstractCompetitionSingleResult() = default;
    virtual void updatePointsSum() {}

protected:
    CompetitionInfo * competitionInfo;
    double pointsSum;

public:
    CompetitionInfo *getCompetitionInfo() const;
    void setCompetitionInfo(CompetitionInfo *newCompetitionInfo);
    double getPointsSum() const;
    void setPointsSum(double newPointsSum);
};

#endif // ABSTRACTCOMPETITIONSINGLERESULT_H
