#include "StartListModel.h"
#include "../../../global/CountryFlagsManager.h"
#include "../../../competitions/AbstractCompetitionManager.h"
#include <QIcon>
#include <QFont>
#include <QColor>
#include <QBrush>

StartListModel::StartListModel(AbstractCompetitionManager * manager, QObject *parent)
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
        if(startListStatus->at(index.row()).getJumpStatus() == StartListCompetitorStatus::Finished){
            return QBrush(QColor(qRgb(77, 77, 77)));
        }
        else{
            return QBrush(QColor("black"));
        }
    }
    else if(role == Qt::BackgroundRole){
        if(startListStatus->at(index.row()).getJumpStatus() == StartListCompetitorStatus::Finished){
            return QBrush(QColor(qRgb(242, 242, 242)));
        }
        else{
            return QBrush(QColor("white"));
        }
    }
    else if(role == Qt::FontRole){
        QFont font("Ubuntu", 10);
        if(startListStatus->at(index.row()).getJumpStatus() == StartListCompetitorStatus::Finished){
            font.setItalic(true);
        }
        else{
            font.setPointSize(11);
        }
    }

    if(type == IndividualCompetiton){
        if(jumpers->count() > index.row())
        {
            //Trzeba dodać numer startowy do "stringa"
            QString string = QString::number(index.row()) + ". " + jumpers->at(index.row())->getNameAndSurname();
            if(role == Qt::DisplayRole){
                if(startListStatus->at(index.row()).getJumpStatus() == StartListCompetitorStatus::Dns)
                    return string + "   (Nie wystartował)";
                else if(startListStatus->at(index.row()).getJumpStatus() == StartListCompetitorStatus::Dsq)
                    return string + "   (Dyskwalifikacja)";
                else
                    return string;
            }
            else if(role == Qt::DecorationRole){
                return QIcon(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumpers->at(index.row())->getCountryCode().toLower())));
            }
        }
    }
    else if(type == TeamCompetition){
        if(role == Qt::DisplayRole){
            QString string = teams->at(index.row())->getCountryCode() + " (" + jumpers->at(index.row())->getNameAndSurname() +")";
            if(startListStatus->at(index.row()).getJumpStatus() == StartListCompetitorStatus::Dns)
                return  string + "  (Nie wystartował)";
            else if(startListStatus->at(index.row()).getJumpStatus() == StartListCompetitorStatus::Dsq)
                return string + "  (Dyskwalifikacja)";
            else
                return string;
        }
        else if(role == Qt::DecorationRole){
            return QIcon(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(teams->at(index.row())->getCountryCode().toLower())));
        }
    }

    // FIXME: Implement me!
    return QVariant();
}

QVector<StartListCompetitorStatus> *StartListModel::getStartListStatus() const
{
    return startListStatus;
}

void StartListModel::setStartListStatus(QVector<StartListCompetitorStatus> *newStartListStatus)
{
    startListStatus = newStartListStatus;
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
