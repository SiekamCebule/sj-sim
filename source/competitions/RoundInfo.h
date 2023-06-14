#ifndef ROUNDINFO_H
#define ROUNDINFO_H


class RoundInfo
{
public:
    RoundInfo(int count = 0, bool sortStartList = false);

    enum SortAfterGroupsType{
        BeforeLastGroup = 0,
        EachGroup,
        AfterFirstGroup,
        Never
    };

private:
    int count; //liczba zawodników/drużyn w danej rundzie
    bool sortStartList;
    int sortAfterGroups;
public:
    int getCount() const;
    void setCount(int newCount);
    bool getSortStartList() const;
    void setSortStartList(bool newSortStartList);
    int getSortAfterGroups() const;
    void setSortAfterGroups(int newSortAfterGroups);
};

#endif // ROUNDINFO_H
