#include "StartListCompetitorStatus.h"

StartListCompetitorStatus::StartListCompetitorStatus()
{

}

bool StartListCompetitorStatus::getQualifiedBy95HSRule() const
{
    return qualifiedBy95HSRule;
}

void StartListCompetitorStatus::setQualifiedBy95HSRule(bool newQualifiedBy95HSRule)
{
    qualifiedBy95HSRule = newQualifiedBy95HSRule;
}

Jumper *StartListCompetitorStatus::getJumper() const
{
    return jumper;
}

void StartListCompetitorStatus::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}

int StartListCompetitorStatus::getAdvanceStatus() const
{
    return advanceStatus;
}

void StartListCompetitorStatus::setAdvanceStatus(int newAdvanceStatus)
{
    advanceStatus = newAdvanceStatus;
}

int StartListCompetitorStatus::getJumpStatus() const
{
    return jumpStatus;
}

void StartListCompetitorStatus::setJumpStatus(int newJumpStatus)
{
    jumpStatus = newJumpStatus;
}
