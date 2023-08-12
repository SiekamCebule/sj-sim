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
    //Ogólne
    int count; //liczba zawodników/drużyn w danej rundzie
    bool sortStartList;

    //Konkursy drużynowe
    int sortAfterGroups;

    //Tryb K.O
    bool KO; //Określa czy ta runda jest rundą K.O
    int countInKOGroup; //Ilość zawodników w grupie K.O
    int advancingFromKOGroup; //Ilość zawodników awansujących z grupy K.O
    //Ilość lucky looserów jest obliczana automatycznie na podstawie następnej rundy/następnego konkursu dla którego K.O jest kwalifikacjami
public:
    int getCount() const;
    void setCount(int newCount);
    bool getSortStartList() const;
    void setSortStartList(bool newSortStartList);
    int getSortAfterGroups() const;
    void setSortAfterGroups(int newSortAfterGroups);
    int getCountInGroup() const;
    void setCountInGroup(int newCountInGroup);
    int getAdvancingFromGroup() const;
    void setAdvancingFromGroup(int newAdvancingFromGroup);
    bool getKO() const;
    void setKO(bool newKO);
    int getAdvancingFromKOGroup() const;
    void setAdvancingFromKOGroup(int newAdvancingFromKOGroup);
    int getCountInKOGroup() const;
    void setCountInKOGroup(int newCountInKOGroup);
};

#endif // ROUNDINFO_H
