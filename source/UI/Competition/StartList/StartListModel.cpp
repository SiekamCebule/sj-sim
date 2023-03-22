#include "StartListModel.h"
#include "../../../global/CountryFlagsManager.h"
#include <QIcon>
#include <QFont>
#include <QColor>
#include <QBrush>

StartListModel::StartListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    jumpers = nullptr;
    teams = nullptr;
}

QVariant StartListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(section == 0 && orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole)
            return "Zawodnik";
        else if(role == Qt::FontRole)
            return QFont("Ubuntu", 9, 2);
        else if(role == Qt::BackgroundRole)
            return QBrush(QColor(qRgb(230, 230, 230)));
        else if(role == Qt::ForegroundRole)
            return QBrush(QColor(qRgb(20, 20, 20)));
    }

    return QVariant();
    // FIXME: Implement me!
}

int StartListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return jumpers->count();

    // FIXME: Implement me!
}

QVariant StartListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    else if(role == Qt::TextAlignmentRole){
        return Qt::AlignHCenter;
    }
    else if(role == Qt::ForegroundRole){
        return QBrush(QColor(qRgb(35, 35, 35)));
    }
    else if(role == Qt::BackgroundRole){
        if(completedJumps.count() < index.row()){
            if(completedJumps.at(index.row() == true))
                return QBrush(QColor(qRgb(228, 237, 230)));
            else return QBrush(QColor("white"));
        }
    }
    else if(role == Qt::FontRole){
        return QFont("Ubuntu", 10);
    }

    if(completedJumps.count() > index.row())
    {
        if(type == IndividualCompetiton){
            if(jumpers->count() > index.row())
            {
                //qDebug()<<"row: "<<index.row();
                if(role == Qt::DisplayRole){
                    if(std::find(hasDNS.begin(), hasDNS.end(), index.row()) != hasDNS.end())
                        return jumpers->at(index.row())->getNameAndSurname() + "   (Nie wystartował)";
                    else if(std::find(hasDSQ.begin(), hasDSQ.end(), index.row()) != hasDSQ.end())
                        return jumpers->at(index.row())->getNameAndSurname() + "   (Dyskwalifikacja)";
                    else
                        return jumpers->at(index.row())->getNameAndSurname();
                }
                else if(role == Qt::DecorationRole){
                    return QIcon(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumpers->at(index.row())->getCountryCode().toLower())));
                }
            }
        }
        else if(type == TeamCompetition){
            if(role == Qt::DisplayRole){
                QString string = teams->at(index.row())->getCountryCode() + " (" + jumpers->at(index.row())->getNameAndSurname() +")";
                if(std::find(hasDNS.begin(), hasDNS.end(), index.row()) != hasDNS.end())
                    return  string + "  (Nie wystartował)";
                else if(std::find(hasDSQ.begin(), hasDSQ.end(), index.row()) != hasDSQ.end())
                    return string + "  (Dyskwalifikacja)";
                else
                    return string;
            }
            else if(role == Qt::DecorationRole){
                return QIcon(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(teams->at(index.row())->getCountryCode().toLower())));
            }
        }
    }

    // FIXME: Implement me!
    return QVariant();
}

void StartListModel::fillCompletedJumpsWithFalse()
{
    switch(type){
    case IndividualCompetiton:
        completedJumps.fill(false, jumpers->count()); break;
    case TeamCompetition:
        completedJumps.fill(false, teams->count()); break;
    }
}

short StartListModel::getType() const
{
    return type;
}

void StartListModel::setType(short newType)
{
    type = newType;
}

QVector<Team *> *StartListModel::getTeams() const
{
    return teams;
}

void StartListModel::setTeams(QVector<Team *> *newTeams)
{
    teams = newTeams;
}

QVector<Jumper *> *StartListModel::getJumpers() const
{
    return jumpers;
}

void StartListModel::setJumpers(QVector<Jumper *> *newJumpers)
{
    jumpers = newJumpers;
}

QVector<int> StartListModel::getHasDSQ() const
{
    return hasDSQ;
}

void StartListModel::setHasDSQ(const QVector<int> &newHasDSQ)
{
    hasDSQ = newHasDSQ;
}

QVector<int> StartListModel::getHasDNS() const
{
    return hasDNS;
}

void StartListModel::setHasDNS(const QVector<int> &newHasDNS)
{
    hasDNS = newHasDNS;
}

QVector<bool> StartListModel::getCompletedJumps() const
{
    return completedJumps;
}

void StartListModel::setCompletedJumps(const QVector<bool> &newCompletedJumps)
{
    completedJumps = newCompletedJumps;
}
