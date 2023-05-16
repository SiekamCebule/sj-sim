#include "StartListCompetitorStatus.h"

StartListCompetitorStatus::StartListCompetitorStatus()
{

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
