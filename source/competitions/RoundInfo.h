#ifndef ROUNDINFO_H
#define ROUNDINFO_H


class RoundInfo
{
public:
    RoundInfo();

    int getCount() const;
    void setCount(int newCount);

private:
    int count; //liczba zawodników/drużyn w danej rundzie
};

#endif // ROUNDINFO_H
