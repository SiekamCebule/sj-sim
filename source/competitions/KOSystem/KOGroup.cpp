#include "KOGroup.h"
#include "../CompetitionInfo.h"
#include "../RoundInfo.h"
#include "../../global/SeasonDatabaseObjectsManager.h"
#include "../../global/MyRandom.h"
#include "../../utilities/functions.h"

extern SeasonDatabaseObjectsManager seasonObjectsManager;

KOGroup::KOGroup()
{

}

QJsonObject KOGroup::getJsonObject(KOGroup &group)
{
    QJsonObject object;
    object.insert("id", QString::number(group.getID()));
    object.insert("number", group.getNumber());
    object.insert("competition-id", QString::number(group.getCompetition()->getID()));
    QJsonArray jumpersArray;
    for(auto & jumper : group.getJumpersReference())
        jumpersArray.push_back(QString::number(jumper->getID()));
    object.insert("jumpers-ids", jumpersArray);

    return object;
}

KOGroup KOGroup::getFromJson(const QJsonObject &json)
{
    KOGroup group;
    group.setID(json.value("id").toString().toULong());
    group.setNumber(json.value("number").toInt());
    group.setCompetition(static_cast<CompetitionInfo*>(seasonObjectsManager.getObjectByID(json.value("competition-id").toString().toULong())));
    QJsonArray jumpersArray = json.value("jumpers-ids").toArray();
    for(auto val : jumpersArray)
        group.getJumpersReference().push_back(static_cast<Jumper *>(seasonObjectsManager.getObjectByID(val.toString().toULong())));

    return group;
}

QVector<KOGroup> KOGroup::constructDefaultKOGroups(RoundInfo *roundInfo, QVector<Jumper *> *jumpers, int selectionType, CompetitionInfo * competition)
{
    QVector<KOGroup> groups;
    switch(selectionType)
    {
    case KOGroup::Classic:{ //Tak jak w TCS
        if(roundInfo->getCountInKOGroup() == 2)
        {
            if(jumpers->count() % 2 == 0) //Jeżeli ilość skoczków jest parzysta
            {
                int number = jumpers->count() / 2;
                for(int i=0; i<number; i++)
                {
                    KOGroup group;
                    group.setNumber(i+1);
                    group.setCompetition(competition);
                    //group.setCompetitionType(competition->getRulesPointer()->getCompetitionType());

                    Jumper * firstJumper = jumpers->at(jumpers->count() - 1 - i); //Ten gorszy w kwalifikacjach
                    Jumper * secondJumper = jumpers->at(i); //Ten lepszy w kwalifikacjach
                    group.setJumpers({secondJumper, firstJumper}); //na odwrót ponieważ w jumpers zawodnicy są od najgorszego do najlepszego

                    groups.push_back(group);
                }
                std::reverse(groups.begin(), groups.end());
                int nm=1;
                for(auto & g : groups){
                    g.setNumber(nm);
                    nm++;
                }
            }
        }
        break;
    }
    case KOGroup::ForLargerGroups:
    {
        int gap = jumpers->count() / roundInfo->getCountInKOGroup();
        int groupsCount = jumpers->count() / roundInfo->getCountInKOGroup(); //bez reszty. Ilość pełnych grup.
        int additionalGroupJumpersCount = jumpers->count() % roundInfo->getCountInKOGroup(); //Np 4 dla 49 % 5, czyli jedna grupa ma czterech zamiast pięciu
        for(int i=0; i<groupsCount; i++)
        {
            KOGroup group;
            group.setNumber(i+1);
            group.setCompetition(competition);
            //group.setCompetitionType(competition->getRulesPointer()->getCompetitionType());

            int jumperIndex = i;
            for(int ii=0; ii<roundInfo->getCountInKOGroup(); ii++)
            {
                Jumper * jumper = jumpers->at(jumperIndex);
                group.getJumpersReference().push_back(jumper);
                jumperIndex += gap; //Zwiększa się o odstęp np. 3, 13, 23, 33, 43
            }

            groups.push_back(group);
        }
        if(additionalGroupJumpersCount > 0) //jest konieczność zrobienia dodatkowej grupy bo nie starcza zawodniów
        {
            KOGroup additionalGroup;
            if(groups.count() > 0)
                additionalGroup.setNumber(groups.last().getNumber() + 1);
            else
                additionalGroup.setNumber(1);
            additionalGroup.setCompetition(competition);
            //additionalGroup.setCompetitionType(competition->getRulesPointer()->getCompetitionType());

            for(int jumperIndex=jumpers->count() - 1; jumperIndex>jumpers->count() - 1 - additionalGroupJumpersCount; jumperIndex--)
            {
                Jumper * jumper = jumpers->at(jumperIndex);
                additionalGroup.getJumpersReference().push_back(jumper);
            }
            groups.push_back(additionalGroup);
        }
        break;
    }
    case KOGroup::BasketsDraw:
    {
        QVector<QVector<Jumper *>> baskets;
        int gap = jumpers->count() / roundInfo->getCountInKOGroup();
        int groupsCount = jumpers->count() / roundInfo->getCountInKOGroup();
        int additionalBasketJumpersCount = jumpers->count() % roundInfo->getCountInKOGroup();
        int from = 0;
        int to = gap - 1; //zakres np od 0 do 9
        if(jumpers->count() % roundInfo->getCountInKOGroup() == 0)
            to += 1;
        if(to > 0)
            for(int i=0; i<roundInfo->getCountInKOGroup(); i++) //tyle jest koszyków
            {
                QVector<Jumper *> basket;
                for(int index=from; index<=to; index++)
                {
                    basket.push_back(jumpers->at(index));
                }
                if(i+1 != roundInfo->getCountInKOGroup()){
                    from += gap;
                    to += gap;
                }
                baskets.push_back(basket);
            }
        QVector<Jumper *> additionalBasket;
        if(additionalBasketJumpersCount > 0) //Tworzymy dodatkowy koszyk dla pozostałych zawodników
            for(int index=to+1; index<to+1+additionalBasketJumpersCount; index++)
                additionalBasket.push_back(jumpers->at(index));

        //Losowanie
        for(int i=0; i<groupsCount; i++)
        {
            KOGroup group;
            group.setNumber(i+1);
            group.setCompetition(competition);
            //group.setCompetitionType(competition->getRulesPointer()->getCompetitionType());

            groups.push_back(group);
        }
        for(auto & basket : baskets)
        {
            for(auto & group : groups)
            {
                int random = MyRandom::randomInt(0, basket.count() - 1);
                Jumper * jumper = basket[random];
                basket.remove(random);
                group.getJumpersReference().push_back(jumper);
            }
        }
        if(additionalBasket.count() > 0) //Jeżeli jest jeszcze dodatkowy koszyk do rozlosowania
        {
            while(additionalBasket.count() > 0){
                int randomGroupIndex = MyRandom::randomInt(0, groups.count() - 1); //Losujemy grupę
                if(groups[randomGroupIndex].getJumpersReference().count() == roundInfo->getCountInKOGroup()) //Sprawdzamy czy nie dodaliśmy jeszcze do tej grupy zawodnika z additionalBasket
                {
                    Jumper * jumper = additionalBasket.first();
                    groups[randomGroupIndex].getJumpersReference().push_back(jumper);
                    additionalBasket.removeFirst();
                }
            }
        }
        break;
    }
    case KOGroup::Random:
    {
        int groupsCount = jumpers->count() / roundInfo->getCountInKOGroup();
        for(int i=0; i<groupsCount; i++)
        {
            KOGroup group;
            group.setNumber(i+1);
            group.setCompetition(competition);
            //group.setCompetitionType(competition->getRulesPointer()->getCompetitionType());

            groups.push_back(group);
        }
        QVector<Jumper *> toDraw = *jumpers;
        toDraw.detach();
        int normalJumpersCountLimit = roundInfo->getCountInKOGroup();
        int higherJumpersCountLimit = roundInfo->getCountInKOGroup() + 1;
        //int canReachHigherLimit = jumpers->count() % roundInfo->getCountInKOGroup(); //Ilość drużyn która może osiągnąć limit zawodników na wyższym poziomie


        for(auto & group : groups)
        {
            if(group.getJumpersReference().count() == normalJumpersCountLimit)
                continue;
            else
            {
                for(int i=0; i<roundInfo->getCountInKOGroup(); i++){
                    if(toDraw.count() > 0){
                        Jumper * randomJumper = toDraw[MyRandom::randomInt(0, toDraw.count() - 1)];
                        group.getJumpersReference().push_back(randomJumper);
                        MyFunctions::removeFromVector(toDraw, randomJumper);
                    }
                }
            }
        }
        if(toDraw.count() > 0)
        {
            KOGroup additionalGroup;
            if(groups.count() > 0)
                additionalGroup.setNumber(groups.last().getNumber());
            else
                additionalGroup.setNumber(1);
            additionalGroup.setCompetition(competition);

            for(auto & j : toDraw)
                additionalGroup.getJumpersReference().push_back(j);
            groups.push_back(additionalGroup);
        }
        break;
    }
    }

    return groups;
}

void KOGroup::setTeams(const QVector<Team *> &newTeams)
{
    teams = newTeams;
}

QVector<Jumper *> KOGroup::getJumpers() const
{
    return jumpers;
}

QVector<Jumper *> &KOGroup::getJumpersReference()
{
    return jumpers;
}

void KOGroup::setJumpers(const QVector<Jumper *> &newJumpers)
{
    jumpers = newJumpers;
}

QVector<Team *> &KOGroup::getTeamsReference()
{
    return teams;
}

int KOGroup::getNumber() const
{
    return number;
}

void KOGroup::setNumber(int newNumber)
{
    number = newNumber;
}

CompetitionInfo *KOGroup::getCompetition() const
{
    return competition;
}

void KOGroup::setCompetition(CompetitionInfo *newCompetition)
{
    competition = newCompetition;
}
