#include "RoundInfo.h"

RoundInfo::RoundInfo()
{

}

int RoundInfo::getCount() const
{
    return count;
}

void RoundInfo::setCount(int newCount)
{
    count = newCount;
}

bool RoundInfo::getSortStartList() const
{
    return sortStartList;
}

void RoundInfo::setSortStartList(bool newSortStartList)
{
    sortStartList = newSortStartList;
}
