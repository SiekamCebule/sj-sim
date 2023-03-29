#ifndef ROUNDINFO_H
#define ROUNDINFO_H


class RoundInfo
{
public:
    RoundInfo(int count = 0, bool sortStartList = false);

    int getCount() const;
    void setCount(int newCount);

    bool getSortStartList() const;
    void setSortStartList(bool newSortStartList);

private:
    int count; //liczba zawodników/drużyn w danej rundzie
    bool sortStartList;
};

#endif // ROUNDINFO_H
