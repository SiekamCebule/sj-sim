#include "RoundInfo.h"

RoundInfo::RoundInfo(int count, bool sortStartList) : count(count),
    sortStartList(sortStartList)
{}

int RoundInfo::getSortAfterGroups() const
{
    return sortAfterGroups;
}

void RoundInfo::setSortAfterGroups(int newSortAfterGroups)
{
    sortAfterGroups = newSortAfterGroups;
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
